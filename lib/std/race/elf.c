/* Do not remove the headers from this file! see /USAGE for more info. */

inherit RACE;

string query_race() {
    return "elf";
}

string short_description() {
    return "Elves get bonuses to charisma and wisdom.  Their strength is below average, their agility is average, their intelligence is above average, and thier willpower is slightly below average.  If this had been a real race instead of an example, this would have been much more interesting to read.\n";
}

int racial_con_bonus() {
    return 0;
}

int racial_wis_bonus() {
    return 30;
}

int racial_cha_bonus() {
    return 15;
}

class stat_roll_mods query_roll_mods() {
    class stat_roll_mods ret = new(class stat_roll_mods);
    
    ret->str_adjust = -8;
    ret->str_range = 10;
    
    ret->agi_adjust = 0;
    ret->agi_range = 20;
    
    ret->int_adjust = 10;
    ret->int_range = 30;
    
    ret->wil_adjust = -2;
    ret->wil_range = 15;

    return ret;
}
