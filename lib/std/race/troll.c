/* Do not remove the headers from this file! see /USAGE for more info. */

inherit RACE;

/*
 * This is the name of the race.  It is used in the initial selection screen,
 * and also in who info etc.
 */
string query_race() {
    return "troll";
}

/*
 * This is the description shown when the user types 'help race' during the
 * selection process
 */
string short_description() {
    return "Trolls get a bonus to constitution.  They have above average strength, below average agility and intelligence, and above average willpower.  Trolls fight well unarmed, and are resistent to damage due to their thick skin.  However, they are highly susceptible to fire.\n";
}

/*
 * These are the bonuses for the derived statistics.  If this is zero, the
 * Normal derivation is used, however, if a bonus exists, then the stat
 * is scaled into that range.
 *
 * For example, consider a race with a racial con bonus of 50.
 * 
 * The derived constitution is a weighted average of 3 base stats.  This
 * gives a number in the range [0..100].  The racial bonus says that that
 * number should be adjusted into the range [50..100].  This has three effects:
 *
 * (1) All members of the race will have at least 50 constitution.
 * (2) 100 is the max for all races.
 * (3) Races that start out with a bonus advance slower (since the range is
 *     compressed).  For example, if your base stats go up by 10, then your
 *     con would only increase by 5.
 */
int racial_con_bonus() {
    return 10;
}

int racial_wis_bonus() {
    return 0;
}

int racial_cha_bonus() {
    return 0; // Documentation is a beautiful thing :-)
}

class stat_roll_mods query_roll_mods() {
    class stat_roll_mods ret = new(class stat_roll_mods);
    
/*
 * These give the adjustments and ranges for base statistics.
 * 
 * The actual score is in the range:
 *
 * BASE_VALUE + adjust +- range/2
 *
 * where BASE_VALUE is in /std/modules/bodystat.c (currently 20)
 *
 * Here are some examples:
 *
 * adjust        range         possible values
 * -10		  20		 0 .. 20
 * -15		   5		 3 .. 8
 *  25		  30		30 .. 60
 *   5            10		20 .. 30
 *   0		  40		 0 .. 40
 */
    ret->str_adjust = 10;
    ret->str_range = 20;
    
    ret->agi_adjust = -5;
    ret->agi_range = 10;
    
    ret->int_adjust = -10;
    ret->int_range = 10;
    
    ret->wil_adjust = 5;
    ret->wil_range = 10;

    return ret;
}

/*
 * Note that this object is used as the player's body object, so you can
 * overload any player functions below.
 */
void create(string userid) {
    ::create(userid);
    set_wield_bonus(0);
#if COMBAT_STYLE == COMBAT_TRADITIONAL
    set_weapon_class(10);
#endif
}

mixed adjust_result(mixed res, string type) {
     if (intp(res)) {
	 if (type == "fire") return res * 2;
	 return res - 1;
     }
     return res;
}
