/* Do not remove the headers from this file! see /USAGE for more info. */

private string resist_type;
private int resist;

#if COMBAT_STYLE == COMBAT_SIMPLE
void set_armor_class(int x) {
}
#define armor_class 0
#else
private int armor_class;

void set_armor_class(int x) {
    armor_class = x;
}

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

void set_resist(string type, int amt) {
    resist = amt;
    resist_type = type;
}

void is_armor()
{
    return 1;
}

