/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed can_cast_str(string spell)
{
    return SPELL_D->can_cast_spell(spell);
}

mixed can_cast_str_at_obj(string spell, object target)
{
    return SPELL_D->can_cast_spell(spell, target);
}

mixed can_cast_str_on_obj(string spell, object target)
{
    return SPELL_D->can_cast_spell(spell, target);
}

mixed direct_cast_str_on_obj()
{
  return 1;
}

mixed can_cast_str_with_obj(string spell, object tool)
{
    return SPELL_D->can_cast_spell(spell, 0, tool);
}

mixed direct_cast_str_with_obj()
{
  return 1;
}

mixed can_cast_str_at_obj_with_obj(string spell, object target, object tool)
{
    return SPELL_D->can_cast_spell(spell, target, tool);
}

mixed direct_cast_str_at_obj_with_obj()
{
  return 1;
}

mixed indirect_cast_str_at_obj_with_obj()
{
  return 1;
}

mixed can_cast_str_on_obj_with_obj(string spell, object target, object tool)
{
    return SPELL_D->can_cast_spell(spell, target, tool);
}

mixed direct_cast_str_on_obj_with_obj()
{
  return 1;
}

mixed indirect_cast_str_on_obj_with_obj()
{
  return 1;
}

mixed can_cast_str_with_obj_at_obj(string spell, object tool, object target)
{
    return SPELL_D->can_cast_spell(spell, target, tool);
}

mixed direct_cast_str_with_obj_at_obj()
{
  return 1;
}

mixed indirect_cast_str_with_obj_at_obj()
{
  return 1;
}

mixed can_cast_str_with_obj_on_obj(string spell, object tool, object target)
{
    return SPELL_D->can_cast_spell(spell, target, tool);
}

mixed direct_cast_str_with_obj_on_obj()
{
  return 1;
}

mixed indirect_cast_str_with_obj_on_obj()
{
  return 1;
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


mixed * query_verb_info()
{
    return ({ ({ "STR on OBJ", "STR", "STR at OBJ", "STR with OBJ", 
		     "STR at OBJ with OBJ", "STR on OBJ with OBJ", 
		     "STR with OBJ on OBJ", "STR with OBJ at OBJ"}), ({}) });
}
