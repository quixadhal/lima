/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust wrote this and was a bad boy and didn't attach a header - Beek

#include <mudlib.h>

inherit CMD;
inherit M_INPUT;

private nomask void confirm_new_password(string s1, string s2)
{
    write("\n");

    if ( s1 != s2 )
    {
	write("Passwords must match.\n");
    }
    else
    {
	this_user()->set_password(s1);
	write("Password changed.\n");
    }
}

private nomask void get_new_password(string s)
{
    write("\n");

    if ( strlen(s) < 5 )
    {
	write("Your password must be 5 characters or more.\nAborting.\n");
    }
    else
    {
	modal_simple((: confirm_new_password, s :), "Again to confirm: ", 1);
    }
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
	modal_simple((: get_new_password :), "New password: ", 1);
    }
}

private void main()
{
    modal_simple((: confirm_current_password :),
		 "Enter your current password: ", 1);
}
