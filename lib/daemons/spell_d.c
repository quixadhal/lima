/* Do not remove the headers from this file! see /USAGE for more info. */

/* Spell Daemon -- keep track of what spells are valid...
**
** This currently keeps track of the dirs that hold spells and the spells
** contained in those dirs.
**
** Rust @ lima.imaginary.com
*/

#include <assert.h>
#include <security.h>
#include <commands.h>

inherit M_ACCESS;

#define SAVE_FILE	"/data/daemons/spell_d"
#define PRIV_REQUIRED	"Mudlib:daemons"

// Save this....
private string array  spell_dirs = ({});

// Map the name of the spell to its object name.
// We build this each create.
private static mapping spell_table = ([ ]);


private void save_me()
{
    unguarded(1, (: save_object, SAVE_FILE :));
}

void add_spell_dir(string dir)
{
    int	i;

    if ( !check_privilege(PRIV_REQUIRED) )
	return;

    ENSURE(dir && is_directory(dir));
    ENSURE(member_array(dir, spell_dirs) == -1);
    
    if ( dir[<1] != '/' )
    {
	dir += "/";
    }

    spell_dirs += ({ dir });
    save_me();
}

void remove_spell_dir(string dir)
{
    if ( !check_privilege(PRIV_REQUIRED) )
	return;

    ENSURE(dir && member_array(dir, spell_dirs) != -1);
    spell_dirs -= ({ dir });
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

string get_spell_file(string spell_name)
{
    return spell_table[spell_name];
}

void create()
{
    set_privilege(1);
    restore_object(SAVE_FILE);
    build_spell_table();
}

/* strip leading "the/a/an"; strip trailing "spell" */
private nomask string neuter_spell_name(string spell)
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
    {
	parts = parts[0..<2];
    }

    return implode(parts, " ");
}

varargs mixed can_cast_spell(string spell, object target, object tool)
{
    string	spell_file;

    ENSURE(previous_object() == find_object(VERB_OB_CAST));
  
    spell = neuter_spell_name(spell);
    spell_file = get_spell_file(spell);
    if (!spell_file)
    {
	return "You know of no such spell.\n";
    }
  
    return spell_file->_can_cast_spell(target, tool);
}

varargs void cast_spell(string spell, object target, object tool)
{
    string 	spell_file;

    ENSURE(previous_object() == find_object(VERB_OB_CAST));
    spell = neuter_spell_name(spell);
    spell_file = get_spell_file(spell);
    ENSURE(spell_file);
    return spell_file->_cast_spell(target, tool);
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

    ENSURE(find_object(get_spell_file(spell_name)) == previous_object() 
	   /* Some other spell already has this name.*/);
}
