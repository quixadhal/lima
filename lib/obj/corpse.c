/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

string the_name;

mixed ob_state() {
    return the_name;
}

void create(string name, string long) {
    ::create();
    if (!name) return;
    
    the_name = name;
    set_id( ({ "corpse", "body" }) );
    if (name[<1] == 's') {
	set_adj( ({ "lifeless", name }) );
	set_proper_name( name + "' corpse" );
    } else {
	set_adj( ({ "lifeless", name + "s" }) );
	set_proper_name( name + "'s corpse" );
    }
    set_long(long || "The lifeless body of " + name + " isn't particularly interesting.");
    set_in_room_desc("The lifeless body of " + name + " lies on the ground.");
    set_plural_in_room_desc("Lying on the ground are %s corpses of " + name + ".");
}

    
