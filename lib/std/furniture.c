/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NON_ROOM;
inherit M_MOUNTABLE;  // Okay, you don't mount furniture, but we make it
                      // So you can't mount it.  But we use all the near-
                      // synonyms found in M_MOUNTABLE

private nosave string entrancemsg;
private nosave string exitmsg;
 
void set_furniture_msg(string inmsg, string outmsg)
{
    entrancemsg = inmsg;
    exitmsg = outmsg;
}

mixed default_message(int enter)
{
    if (is_player_inside() && enter)
    {
        return exitmsg;
    }
    else if (!is_player_inside() && enter)
    {
        return entrancemsg;
    }
    return "";
}

mixed default_destination()
{
    if (is_player_inside())
    {
        return file_name(environment(this_object()));
    }
    else
    {
        return file_name(this_object());
    }
}

int can_hold_water()
{
  return 0;
}

int direct_mount_obj()
{
  return 0;
}

int direct_dismount_obj()
{
  return 0;
}

mixed dismount()
{
  return 0;
}

void mudlib_setup()
{
    ::mudlib_setup();
    set_default_destination( (: default_destination :) );
    set_default_enter_msg( (: default_message, 1 :) );
    set_default_exit_msg((: default_message, 0 :));
    add_prep("on", file_name(this_object()));
    add_prep("off", (: environment, this_object() :) );
    set_prep_enter_msg("on",  (: default_message, 1 :) );
    set_prep_exit_msg("on", (: default_message, 0 :) );
    set_prep_enter_msg("off", (: default_message, 1 :) );
    set_prep_exit_msg("off",  (: default_message, 0 :) );
    set_prep_check("on", (: !is_player_inside() ? 1 : "#You can't get on something you're already on." :) );
    set_prep_check("off", (: is_player_inside() ? 1 : "#You can't get off something you aren't on." :) );
}
