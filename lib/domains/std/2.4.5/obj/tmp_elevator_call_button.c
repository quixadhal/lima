#pragma no_warnings
inherit "/std/object";

mixed _where;
void setup(string w);
mixed direct_press_obj();
void do_press() {
    int state = 0;

    this_body()->simple_action("$N $vpress the $o.", this_object());
    state = call_other("/domains/std/2.4.5/elevator", "call_elevator", _where);

    if(state) {
        tell_from_inside(environment(this_object()), "The lamp on the elevator button lights up.\n");
        environment(this_object())->set_room_state("lamp");
    }
}
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


mixed direct_press_obj() {
{
return 1;;
}
;
}

