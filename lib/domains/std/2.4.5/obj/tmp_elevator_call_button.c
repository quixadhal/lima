/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/object";

mixed _where;
void setup(string w);
mixed press();

void setup(string w) {
function f;
set_flag(0 | ATTACHED);
set_adj("elevator");
set_id("button");
{
_where = w
;
}
;
}


mixed press() {
{
call_other("/domains/std/2.4.5/elevator.scr", "call_elevator",_where);
call_other(environment(this_object()), "set_room_state","lamp");
return 1;;
}
;
}

