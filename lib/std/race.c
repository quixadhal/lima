/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BODY;

#ifdef USE_STATS
/*
 * Ok, quick explanation:
 *
 * FOO_BAR_FACTOR gives the percentage of FOO that is based on BAR.
 * 
 * derived stats are based on the main stats, plus a portion which
 * depends on race, which is scaled by an amount which is the racial
 * bonus.
 * 
 * Derive the coefficients of the transformation, to make things easier
 * later.
 */
/*
** Effects of the base stats on the derived stats
*/
#define CON_STR_FACTOR		20
#define CON_AGI_FACTOR		20
#define CON_WIL_FACTOR		60

#define WIS_STR_FACTOR		7
#define WIS_AGI_FACTOR		7
#define WIS_INT_FACTOR		31
#define WIS_WIL_FACTOR		25
#define WIS_SKILL_FACTOR	30

#define CHA_STR_FACTOR		15
#define CHA_AGI_FACTOR		10
#define CHA_INT_FACTOR		25
#define CHA_WIL_FACTOR		25
#define CHA_SKILL_FACTOR	25

int racial_con_bonus() {
}

int racial_wis_bonus() {
}

int racial_cha_bonus() {
}

/* influence combat :-) */
int to_hit_base() {
    return 50 - query_agi();
}

int hit_skill() {
    return fuzzy_divide(query_agi(), 2) /* + something based on skill... */;
}

int damage_bonus() {
    return fuzzy_divide(query_str(), 10);
}

nomask int *query_transformation_matrix() {
    if (clonep())
	return base_name()->query_transformation_matrix();
    
    return ({
	map( ({ CON_STR_FACTOR, CON_AGI_FACTOR, 0, CON_WIL_FACTOR }),
	    (: $1 * $(100 - racial_con_bonus()) :) ),
	map( ({ WIS_STR_FACTOR, WIS_AGI_FACTOR, WIS_INT_FACTOR,
	    WIS_WIL_FACTOR }), (: $1 * $(100 - racial_wis_bonus() - WIS_SKILL_FACTOR) :) ),
	map( ({ CHA_STR_FACTOR, CHA_AGI_FACTOR, CHA_INT_FACTOR,
	    CHA_WIL_FACTOR }), (: $1 * $(100 - racial_cha_bonus() - CHA_SKILL_FACTOR) :) )
    });
}

nomask int *query_constant_vector() {
    if (clonep())
	return base_name()->query_constant_vector();

    return ({ racial_con_bonus(), racial_wis_bonus(), racial_cha_bonus() });
}
#else
// Maybe make these level based ?
int to_hit_base() {
    return 50;
}

int hit_skill() {
    return 10;
}

int damage_bonus() {
    return 0;
}
#endif

// A default query_race, just in case it is forgotten

string query_race()
{
  error("No valid query_race() defined...\n");
}
