#pragma no_warnings
inherit "/std/object";

mixed _dest;
void setup(string d);
mixed do_press();
mixed direct_press_obj();
string long() {
  return "The button is marked '" + _dest + "'\n";
}
void setup(string d) {
function f;
set_flag(0 | ATTACHED);
set_adj("elevator");
set_id("button");
{
_dest = d;
add_adj(d);
;
}
;
}


mixed do_press() {
{
call_other(environment(this_object()), "handle_press",_dest);
return 1;;
}
;
}


mixed direct_press_obj() {
{
return 1;;
}
;
}

