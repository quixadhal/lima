/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Alias module by John Viega (rust@virginia.edu) Jul 2, 1995
** 
** NOTE:
** This had to be munged rather badly to get an alias w/ $1 in it to work 
** at all.  (I realized after the fact I wanted it to work, since you can
** set an alias to code, and some code uses $1, eg: (: $1 :) )
** use a \\\$1....  It's ugly, nod, but I don't have time at the moment to 
** make it nicer. =/
*/

//:MODULE
//Implements generic alias handling code.

#include <classes.h>

inherit CLASS_ALIAS;


/*
** Xverb aliases are for aliases where you can omit the space between the verb 
** and the rest of your command.  It's got it's own structure so we can limit 
** our search (That is, the side list is an optimization).
*/
private static mapping aliases = ([]);
private static string * xaliases = ({});
private static mapping alias_save = ([]);
private static string* xalias_save = ({});

static void init_alias_editor()
{
    new(ALIASMENU)->begin_menu(this_object());
}

/*
** Access functions for the alias editor
*/
nomask int query_alias_count()
{
    return sizeof(aliases);
}

nomask int is_xalias(string alias_name)
{
    return member_array(alias_name, xaliases) != -1;
}
nomask void add_xalias(string alias_name)
{
    if ( base_name(previous_object()) != ALIASMENU )
	error("invalid attempt to add an xalias\n");
#ifdef MAX_NUMBER_OF_ALIASES
    if (query_alias_count() >= MAX_NUMBER_OF_ALIASES)
      {
	write("Sorry, you already have too many aliases.\n");
	return;
      }
#endif

    xaliases += ({ alias_name });
}

nomask string * query_alias_names()
{
    return keys(aliases);
}
nomask class alias query_one_alias(string alias_name)
{
    return aliases[alias_name];
}

//:FUNCTION remove_alias
//Add an alias to an alias set.
nomask void add_alias_simple(string alias_name, class alias the_alias)
{
    if ( base_name(previous_object()) != ALIASMENU )
	error("invalid attempt to add an alias\n");

#ifdef MAX_NUMBER_OF_ALIASES
    if (query_alias_count() >= MAX_NUMBER_OF_ALIASES)
      {
	write("Sorry, you already have too many aliases.\n");
	return;
      }
#endif


    aliases[alias_name] = the_alias;

//### really do this? it was in here... leaving it (DB)
    this_object()->save();
}

private nomask void internal_remove_alias(string alias_name)
{
    map_delete(aliases, alias_name);
    xaliases -= ({ alias_name });
}

//:FUNCTION remove_alias
//Remove an alias from an alias set.
nomask void remove_alias(string alias_name)
{
    if ( base_name(previous_object()) != ALIASMENU )
	error("invalid attempt to remove an alias\n");

    internal_remove_alias(alias_name);
}


/*
** Save functions
*/

private string* prep_aliases_for_save(string state)
{
  string a;
  class alias expansion;
  alias_save = ([]);
  xalias_save = ({});
  if(state == "saving")
    foreach(a, expansion in aliases)
      {
	// TMP HACK =(
	if(sizeof(expansion) != 4)
	  {
	    class alias new_expansion;
	    new_expansion = new(class alias);
	    new_expansion->template = expansion->template;
	    new_expansion->defaults = expansion->defaults;
	    new_expansion->num_args = expansion->num_args;
	    new_expansion->global_alias_status = 0;
	    expansion = new_expansion;
	  }
	if(!(expansion->global_alias_status))
	  {
	    alias_save[a] = expansion;
	    if(member_array(a,xaliases) != -1)
	      xalias_save += ({a});
	  }
      }
  return ({ "alias_save", "xalias_save" });
}

//:FUNCTION setup_for_save
//Sets up M_SAVE to save some variables
void setup_for_save()
{
    /*
    ** Use the call_other() interface so that we are not statically
    ** bound to require M_SAVE.  This object this modules is applied
    ** to may save natively rather than via M_SAVE.
    */
  this_object()->add_save(({ (: prep_aliases_for_save:) }));
}

//:FUNCTION add_alias
//Add an alias to an alias set.
varargs void
add_alias(string name, string template, string* defaults, int xverb)
{
  class alias new_alias;
  int i;

#ifdef MAX_NUMBER_OF_ALIASES
    if (query_alias_count() >= MAX_NUMBER_OF_ALIASES)
      {
	write("Sorry, you already have too many aliases.\n");
	return;
      }
#endif

  if(strsrch(template,"$*") == -1)
    template += " $*";

  new_alias = new(class alias);
  new_alias->template = template;
  new_alias->defaults = defaults;
  new_alias->num_args = 
    max(map(explode(template[strsrch(template,"$")..],"$"), 
		   function(string s){
		     int d;
		     sscanf(s, "%d%s",d,s);
		     return d;}));

// Pad the default array with empty strings if there aren't enough
// defaults provided to us.
  if(!arrayp(new_alias->defaults))
    new_alias->defaults = ({});
  i = new_alias->num_args - (sizeof(defaults) - 1);
  while(i--)
    new_alias->defaults += ({""});

  if(xverb)
    {
      xaliases += ({name});
    }
  aliases[name] = new_alias;

  this_object()->save();
}


//:FUNCTION expand_alias
//Expand an argv with any aliases if applicable.
mixed expand_alias(string input)
{
  string * argv = explode(input, " ");
  class alias this_alias;
  string* xverb_matches;
  string expanded_input;
  int numargs = sizeof(argv) - 1;
  int i, j;

  xverb_matches = filter_array(xaliases, (: strsrch($2, $1) == 0 :), argv[0]);
  switch(sizeof(xverb_matches))
    {
    case 0:
      break;
    case 1:
      sscanf(argv[0],xverb_matches[0]+"%s",argv[0]);
      if(argv[0]=="")
	argv[0] = xverb_matches[0];
      else
	{
	  numargs++;
	  argv = xverb_matches + argv;
	}
      break;
    default:
      printf("ERROR: Alias conflict: can't destinguish between: %s.\n", 
	     implode(xverb_matches,", "));
      return "";
    }
  if(!(this_alias = aliases[argv[0]]))
    return trim_spaces(implode(argv, " "));

  expanded_input = replace_string(this_alias->template,"\\\\$",sprintf("%c",255));
  j = numargs;


  for(i=1;i<=this_alias->num_args;i++, j--)
    {
      if(j<1)
	expanded_input = replace_string(expanded_input, sprintf("$%d",i), 
					this_alias->defaults[i]);
      else
	expanded_input = replace_string(expanded_input, 
					sprintf("$%d",i), argv[i]);
    }
  if(j>0)
    expanded_input = replace_string(expanded_input, "$*", 
				    implode(argv[i..]," "));
  else
    expanded_input = replace_string(expanded_input, "$*", 
				    this_alias->defaults[0]);

  return trim_spaces(replace_string(expanded_input,sprintf("%c",255), "$"));
}


private void initialize_aliases()
{
  mixed info;
  info = call_other(ALIAS_D,"query_default_aliases");

  if(wizardp(this_user()))
    {
      aliases = info[0] + info[2] + alias_save;
      xaliases = clean_array(info[1] + info[3] +  xalias_save);
    }
  else
    {
      aliases = info[0] + alias_save;
      xaliases = clean_array(info[1] + xalias_save);
    }
}

void create()
{
    if ( clonep(this_object()) && base_name(this_object()) != M_ALIAS )
    {
	call_out((: initialize_aliases :), 0);
    }
}

varargs static void cmd_alias(mixed argv, string array implode_info)
{
    switch(sizeof(argv))
    {
    case 1:
	init_alias_editor();
	return;
    case 2:
	ALIASMENU->display_one_alias(argv[1], this_object());
	return;
    default:
	if(strlen(argv[1]) > 1 && argv[1][0] == '$')
	{
	  if(implode_info)
	    add_alias(argv[1][1..], implode_by_arr(argv[2..],
		      implode_info[2..])[1..],0,1);
	  else
	    add_alias(argv[1][1..], implode(argv[2..]," "),0,1);
	    printf("Xverb alias '%s' set to: %s.\n", argv[1][1..], 
		   ((class alias)aliases[argv[1][1..]])->template);
	    return;
	}
	if(implode_info)
	  add_alias(argv[1], implode_by_arr(argv[2..],implode_info[2..])[1..]);
	else
	  add_alias(argv[1], implode(argv[2..]," "));
	printf("Alias '%s' set to: %s.\n", argv[1],
	       ((class alias)aliases[argv[1]])->template);
    }
}

varargs static void cmd_remove_alias(mixed argv)
{
    if ( sizeof(argv) != 2 )
    {
	printf("Usage: unalias <aliasname>\n");
	return;
    }

    if ( !aliases[argv[1]] )
    {
	printf("You don't have '%s' as an alias.\n", argv[1]);
    }
    else
    {
	internal_remove_alias(argv[1]);
	printf("Alias '%s' removed.\n", argv[1]);
    }
}

