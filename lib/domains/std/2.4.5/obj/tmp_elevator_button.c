/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/object";

mixed _dest;
void setup(string d);
mixed press();
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


mixed press() {
{
call_other(environment(this_object()), "handle_press",_dest);
return 1;;
}
;
}

