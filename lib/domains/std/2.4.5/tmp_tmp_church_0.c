#pragma no_warnings
inherit "/std/object";

mixed _last_reset;
void setup();
mixed reset();
mixed long();

void setup() {
function f;
set_flag(0 | ATTACHED);
set_id("clock");
{
_last_reset = time();
;
}
;
}


mixed reset() {
{
_last_reset = time();
;
}
;
}


mixed long() {
{
return "The clock shows " + convert_time(uptime()) + "\n" + "Time since reset is " + convert_time(time() - _last_reset) + "\n";
;
}
;
}

