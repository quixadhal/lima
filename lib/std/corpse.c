/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_DECAY;
inherit M_MESSAGES;

string the_name;
object link;

int is_living() {
    return 0;
}

string in_room_desc() {
    return query_in_room_desc();
}

mixed ob_state()
{
    return the_name;
}

object query_link() {
    return link;
}

void mudlib_setup(string name, string long, object l)
{
    ::mudlib_setup();

    link = l;
    if ( !name )
	return;

    the_name = name;
    set_long(long || "The lifeless body of " + capitalize(the_name) + " isn't particularly interesting.");

    set_id("corpse", "body");
    if (the_name[<1] == 's') {
	set_adj("lifeless", the_name);
	set_proper_name(capitalize(the_name) + "' body");
    } else {
	set_adj("lifeless", the_name + "s");
	set_proper_name(capitalize(the_name) + "'s body");
    }
    set_in_room_desc(capitalize(the_name) + " is lying here slumped on the ground.");
    set_plural_in_room_desc("Lying on the ground are %s bodies of " + the_name + ".");
    add_relation("on");
    set_default_relation("on");
    set_max_capacity(VERY_LARGE);
#ifdef USE_SIZE
    set_size(VERY_LARGE);
#endif
#ifdef USE_MASS
    set_mass(VERY_LARGE);
#endif
    set_num_decays(3);
    set_decay_action("Insects gnaw at the withering corpse.\n");
    set_last_decay_action("A corpse $vdecay into dust and ash.\n");
    set_decay_time(20);
    set_decay_auto_remove(1);
}

string introduce_contents()
{
    return "On " + the_short() + " you see:\n";
}    

mixed indirect_get_obj_from_obj(object ob1, object ob2)
{
  return 1;
}
