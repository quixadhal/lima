/* Do not remove the headers from this file! see /USAGE for more info. */

inherit RACE;

/*
 * This is the name of the race.  It is used in the initial selection screen,
 * and also in who info etc.
 */
string query_race() {
    return "human";
}

/*
 * This is the description shown when the user types 'help race' during the
 * selection process
 */
string short_description() {
  return "Humans are pretty average.  If this were a real mud, a better description would be provided (we hope).\n";
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
    return 0;
}

int racial_wis_bonus() {
    return 0;
}

int racial_cha_bonus() {
    return 0;
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
    ret->str_adjust = 9;
    ret->str_range = 10;
    
    ret->agi_adjust = 3;
    ret->agi_range = 10;
    
    ret->int_adjust = 3;
    ret->int_range = 5;
    
    ret->wil_adjust = -15;
    ret->wil_range = 10;

    return ret;
}

