/* Do not remove the headers from this file! see /USAGE for more info. */

// This is a simple skills module, demonstrating one way to integrate the
// Lima skills system into the combat system
// It is designed to return a number in the range 0 to 100 for chance to hit,
// based on skills in the range 0 to MAX_SKILL_VALUE

#include <combat_modules.h>
#include <skills.h>

int query_weapon_class();
int aggregate_skill(string skill);

int chance_to_hit(object weapon, object target)
{
  string attack_skill = weapon->query_skill_used();
  string defend_skill = target->query_defend_skill_used();
  int attack_value = aggregate_skill(attack_skill);
  int defend_value = target->aggregate_skill(defend_skill);
  int res;

// Skill tests to potentially increase the skills
  this_body()->test_skill(attack_skill, defend_value);
  target->test_skill(defend_skill, attack_value);

// Calculate the value
  res = attack_value-defend_value+MAX_SKILL_VALUE;
  res = res*100/(2*MAX_SKILL_VALUE);
LBUG( res );
  return res;
}

int disarm_chance(object target)
{
  return chance_to_hit(this_body()->query_weapon(), target);
}

int calculate_damage(object weapon, object target)
{
  return random(weapon->query_weapon_class()) + 1;
}
