/* Do not remove the headers from this file! see /USAGE for more info. */

/* Spell Daemon -- keep track of what spells are valid...
**
** This currently keeps track of the dirs that hold spells, 
** but lets anyone add new dirs at will. 
** Well, feel free to add security to prevent that.
**
** Rust @ lima.imaginary.com
*/

#define SAVE_FILE "/data/daemons/spell_d"
#include <assert.h>
#include <security.h>
#include <commands.h>

inherit M_ACCESS;

// Save this....
private string array  spell_dirs = ({});
// Map the name of the spell to the dir it is in.  Just use the index into
// the spell dir array, tho...
// We build this each create.
private static mapping spell_table = ([]);

private void save_me ()
{
  unguarded (1, (: save_object, SAVE_FILE :));
}

void add_spell_dir (string dir)
{
  int	i;

  ASSERT (dir && is_directory (dir));
  ASSERT (member_array (dir, spell_dirs) == -1);
  if (dir[<1] != '/')
    {
      dir += "/";
    }

  spell_dirs += ({dir});
  save_me();
}

void remove_spell_dir (string dir)
{
  if (!check_previous_privilege (1))
    return;

  ASSERT (dir && member_array (dir, spell_dirs) != -1);
  spell_dirs -= ({dir});
}  

// We need to do this due to parser limitations.
private int valid_spell_name (string spell_name)
{
  if (!stringp (spell_name))
    {
      return 0;
    }
  return regexp (spell_name, "^[a-zA-Z0-9 '-]+$");
}


private void build_spell_table ()
{
  // Not necessary, and would be a bad thing if we didn't rebuild the spell
  // table each time.
  spell_dirs -= ({0}); 

  for (int i = 0; i < sizeof (spell_dirs); i++)
    {
      string 	dir;

      dir = spell_dirs [i];
      if (!is_directory (dir))
	{
	  // Dir has been removed
	  spell_dirs -= ({dir});
	  save_me ();
	  continue;
	}
      foreach (string file in get_dir (dir+"*.c"))
	{
	  string spell_name;

	 if (catch (spell_name = (dir+file)->get_spell_name ()))
	   continue;
	  // dir+file->remove();
	  if (!valid_spell_name (spell_name))
	    {
	      // log it...
	      continue;
	    }
	  if (spell_table[spell_name])
	    {
	      // log it....
	      continue;
	    }
	  spell_table[spell_name] = ({i, file});
	}
    }
}

string get_spell_file (string spell_name)
{
  mixed array 	entry = spell_table [spell_name];

  if (!entry)
    {
      return 0;
    }
  return spell_dirs[entry[0]] + entry[1];
}

void create ()
{
  restore_object (SAVE_FILE);
  build_spell_table ();
}

private nomask string neuter_spell_name (string spell)
{
  string array	parts = explode (spell, " ");
  
  if (sizeof (parts) > 1)
    {
      switch (parts[0])
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
  if ((sizeof (parts) > 1) && (parts [<1] == "spell"))
    {
      parts = parts[0..<2];
    }

  return implode (parts, " ");
}

varargs mixed can_cast_spell (string spell, object target, object tool)
{
  string	spell_file;

  ASSERT (previous_object () == find_object (VERB_OB_CAST));
  
  spell = neuter_spell_name (spell);
  spell_file = get_spell_file (spell);
  if (!spell_file)
    {
      return "You know of no such spell.\n";
    }
  
  return spell_file->_can_cast_spell (target, tool);
}

varargs void cast_spell (string spell, object target, object tool)
{
  string 	spell_file;

  ASSERT (previous_object () == find_object (VERB_OB_CAST));
  spell = neuter_spell_name (spell);
  spell_file = get_spell_file (spell);
  ASSERT (spell_file);
  return spell_file->_cast_spell (target, tool);
}


int register_spell ()
{
  string array	file_info  = split_path (file_name (previous_object ()));
  string 	spell_name = previous_object ()->get_spell_name ();
  mixed array   spell_info = spell_table [spell_name];
  int		dir_index  = member_array (file_info [0], spell_dirs);

  ASSERT (spell_name /* You didn't provide a spell name. */);
  ASSERT (valid_spell_name (spell_name) /* Your spell name wasn't valid. */);

  if (!spell_info) 
    {
      if (dir_index == -1)
	{
	  add_spell_dir (file_info [0]);
	  return 1;
	}
      else
	{
	  spell_table [spell_name] = ({ dir_index, file_info[1] });
	}
    }

 ASSERT (find_object (get_spell_file (spell_name)) == previous_object() 
	 /* Some other spell already has this name.*/);
 
 return 1;
}
