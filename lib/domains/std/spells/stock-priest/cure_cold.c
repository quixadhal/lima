/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/domains/std/spells/stock-priest/std_priest_spell";

void setup ()
{
  set_spell_name("cure cold");
  set_cast_time(5);
  set_difficulty(10);
  set_skill_used("spell/disease");
}

mixed valid_target(object ob)
{
  if ( !ob )
    return "You need to specify what you want to cast cure cold on.\n";

  return 1;
}

void do_effects(object target, object reagent)
{
  this_body()->targetted_action(
    "$N hands start to glow, as $p $vlay $p hands on $T."
    );
  if(target->remove_effect_named("disease.cold"))
    this_body()->targetted_action("$P spell cures $P1 cold.");
  else
    this_body()->simple_action("$P spell has no effect.");
}

void do_fail(object target, object reagent)
{
  this_body()->simple_action(
    "The Gods do not favour $p casting of this spell, "
    "and it has no effect."
    );
}
