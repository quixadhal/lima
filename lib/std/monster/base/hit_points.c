/* Do not remove the headers from this file! see /USAGE for more info. */

// Keeps track of hit points, for combat systems that use them

void die();

private int hp = 1;
private int max_hp = 1;
private static int hp_time = time();
private int heal_rate = 60;

void set_heal_rate(int x) {
    heal_rate = x;
}

//:FUNCTION set_max_hp
//Set the maximum number of hit points of a monster, and also set it's 
//hit points to the new max
void set_max_hp(int x) {
    hp = max_hp = x;
}

//:FUNCTION set_hp
//Set the hit points of a monster.  Usually, you want to use heal_us() or
//set_max_hp instead.
//see: set_max_hp
void set_hp(int x) {
    if (hp > max_hp)
        error("Attempt to set hp > max_hp.\n");
    hp = x;
    if (hp <= 0) {
        die();
        hp = 0;
    }
}

private void reduce_hp(int x) {
   hp -= x;
   if (hp <= 0) {
       hp = 0;
       die();
   }
}

private void increase_hp(int x) {
   if (hp <= 0) return;
   hp += x;
   if (hp > max_hp)
       hp = max_hp;
}

//:FUNCTION reincarnate
//Makes us alive again
void reincarnate() {
    hp = 1;
    hp_time = time();
}

//:FUNCTION query_hp
//Find the current number of hitpoints of a monster
int query_hp() {
    if (hp > 0 && time() - hp_time) {
        increase_hp(fuzzy_divide(time()-hp_time,heal_rate));
        hp_time = time();
    }
    return hp;
}

//:FUNCTION heal_us
//Heal us a specified amount, truncating at max_hp
void heal_us(int amt) {
    query_hp(); // healing since last checked
    if( hp <= 0 ) return;
    increase_hp(amt);
}

//:FUNCTION hurt_us
//Hurt us a specified amt; you probably want to use do_damage() instead
void hurt_us(int amt) {
    query_hp(); // healing since last checked
    if( hp <= 0 ) return;
    reduce_hp(amt);
}

//:FUNCTION query_max_hp
//Find the maximum number of hitpoints of a monster
int query_max_hp() {
    return max_hp;
}

//:FUNCTION query_ghost
//return 1 if the monster is dead
int query_ghost() {
    return (hp==0);
}
