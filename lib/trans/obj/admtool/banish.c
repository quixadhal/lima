/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** news.c -- News administration
** Barnes: Urm, no. It looks more like banish.c - banish administration.
**
** 951124, Rust: created
** 960903, Barnes@Xyzzy: Rewritten with better structure.
** 		 Banish reasons can now be more than one word.
** 		 Nuke & Pave option: nukes a user and banishes their name.
*/

#include <mudlib.h>
#include <security.h>

void std_handler(string str);
void do_one_arg(string arg_prompt, function fp, string arg);
void do_two_args(string arg1_prompt, string arg2_prompt,
                 function fp, string arg);
varargs void modal_simple(function input_func, int secure);
varargs void modal_func(function input_func, mixed prompt_func, int secure);
void receive_banish_input(string);

#define PROMPT_BANISH     "(AdmTool:banish)   [LlBbUunmq?] > "

private nomask void write_banish_menu()
{
    write("Administration Tool: banish administration\n"
	  "\n"
	  "    L                   - list banished names\n"
	  "    l                   - list banished sites\n"
	  "    B <name> <reason>   - banish a name\n"
	  "    b <name> <reason>   - banish a site\n"
	  "    U <name>            - unbanish a name\n"
	  "    u <name>            - unbanish a site\n"
	  "    n <name>            - Nuke and Pave a user\n"
	  
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void receive_banish_name(string name, string reason)
{
        BANISH_D->banish_name(name, reason ? reason : "(None given)");
        write("Done.\n");
}


private nomask void receive_banish_site(string name, string reason)
{
        BANISH_D->banish_site(name, reason ? reason : "(None given)");
        write("Done.\n");
}

private nomask void pave_user(string userid, int skip_save)
{
    object o;
    mixed err;

    if ( o = find_user(userid) )
    {
	o->receive_private_msg("Sorry.  You're being nuked.\n");
	o->quit();
    }

    MAILBOX_D->get_mailbox(userid)->nuke_mailbox(1);
    MAILBOX_D->unload_mailbox(userid);

    /* remove a bunch of files. note: some might not exist. */
    err = rm(LINK_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(USER_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(PSHELL_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(WSHELL_PATH(userid) + __SAVE_EXTENSION__);

    LAST_LOGIN_D->remove_user(userid, skip_save);

    err = SECURE_D->delete_wizard(userid);

//### deal with clearing privs and stuff
//### this should be enough, but may need more thought (this was a quicky)
//### need to set it to something like @disabled so that unguarded() code
//### in the wiz dir doesn't have priv 1 now.
    SECURE_D->set_protection(WIZ_DIR "/" + userid, 1, -1);
    
    printf("'%s' has been nuked.\n", capitalize(userid));
    BANISH_D->banish_name(userid, "Nuked and paved over.");
}


private nomask void confirm_paving(string name, string str)
{
    str = lower_case(str);
    if ( str != "y" && str != "yes" )
    {
	write("Nuke aborted!\n");
	return;
    }

    pave_user(name, 0);
}

private nomask void receive_name_for_paving(string name)
{
    name = lower_case(name);
    printf("Are you sure you want to nuke '%s' ? ", capitalize(name));
    modal_simple((: confirm_paving, name :));
}


private nomask void receive_banish_input(mixed str)
{
    string arg;

    sscanf(str, "%s %s", str, arg);

    switch ( str )
    {

    case "L":
        more(implode(BANISH_D->show_banishes()[0],", "));
        break;

    case "l":
        more(implode(BANISH_D->show_banishes()[1],", "));  
        break;

    case "B":
        do_two_args("Banish who? ",
                    "Banish them why? ",
                    (: receive_banish_name :),
                    arg);
        break;

    case "b":
        do_two_args("Banish which site? ",
                    "Banish them why? ",
                    (: receive_banish_site :),
                    arg);
        break;

    case "U":
        BANISH_D->unbanish_name(arg);
        write("Done.\n");
        break;

    case "u":
        BANISH_D->unbanish_site(arg);
        write("Done.\n");
        break;
        
    case "n":
		do_one_arg("Who should be nuked and paved? ",
		   (: receive_name_for_paving :),
		   arg);
	break;


    case "?":
        write_banish_menu();
        break;

    default:
        std_handler(str);
        break;
    }
}

static nomask void begin_banish_menu()
{
    if ( !check_privilege(1) )
    {
	write("Sorry... admin only.\n");
	return;
    }
    modal_func((: receive_banish_input :), PROMPT_BANISH);
    write_banish_menu();
}

