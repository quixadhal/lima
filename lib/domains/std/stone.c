/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;

object sword;
object king;

private void
find_a_king() {
    array bods;
    
    // Randomly pick someone logged on.  If king == 0, then either we
    // haven't done this yet, or the person we picked logged out.
    if (king) return;
    king = choice(bodies());
}

void setup() {
    set_id("stone", "rock");
    set_long("It is about 3 feet round, but otherwise fairly nondescript.\n");
    set_in_room_desc("A large stone lies off to the side of the room.");
    // Not using set_objects() here since we don't want to reset
    set_max_capacity(MEDIUM);
    sword = new(__DIR__ "sword_in_stone");
    sword->move(this_object());
}

mixed receive_object( object target, string relation ) {
    if (target != sword) return 0;
    return ::receive_object(target, relation);
}

mixed release_object( object target, int force ) {
    find_a_king();
    
    if (!force && this_body() != king)
	return "You can't seem to budge it.\n";
    write("The sword slides easily out of the stone.\n");
    return ::release_object(target, force);
}

string inventory_header() {
    return "Deeply embedded in the stone is:\n";
}

string introduce_contents() {
    return "Deeply embedded in the stone is:\n";
}
