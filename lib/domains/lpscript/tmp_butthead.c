/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/living";
inherit "/std/modules/m_actions";

mixed _norecurse;
void setup();
mixed start();
mixed insulted();
array patterns = ({ "%slaughs."});
array num = ({ 1});
	
void receive_outside_msg(string str) {
string _1, _2, _3, _4, _5, _6;
if (str[<1] == '\n') str = str[0..<2];
for (int i = 0; i < sizeof(patterns); i++) {
if (sscanf(str, patterns[i], _1, _2, _3, _4, _5, _6) == num[i]) {
switch (i) {
case 0:
{
if (!_norecurse) {
{
_norecurse=1;
start();
;
}
}
;
}
return;
}}}}
void setup() {
function f;
set_name("Butthead");
set_gender(1);
set_proper_name("Butthead");
set_in_room_desc("Butthead is here, scoping for chicks.");
set_long("His body odor prevents you from getting to close.");
}


mixed start() {
{
mixed _beavis;
_beavis=present("beavis", environment(this_body()));
if (!_beavis) {
{
_beavis=new("/domains/lpscript/beavis.scr");
call_other(_beavis, "move",environment(this_body()));
;
}
}
call_out("unnamed0", 2, _beavis); }
}

void unnamed0(mixed _beavis) {
{
;
}
{
respond("say Beavis knows how to play an instrument.");
respond("say he plays the skin flute.");
respond("huhuh");
call_other(_beavis, "insulted");
;
}
;
}


mixed insulted() {
{
call_out("unnamed1", 2); }
}

void unnamed1() {
{
;
}
{
respond("kick beavis in the nads");
_norecurse=0;
;
}
;
}

