/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/monster";

void setup();
array patterns = ({ "%s gives %s", "%s gives you %s coins."});
array num = ({ 2, 2});
	
void receive_outside_msg(string str) {
string _1, _2, _3, _4, _5, _6;
if (str[<1] == '\n') str = str[0..<2];
for (int i = 0; i < sizeof(patterns); i++) {
if (sscanf(str, patterns[i], _1, _2, _3, _4, _5, _6) == num[i]) {
switch (i) {
case 0:
{
this_body()->do_game_command("say Thank you.");
continue;;
}
return;
case 1:
{
if (query_money() > 12) {
{
this_body()->do_game_command("east");
this_body()->do_game_command("buy beer");
this_body()->do_game_command("drink beer");
this_body()->do_game_command("west");
;
}
}
;
}
return;
}}}}
void setup() {
function f;
set_proper_name("beggar");
set_in_room_desc("A beggar");
add_id("beggar");
set_long("A really filthy looking poor beggar.");
set_max_hp(30);
}

