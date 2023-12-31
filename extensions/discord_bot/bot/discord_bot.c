#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <concord/discord.h>
#include <concord/log.h>
#include "no_export.h" /* #defines DISCORD_BOT_TOKEN and CHANNEL_ID */

struct discord *bot;

void on_sigint_signal(int signum) {
    (void) signum;
    log_info("SIGINT");
    discord_shutdown(bot);
}

void on_ready_event(struct discord *client, const struct discord_ready *event) {
    log_info("%s#%s connected to Discord server.", event->user->username, event->user->discriminator);
}

void on_message_create_event(struct discord *client, const struct discord_message *event) {
    if (event->author->bot || event->channel_id != CHANNEL_ID) {
        return;
    }

    char buf[DISCORD_MAX_MESSAGE_LEN];
    int i = snprintf(buf, DISCORD_MAX_MESSAGE_LEN, "%s: %s\n", event->author->username, event->content);
    write(1, buf, i);
}

void *start_bot_read_thread(void *p_bot) {
    struct discord *client = p_bot;
    char buf[DISCORD_MAX_MESSAGE_LEN];
    pthread_detach(pthread_self());
    log_info("Bot read thread detached.");

    while (1) {
        memset(buf, 0, sizeof(buf));
        read(0, buf, sizeof(buf));
        if (!*buf) {
            continue;
        }
        snprintf(buf, DISCORD_MAX_MESSAGE_LEN, "%s\n", buf);
        struct discord_create_message params = {
                .content = buf
        };
        discord_create_message(client, CHANNEL_ID, &params, NULL);
    }
}

void bot_init(void) {
    log_info("Initializing bot.");
    ccord_global_init();
    bot = discord_init(DISCORD_BOT_TOKEN);
    assert(NULL != bot && "Failed to initialize bot.");
    discord_add_intents(bot, DISCORD_GATEWAY_MESSAGE_CONTENT);
    signal(SIGINT, &on_sigint_signal);
    discord_set_on_ready(bot, &on_ready_event);
    discord_set_on_message_create(bot, &on_message_create_event);
}

void start_bot() {
    pthread_t read_thread_id;
    pthread_create(&read_thread_id, NULL, &start_bot_read_thread, bot);
    log_info("Starting bot.");
    discord_run(bot);
}

void clean_up() {
    discord_cleanup(bot);
    ccord_global_cleanup();
    log_info("Exiting bot.");
}

int main(void) {
    bot_init();
    start_bot();
    clean_up();
    return EXIT_SUCCESS;
}
