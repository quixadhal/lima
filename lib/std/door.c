/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;

int is_open = 0;
string is_locked;

void set_locked(string key) {
  is_locked = key;
}

int openable() { return 1; }

int open_with(object ob) {
  if (is_locked) {
    write(capitalize(the_short()) + " is locked.\n");
    return 1;
  }
  write("You open the "+short()+".\n");
  say("The "+short()+" opens.\n", this_body());
  is_open = 1;
  return 1;
}

int close() {
  write("You close the "+short()+".\n");
  say("The "+short()+" closes.\n", this_body());
  is_open = 0;
  return 1;
}

int query_open() {
  return is_open;
}
