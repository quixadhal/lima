/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/object";

mixed _where;
void setup(int w);
mixed direct_open_obj();
mixed direct_close_obj();
string long() {
    if ("/domains/std/2.4.5/elevator.scr"->query_open_at(_where))
        return "The door is open.\n";
    else
        return "The door is closed.\n";
}

void open_with() {
  this_body()->simple_action("$N $vopen the west door.\n");
  "/domains/std/2.4.5/elevator.scr"->set_room_state("door", "open");
  tell_from_inside("/domains/std/2.4.5/elevator.scr", "The door opens.\n");
}

void close() {
  this_body()->simple_action("$N $vclose the west door.\n");
  "/domains/std/2.4.5/elevator.scr"->clear_room_state("door", "open");
  tell_from_inside("/domains/std/2.4.5/elevator.scr", "The door closes.\n");
}
void setup(int w) {
function f;
set_id("door");
set_adj("elevator");
set_flag(0 | ATTACHED);
{
_where = w
;
}
;
}


mixed direct_open_obj() {
{
if (call_other("/domains/std/2.4.5/elevator.scr", "query_where") != _where)
  return "You can't when the elevator isn't here.\n";
if (call_other("/domains/std/2.4.5/elevator.scr", "query_door_open"))
  return "It is already open!\n";
return 1;;
}
;
}


mixed direct_close_obj() {
{
if (!call_other("/domains/std/2.4.5/elevator.scr", "query_door_open"))
  return "It is already closed!\n";
return 1;;
}
;
}

