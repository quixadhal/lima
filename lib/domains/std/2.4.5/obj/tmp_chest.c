/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/container";
inherit "/std/modules/m_openable";
inherit "/std/modules/m_valuable";

void setup(int money);

void setup(int money) {
function f;
add_id("chest");
set_long("A chest that seems to be of a high value.");
set_value(200);
{
new("/std/coins", money, "gold")->move(this_object())
;
}
;
}

