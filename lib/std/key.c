/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;

string key_type;


int is_key(){ return 1; }

int indirect_lock_obj_with_obj() { return 1; }
int indirect_unlock_obj_with_obj() { return 1; }

void set_key_type( string key_t ){ key_type = key_t; }

mixed get_key_type(){ return key_type; }

void mudlib_setup()
{
    set_id("key");
}

mapping lpscript_attributes()
{
  return ([
    "keytype" : ({ LPSCRIPT_STRING, "setup", "set_key_type" }),
  ]);
}
