/* Do not remove the headers from this file! see /USAGE for more info. */

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

mixed adjust_result(mixed result, string type) {
    if (intp(result) && resist_type == type) {
	result -= random(resist + armor_class);
    } else
	result -= random(armor_class);
    if (result <= 0) return "none";
    return result;
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

