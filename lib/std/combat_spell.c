/* Do not remove the headers from this file! see /USAGE for more info. */

// combat_spell.c - Base object for damage-causing magic spells. Whenever
//                  your spells cause the damage, call do_spell_damage().
//                  Combat will automatically start between the caster and
//                  the target.
// Iizuka, 4-28-98: Created.

inherit M_DAMAGE_SOURCE;

void do_spell_damage(object target, function damage_func)
{
   this_body()->start_fight(target);
   this_body()->add_event(target, this_object(), 0,
                         evaluate(damage_func) );
}

void setup()
{
   set_damage_type("magic");
#ifdef USE_SKILLS
   set_skill_used("spell");
#endif
}
