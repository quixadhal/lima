/* Do not remove the headers from this file! see /USAGE for more info. */

// Based on passwd.c and admtool's nuke.
// --OH.

inherit M_INPUT;
inherit CMD;

private nomask void nuke_player()
{
    object o;
    mixed err;
    string userid = this_user()->query_userid();
 
    if ( o = find_user(userid) )
    {
    o->receive_private_msg("Farewell.\n");
        o->quit();
    }
 
    MAILBOX_D->get_mailbox(userid)->nuke_mailbox(1);
    MAILBOX_D->unload_mailbox(userid);
 
    /* remove a bunch of files. note: some might not exist. */
    err = rm(LINK_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(USER_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(PSHELL_PATH(userid) + __SAVE_EXTENSION__);
    err = rm(WSHELL_PATH(userid) + __SAVE_EXTENSION__);
 
    LAST_LOGIN_D->remove_user(userid, 1);
 
    err = SECURE_D->delete_wizard(userid);
 
    SECURE_D->set_protection(WIZ_DIR "/" + userid, 1, -1);
}


private nomask void confirm_current_password(string s)
{
    write("\n");
 
    if ( !this_user()->matches_password(s) )
    {
        write("Invalid password.\nAborting.\n");
    }
    else
    {
    nuke_player();
    }
}
 
private void main()
{
    modal_simple((: confirm_current_password :),
                 "Enter your current password to confirm suicide: ", 1);
}
