# include <kernel/kernel.h>
# include <kernel/access.h>
# include <kernel/user.h>
# include <status.h>
# include <type.h>
# include <Array.h>
# include <Iterator.h>
# include <Terminal.h>
# include <Time.h>
# include <kfun.h>

inherit auto	"~/lib/auto";
inherit user	LIB_USER;
inherit wiztool	LIB_WIZTOOL;

private inherit	"/lib/util/string";


# define USER			"/usr/System/obj/user"
# define USERSERVER		"/usr/System/sys/userd"
# define OBJECTSERVER		"/usr/System/sys/objectd"
# define UPGRADESERVER		"/usr/System/sys/upgraded"

# define STATE_NORMAL		0
# define STATE_LOGIN		1
# define STATE_OLDPASSWD	2
# define STATE_NEWPASSWD1	3
# define STATE_NEWPASSWD2	4
# define STATE_PASTING		5

static string name;		/* user name */
static string Name;		/* capitalized user name */
static mapping state;		/* state for a connection object */
string password;		/* user password */
static string newpasswd;	/* new password */
static string paste_buffer;	/* buffer holding text being pasted */
static int nconn;		/* # of connections */
static object local_wiztool;
static mixed *idle;

/*
 * NAME:	create()
 * DESCRIPTION:	initialize object
 */
static create()
{
    wiztool::create(200);
    state = ([ ]);
}

void println(string str) {
    message(str + "\n");
}

/*
 * NAME:	find_object()
 * DESCRIPTION:	find_object wrapper
 */
static object find_object(string path)
{
    return auto::find_object(path);
}

/*
 * NAME:	clone_object()
 * DESCRIPTION:	clone_object wrapper
 */
static object clone_object(string path, mixed args...)
{
    string owner;

    owner = query_owner();
    path = DRIVER->normalize_path(path, query_directory(), owner);
    if (sscanf(path, "/kernel/%*s") != 0 || !access(owner, path, READ_ACCESS)) {
	message(path + ": Permission denied.\n");
	return nil;
    }
    return auto::clone_object(path, args...);
}

/*
 * NAME:	new_object()
 * DESCRIPTION:	new_object wrapper
 */
static object new_object(string path, mixed args...)
{
    string owner;

    owner = query_owner();
    path = DRIVER->normalize_path(path, query_directory(), owner);
    if (sscanf(path, "/kernel/%*s") != 0 || !access(owner, path, READ_ACCESS)) {
	message(path + ": Permission denied.\n");
	return nil;
    }
    return auto::new_object(path, args...);
}

/*
 * NAME:	compile_object()
 * DESCRIPTION:	compile an object
 */
static object compile_object(string path, string source...)
{
    if (path && sscanf(path, "%*s/@@@/") != 0) {
	message(path + ": Cannot compile leaf object.\n");
	return nil;
    }
    return wiztool::compile_object(path, source...);
}

/*
 * NAME:	ident()
 * DESCRIPTION:	find user
 */
static object ident(string str)
{
    return USERSERVER->find_user(str);
}


/*
 * NAME:	cmd_issues()
 * DESCRIPTION:	list all object and include issues of a certain file
 */
static void cmd_issues(object user, string cmd, string str)
{
    mixed *files, *issues;
    object objectd;
    int i, j, sz, len;
    string file;

    if (!str) {
	message("Usage: " + cmd + " <file> [<file> ...]\n");
	return;
    }
    files = expand(str, -1, TRUE)[0]; /* May not exist, full filenames */
    objectd = find_object(OBJECTSERVER);

    for (i = 0, sz = sizeof(files); i < sz; i++) {
	file = files[i];
	str = "No issues.";
	len = strlen(file);
	if (len >= 2 && file[len - 2 ..] == ".c") {
	    issues = objectd->query_issues(file[.. len - 3]);
	    j = sizeof(issues);
	    if (j != 0) {
		do {
		    --j;
		    issues[j] = (string) issues[j];
		} while (j != 0);
		str = implode(issues, ", ");
	    }
	}
	message(indent_string(file + ": ", str));
    }
}

/*
 * NAME:	cmd_upgrade()
 * DESCRIPTION:	upgrade a source file and all that depends on it
 */
static void cmd_upgrade(object user, string cmd, string str)
{
    string *sources;
    int atom, i, sz, len;
    object patcher;

    if (!str || str == "-a") {
	message("Usage: " + cmd + " [-a|-p] <file> [<file> ...]\n");
	return;
    }
    atom = sscanf(str, "-a %s", str);
    if (!atom) {
	atom = sscanf(str, "-p %s", str);
	if (atom) {
	    patcher = this_object();
	}
    }
    sources = expand(str, 1, TRUE)[0];
    if (sizeof(sources) != 0) {
	mixed result;

	result = UPGRADESERVER->upgrade(query_owner(), sources, atom, patcher);
	if (typeof(result) == T_STRING) {
	    message(result + "\n");
	} else {
	    if (!atom || sizeof(result) == 0) {
		sources -= ({ nil });
		if (sizeof(sources) != 0) {
		    message("Successfully upgraded:\n" +
			    break_string(implode(sources, ", "), 0, 2));
		}
	    }

	    if (sizeof(result) != 0) {
		message("Errors occured compiling:\n" +
			break_string(implode(result, ".c, ") + ".c", 0, 2));
	    }
	}
    }
}


/*
 * NAME:	do_patch()
 * DESCRIPTION:	patch an object with this owner's resource limits
 */
void do_patch(object obj)
{
    if (previous_program() == UPGRADESERVER) {
	call_limited("call_patch", obj);
    }
}

/*
 * NAME:	call_patch()
 * DESCRIPTION:	patch an object
 */
static void call_patch(object obj)
{
    call_other(obj, "???");
}


/*
 * NAME:	access()
 * DESCRIPTION:	list special access
 */
static void cmd_access(object user, string cmd, string str)
{
    mapping access, *values;
    mixed *files;
    string *users;
    int i, sz;

    if (!str) {
	str = query_owner();
    }

    if (str == "global") {
	str = implode(query_global_access(), "\n /usr/");
	if (strlen(str) != 0) {
	    message("Global read access:\n /usr/" + str + "\n");
	}
    } else if (sizeof(query_users() & ({ str })) != 0) {
	access = query_user_access(str);
	str = capitalize(str);
	switch (map_sizeof(access)) {
	case 0:
	    message(str + " has no special access.\n");
	    break;

	case 1:
	    message(str + " has access to:" + list_access(access));
	    break;

	default:
	    message(str + " has access to:\n" + list_access(access));
	    break;
	}
    } else {
	if (sscanf(str, "%*s ") != 0 || (files=expand(str, 0, TRUE))[4] != 1) {
	    message("Usage: " + cmd + " <user> | global | <directory>\n");
	    return;
	}
	str = files[0][0];
	access = query_file_access(str);
	users = map_indices(access);
	if (sizeof(users) != 0) {
	    values = map_values(access);
	    for (i = 0, sz = sizeof(users); i < sz; i++) {
		access = values[i];
		message(capitalize(users[i]) +
			((map_sizeof(access) == 1) ?
			  " has access to:" : " has access to:\n") +
			list_access(values[i]));
	    }
	} else {
	    message("No special access to " + str + ".\n");
	}
    }
}

/*
 * NAME:	cmd_grant()
 * DESCRIPTION:	grant access
 */
static void cmd_grant(object user, string cmd, string str)
{
    string who, dir;
    mixed type, *files;

    if (!str ||
	(sscanf(str, "%s %s %s", who, dir, type) != 3 &&
	 sscanf(str, "%s %s", who, dir) != 2) ||
	(who == "global" && type) ||
	((dir == "access") ? type : (files=expand(dir, 0, TRUE))[4] != 1)) {
	message(
	    "Usage: " + cmd + " <user> access\n" +
	    "       " + cmd + " <user> <directory> [read | write | full]\n" +
	    "       " + cmd + " global <directory>\n");
	return;
    }

    str = (dir == "access") ? dir : files[0][0];
    switch (type) {
    case "read":
	type = READ_ACCESS;
	break;

    case nil:
    case "write":
	type = WRITE_ACCESS;
	break;

    case "full":
	type = FULL_ACCESS;
	break;

    default:
	message(
	    "Usage: " + cmd + " <user> access\n" +
	    "       " + cmd + " <user> <directory> [read | write | full]\n" +
	    "       " + cmd + " global <directory>\n");
	return;
    }

    if (who == "global") {
	/*
	 * global access
	 */
	if (sscanf(str, "/usr/%s", str) == 0 || sscanf(str, "%*s/") != 0) {
	    message("Global read access is for directories under /usr only.\n");
	} else if (sizeof(query_global_access() & ({ str })) != 0) {
	    message("That global access already exists.\n");
	} else {
	    set_global_access(str, TRUE);
	}
    } else if (who == "admin") {
	message("Admin has implicit full access.\n");
    } else if (dir == "access") {
	/*
	 * file access
	 */
	if (sscanf(who, "%*s/") != 0) {
	    message("Invalid user name.\n");
	} else if (sizeof(query_users() & ({ who })) != 0) {
	    message(capitalize(who) + " already has file access.\n");
	} else if (!access(query_owner(), "/", FULL_ACCESS)) {
	    message("Insufficient access granting privileges.\n");
	} else {
	    ::add_user(who);
	    ::add_owner(who);
	    wiztool::make_dir("/usr/" + who);
	}
    } else {
	/*
	 * special access
	 */
	if (sizeof(query_users() & ({ who })) == 0) {
	    message(capitalize(who) + " has no file access.\n");
	} else if (access(who, str + "/*", type)) {
	    message(capitalize(who) + " already has that access.\n");
	} else {
	    set_access(who, str, type);
	}
    }
}

/*
 * NAME:	cmd_ungrant()
 * DESCRIPTION:	remove access
 */
static void cmd_ungrant(object user, string cmd, string str)
{
    string who, dir;
    mixed *files;

    if (!str || sscanf(str, "%s %s", who, dir) != 2 ||
	(dir != "access" &&
	 (sscanf(dir, "%*s ") != 0 || (files=expand(dir, 0, TRUE))[4] != 1))) {
	message("Usage: " + cmd + " <user> access\n" +
		"       " + cmd + " <user> <directory>\n" +
		"       " + cmd + " global <directory>\n");
	return;
    }

    str = (dir == "access") ? dir : files[0][0];

    if (who == "global") {
	/*
	 * global access
	 */
	if (sscanf(str, "/usr/%s", str) == 0 || sscanf(str, "%*s/") != 0) {
	    message("Global read access is for directories under /usr only.\n");
	} else if (sizeof(query_global_access() & ({ str })) == 0) {
	    message("That global access does not exist.\n");
	} else {
	    set_global_access(str, FALSE);
	}
    } else if (who == "admin") {
	message("Access for Admin cannot be removed.\n");
    } else if (dir == "access") {
	/*
	 * file access
	 */
	if (!(user=USERSERVER->find_user(who))) {
	    message("No such user.\n");
	} else if (sizeof(query_users() & ({ who })) == 0) {
	    message(capitalize(who) + " has no file access.\n");
	} else {
	    remove_user(who);
	}
    } else {
	/*
	 * special access
	 */
	if (sizeof(query_users() & ({ who })) == 0) {
	    message(capitalize(who) + " has no file access.\n");
	} else if (!query_user_access(who)[str]) {
	    message(capitalize(who) + " has no such access.\n");
	} else {
	    set_access(who, str, 0);
	}
    }
}

/*
 * NAME:	cmd_snapshot()
 * DESCRIPTION:	create a snapshot
 */
static void cmd_snapshot(object user, string cmd, string str)
{
    int incr;

    if (str) {
	if (str != "-f") {
	    message("Usage: " + cmd + " [-f]\n");
	    return;
	}
	incr = FALSE;
    } else {
	incr = TRUE;
    }

    dump_state(incr);
}

/*
 * NAME:	cmd_hotboot()
 * DESCRIPTION:	hotboot system
 */
static void cmd_hotboot(object user, string cmd, string str)
{
    if (str) {
	message("Usage: " + cmd + "\n");
	return;
    }

    if (!access(query_owner(), "/", FULL_ACCESS)) {
	message("Permission denied.\n");
    } else {
	dump_state(TRUE);
	shutdown(TRUE);
    }
}

mixed *queryIdle(void) {
    return idle;
}

private string idleTime(object user) {
    Time now;
    mixed *idle;

    now = new Time(time());
    idle = user->queryIdle();

    if (idle == nil) {
        return "";
    }

    return (now - new Time(idle[0], idle[1]))->asDuration()[4];
}

private void fetch_local_wiztool(void) {
    string err_str, obj_str;

    obj_str = USR_DIR + "/" + name + "/obj/wiztool";
    if (file_info(obj_str + ".c")) {
        if (local_wiztool) {
            destruct_object(local_wiztool);
        }

        err_str = catch(compile_object(obj_str));
        if (err_str) {
            message("Failed to compile your local wiztool:\n\t" + err_str + "\n");
            return;
        }

        local_wiztool = clone_object(obj_str);
        message("Created your local wiztool.\n");
    }
}

void showPrompt(void) {
    if (local_wiztool && function_object("getPrompt", local_wiztool)) {
        message(local_wiztool->getPrompt(this_object()));
    } else {
        message("> ");
    }
}

/*
 * NAME:	command()
 * DESCRIPTION:	process user input
 */
static int command(string str)
{
    string arg;

    if (query_editor(this_object())) {
	if (strlen(str) != 0 && str[0] == '!') {
	    str = str[1 ..];
	} else {
	    str = editor(str);
	    if (str) {
		message(str);
	    }
	    return TRUE;
	}
    }

    if (str == "") {
	return TRUE;
    }

    sscanf(str, "%s %s", str, arg);

    switch (str) {
    case "code":
    case "history":
    case "clear":
    case "clone":
    case "destruct":
    case "new":

    case "compile":
    case "issues":
    case "upgrade":

    case "cd":
    case "pwd":
    case "ls":
    case "cp":
    case "mv":
    case "rm":
    case "mkdir":
    case "rmdir":
    case "ed":

    case "access":
    case "grant":
    case "ungrant":
    case "quota":
    case "rsrc":

    case "people":
    case "status":
    case "swapout":
    case "snapshot":
    case "reboot":
    case "hotboot":

    case "cls":
    case "time":
    case "who":
    case "test":
    case "regex":
	    call_other(this_object(), "cmd_" + str, this_object(), str, arg);
	    break;

    case "halt":
        call_other(this_object(), "cmd_shutdown", this_object(), str, arg);
        break;

    default:
        if (local_wiztool && function_object("cmd_" + str, local_wiztool)) {
            call_other(local_wiztool, "cmd_" + str, this_object(), str, arg);
            break;
        }
        message("No command: " + str + "\n");
        break;
    }

    return TRUE;
}


/*
 * NAME:	tell_audience()
 * DESCRIPTION:	send message to listening users
 */
private void tell_audience(string str)
{
    object *users, user;
    int i;

    users = users();
    for (i = sizeof(users); --i >= 0; ) {
	user = users[i];
	if (user != this_object() &&
	    sscanf(object_name(user), USER + "#%*d") != 0) {
	    user->message(str);
	}
    }
}

private void saveUser(void) {
    if (name == "admin") {
        save_object(DEFAULT_USER_DIR + "/admin.pwd");
        return;
    }
    
    save_object(USR_SAVE_DIR + "/" + name + ".pwd");
}

private void restoreUser(void) {
    if (name == "admin") {
        restore_object(DEFAULT_USER_DIR + "/admin.pwd");
        return;
    }
    
    restore_object(USR_SAVE_DIR + "/" + name + ".pwd");
}

/*
 * NAME:	login()
 * DESCRIPTION:	login a new user
 */
int login(string str)
{
    if (previous_program() == LIB_CONN) {
	if (nconn == 0) {
	    ::login(str);
	}
	nconn++;
	if (strlen(str) == 0 || sscanf(str, "%*s ") != 0 ||
	    sscanf(str, "%*s/") != 0) {
	    return MODE_DISCONNECT;
	}
	Name = name = str;
	if (Name[0] >= 'a' && Name[0] <= 'z') {
	    Name[0] -= 'a' - 'A';
	}

	restoreUser();

	if (password) {
	    /* check password */
	    previous_object()->message("Password:");
	    state[previous_object()] = STATE_LOGIN;
	} else {
	    /* no password; login immediately */
	    connection(previous_object());
	    tell_audience(Name + " logs in.\n");
	    if (str != "admin") {
	        showPrompt();
		state[previous_object()] = STATE_NORMAL;
		return MODE_ECHO;
	    }
	    message("Pick a new password:");
	    state[previous_object()] = STATE_NEWPASSWD1;
	}
	return MODE_NOECHO;
    }
}

/*
 * NAME:	logout()
 * DESCRIPTION:	logout user
 */
void logout(int quit)
{
    if (previous_program() == LIB_CONN && --nconn == 0) {
        if (query_conn()) {
            if (quit) {
                tell_audience(Name + " logs out.\n");
            } else {
                tell_audience(Name + " disconnected.\n");
            }
        }
        if (local_wiztool) {
            destruct_object(local_wiztool);
            message("Destroyed your local wiztool.\n");
        }
        ::logout(name);
    }
}

/*
 * NAME:	query_name()
 * DESCRIPTION:	return this user's name
 */
string query_name()
{
    return name;
}

/*
 * NAME:	receive_message()
 * DESCRIPTION:	process a message from the user
 */
int receive_message(string str) {
    if (previous_program() == LIB_CONN) {
        string cmd;
        object user, *users;
        int i, sz;

        idle = millitime();

        switch (state[previous_object()]) {
            case STATE_NORMAL:
                cmd = str;
                if (strlen(str) != 0 && str[0] == '!') {
                    cmd = cmd[1..];
                }

                if (!query_editor(this_object()) || cmd != str) {
                    /* check standard commands */
                    if (strlen(cmd) != 0) {
                        switch (cmd[0]) {
                            case '\'':
                                if (strlen(cmd) > 1) {
                                    cmd[0] = ' ';
                                    str = cmd;
                                }
                                cmd = "say";
                                break;

                            case ':':
                                if (strlen(cmd) > 1) {
                                    cmd[0] = ' ';
                                    str = cmd;
                                }
                                cmd = "emote";
                                break;

                            default:
                                sscanf(cmd, "%s ", cmd);
                                break;
                        }
                    }

                    switch (cmd) {
                        case "say":
                            if (sscanf(str, "%*s %s", str) == 0) {
                                message("Usage: say <text>\n");
                            } else {
                                tell_audience(Name + " says: " + str + "\n");
                            }
                            str = nil;
                            break;

                        case "emote":
                            if (sscanf(str, "%*s %s", str) == 0) {
                                message("Usage: emote <text>\n");
                            } else {
                                tell_audience(Name + " " + str + "\n");
                            }
                            str = nil;
                            break;

                        case "tell":
                            if (sscanf(str, "%*s %s %s", cmd, str) != 3 ||
                                !(user = find_user(cmd))) {
                                message("Usage: tell <user> <text>\n");
                            } else {
                                user->message(Name + " tells you: " + str + "\n");
                            }
                            str = nil;
                            break;

                        case "users":
                            users = users();
                            str = "Logged on:";
                            for (i = 0, sz = sizeof(users); i < sz; i++) {
                                cmd = users[i]->query_name();
                                if (cmd) {
                                    str += " " + cmd;
                                }
                            }
                            message(str + "\n");
                            str = nil;
                            break;

                        case "password":
                            if (password) {
                                message("Old password:");
                                state[previous_object()] = STATE_OLDPASSWD;
                            } else {
                                message("New password:");
                                state[previous_object()] = STATE_NEWPASSWD1;
                            }
                            return MODE_NOECHO;

                        case "paste":
                            ::message("End your pasting with a single period.\n\"\b");
                            state[previous_object()] = STATE_PASTING;
                            paste_buffer = "";
                            return MODE_ECHO;

                        case "quit":
                            return MODE_DISCONNECT;
                    }
                }

                if (str) {
                    call_limited("command", str);
                }
                break;

            case STATE_LOGIN:
                if (hash_string("crypt", str, password) != password) {
                    previous_object()->message("\nBad password.\n");
                    return MODE_DISCONNECT;
                }
                connection(previous_object());
                message("\n");
                fetch_local_wiztool();
                tell_audience(Name + " logs in.\n");
                break;

            case STATE_OLDPASSWD:
                if (hash_string("crypt", str, password) != password) {
                    message("\nBad password.\n");
                    break;
                }
                message("\nNew password:");
                state[previous_object()] = STATE_NEWPASSWD1;
                return MODE_NOECHO;

            case STATE_NEWPASSWD1:
                newpasswd = str;
                message("\nRetype new password:");
                state[previous_object()] = STATE_NEWPASSWD2;
                return MODE_NOECHO;

            case STATE_NEWPASSWD2:
                if (newpasswd == str) {
                    password = hash_string("crypt", str);
                    saveUser();
                    message("\nPassword changed.\n");
                } else {
                    message("\nMismatch; password not changed.\n");
                }
                newpasswd = nil;
                break;

            case STATE_PASTING:
                if (str != ".") {
                    paste_buffer += str + "\n";
                    ::message("\"\b");
                    return MODE_ECHO;
                }
                if (strlen(paste_buffer)) {
                    tell_audience("--- Pasted segment from " + Name +
                                  " ---\n" + paste_buffer +
                                  "--- Pasted segment ends ---\n");
                    state[previous_object()] = STATE_NORMAL;
                    paste_buffer = nil;
                }
                break;
        }

        str = query_editor(this_object());
        if (str) {
            message((str == "insert") ? "*\b" : ":");
        } else {
            showPrompt();
        }
        state[previous_object()] = STATE_NORMAL;
        return MODE_ECHO;
    }
}

void cmd_cls(object user, string cmd, string arg) {
    if (previous_object() != user) {
        return;
    }

    user->message(new Terminal()->clear());
}

void cmd_time(object user, string cmd, string arg) {
    user->println(ctime(time()));
}

void cmd_who(object user, string cmd, string arg) {
    object *users;
    string *list;
    int sz;
    Iterator i;

    users = users() - ({ user });
    sz = sizeof(users);
    if (sz == 0) {
        user->println("One is the loneliest number.");
        return;
    }
    list = allocate(sz);
    i = new IntIterator(0, sz - 1);
    while (!i->end()) {
        list[i->next()] = users[i->current()]->query_name() + " " + idleTime(users[i->current()]);
    }
    arg = "Users logged in:\n" +
          new Array(list)->reduce(new ArrayToListReducer(), 0, sz - 1, 1);

    user->message(arg);
}

#include <Array.h>
#include <Continuation.h>
#include <Function.h>
#include <Json.h>
#include <Maths.h>
#include <Queue.h>
#include <Sort.h>
#include <String.h>

static void cmd_test(object user, string cmd, string str) {
    Array data;
    Complex *roots;
    Function reducer;
    Integrator simpson;
    EigenvalueDecomposition Eig;
    Matrix A, D, V, M;
    MergeSort mergeSort;
    Number number;
    Polynomial poly;
    Stats stats;
    String libString;
    Vector v1, v2, v3;
    float *violentCrimeRateUsa1997;
    mapping moments;
    int i, n;
    float k1, k2, median;
    mapping probs;

    violentCrimeRateUsa1997 = ({ 611.0, 567.6, 523.0, 506.5, 504.5, 494.4, 475.8, 463.2, 469.0, 479.3,
                                 471.8, 458.6, 431.9, 404.5, 387.1, 387.8, 369.1, 361.6, 373.7, 386.3 });
    mergeSort = new MergeSort();
    mergeSort->sort(violentCrimeRateUsa1997);
    n = sizeof(violentCrimeRateUsa1997);

    stats = new Stats();

    moments = stats->get(violentCrimeRateUsa1997);

    user->println(
        "Sum:                " + moments["sum"] + "\n" +
        "Average:            " + moments["average"] + "\n" +
        "Standard deviation: " + moments["standardDeviation"] + "\n" +
        "Variance:           " + moments["variance"]);

    if (n % 2) {
        median = violentCrimeRateUsa1997[((n + 1) / 2) - 1];
    } else {
        k1 = violentCrimeRateUsa1997[(n / 2) - 1];
        k2 = violentCrimeRateUsa1997[n / 2];
        median = (k1 + k2) / 2.0;
    }

    user->println("Median:             " + median);

    number = new Number(pi());
    user->println("number: " + number->toString());

    libString = new String("foo");

    simpson = new SimpsonIntegrator();
    poly = new Polynomial(({ 2.0, 3.0, 4.0, 5.0 }));
    number = new Rational(poly->integrate(0.0, 15.0));
    user->println("Polynomial: " + poly->toString());
    user->println("Integrated polynomial: " + simpson->integrate(poly, 0.0, 15.0));
    user->println("Integrated polynomial: " + poly->integrate(0.0, 15.0));
    user->println("number: " + number->toString());

    poly = poly->differentiate();
    user->println("Differentiated poly: " + poly->toString());

    poly = new Polynomial(({
        0.0, 0.0, 0.0, 1.0, 4.0, 10.0, 21.0, 38.0, 62.0, 91.0, 122.0,
        148.0, 167.0, 172.0, 160.0, 131.0, 94.0, 54.0, 21.0 })
    );

    reducer = new PolynomialProbabilityReducer(poly, 6, 4);
    reducer->apply(10);
    user->println("Probability 3D6: " + poly->toString());
    probs = reducer->evaluate();
    user->println("P(10): " + dump_value(probs, ([])));

    data = new Array(({ 1.0, 4.0, 10.0, 21.0, 38.0, 62.0, 91.0, 122.0,
                148.0, 167.0, 172.0, 160.0, 131.0, 94.0, 54.0, 21.0 }));
    data->sort();
    user->println("Sorted: " + data->toString());

    reducer = new ArrayTabularReducer();
    str = reducer->evaluate(data->getArray());
    user->println(str);

    reducer = new ArrayToListReducer();
    str = data->reduce(reducer, 0, data->size() - 1, 1);
    user->println("Data:\n" + str);

    reducer = new ArrayToMarkedListReducer();
    str = data->reduce(reducer, 0, data->size() - 1, 1);
    user->println(str);

    reducer = new ArrayToMarkedListReducer(3);
    str = data->reduce(reducer, 0, data->size() - 1, 1);
    user->println(str);

    v1 = new Vector(({ new Number(2), new Number(4), new Number(6) }));
    v2 = new Vector(({ new Number(3), new Number(5), new Number(7) }));
    v3 = v1->cross(v2);
    user->println("Cross product: " + v3->toString());

    number = v1->dot(v2);
    user->println("Dot product: " + number->toString());

    poly = new Polynomial(({ -1., 0., 0., 0., 1. }));
    roots = poly->roots();

    user->println("" + roots[0]->toString());
    user->println("" + roots[1]->toString());
    user->println("" + roots[2]->toString());
    user->println("" + roots[3]->toString());

    poly = new Polynomial(({ 5.0, 4.0, 3.0, 2.0, 1.0 }));
    roots = poly->roots();

    user->println("" + roots[0]->toString());
    user->println("" + roots[1]->toString());
    user->println("" + roots[2]->toString());
    user->println("" + roots[3]->toString());

    A = new Matrix(3, 3);
    A->setMatrix(({ ({ 4.0, 6.0, 7.0 }), ({ 6.0, 3.0, 2.0 }), ({ 7.0, 2.0, 1.0 }) }));
    Eig = A->eig();
    V = Eig->getV();
    D = Eig->getD();

    user->println("Nonsymmetric A\n" + A->toString());
    user->println("V\n" + V->toString());
    user->println("D\n" + D->toString());

    A = new Matrix(3, 3);
    A->setMatrix(({ ({ 5.0, 1.0, 3.0 }), ({ 2.0, 0.0, 2.0 }), ({ 3.0, 1.0, 5.0 }) }));
    Eig = A->eig();
    V = Eig->getV();
    D = Eig->getD();

    user->println("Symmetric A\n" + A->toString());
    user->println("V\n" + V->toString());
    user->println("D\n" + D->toString());
}

/*
>> nonsymmetric = [ [4,6,7]; [6,3,2]; [7,2,1] ]

nonsymmetric =

     4     6     7
     6     3     2
     7     2     1

>> symmetric = [ [5,1,3]; [2,0,2]; [3,1,5] ]

symmetric =

     5     1     3
     2     0     2
     3     1     5

>> [Vn,Dn]=eig(nonsymmetric)

Vn =

    0.6887    0.1638    0.7063
   -0.3367   -0.7905    0.5116
   -0.6421    0.5902    0.4893


Dn =

   -5.4594         0         0
         0    0.2637         0
         0         0   13.1957



>> [Vs,Ds]=eig(symmetric)

Vs =

   -0.6707   -0.7071    0.1164
   -0.3167    0.0000   -0.9864
   -0.6707    0.7071    0.1164


Ds =

    8.4721         0         0
         0    2.0000         0
         0         0   -0.4721
 */
