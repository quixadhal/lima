/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NON_ROOM;
inherit M_MOUNTABLE;  // Okay, you don't mount furniture, but we make it
                      // So you can't mount it.  But we use all the near-
                      // synonyms found in M_MOUNTABLE

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
