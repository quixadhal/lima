/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/living";
inherit "/std/modules/m_actions";

void setup();
array patterns = ({ "%s", "%s smiles.", "%s has left the game.", "%s gets %s.", "%s drops %s.", "%s kicks you.", "%s kisses you.", "%s nods.", "%s laughs.", "%s chuckles.", "%s boggles.", "%s enters.", "%s leaves %s.", "%s says: %syes%s", "%s says: %sno%s", "%s says: %sshut up%s", "%s says: %shello%s", "%s says: %sLPC%s", "%s says: %sstay here%s", "%s says: %snot follow%s", "%s says: %sget lost%s", "%s says: %s."});
array num = ({ 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2});
	
void receive_outside_msg(string str) {
string _1, _2, _3, _4, _5, _6;
if (str[<1] == '\n') str = str[0..<2];
for (int i = 0; i < sizeof(patterns); i++) {
if (sscanf(str, patterns[i], _1, _2, _3, _4, _5, _6) == num[i]) {
switch (i) {
case 0:
{
if (this_body() != this_object()) {
{
continue;;
}
}
;
}
return;
case 1:
{
respond("smile");
;
}
return;
case 2:
{
respond("say Why did " + _1 + " quit the game ?");
;
}
return;
case 3:
{
respond("say Why did " + _1 + " take " + _2 + " ?");
;
}
return;
case 4:
{
respond("say Why did " + _1 + " drop " + _2 + " ?");
;
}
return;
case 5:
{
respond("kick " + _1 + "");
;
}
return;
case 6:
{
respond("blush");
;
}
return;
case 7:
{
respond("nod solemnly");
respond("think did I miss something?");
;
}
return;
case 8:
{
respond("say Did I miss a joke?");
;
}
return;
case 9:
{
respond("smirk");
;
}
return;
case 10:
{
respond("bog");
;
}
return;
case 11:
{
respond("say Hi " + _1 + ", nice to see you !");
;
}
return;
case 12:
{
respond("go " + _2 + "");
;
}
return;
case 13:
{
respond("say I disagree");
;
}
return;
case 14:
{
respond("say I disagree");
;
}
return;
case 15:
{
respond("say Why do you want me to shut up?");
;
}
return;
case 16:
{
respond("say Pleased to meet you!");
;
}
return;
case 17:
{
respond("turing " + _1 + "");
;
}
return;
case 18:
{
respond("say Ok then.");
;
}
return;
case 19:
{
respond("say Ok then.");
;
}
return;
case 20:
{
respond("say Ok then " + _1 + ".");
;
}
return;
case 21:
{
if (random(100) < 20) {
{
respond("say Why do you say '" + _2 + "' ???");
;
}
}
;
}
return;
}}}}
void setup() {
function f;
set_name("Harry");
set_gender(1);
set_proper_name("Harry the affectionate");
set_in_room_desc("Harry the affectionate");
add_id("dude");
set_long("Harry has an agreeable look.");
f = function(function f) { {
switch (random(13)) {
case 0: {
{
respond("say What are you waiting for?");
;
}
break; }
case 1: {
{
respond("say Hello there!");
;
}
break; }
case 2: {
{
respond("say I don't like winter.");
;
}
break; }
case 3: {
{
respond("say I don't like snow.");
;
}
break; }
case 4: {
{
respond("say I don't like rain.");
;
}
break; }
case 5: {
{
respond("say Who are you?");
;
}
break; }
case 6: {
{
respond("say Why do you look like that?");
;
}
break; }
case 7: {
{
respond("say What are you doing here?");
;
}
break; }
case 8: {
{
respond("say Nice weather, isn't it?");
;
}
break; }
case 9: {
{
respond("smile");
;
}
break; }
case 10: {
{
respond("lsfbog");
;
}
break; }
case 11: {
{
respond("ack");
;
}
break; }
case 12: {
{
respond("rock");
;
}
break; }
}
;
}
 call_out(f, 2 + random(8), f); }; call_out(f, 2 + random(8), f);
}

