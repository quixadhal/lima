/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Alias module by John Viega (rust@virginia.edu) Jul 2, 1995
*/

DOC_MODULE("Implements generic alias handling code.")
#include <mudlib.h>
#include <commands.h>
#include <security.h>

inherit M_ACCESS;

#define ALIAS_SAVE_FILE "/data/std/alias"


class alias{
  string template;
  string* defaults;
  int num_args;
  int global_alias_status;
}

/*
** Xverb aliases are for aliases where you can omit the space between the verb 
** and the rest of your command.  It's got it's own structure so we can limit 
** our search (That is, the side list is an optimization).
*/
private static mapping aliases = ([]);
private static string * xaliases = ({});
private static mapping alias_save = ([]);
private static string* xalias_save = ({});


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
DOC(setup_for_save,"Sets up M_SAVE to save some variables")
void setup_for_save()
{
    /*
    ** Use the call_other() interface so that we are not statically
    ** bound to require M_SAVE.  This object this modules is applied
    ** to may save natively rather than via M_SAVE.
    */
  this_object()->add_save(({ (: prep_aliases_for_save:) }));
}

DOC(add_alias,"Add an alias to an alias set.")
varargs void
add_alias(string name, string template, string* defaults, int xverb)
{
  class alias new_alias;
  int i;

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

DOC(remove_alias, "Remove an alias from an alias set.")
void
remove_alias(string name)
{
  map_delete(aliases, name);
  xaliases -= ({name});
}

DOC(expand_alias, "Expand an argv with any aliases if applicable.")
string* expand_alias(string* argv)
{
  class alias this_alias;
  string* xverb_matches;
  string expanded_input;
  int numargs = sizeof(argv) - 1;
  int i, j;
  string tmp;
  mixed tmp2;
// The next line should be removed before the release.
  xaliases = clean_array(xaliases);
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
      return 0;
    }
  if(!this_alias = aliases[argv[0]])
    return argv;
  expanded_input = this_alias->template;
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

  return map(reg_assoc(expanded_input, ({"[^ ]* *"}), ({0}))[0] - ({""}),
	     (: $1[<1] == ' ' ? $1[0..<2] : $1 :));
	  
}


/*
**  From here on lies a menu interface to aliases.
**  It's probably a bit messy. =/
**  (Written in a hurry, unfortunatly.)
*/

void input_from_main_menu(string);
string main_menu_prompt();
void display_main_menu();
static void init_alias_editor();
static mixed display_one_alias(string);

varargs static void cmd_alias(mixed argv)
{

  switch(sizeof(argv)){
  case 1:
    init_alias_editor();
    return;
  case 2:
    display_one_alias(argv[1]);
      return;
  default:
    if(strlen(argv[1]) > 1 && argv[1][0] == '$')
      {
	add_alias(argv[1][1..], implode(argv[2..]," "),0,1);
	printf("Xverb alias '%s' set to: %s.\n", argv[1][1..], 
	       ((class alias)aliases[argv[1][1..]])->template);
	return;
      }
    add_alias(argv[1], implode(argv[2..]," "));
    printf("Alias '%s' set to: %s.\n", argv[1],
	       ((class alias)aliases[argv[1]])->template);
  }
}

static 
void
init_alias_editor()
{
  this_user()->modal_push((:input_from_main_menu:), (:main_menu_prompt:));
  display_main_menu();
}

private
void
goto_main_menu()
{
  display_main_menu();
  this_user()->modal_func((:input_from_main_menu:), (:main_menu_prompt:));
}

private
void
quit_alias_editor()
{
  write("Exiting alias editor.\n");
  this_user()->modal_pop();
}

private
void
display_main_menu()
{
  write("Alias Editor\n\n"
	"l - list all aliases\n"
	"s - show one alias\n"
	"a - add or adjust an alias\n"
	"r - remove an alias\n"
	"\n"
	"q - quit\n");
}

private
string
main_menu_prompt()
{
  return "[lsarq] ";
}

private varargs mixed
display_one_alias(string alias)
{
  class alias this_alias;
  string output = "";
  int i;


  this_alias = aliases[alias];
  if(!this_alias)
    {
      printf("You have no alias for %s.\n", alias);
      return 0;
    }
  output += sprintf("Alias: %-15s Expansion: %s\n", alias, this_alias->template);
  if(sizeof(this_alias->defaults) > 1 || this_alias->defaults[0] != "")
    {
      output += sprintf("\tDefaults:\n"
	   "  $*: %s\n", this_alias->defaults[0]);
      if(sizeof(this_alias->defaults) > 1)
	for(i=1;i<sizeof(this_alias->defaults);i++)
	  output +=sprintf("  $%d: %s\n", i, this_alias->defaults[i]);
    }
  if(member_array(alias,xaliases) != -1)
    output += "You don't have to type a space after you type this alias.\n\n";
  write(output);
}


private 
void
display_all_aliases()
{
  string a;
  class alias val;
  string output;
  int i;

  output = sprintf("Alias:%9sExpansion:%29sDefaults:\n","","");
  output += 
    "---------------------------------------------------------------------------\n";
  foreach(a, val in aliases)
    {
      output += sprintf("%-14s %-38s ", a, val->template);
      if(sizeof(val->defaults) == 1 && val->defaults[0] == "")
	output += "\n";
      else
	{
	  output += "$*: " + val->defaults[0] + "\n";
	  for(i=1;i<sizeof(val->defaults);i++)
	    output += sprintf("%54s$%d: %s\n","",i,val->defaults[i]);
	}
    }
    new(MORE_OB)->more_string(output);
}


private static string * display_menu_options;
private
void
process_display_alias_menu_choice(string input)
{
  
  int index;
  if(input=="q")
    {
      quit_alias_editor();
      return;
    }
  if(!sscanf(input,"%d",index) || index < 1 || 
     index > sizeof(display_menu_options))
    {
      write("Invalid selection.\n");
      return;
    }
  display_one_alias(display_menu_options[index-1]);
  display_menu_options = 0;
  goto_main_menu();
}

private
void
display_alias_menu()
{
  int i;

  display_menu_options = sort_array(keys(aliases),1);
  write("Select alias to display:\n");
  for(i=0; i<sizeof(display_menu_options); i++)
    {
      printf("%-4d%-6s  ", i+1, display_menu_options[i]);
      if(i%5==4)
	write("\n");
    }
  write("\n");
  this_user()->modal_func((:process_display_alias_menu_choice:),
			    (:"[#q] ":));
}

private 
void
display_alias(string s)
{
  string* matches;
  int i;


  matches = M_COMPLETE->complete(s, keys(aliases));

  switch(sizeof(matches))
    {
    case 0:
      write("No such alias.\n");
      goto_main_menu();
      return;

    case 1:
      display_one_alias(matches[0]);
      goto_main_menu();
      return;

    default:
      write("Possible completions are:\n");
      matches = sort_array(matches,1);
      for(i = 0; i<sizeof(matches); i++)
	  printf("  %-4d%s\n",i,matches[i]);
      display_menu_options = matches;	  
      this_user()->modal_func((:process_display_alias_menu_choice:),
				(:"Select one by number, or q to quit: ":));
      return;
    }
}

private static class alias new_alias_in_progress;
private static string New_alias_name;
private static int num_defaults;
private static int default_index;


private string
get_arg_default_prompt()
{
  return sprintf("What's the default for $%d (enter for none)? ", 
		 default_index+1);
}

private void
get_xverb_status(string s)
{
  if(s == "")
    return;
  switch(s[0])
    {
    case '?':
      write(
	"Do you want to be able to omit the space between the alias and the "
	"rest of\nwhat you type?  Ie, if your alias is for ', do you want "
	"to be able to do:\n'foo\nor do you want to be forced to put a space "
	"between the ' and the foo??\n");
      return;
    case 'y':
      xaliases += ({ New_alias_name });
      break;
    case 'n':
      break;
    default:
      write("Invalid response.\n");
      return;
    }
  aliases[New_alias_name] = new_alias_in_progress;
  this_object()->save();
  printf("Alias %s defined.\n", New_alias_name);
  goto_main_menu();
}
  

private void
ask_for_xverb_status()
{
  this_user()->modal_func((: get_xverb_status :), 
			    (: "Allow space ommision? [yn?] " :));
}
private void
set_arg_default(string s)
{
  default_index++;
  new_alias_in_progress->defaults[default_index] = s;
  if(default_index == num_defaults)
    {
      ask_for_xverb_status();
    }
}

private void
set_star_default(string s)
{
  new_alias_in_progress->defaults[0] = s;
  default_index = 0;
  num_defaults  = new_alias_in_progress->num_args;
  if(!num_defaults)
    {
      ask_for_xverb_status();
      return;
    }

  this_user()->modal_func( (: set_arg_default :),
			    (: get_arg_default_prompt :) );
}


private void
new_template(string s)
{
  if(s == "")
    return;
  if(s == "?")
    {
      write(
"An alias definition can change each time you use the alias:\n"
"$1  -- Wherever you put this in your alias definition,\n"
"       the $1 will be replaced with the first word after\n"
"       the alias when you use the alias.\n"
"       You can do the same with $2, $3, $4, ad infinitum.\n"
"       If you have $1 in your alias definition, but don't\n"
"       provide a first argument when you use an alias,\n"
"       the default value for $1 is used.  You can set the\n"
"       default value when you are done with the alias definition.\n\n"
"       Example:  If you alias \"you're\" to:\n"
"                  say Hey, $2, you're $1!\n"
"       When you type:\n"
"                  you're studly Rust\n"
"       You'll say: \"Hey, Rust, you're studly!\"\n"
"\n"	   
"$*  -- It's the same as $1, but it means 'everything else'. \n"
"       Example:  If you alias \"you're\" to:\n"
"                  say $2, you're $1!  Do you $*?\n"
"       When you type:\n"
"                 you're studly Rust excersise much\n"
"	You'll say: \"Hey, Rust, you're studly!  Do you excersise much?\"\n"
"\n"
"Notice $* matches any number of words, whereas $1, $2, etc...\n"
"only match one word.\n\n"
"If you don't put a $* in your expansion, one will be automatically\n"
"added to the end of the alias for you.\n");
      return;
    }
  if(strsrch(s,"$*") == -1)
    s = s + " $*";

  new_alias_in_progress->template = s;

  new_alias_in_progress->num_args =
    max(map(explode(s[strsrch(s,"$")..],"$"), 
		   function(string s){
		     int d;
		     sscanf(s, "%d%s",d,s);
		     return d;}));

  new_alias_in_progress->defaults = 
    allocate(new_alias_in_progress->num_args+1);
					    

  this_user()->modal_func((: set_star_default :), 
			  (: "What's the default for $* (enter for none)? " :));

}


private void
new_alias_name(string s)
{
  if(strsrch(s," ") != -1)
    {
      write("Aliases may not have spaces.  Try again.\n");
      return;
    }
  if(s=="")
    return;
  
  new_alias_in_progress = new(class alias);
  New_alias_name = s;
  write("Type in the expansion for this alias.\n");
  this_user()->modal_func((: new_template :), 
			  (: "Expansion (? for help): " :));
}
  
  
private 
void
cmd_adjust_alias(string s)
{
  if(strsrch(s," ") != -1)
    {
      write("Aliases may not have spaces.  Try again.\n");
      this_user()->modal_func((: new_alias_name :),
				(: "Name of alias: " :));
      return;
    }
  if(s=="")
    {
      this_user()->modal_func((: new_alias_name :), 
				(: "Name of alias: " :));
      return;
    }
  new_alias_name(s);
}

private
varargs void
cmd_remove_alias(mixed s, int not_using_menu)
{

  if(not_using_menu)
    {
      if(sizeof(s) == 1)
	{
	  printf("Usage:  unalias <aliasname>\n");
	  return;
	}
      s = s[1];
    }
  if(undefinedp(aliases[s]))
    {
      printf("You didn't have '%s' as an alias.\n",s);
      if(!not_using_menu)
	goto_main_menu();
      return;
    }
  remove_alias(s);
  printf("Alias '%s' removed.\n", s);
  if(!not_using_menu)
    goto_main_menu();
  return;
}


private
void
input_from_main_menu(string arg)
{
  int cmd;
  if(arg == "")
    return;
  if(strlen(arg) == 1)
    {
      cmd = arg[0];
      arg = 0;
    }
  else
    {
      if(arg[1] != ' ')
	{
	  write("Invalid command.\n");
	  display_main_menu();
	  return;
	}
      if(strlen(trim_spaces(arg)) > 1)
	arg = arg[2..];
      else
	arg = 0;
    }
  switch(cmd)
    {
    case 'q':
      quit_alias_editor();
      return;
    case 'l':
      if(!sizeof(aliases))
	{
	  write("You currently have no aliases defined.\n");
	  goto_main_menu();
	  return;
	}
      display_all_aliases();
      return;
    case 's':
      if(!sizeof(aliases))
	{
	  write("You currently have no aliases defined.\n");
	  goto_main_menu();
	  return;
	}
      if(!arg)
	display_alias_menu();
      else
	display_alias(arg);
      return;
    case 'a':
      if(!arg)
	  this_user()->modal_func((: cmd_adjust_alias :), 
				    (:"Alias name? ":));
      else
	cmd_adjust_alias(arg);
      return;
    case 'r':
      if(!sizeof(aliases))
	{
	  write("You currently have no aliases defined.\n");
	  goto_main_menu();
	  return;
	}
      if(!arg)
	  this_user()->modal_func((: cmd_remove_alias :), 
				    (:"Alias name? ":));
      else
	cmd_remove_alias(arg);
      return;
    default:
      write("Invalid command.\n");
      return;
    }
}
	
private varargs mapping add_alias_to_mapping( mapping m, string a,
				     string template, int global_status, string*defaults )
{
  m[a] = new(class alias);
  if(strsrch(template,"$*") == -1)
    template += " $*";
  ((class alias) m[a])->template = template;
  ((class alias) m[a])->defaults = pointerp(defaults) ? defaults : ({""});
  ((class alias) m[a])->num_args = sizeof(((class alias) m[a])->defaults) - 1;
  ((class alias) m[a])->global_alias_status = global_status;
}

  mapping defaults;
  string * xdefaults;
  mapping  wizdefaults = ([]);
  string * wizxdefaults = ({});



private void initialize_aliases()
{
  mixed info;
  info = call_other(M_ALIAS,"query_default_aliases");

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
    if(clonep(this_object()) && base_name(this_object()) != M_ALIAS)
      {
	call_out((: initialize_aliases :), 0);
	return;
      }
    restore_object(ALIAS_SAVE_FILE);
    if(!wizdefaults) wizdefaults = ([]);
    if(!wizxdefaults) wizxdefaults = ({});
    if(!defaults){   //Default default aliases P-)

      defaults = ([]);
      xdefaults = ({});
      add_alias_to_mapping(defaults, "'", "say $*");
      add_alias_to_mapping(defaults, "gossip", "chan gossip $*");
      add_alias_to_mapping(defaults, "l", "look $*");
      add_alias_to_mapping(defaults, "x", "examine $*");
      add_alias_to_mapping(defaults, ":", "emote $*");
      add_alias_to_mapping(defaults, "newbie", "chan newbie $*");
      add_alias_to_mapping(defaults, "n", "go north $*");
      add_alias_to_mapping(defaults, "ne", "go northeast $*");
      add_alias_to_mapping(defaults, "nw", "go northwest $*");
      add_alias_to_mapping(defaults, "s", "go south $*");
      add_alias_to_mapping(defaults, "se", "go southeast $*");
      add_alias_to_mapping(defaults, "sw", "go southwest $*");
      add_alias_to_mapping(defaults, "w", "go west $*");
      add_alias_to_mapping(defaults, "e", "go east $*");

      xdefaults = ({"'",":",});
    }
}

mixed query_default_aliases()
{
    return({ defaults, xdefaults, wizdefaults, wizxdefaults}) ;
}

varargs int
add_default_alias(string name, string expansion, int xalias, int devalias){
  if(base_name(previous_object()) != ADMTOOL || clonep(this_object()))
    return 0;

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
  unguarded(1, (: save_object, ALIAS_SAVE_FILE :));
}

varargs int
remove_default_alias(string name, int devalias){
  if(base_name(previous_object()) != ADMTOOL || clonep(this_object()))
    return 0;

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
  unguarded(1, (: save_object, ALIAS_SAVE_FILE :));
}
