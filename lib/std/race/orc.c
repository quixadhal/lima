/* Do not remove the headers from this file! see /USAGE for more info. */

inherit RACE;

string query_race() {
    return "orc";
}

string short_description() {
    return "Orcs get bonuses to constitution.  Their strength is very high, their agility is slightly above average, their intelligence is very low, and thier willpower is below average.  If this had been a real race instead of an example, this would have been much more interesting to read.\n";
}

int racial_con_bonus() {
    return 30;
}

int racial_wis_bonus() {
    return 0;
}

int racial_cha_bonus() {
    return 0;
}

class stat_roll_mods query_roll_mods() {
    class stat_roll_mods ret = new(class stat_roll_mods);
    
    ret->str_adjust = 20;
    ret->str_range = 10;
    
    ret->agi_adjust = 5;
    ret->agi_range = 20;
    
    ret->int_adjust = -15;
    ret->int_range = 10;
    
    ret->wil_adjust = -10;
    ret->wil_range = 10;

    return ret;
}
