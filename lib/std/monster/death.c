/* Do not remove the headers from this file! see /USAGE for more info. */

void remove();

#ifdef DEATH_USES_CORPSES
string query_name();

private string corpse_long;
private string corpse_filename = CORPSE;

void set_corpse_long(string str) {
    corpse_long = str;
}

void set_corpse_filename(string str) {
    corpse_filename = str;
}

//:FUNCTION drop_corpse
//replace us with a corpse
void drop_corpse() {
    object corpse = new(corpse_filename, query_name(), corpse_long);
    all_inventory()->move(corpse);
    corpse->move(environment());
    remove();
}

void die() {
    drop_corpse(); // subtle pun
}
#else
void simple_action(string);
void set_hp(int);

//:FUNCTION vaporize
//remove the monster in a puff of black smoke
void vaporize() {
    simple_action("Soon after $n $vhave breathed $p last, $p body disappears in a puff of black smoke.");
    all_inventory(this_object())->move(environment());
    remove();
}

void die() {
    call_out( (: vaporize :), 1);
}
#endif
