/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/living";
inherit "/std/modules/m_actions";

void setup();
mixed greet();
mixed nocastle();
array patterns = ({ "%shello%s", "%shi%s", "%sHi%s", "%sgive%scastle%s", "%swant%scastle%s"});
array num = ({ 2, 2, 2, 3, 3});
	
void receive_outside_msg(string str) {
string _1, _2, _3, _4, _5, _6;
if (str[<1] == '\n') str = str[0..<2];
for (int i = 0; i < sizeof(patterns); i++) {
if (sscanf(str, patterns[i], _1, _2, _3, _4, _5, _6) == num[i]) {
switch (i) {
case 0:
{
greet();
;
}
return;
case 1:
{
greet();
;
}
return;
case 2:
{
greet();
;
}
return;
case 3:
{
nocastle();
;
}
return;
case 4:
{
nocastle();
;
}
return;
}}}}
void setup() {
function f;
set_proper_name("Leo the Archwizard");
set_in_room_desc("Leo the Archwizard");
set_long("Leo the Archwizard:");
add_id("leo", "archwizard");
f = function(function f) { {
respond("smile");
;
}
 call_out(f, 5 + random(95), f); }; call_out(f, 5 + random(95), f);
}


mixed greet() {
{
mixed _who;
_who=call_other(this_body(), "query_name");
respond("say Welcome, " + _who + ".");
;
}
;
}


mixed nocastle() {
{
respond("say I no longer give out castles.");
;
}
;
}

