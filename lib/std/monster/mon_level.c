/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
// This module supports monster level setup
// ie. setting the monsters level sets various properties in the monster
// this module is available using USE_MON_LEVEL definition
// main use is to allow stanardization in balancing monsters
//
// Vette  April 3, 1997
//

// The following defnitions may be folded out into an include file if deemed
// necessary
#define HP_MUL 75
#define AC_MUL 1
#define SKILLS_MUL 5


private nosave int mon_level;

void set_hp(int i);
void set_max_hp(int i);

void set_level(int i) { mon_level = i; }
int query_level() { return mon_level; }

//:FUNCTION set_monster_level(int to)  Sets monster's level and various info
nomask void set_monster_level(int to){
  if (this_object()->query_link()) return;
  set_level(to);
  set_max_hp(HP_MUL*to);
  set_hp(HP_MUL*to);
 
}
