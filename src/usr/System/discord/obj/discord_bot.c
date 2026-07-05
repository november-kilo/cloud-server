#include <kernel/user.h>
#include <kernel/kernel.h>
#include <Array.h>
#include <no_export.h>

inherit "~System/discord/lib/discord_bot";
inherit format "/lib/util/format";
inherit "/lib/util/mapping";

#define JSON_DECODER "/sys/jsondecode"
#define JSON_ENCODER "/sys/jsonencode"
#define SYSTEM_USER "/usr/System/obj/user"
#define DISCORD_BOT "/usr/System/discord/obj/discord_bot"
#define DATA_FILE "/usr/System/data/discord_data.o"

mapping linked_accounts;  /* ([ discord_id: mud_username ]) */
mapping pending_links;    /* ([ mud_username: ({ discord_id, verification_code, timestamp }) ]) */
mapping reverse_links;    /* ([ mud_username: discord_id ]) */

static void create(void) {
    int restored;

    ::create();

    restored = restore_object(DATA_FILE);
    if (!restored) {
        linked_accounts = ([ ]);
        pending_links = ([ ]);
        reverse_links = ([ ]);
        DRIVER->message("Discord bot data initialized.\n");
    }
}

/*
 * NAME: login()
 * DESCRIPTION: Log the Discord bot in
 */
int login(string str) {
    if (previous_program() == LIB_CONN && str == DISCORD_BOT_LOGIN) {
        ::connection(previous_object());
        DRIVER->message("Discord bot (" + str + ") logged in from " + ip_number() + "\n");
        return MODE_NOCHANGE;
    }
}

/*
 * NAME:    validate_mudname()
 * DESCRIPTION: Validates MUD name in link request
 */
 private int validate_mud_name(string mud_name) {
    Array mud_names;
    int is_valid_mud_name;
    string found_first;

    mud_names = new Array(get_dir("/usr/[a-z]*")[0]);
    found_first = mud_names->find_first(mud_name)[1];
    is_valid_mud_name = !!found_first;
    return is_valid_mud_name;
 }

 /*
  * NAME:   is_expired()
  * DESCRIPTION: Whether a pending link is expired
  */
private int is_expired(mixed *pending_link) {
    return time() - pending_link[2] > 600;
}

 /*
  * NAME:   clear_expired_link_requests()
  * DESCRIPTION: Removes expired link requests from memory
  */
private void clear_expired_link_requests(void) {
    string *mud_names;
    mixed *pending;
    int i, sz;
    
    if (pending_links && map_sizeof(pending_links) > 0) {
        mud_names = map_indices(pending_links);
        sz = sizeof(mud_names);
        
        for (i = 0; i < sz; i++) {
            pending = pending_links[mud_names[i]];
            if (is_expired(pending)) {
                pending_links = map_remove(pending_links, mud_names[i]);
            }
        }
    }
}

/*
 * NAME: save_me()
 * DESCRIPTION: Persist data
 */
private save_me(void) {
    clear_expired_link_requests();
    save_object(DATA_FILE);
    DRIVER->message("Discord bot data saved.\n");
}

private object find_player(string name) {
    return "/usr/System/sys/userd"->find_user(name);
}

/*
 * NAME: generate_code()
 * DESCRIPTION: Generate verification code
 */
private string generate_code() {
    return sprintf("%06d", random(1000000));
}

/*
 * NAME:	is_linked()
 * DESCRIPTION:	Check if MUD user is linked to Discord
 */
int is_linked(string mud_username) {
    DRIVER->message("previous program: " + previous_program() + "\n");
    if (previous_program() == DISCORD_BOT) {
        return reverse_links[mud_username] != nil;
    }
    return FALSE;
}

/*
 * NAME:	get_discord_id()
 * DESCRIPTION:	Get Discord ID for MUD username
 */
string get_discord_id(string mud_username) {
    return reverse_links[mud_username];
}

/*
 * NAME: do_status()
 * DESCRIPTION: Tell user whether or not their accounts are linked
 */
void do_status(object user) {
    int linked;
    string name, discord_id;

    if (previous_program() == SYSTEM_USER) {
        name = user->query_name();
        linked = is_linked(name);
        
        if (linked) {
            discord_id = get_discord_id(name);
            user->message("Your account is linked to Discord.\n");
            user->message("Discord ID: " + discord_id + "\n");
            user->message("\nTo unlink: discord unlink\n");
        } else {
            user->message("Your account is not linked to Discord.\n");
            user->message("To link: Use '/mudlink " + name + "' in Discord\n");
        }
    }
}

/*
 * NAME:	send_to_discord()
 * DESCRIPTION:	Send JSON payload to Discord
 */
void send_to_discord(mapping data) {
    string json;
    json = JSON_ENCODER->encode(data);
    send("@@JSON@@" + json + "\n");
}

/*
 * NAME:	handle_chat_from_discord()
 * DESCRIPTION:	Relay Discord message to MUD users
 */
private void handle_chat_from_discord(mapping data) {
    string mud_username, message;
    object user;
    object *users;
    int i, sz;
    
    mud_username = linked_accounts[data["discord_id"]];
    
    if (!mud_username) {
        users = users();
        for (i = 0, sz = sizeof(users); i < sz; i++) {
            users[i]->message("[Discord] " + data["user"] + ": " + data["content"] + "\n");
        }
        return;
    }
    
    user = find_player(mud_username);
    if (user) {
        user->message(mud_username + " [via Discord]: " + data["content"] + "\n");
    }
}

/*
 * NAME:    handle_command_from_discord()
 * DESCRIPTION: Process command from Discord to MUD
 */
 private void handle_command_from_discord(mapping data) {
    /* ([ "command":"who", "discord_id":"1216044159018729489", "type":"command" ]) */
    string command;
    object *users;
    string results;
    Array array;
    string mud_username;
    object user;

    DRIVER->message("[DISCORD_BOT]: handling command: " + format::to_string(data) + "\n");

    command = data["command"];
    switch (command) {
        case "who": {
            array = new Array(users());
            array = array->map(new WhoMapper());
            results = "Users: " + array->joining(", ");
            DRIVER->message("who: " + results + "\n");
            break;
        }
        case "stats": {
            /* this will do for now */
            mud_username = linked_accounts[data["discord_id"]];
            user = find_player(mud_username);
            if (user) {
                user->message("[DISCORD_BOT]: sending your stats to discord.\n");
            }
            results = "stats for " + mud_username;
            break;
        }
        default: results = "Unknown command: " + command;
    }
    
        send_to_discord(([
            "type": "chat", "message": results, "sender": "DGD"
        ]));
 }

/*
 * NAME:    unlink_account()
 * DESCRIPTION: persist unlinked account
 */
 private int unlink_account(string mud_username) {
    string discord_id;
    
    if (!reverse_links[mud_username]) {
        return 0;
    }
    
    discord_id = reverse_links[mud_username];
    
    linked_accounts = map_remove(linked_accounts, discord_id);
    reverse_links = map_remove(reverse_links, mud_username);
    save_me();
    
    send_to_discord(([
        "type": "unlink",
        "discord_id": discord_id,
        "mud_username": mud_username
    ]));
    
    return 1;
}

/*
 * NAME:	handle_link_request()
 * DESCRIPTION:	Process account linking request
 */
private void handle_link_request(mapping data) {
    string mud_username, verification_code;
    object user;
    
    mud_username = data["mud_username"];

    if (!validate_mud_name(mud_username)) {
        DRIVER->message("[DISCORD_BOT] link request: invalid mud name: " + mud_username + "\n");
        return;
    }
    
    verification_code = generate_code();
    
    pending_links[mud_username] = ({ 
        data["discord_id"], 
        verification_code, 
        time() 
    });
    save_me();
    
    user = find_player(mud_username);
    if (user) {
        user->message(
            "Discord Link Request\n" +
            "--------------------\n" +
            "Discord User: " + data["discord_username"] + "\n" +
            "Verification Code: " + verification_code + "\n\n" +
            "To complete linking, type: discord verify " + verification_code + "\n" +
            "This code expires in 10 minutes.\n"
        );
    }
}

/*
 * NAME:    handle_unlink_request()
 * DESCRIPTION: unlink accounts and notify
 */
private void handle_unlink_request(mapping data) {
    string discord_id, mud_username;
    object user;
    
    discord_id = data["discord_id"];
    mud_username = linked_accounts[discord_id];
    
    if (!mud_username) {
        send_to_discord(([
            "type": "unlink_error",
            "discord_id": discord_id,
            "message": "Account is not linked"
        ]));
        return;
    }
    
    if (unlink_account(mud_username)) {
        user = find_player(mud_username);
        if (user) {
            user->message("Your Discord account has been unlinked via Discord.\n");
        }
    }
}

/*
 * NAME:	verify_link()
 * DESCRIPTION:	Verify and complete account linking
 */
int verify_link(string mud_username, string code) {
    mixed *pending;
    string discord_id;
    
    pending = pending_links[mud_username];
    
    if (!pending) {
        return 0;
    }
    
    if (time() - pending[2] > 600) {
        pending_links = map_remove(pending_links, mud_username);
        save_me();
        return 0;
    }
    
    if (pending[1] != code) {
        return 0;
    }
    
    discord_id = pending[0];
    
    DRIVER->message("[DISCORD_BOT] verify link: " + mud_username + ", code: " + code + "\n");
    linked_accounts[discord_id] = mud_username;
    reverse_links[mud_username] = discord_id;
    pending_links = map_remove(pending_links, mud_username);
    save_me();
    
    send_to_discord(([
        "type": "link_complete",
        "discord_id": discord_id,
        "mud_username": mud_username
    ]));
    
    return 1;
}

/*
 * NAME:    do_verify()
 * DESCRIPTION: Tell user if their accounts are linked
 */
void do_verify(object user, string code) {
    if (previous_program() == SYSTEM_USER) {
        if (verify_link(user->query_name(), code)) {
            user->message("Your Discord account is now linked.\n");
            save_me();
        } else {
            user->message("Invalid or expired verification code.\n");
        }
    }
}

/*
 * NAME:	get_linked_username()
 * DESCRIPTION:	Get MUD username for a Discord ID
 */
string get_linked_username(string discord_id) {
    return linked_accounts[discord_id];
}

/*
 * NAME:    do_unlink()
 * DESCRIPTION: Unlink MUD and Discord accounts
 */
void do_unlink(object user) {
    object bridge;
    string mud_username;

    if (previous_program() == SYSTEM_USER) {
        mud_username = user->query_name();
        
        if (!is_linked(mud_username)) {
            user->mesage("Your account is not currently linked to Discord.\n");
            return;
        }
        
        if (unlink_account(mud_username)) {
            user->message("Your Discord account has been unlinked successfully.\n");
        } else {
            user->message("Failed to unlink account. Please contact an administrator.\n");
        }
    }
}

/*
 * NAME:	handle_discord_message()
 * DESCRIPTION:	Process JSON payload from Discord
 */
void handle_discord_message(mapping data) {
    string msg_type;
    
    msg_type = data["type"];
    
    switch (msg_type) {
        case "discord_message":
            handle_chat_from_discord(data);
            break;
        case "link_request":
            handle_link_request(data);
            break;
        case "unlink_request":
            handle_unlink_request(data);
            break;
        case "command":
            handle_command_from_discord(data);
            break;
    }
}

/*
 * NAME: receive_message()
 * DESCRIPTION: Receive a message from Discord
 */
int receive_message(string str) {
    if (previous_program() == LIB_CONN) {
        mapping payload;

        DRIVER->message("[DISCORD_BOT] message received: " + str + "\n");

        if (str[0..7] == "@@JSON@@") {
            payload = JSON_DECODER->decode(str[8..]);
            handle_discord_message(payload);
        }

        return MODE_NOCHANGE;
    }
}
