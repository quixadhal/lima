/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;

object sword;
object king;

private void find_a_king()
{
    // Randomly pick someone logged on.  If king == 0, then either we
    // haven't done this yet, or the person we picked logged out.
    if (king) return;
    king = choice(bodies());
}

void setup() {
    set_id("stone", "rock");
    set_long("It is about 3 feet round, but otherwise fairly nondescript.");
    set_in_room_desc("A large stone lies off to the side of the room.");
    // Not using set_objects() here since we don't want to reset
    add_relation("in", MEDIUM);
    set_default_relation("in");
    sword = give_new_obj( this_object(), "sword_in_stone", 0 );
    set_unique(1);
}

mixed receive_object( object target, string relation ) {
    if (!target->id("sword")) return 0;
    set_hide_contents(0, "in");
    return ::receive_object(target, relation);
}

mixed release_object( object target, int force ) {
    find_a_king();

    if (!force && this_body() != king)
    {
	this_body()->other_action( "$N $vtug on the sword, but can't seem to budge it." );
	return "You can't seem to budge it.\n";
    }
    this_object()->simple_action( "$N $vpull on the sword, and $vfind it comes out easily in $p hand." );
    set_hide_contents(1);
    return ::release_object(target, force);
}

string inventory_header() {
    return "Deeply embedded in the stone is:\n";
}

string introduce_contents() {
    return "Deeply embedded in the stone is:\n";
}
