/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

#define NO_TARGET	0
#define HAS_TARGET	1

#define NO_REAGENT	0
#define HAS_REAGENT	1

mixed can_cast_str(string spell)
{
    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;
    return SPELL_D->check_valid_spell(spell, NO_TARGET, NO_REAGENT);
}

mixed can_cast_str_at_obj(string spell, object target)
{
    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;
    return SPELL_D->check_valid_spell(spell, HAS_TARGET, NO_REAGENT);
}

mixed direct_cast_str_at_obj(string spell, object target)
{
    return SPELL_D->check_valid_target(spell, target, NO_REAGENT);
}

mixed can_cast_str_on_obj(string spell, object target)
{
    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;
    return SPELL_D->check_valid_spell(spell, HAS_TARGET, NO_REAGENT);
}

mixed direct_cast_str_on_obj(string spell, object target)
{
    return SPELL_D->check_valid_target(spell, target, NO_REAGENT);
}

mixed can_cast_str_with_obj(string spell, object reagent)
{

    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;

    return SPELL_D->check_valid_spell(spell, NO_TARGET, HAS_REAGENT);
}

mixed direct_cast_str_with_obj(string spell, object reagent)
{
    return SPELL_D->check_valid_reagent(spell, reagent);
}

mixed can_cast_str_at_obj_with_obj(string spell, object target, object reagent)
{

    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;

    return SPELL_D->check_valid_spell(spell, HAS_TARGET, HAS_REAGENT);
}

mixed direct_cast_str_at_obj_with_obj(string spell, object target, object reagent)
{
    return SPELL_D->check_valid_target(spell, target, HAS_REAGENT);
}

mixed indirect_cast_str_at_obj_with_obj(string spell, object target, object reagent)
{
    return SPELL_D->check_valid_reagent(spell, reagent, target);
}

mixed can_cast_str_on_obj_with_obj(string spell, object target, object reagent)
{

    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;

    return SPELL_D->check_valid_spell(spell, HAS_TARGET, HAS_REAGENT);
}

mixed direct_cast_str_on_obj_with_obj(string spell, object target, object reagent)
{
    return SPELL_D->check_valid_target(spell, target, HAS_REAGENT);
}

mixed indirect_cast_str_on_obj_with_obj(string spell, object target, object reagent)
{
    return SPELL_D->check_valid_reagent(spell, reagent, target);
}

mixed can_cast_str_with_obj_at_obj(string spell, object reagent, object target)
{
    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;

    return SPELL_D->check_valid_spell(spell, HAS_TARGET, HAS_REAGENT);
}

mixed direct_cast_str_with_obj_at_obj(string spell, object reagent, object target)
{
    return SPELL_D->check_valid_reagent(spell, reagent, HAS_TARGET);
}

mixed indirect_cast_str_with_obj_at_obj(string spell, object reagent, object target)
{
    return SPELL_D->check_valid_target(spell, target, reagent);
}

mixed can_cast_str_with_obj_on_obj(string spell, object reagent, object target)
{
    mixed ret = check_ghost();

    if(stringp(ret))
	return ret;
    return SPELL_D->check_valid_spell(spell, HAS_TARGET, HAS_REAGENT);
}

mixed direct_cast_str_with_obj_on_obj(string spell, object reagent, object target)
{
    return SPELL_D->check_valid_reagent(spell, reagent, HAS_TARGET);
}

mixed indirect_cast_str_with_obj_on_obj(string spell, object reagent, object target)
{
    return SPELL_D->check_valid_target(spell, target, reagent);
}


void do_cast_str(string spell)
{
    SPELL_D->cast_spell(spell);
}

void do_cast_str_on_obj(string spell, object target)
{
    SPELL_D->cast_spell(spell, target);
}

void do_cast_str_at_obj(string spell, object target)
{
    SPELL_D->cast_spell(spell, target);
}

void do_cast_str_with_obj(string spell, object tool)
{
    SPELL_D->cast_spell(spell, 0, tool);
}

void do_cast_str_on_obj_with_obj(string spell, object target, object tool)
{
    SPELL_D->cast_spell(spell, target, tool);
}

void do_cast_str_with_obj_on_obj(string spell, object tool, object target)
{
    SPELL_D->cast_spell(spell, target, tool);
}

void do_cast_str_with_obj_at_obj(string spell, object tool, object target)
{
    SPELL_D->cast_spell(spell, target, tool);
}


array query_verb_info()
{
    return ({ ({ "STR on OBJ", "STR", "STR at OBJ", "STR with OBJ", 
	"STR at OBJ with OBJ", "STR on OBJ with OBJ", 
	"STR with OBJ on OBJ", "STR with OBJ at OBJ"}), ({}) });
}
