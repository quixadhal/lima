/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** flags.c
**
** Standard flag (bit) handling system.  Provides arbitrary sets
** of flags for subclasses to define and use.  Sets may be
** configured to save/restore or be "non-persistent".
**
** All flags come in sets of 32 bits.  The flags are keyed with
** single integer values composed with the MakeFlag() macro
** found in /include/flags.h.  See that header for more info.
**
** Flag sets default to zero-values that WILL be saved/restored.
** The configure_set() function may be used to alter the persistence
** of a set and optionally define a set of closures to use to
** access the flag values.  The get_closure closure should return
** all 32 bits of a set.  The set_closure closure will be passed
** all 32 bits.  secure_closure should return 1 to prevent setting
** a flag (in which case set_closure will not be evaluated).
** change_closure is invoked after any flag has been changed.  It
** will be called after set_closure (if provided).
**
** Typical use of the closures are to provide security, alternate
** storage mechanisms, or responses to changes in the flags.  0 may
** be supplied for any closure to indicate default behavior.
**
** 29-Sep-94  Deathblade  Created.
*/

#include <flags.h>

/*
** Defines the sets of flags.  These are keyed with an integer
** and map to an array of three elements: a flag indicating
** whether the flags are non-persistent and two (optional)
** closures that are used to get/set flag values.
*/
private static mapping flag_sets;

/*
** Stores the persistent and non-persistent flags
*/
private static mapping non_persist_flags;
private        mapping persist_flags;


#define BITMASK(w)	(1 << ((w) & 0x1F))

private void init_vars()
{
    flag_sets = ([ ]);
    non_persist_flags = ([ ]);
    persist_flags = ([ ]);
}

nomask int get_flags(int set_key)
{
    mixed * set_info;

    if ( !flag_sets ) init_vars();

    set_info = flag_sets[set_key];
    if ( !set_info )
	set_info = ({ 0, 0, 0, 0, 0 });

    if ( set_info[3] )
	return evaluate(set_info[3]);
    if ( set_info[0] )
	return non_persist_flags[set_key];
    return persist_flags[set_key];
}

private void set_flags(int which, int state)
{
    int set_key;
    mixed * set_info;
    int value;

    if ( !flag_sets ) init_vars();

    set_key = FlagSet(which);
    set_info = flag_sets[set_key];
    if ( !set_info )
	set_info = flag_sets[set_key] = ({ 0, 0, 0, 0, 0 });

    /*
    ** Use the secure_closure if provided
    */
    if ( set_info[1] && evaluate(set_info[1], which, state) )
	return;

    value = get_flags(set_key);
    if ( state )
	value |= BITMASK(which);
    else
	value &= ~BITMASK(which);

    /*
    ** Use the set_closure if provided; otherwise, set the flags
    ** in the appropriate in the appropriate mapping.
    */
    if ( set_info[4] )
	evaluate(set_info[4], value);
    else if ( set_info[0] )
	non_persist_flags[set_key] = value;
    else
	persist_flags[set_key] = value;

    /*
    ** Call the change_closure
    */
    if ( set_info[2] )
	evaluate(set_info[2], which, state);
}

varargs nomask void configure_set(
  int set_key,
  int non_persistent,
  function secure_closure,
  function change_closure,
  function get_closure,
  function set_closure
)
{
    if ( !flag_sets ) init_vars();

    flag_sets[set_key] = ({
	non_persistent,
	secure_closure,
	change_closure,
	get_closure,
	set_closure
	});
}

nomask int test_flag(int which)
{
    return (get_flags(FlagSet(which)) & BITMASK(which)) != 0;
}

nomask void set_flag(int which)
{
    set_flags(which, 1);
}

nomask void clear_flag(int which)
{
    set_flags(which, 0);
}

nomask void assign_flag(int which, int state)
{
    set_flags(which, state);
}

void create()
{
#ifdef NOPE
    if ( flag_sets )
    {
	unguarded(1, #'write_file,
		  ({ "/log/log",
			 sprintf("%s does not inherit create() properly.\n",
				 file_name())
		 }));
    }
#endif /* NOPE */

    init_vars();
}
