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
** The configure_set() function may be used to define a function
** that is invoked after any flag has been changed.
**
** 961209  Deathblade  Trimmed back unneeded functionality.
** 940929  Deathblade  Created.
*/

#include <flags.h>

#define WONKY__USE_CLASS

#ifdef WONKY__USE_CLASS
private class flag_set_info
{
    int is_non_persistent;
    function change_func;
}
#endif

/*
** Defines the sets of flags.  Maps a set key to a flag_set_info.
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

//:FUNCTION get_flags
//
//get_flags(set_key) returns the flags associated with the key 'set_key'
//Any 'get' function for the flag set is also used.
nomask int get_flags(int set_key)
{
#ifdef WONKY__USE_CLASS
    class flag_set_info set_info;
#else
    mixed * set_info;
#endif

    if ( !flag_sets ) init_vars();

    set_info = flag_sets[set_key];
    if ( !set_info )
#ifdef WONKY__USE_CLASS
	set_info = new(class flag_set_info);
#else
	set_info = ({ 0, 0 });
#endif

#ifdef WONKY__USE_CLASS
    if ( set_info->is_non_persistent )
#else
    if ( set_info[0] )
#endif
	return non_persist_flags[set_key];
    return persist_flags[set_key];
}

//:FUNCTION set_flags
//
//set_flags(which, state) sets the flag specified by 'which', which includes
//both flag set and information about which bit, to 1 if state is nonzero
//and 0 if state is zero.  The secure, set, and change functions are also
//called.
private void set_flags(int which, int state)
{
    int set_key;
#ifdef WONKY__USE_CLASS
    class flag_set_info set_info;
#else
    mixed * set_info;
#endif
    int value;

    if ( !flag_sets ) init_vars();

    set_key = FlagSet(which);
    set_info = flag_sets[set_key];
    if ( !set_info )
#ifdef WONKY__USE_CLASS
	set_info = flag_sets[set_key] = new(class flag_set_info);
#else
	set_info = flag_sets[set_key] = ({ 0, 0 });
#endif

    value = get_flags(set_key);
    if ( state )
	value |= BITMASK(which);
    else
	value &= ~BITMASK(which);

    /*
    ** Use the set_closure if provided; otherwise, set the flags
    ** in the appropriate in the appropriate mapping.
    */
#ifdef WONKY__USE_CLASS
    if ( set_info->is_non_persistent )
#else
    if ( set_info[0] )
#endif
	non_persist_flags[set_key] = value;
    else
	persist_flags[set_key] = value;

    /*
    ** Call the change notification function
    */
#ifdef WONKY__USE_CLASS
    if ( set_info->change_func )
	evaluate(set_info->change_func, which, state);
#else
    if ( set_info[1] )
	evaluate(set_info[1], which, state);
#endif
}

//:FUNCTION configure_set
//configure_set allows one to specify whether a flag set is persistent,
//and a function that can be called when a flag changes.
varargs nomask void configure_set(
  int set_key,
  int is_non_persistent,
  function change_func
)
{
    if ( !flag_sets ) init_vars();

#ifdef WONKY__USE_CLASS
    flag_sets[set_key] = new(class flag_set_info,
			     is_non_persistent : is_non_persistent,
			     change_func : change_func);
#else
    flag_sets[set_key] = ({ is_non_persistent, change_func });
#endif
}

//:FUNCTION test_flag
//
//test_flag(which) returns 1 if a flag is set, and zero if not.  'which'
//includes information both about which flag set and which bit.
nomask int test_flag(int which)
{
    return (get_flags(FlagSet(which)) & BITMASK(which)) != 0;
}

//:FUNCTION set_flag
//
//set_flag(which) sets a given flag to 1.  'which'
//includes information both about which flag set and which bit.
nomask void set_flag(int which)
{
    set_flags(which, 1);
}

//:FUNCTION clear_flag
//
//clear_flag(which) sets a given flag to 0.  'which'
//includes information both about which flag set and which bit.
nomask void clear_flag(int which)
{
    set_flags(which, 0);
}

//:FUNCTION assign_flag
//
//assign_flag(which, state) sets a given flag to 1 if state is
//nonzero and 0 if state is zero.  'which' includes information
//both about which flag set and which bit.
nomask void assign_flag(int which, int state)
{
    set_flags(which, state);
}

void create()
{
    init_vars();
}
