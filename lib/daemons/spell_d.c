/* Do not remove the headers from this file! see /USAGE for more info. */

/* Spell Daemon -- keep track of what spells are valid...
**
** This currently keeps track of the dirs that hold spells and the spells
** contained in those dirs.
**
** Rust @ lima.mudlib.org
*/

#include <assert.h>
#include <security.h>
#include <commands.h>

inherit M_DAEMON_DATA;

#define PRIV_REQUIRED	"Mudlib:daemons"

// Save this....
private string array  spell_dirs = ({});

// Map the name of the spell to its object name.
// We build this each create.
private nosave mapping spell_table = ([ ]);

void add_spell_dir(string dir)
{
  if ( !check_privilege(PRIV_REQUIRED) )
    return;

  ENSURE(dir && is_directory(dir));
  ENSURE(member_array(dir, spell_dirs) == -1);
    
  if ( dir[<1] != '/' )
    dir += "/";

  spell_dirs += ({ dir });
  save_me();
}

void remove_spell_dir(string dir)
{
  if ( !check_privilege(PRIV_REQUIRED) )
    return;

  ENSURE(dir && member_array(dir, spell_dirs) != -1);
  spell_dirs -= ({ dir });
  save_me();
}  

// We need to do this due to parser limitations.
private int valid_spell_name(string spell_name)
{
  return stringp(spell_name) && regexp(spell_name, "^[a-zA-Z0-9 '-]+$");
}

private void build_spell_table()
{
  string * bad_dirs = ({ });

// Not necessary, and would be a bad thing if we didn't rebuild the spell
// table each time.
  spell_dirs -= ({ 0 }); 

  foreach ( string dir in spell_dirs )
  {
    if ( !is_directory(dir) )
    {
 // Dir has been removed
      bad_dirs += ({ dir });
      continue;
    }

    foreach ( string file in get_dir(dir + "*.c") )
    {
      string obname = dir + file[0..<3];
      string spell_name;

      if ( catch(spell_name = obname->get_spell_name()) )
        continue;

      if ( !valid_spell_name(spell_name) )
      {
// log it...
        continue;
      }
      if ( spell_table[spell_name] )
      {
// log it....
        continue;
      }
      spell_table[spell_name] = obname;
    }
  }

  if ( sizeof(bad_dirs) )
  {
    spell_dirs -= bad_dirs;
    save_me();
  }
}

string get_spell_obname(string spell_name)
{
  return spell_table[spell_name];
}

void create()
{
  ::create();
  build_spell_table();
}

/* strip leading "the/a/an"; strip trailing "spell" */
private nomask string find_spell_obname(string spell)
{
  string array parts = explode(spell, " ");
  
  if ( sizeof(parts) > 1 )
  {
    switch ( parts[0] )
    {
      case "the":
      case "a":
      case "an":
        parts = parts[1..];
        break;
	    
      default:
        break;
    }
  }
  if ( sizeof(parts) > 1 && parts[<1] == "spell" )
    parts = parts[0..<2];

  return spell_table[implode(parts, " ")];
}

varargs mixed check_valid_spell(string spell, int has_target, int has_reagent)
{
  string	spell_obname;

  ENSURE(previous_object() == find_object(VERB_OB_CAST));
  
  spell_obname = find_spell_obname(spell);
  if ( !spell_obname )
    return "You know of no such spell.\n";
  
  return spell_obname->_check_valid_spell(has_target, has_reagent);
}

varargs mixed check_valid_target(string spell,
				 object target, mixed has_reagent)
{
  string	spell_obname;

  ENSURE(previous_object() == find_object(VERB_OB_CAST));
  
  spell_obname = find_spell_obname(spell);
  if ( !spell_obname )
    return "You know of no such spell.\n";
  
  return spell_obname->_check_valid_target(target, has_reagent);
}

varargs mixed check_valid_reagent(string spell,
				  object reagent, mixed has_target)
{
  string	spell_obname;

  ENSURE(previous_object() == find_object(VERB_OB_CAST));
  
  spell_obname = find_spell_obname(spell);
  if ( !spell_obname )
    return "You know of no such spell.\n";
  
  return spell_obname->_check_valid_reagent(reagent, has_target);
}

varargs void cast_spell(string spell, object target, object reagent)
{
  string 	spell_obname;

  ENSURE(previous_object() == find_object(VERB_OB_CAST));
    
  spell_obname = find_spell_obname(spell);
  ENSURE(spell_obname);

  return spell_obname->_cast_spell(target, reagent);
}

void register_spell()
{
  string spell_name = previous_object()->get_spell_name();
  string spell_info = spell_table[spell_name];

  ENSURE(spell_name /* You didn't provide a spell name. */);
  ENSURE(valid_spell_name(spell_name) /* Your spell name wasn't valid. */);

  if ( !spell_info )
  {
    string obname = file_name(previous_object());
    string * file_info = split_path(obname);
    int dir_index = member_array(file_info[0], spell_dirs);

    if ( dir_index == -1 )
      error("spell is not in a legal directory\n");

    spell_table[spell_name] = obname;
  }

  ENSURE(find_object(get_spell_obname(spell_name)) == previous_object() 
/* Some other spell already has this name.*/);
}
