/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;


void do_knock_on_obj( object obj )
{
    obj->do_knock();
}

void do_knock_at_obj(object obj)
{
    obj->do_knock();
}

void create()
{
  add_rules( ({ "on OBJ", "at OBJ" }), ({ "rap" }) );
}

