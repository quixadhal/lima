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
}

//:FUNCTION reduce_hp
//DO NOT under pain of banishment, use this to do damage to monsters/players
static void reduce_hp(int x) {
   hp -= x;
   if (hp <= 0) {
       hp = 0;
       die();
   }
}

//:FUNCTION heal_us
//Heal us a specified amount, truncating at max_hp
void heal_us(int amt) {
    hp += amt;
    if (hp>max_hp) hp = max_hp;
}

//:FUNCTION query_max_hp
//Find the maximum number of hitpoints of a monster
int query_max_hp() {
    return max_hp;
}

//:FUNCTION query_hp
//Find the current number of hitpoints of a monster
int query_hp() {
    heal_us(fuzzy_divide(time()-hp_time,heal_rate));
    hp_time = time();

    return hp;
}

//:FUNCTION query_ghost
//return 1 if the monster is dead
int query_ghost() {
    return (hp==0);
}

