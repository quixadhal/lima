/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * The concept is a bit abstract, so the name isn't entirely correct.  This
 * is an abstraction of the concept of something that one attacks with;
 * either a weapon or one's self.  It controls the combat messages, and
 * modifies the result based on the target, etc.
 *
 * This means that when you fight with a weapon, the _weapon_ is really
 * attacking, not you.  Some of the names in this are a bit backwards compat,
 * though.
 */
private int to_hit_bonus;
private string damage_kind = "blow";
private object wielded_by;

private static mapping def_combat_messages;
private mapping combat_messages = ([]);

mixed adjust_my_result(mixed result)
{
    return result;
}

int query_to_hit_bonus(object target)
{
    return to_hit_bonus;
}

void set_to_hit_bonus(int x)
{
    to_hit_bonus = x;
}

string query_damage_kind()
{
    return damage_kind;
}

void set_damage_kind(string str)
{
    damage_kind = str;
}

int is_weapon()
{
    return 1;
}

void unwield_me()
{
    if ( wielded_by )
	wielded_by->unwield();
}

void remove()
{
    unwield_me();
}

//:FUNCTION set_combat_messages
//Set the set of combat messages which are used by default
void set_combat_messages(string type) {
    if (!(def_combat_messages = MESSAGES_D->get_messages(type)))
	error("No messages of that type.\n");
}

//:FUNCTION query_combat_message
//Returns the combat message for a given type
mixed query_combat_message(string type) {
    return combat_messages[type] || def_combat_messages[type];
}

private mixed merge_lists(mixed list, mixed item, mixed def) {
    if (pointerp(list))
	return list + ({ item });
    else if (list)
	return ({ list, item });
    else if (def)
	return merge_lists(def, item, 0);
    else
	return item;
}

void add_combat_message(string type, string msg) {
    combat_messages[type] = merge_lists(combat_messages[type], msg, 
					def_combat_messages[type]);
}

void mark_wielded_by(object which) {
    wielded_by = which;
}

object query_wielded_by()
{
    return wielded_by;
}

#if COMBAT_STYLE == COMBAT_SIMPLE
void set_weapon_class(int x) {
}
#else
private int weapon_class;

void set_weapon_class(int x) {
    weapon_class = x;
}

int query_weapon_class() {
    return weapon_class;
}
#endif

#ifdef USE_SKILLS
private string skill_used = "combat/unarmed";

string query_skill_used()
{
    return skill_used;
}
void set_skill_used(string new_skill_used)
{
    skill_used = new_skill_used;
}
#endif

mapping lpscript_attributes() {
    return ([
        "weapon_class" : ({ LPSCRIPT_INT, "setup", "set_weapon_class" }),
        "skill_used" : ({ LPSCRIPT_STRING, "setup", "set_skill_used" }),
    ]);
}
