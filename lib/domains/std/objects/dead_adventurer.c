/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CORPSE;

void create()
{
    ::create("adventurer");
}

void setup()
{
    set_proper_name(0);
    set_adj("adventurer's", "brave");
    set_objects(([ 
      "/domains/std/objects/pac_sword" : 1,
      "/domains/std/objects/platemail" : 1,
      "/domains/std/objects/lantern" : 1,
      "/domains/std/objects/backpack" : 1,
    ]));
    set_unique(1);
}
