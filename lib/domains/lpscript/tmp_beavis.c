/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/living";
inherit "/std/modules/m_actions";

void setup();
mixed insulted();
mixed introduce();
array patterns = ({ "%s kicks you in the nads."});
array num = ({ 1});
	
void receive_outside_msg(string str) {
string _1, _2, _3, _4, _5, _6;
if (str[<1] == '\n') str = str[0..<2];
for (int i = 0; i < sizeof(patterns); i++) {
if (sscanf(str, patterns[i], _1, _2, _3, _4, _5, _6) == num[i]) {
switch (i) {
case 0:
{
respond("scream in pain!");
;
}
return;
}}}}
void setup() {
function f;
set_name("Beavis");
set_gender(1);
set_proper_name("Beavis");
set_in_room_desc("Beavis mills about, periodically scratching his nads.");
set_long("His body odor prevents you from getting too close.");
}


mixed insulted() {
{
call_out("unnamed0", 2); }
}

void unnamed0() {
{
;
}
{
mixed _butthead;
respond("slut butthead");
_butthead=present("butthead", environment(this_body()));
call_other(_butthead, "insulted");
;
}
;
}


mixed introduce() {
{
simple_action("$N $vstroll out of the shadows, laughing about $p nads.");
;
}
;
}

