/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** alias.c -- alias daemon
**
** Records the system wide aliases
**
** 960114, Deathblade: created
*/

#include <classes.h>
#include <commands.h>	/* for CMD_OB_ADMTOOL */
#include <security.h>

inherit M_DAEMON_DATA;
inherit CLASS_ALIAS;

private mapping  defaults = ([ ]);
private string * xdefaults = ({ });
private mapping  wizdefaults = ([ ]);
private string * wizxdefaults = ({ });


mixed query_default_aliases()
{
    return ({ defaults, xdefaults, wizdefaults, wizxdefaults });
}

private varargs void add_alias_to_mapping(mapping m,
					  string a,
					  string template,
					  int global_status,
					  string * defaults)
{
    m[a] = new(class alias);
    if(strsrch(template,"$*") == -1)
	template += " $*";
    ((class alias) m[a])->template = template;
    ((class alias) m[a])->defaults = pointerp(defaults) ? defaults : ({""});
    ((class alias) m[a])->num_args = sizeof(((class alias) m[a])->defaults) - 1;
    ((class alias) m[a])->global_alias_status = global_status;
}

varargs void add_default_alias(string name,
			       string expansion,
			       int xalias,
			       int devalias)
{
    if ( base_name(previous_object()) != ADMTOOL || clonep(this_object()) )
	return;

    if(devalias)
    {
	add_alias_to_mapping(wizdefaults, name, expansion, 2);
	if(xalias)
	    wizxdefaults = clean_array(wizxdefaults + ({name}));
    }
    else
    {
	add_alias_to_mapping(defaults, name, expansion, 1);
	if(xalias)
	    xdefaults = clean_array(xdefaults + ({ name }));
    }
    save_me();
}

varargs void remove_default_alias(string name, int devalias)
{
    if ( base_name(previous_object()) != ADMTOOL || clonep(this_object()) )
	return;

    if(devalias)
    {
	map_delete(wizdefaults, name);
	wizxdefaults -= ({ name });
    }
    else
    {
	map_delete(defaults,name);
	xdefaults -= ({ name });
    }
    save_me();
}

void clean_up() {
    destruct(this_object());
}
