/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULES
// Simple Experience Module
// Supports simplistic query,set,add of exp
// Vette  April 3, 1997
//

void save_me();

private int experience = 0;

//### Suggestions of additional security checks welcome here
nomask private int allow_change() {
  if (wizardp(previous_object())) return 0;
  return 1;
}
//:FUNCTION query_exp()    queries experience if SIMPLE_EXP defined
nomask int query_exp(){ return experience; }

//:FUNCTION set_exp(int to)    sets experience if SIMPLE_EXP defined
nomask void set_exp(int to){
  if (!allow_change()) return;
  experience = to;
  if (experience < 0) experience = 0;
  save_me();  
}

//:FUNCTION add_exp(int to)    adds experience if SIMPLE_EXP defined
nomask void add_exp(int to) {
  if (!allow_change()) return;
  experience += to;
  if (experience < 0) experience = 0;
  save_me();
}
