/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** This beast manages function dispatch for shell-specific functions.
** This is a bloody quick hack to get something working.
** It should probably use classes, and definitly should do 
** simple checking on number of args.
** Created.  John Viega(rust@Virginia.EDU).     July 2, 1995
*/


static mapping dispatch = ([]);
private static mapping personal_bindings = ([]);
private static string* modules = ({});
private static mapping module_objects = ([]);
private static mapping module_func_names = ([]);

static void call_user_func(string, mixed);


//:FUNCTION setup_for_save
//Sets up M_SAVE to save some variables
void setup_for_save()
{
    /*
    ** Use the call_other() interface so that we are not statically
    ** bound to require M_SAVE.  This object this modules is applied
    ** to may save natively rather than via M_SAVE.
    ** Hmm, I don't think saving personal_bindings here is going to do
    ** any good... =)
    */
    this_object()->add_save(({ "personal_bindings", "modules" }));
}

static void
shell_bind(string command, function f)
{
  dispatch[command] = f;
}

static void
shell_bind_if_undefined(string command, function f)
{
    if ( !dispatch[command] )
	dispatch[command] = f;
}

static void
shell_unbind(string command)
{
  map_delete(dispatch, command);
}

static int
bind(string command, string *argv)
{
  string fname;

  if(sizeof(argv) != 1)
    return -1;
  fname = argv[0];
  if(undefinedp(personal_bindings[command]) && dispatch[command])
    return -2;
  
  personal_bindings[command] = fname;
  dispatch[command] = (:call_user_func, command :);
  this_object()->save();
}

static void
unbind(string* argv)
{
  string command;

  if(sizeof(argv) != 1)
    return;
  command = argv[0];
  if(undefinedp(personal_bindings[command]))
    return;

  map_delete(personal_bindings, command);
  map_delete(dispatch, command);
  this_object()->save();
}


static void 
call_user_func(string fname, mixed argv)
{
  string module;

  foreach(module in modules)
    {
      if(member_array(fname, module_func_names[module]) != -1)
	{
	  call_other(module_objects[module],fname,argv);
	  return;
	}
    }
}

static int
load_module(mixed argv)
{
  string* flist;
  mapping finfo = ([]);
  mixed item;
  object  module_ob;
  string* funcnames = ({});

  if(!(stringp(argv) || (arrayp(argv) && sizeof(argv) == 1 && 
			 stringp(argv=argv[0]))))
    return 0;
  
  if(!module_ob = load_object(argv))
    return 0;

  module_objects[argv] = module_ob;
  
  flist = functions(module_ob,1);

  foreach(item in flist)
    {
      if(strsrch(item[2],"static") != -1 || strsrch(item[2],"private") != -1)
	continue;
      finfo[item[0]];
      funcnames += ({item[0]});
    }
  module_func_names[argv] = funcnames;
  return 1;
}
     

static void
set_module_path(string* mpath)
{
  modules = mpath;
  map(modules, (: load_module :));
}

