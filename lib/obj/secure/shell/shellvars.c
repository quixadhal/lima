/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

private static mapping variables = ([]);
private static mapping set_var_hooks = ([]);


//:FUNCTION setup_for_save
//Sets up M_SAVE to save some variables
void setup_for_save()
{
    /*
    ** Use the call_other() interface so that we are not statically
    ** bound to require M_SAVE.  This object this modules is applied
    ** to may save natively rather than via M_SAVE.
    */
    this_object()->add_save(({ "variables" }));
}

int is_variable(string var)
{
  return !undefinedp(variables[var]);
}

void
set_variable(string var, mixed value)
{
  variables[var] = value;
  this_object()->save();
  if(set_var_hooks[var])
    evaluate(set_var_hooks[var], value);
}

void
unset_variable(string var)
{ 
    map_delete(variables, var);
    this_object()->save();
}

static void
set_if_undefined(string var, mixed value)
{
    if ( !is_variable(var) )
	set_variable(var, value);
}

mixed 
get_variable(string var)
{
    return variables[var];
}

static
void
print_variable(string v)
{
  if(undefinedp(variables[v]))
    printf("%s: undefined variable.\n",v);
  else
    printf("The value of %s is: %O\n", v, variables[v]);
}

varargs static mixed expand_if_variable(string arg, int only_expand_if_string)
{
  mixed a;

  if(!stringp(arg))
    return arg;
  if(!(strlen(arg) > 1 && arg[0] == '$'))
    {
// Strip off the \ before the $ if someone does \$var.
//      if(!only_expand_if_string && (strlen(arg) > 1) && arg[0] == '\\' && arg[1] == '$')
//	return arg[1..];
      return arg;
    }

  a = M_REGEX->regexplode(arg[1..],"[a-zA-Z0-9]+");
  if(sizeof(a) < 2 || a[0] != "" || undefinedp(variables[a[1]]))
    return arg;
  if(!stringp(variables[a[1]]))
    {
      if(only_expand_if_string)
	return arg;
      else
	return variables[a[1]];
    }
  return variables[a[1]]+((sizeof(a) > 2) ? implode(a[2..],"") : "");
}

mixed* substitute_variables(mixed* argv)
{
  return map(argv, (:expand_if_variable:));
}

/*
** This is the implementation of the command interface to
** this module from a shell command line.
*/
private string implode_rest(int start, string array argv, string array implode_info) {
    // we don't want to use the first separator
    return implode_by_arr(argv[start..], ({ "" }) + implode_info[start+1..]);
}

int cmd_unset(string array argv, string array implode_info)
{
    string name;
    
    if(sizeof(argv) < 2) {
	printf("Usage: unset variable\n");
	return 1;
    }
    // Variable names usually can't have spaces in them, but this allows
    // ones with spaces to be removed if they do exist somehow.
    // Safety feature.
    name = implode_rest(1, argv, implode_info);
    if (variables[name]) {
	unset_variable(name);
	printf("Ok.\n");
    } else {
	printf("No such variable '%s' set.\n", name);
    }
    return 1;
}


int cmd_set(string array argv, string array implode_info)
{
  string var, val;

  switch(sizeof(argv))
    {
    case 1:
      write("Current variables\n"
	    "(unset varname to unset):\n"
	    "-----------------------------\n");
      foreach(var, val in variables)
	if(val == "")
	  write(var+" ---->  is set.\n");
	else
	  printf("%-10s%O\n",var+" ----> ", val);
      write("\n");
      return 1;
    case 2:
      if (!undefinedp(variables[argv[1]]))
	{
	  printf("Variable %s is already set.\n", argv[1]);
	  return 1;
	}
      set_variable(argv[1],"");
      printf("Variable %s set.\n",argv[1]);
      return 1;
    case 3:
      set_variable(argv[1],argv[2]);
      printf("Variable %s set to %O.\n",argv[1],argv[2]);
      return 1;
    default:
      set_variable(argv[1], implode_rest(2, argv, implode_info));
      printf("Variable %s set to %s.\n", 
	     argv[1], get_variable(argv[1]));
      return 1;
    }
}

void add_variable_hook(string variable, function f)
{
  set_var_hooks[variable] = f;
}
