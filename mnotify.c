#include <string.h>
#include <libnotify/notify.h>

#include <mcabber/logprint.h>
#include <mcabber/commands.h>
#include <mcabber/compl.h>
#include <mcabber/hooks.h>
#include <mcabber/screen.h>
#include <mcabber/settings.h>
#include <mcabber/modules.h>
#include <mcabber/config.h>
#include <mcabber/roster.h>


static void mnotify_init   (void);
static void mnotify_uninit (void);

/* Module description */
module_info_t info_mnotify = {
        .branch          = MCABBER_BRANCH,
        .api             = MCABBER_API_VERSION,
        .version         = MCABBER_VERSION,
        .description     = "Simple notify module\n"
                           " Send messages like notify-send ",
        .requires        = NULL,
        .init            = mnotify_init,
        .uninit          = mnotify_uninit,
        .next            = NULL,
};

guint mnotify_hid = 0;

static void notify_send(const char * bjid, const char * message) {
	NotifyNotification *n;

	notify_init("mcabber");
	n = notify_notification_new (bjid,message, NULL);
	notify_notification_set_timeout(n, settings_opt_get_int("mnotify_timeout")); //3 seconds
	if (!notify_notification_show (n, NULL)) {
		scr_log_print(LPRINT_NORMAL, "ERROR* Notify not send!");
	}
	g_object_unref(G_OBJECT(n));
}

//static hk_handler_t notify_handler(const char *bjid, const char *resname,
//                   time_t timestamp, const char *msg, LmMessageSubType type,
//                   guint encrypted) {

static guint notify_handler(const gchar * hook, hk_arg_t * args, void * usedata) {
	int i, group_chat = 0;
	const char * bjid = NULL, * msg = NULL, * resource = NULL;

	char * buf;

	if (settings_opt_get_int("mnotify_enable")) {
		for (i = 0; args[i].name; i++) {
			buf = g_strdup_printf("%s %s =  \"%s\"", buf, args[i].name, args[i].value);
			if (!strcmp(args[i].name, "jid")) {
				bjid = roster_getname(args[i].value);
				if (!bjid) {
					bjid = args[i].value;
				}
			}
			if (!strcmp(args[i].name, "message"))
				msg = args[i].value;
			if (!strcmp(args[i].name, "resource"))
				resource = args[i].value;
			if (!strcmp(args[i].name, "groupchat"))
				group_chat = !strcmp(args[i].value, "true");
		}

		if (group_chat) {
			bjid = g_strdup_printf("%s/%s", bjid, resource);
		}

		scr_log_print (LPRINT_DEBUG, "Notify: %s", buf);

		notify_send(bjid, msg);
	}

	return HOOK_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
}


static void mnotify_init(void) {
	mnotify_hid = hk_add_handler(notify_handler, HOOK_PRE_MESSAGE_IN, G_PRIORITY_DEFAULT_IDLE, NULL);

	if (!settings_opt_get_int("mnotify_timeout")) {
		settings_set(SETTINGS_TYPE_OPTION, "mnotify_timeout", "1000");
	}
}

static void mnotify_uninit(void) {
	hk_del_handler(HOOK_PRE_MESSAGE_IN, mnotify_hid);
}
