/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat.h>

//### um ....
inherit CLASS_COMBAT_RESULT;

private string resist_type;
private int resist;

#if COMBAT_STYLE == COMBAT_SIMPLE
//:FUNCTION set_armor_class
//Set the protection of the particular damage sink.  random(class) points
//of damage will be prevented.
void set_armor_class(int x) {
}
#define armor_class 0
#else
private int armor_class;

void set_armor_class(int x) {
    armor_class = x;
}

//:FUNCTION query_armor_class
//Query the current armor class of an object.  Higher is better.
int query_armor_class() {
    return armor_class;
}
#endif

class combat_result array
adjust_result(class combat_result array res) {
    foreach (class combat_result result in res) {
	int ac = (result->special & RES_NONPHYSICAL) ? 0 : armor_class;
	if (resist_type == result->kind)
	    result->damage -= random(resist + ac);
	else
	    result->damage -= random(ac);
	if (result->damage < 0)
	    result->damage = 0;
    }
    return res;
}

//:FUNCTION set_resist
//set_resist(type, amt) specifies that for damage of type 'type', the armor
//class should be increased by 'amt'
void set_resist(string type, int amt) {
    resist = amt;
    resist_type = type;
}

//:FUNCTION is_armor
//Returns 1 if the object is a damage_sink.
int is_armor()
{
    return 1;
}

