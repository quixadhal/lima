/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * ADVERSARY: Beek, April 7, 1997.  Loosely based on the old monster.c
 *   Lots of work done by Iizuka to get this working.
 */


inherit LIVING;
inherit M_DAMAGE_SOURCE;

#define SUBDIR __DIR__ "adversary/"

inherit SUBDIR "main";
inherit SUBDIR "behaviors";
inherit SUBDIR "state_of_mind";
inherit SUBDIR "target";
inherit SUBDIR "messages";
inherit SUBDIR "mod_config";
inherit SUBDIR "armor/armor_base";
inherit SUBDIR "wield/wield_base";

inherit MODULE("death", DEATH_MODULE);
inherit MODULE("health", HEALTH_MODULE);
inherit MODULE("wield", WIELD_MODULE);
inherit MODULE("armor", ARMOR_MODULE);
inherit MODULE("pulse", PULSE_MODULE);
inherit MODULE("blows", BLOW_MODULE);
inherit MODULE("formula", FORMULA_MODULE);
inherit MODULE("advancement", ADVANCEMENT_MODULE);

void mudlib_setup()
{
   living::mudlib_setup();
   set_to_hit_bonus(-25);  // -25% to hit bare hand.
   set_weapon_class(3);    // and low WC
   set_combat_messages("combat-unarmed");
   set_death_message(query_default_death_message());

   /* by pinging query_weapon(), we will default to self as a weapon */
   query_weapon();
}

void remove()
{
   living::remove();
}

//:FUNCTION start_fight
// Add someone to the list of people we are attacking.  If we were already
// attacking them, make them the primary person we are attacking.  Then
// take a swing at them.
int start_fight(object who)
{
   if(!(who->attackable()))
      return 0;
   attacked_by(who, 1);
   return 1;
}

//:FUNCTION attackable
// return 1 if we can be attacked.
int attackable()
{
   return 1;
}

void create(mixed array args...)
{
   ::create(args...);

   // Please read the headers in /std/adversary/mod_config.c before
   // removing this line.
   check_combat_config();
}

mixed direct_diagnose_liv(object ob)
{
   return 1;
}

mixed direct_kill_liv(object ob)
{
   if(query_ghost())
      return "Someone beat you to it; " + the_short() + " is already dead.\n";
   return 1;
}

mapping lpscript_attributes()
{
   return living::lpscript_attributes() +
          m_damage_source::lpscript_attributes() +
          ([
             "max_health" : ({ LPSCRIPT_INT, "setup", "set_max_health" }),
          ]);
}

//:FUNCTION query_ghost
// Returns 1 if the adversary is dead.
int query_ghost()
{
   return HEALTH_MODULE::query_ghost();
}

int event_damage(class event_info evt)
{
   return BLOW_MODULE::event_damage(evt);
}

//:FUNCTION diagnose
// Returns a string describing the current state of the adversary.
string diagnose()
{
   return HEALTH_MODULE::diagnose();
}
