/* Do not remove the headers from this file! see /USAGE for more info. */

 /* Do not remove the headers from this file! see /USAGE for more info. */
 // Coder race for wizards. Yaynu @ Red Dragon Nov 17 1995.
 // Coder is a above average race when it comes to stats. But no crazy stats
 // so wizards can test their monsters effectively.

inherit RACE;
string query_race() {
    return "coder";
}
string short_description() {
    return (@CODER
Coder race is a misterious race of supernatural beings. You need to be a
wizard in order to be a member of the coder race.
CODER);

}
int racial_con_bonus() {
    return 20;
}
int racial_wis_bonus() {
    return 20;
}                                                                                                                                   

int racial_cha_bonus() {
    return 20;
}

class stat_roll_mods query_roll_mods() {
    class stat_roll_mods ret = new(class stat_roll_mods);
    
    ret->str_adjust = 10;
    ret->str_range = 10;
    
    ret->agi_adjust = 10;
    ret->agi_range = 10;
    
    ret->int_adjust = 10;
    ret->int_range = 10;
    
    ret->wil_adjust = 10;
    ret->wil_range = 10;

    return ret;
}
