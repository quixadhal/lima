/* Do not remove the headers from this file! see /USAGE for more info. */

/* This will be converted to an inherited part of shells when:
 * 1) We have programatic security
 * 2) We have shared variables
 * (I guess that should hold for all daemons, though...)
 *  Someday might want to have cfile file and dir all accept string*'s in case people want to 
 *  use variables for them.  Right now, you can only use strings....
 */

//  cmd_d.c written by Rust (John Viega, rust@virginia.edu) for the Lima mudlib, July 1995.

#include <mudlib.h>
#include <security.h>
#define 	PLURAL	1
#define	STR	2
#define	FILE	4
#define 	DIR	8
#define	OBJECT	16
#define	USER	32
#define	FNAME	64
#define	NUM	128
#define 	CFILE	256

inherit M_GLOB;
inherit M_REGEX;
inherit M_GETOPT;
inherit M_ACCESS;


private mixed parse_arg(int,string);

class proto_info {
  string 	options;
  int*		prototype;
  int		first_optional_arg;
  string	proto_string;
}


private static mapping cmd_info = ([]);
private static mapping proto_info = ([]);

void
create()
{
  set_privilege(1);
}

private void parse_verb_defs(string dir, string filecontents) 
{
  mapping pathmap = ([]);
  string* 	lines = explode(filecontents,"\n");
  string* 	words;
  string 	line, cmd, item;
  int 	size, flags, error_flag, i;
  string	errors = "";

  foreach(line in lines)
    {
      words = split(line, "[ \t]+") - ({""," "});
      size = sizeof(words);
      if(!size || words[0][0] == '#')
	continue;
      cmd = words[0];
      if(!undefinedp(pathmap[cmd]))
	{
	  errors += sprintf("Warning: %s redefined.  Previous definition being clobbered.\n", 
			    cmd);
	}
      pathmap[cmd] = new(class proto_info);
      ((class proto_info)pathmap[cmd])->proto_string = 
	replace_string( replace_string( replace_string( replace_string 
	  (replace_string (replace_string( implode (words," "),
	    "num","number"), "str","string"), "obj", "object"), "fname", "filename"), 
	      "*", "(s)"), "cfile", "file");
      if(!(--size))
	continue;
      words = words[1..];
      if(words[0][0] == '-' && strlen(words[0]) > 1)
	{
	  ((class proto_info)pathmap[cmd])->options = words[0][1..];
	  if(!(--size))
	    {
	      errors += "Bad line: " + line +"\n";
	      continue;
	    }
	  words = words[1..];
	}
      ((class proto_info)pathmap[cmd])->prototype = allocate(size);
      ((class proto_info)pathmap[cmd])->first_optional_arg = -1;
      for(i=0; i<size;i++) {
	  flags = 0;
	  if(words[i][0] == '[' && words[i][<1] == ']')
	    {
	      words[i] = words[i][1..<2];
	      if(((class proto_info)pathmap[cmd])->first_optional_arg == -1)
		((class proto_info)pathmap[cmd])->first_optional_arg = i;
	    }
	  if(words[i][<1] == '*')
	    {
	      flags |= PLURAL;
	      words[i] = words[i][0..<2];
	    }
	  if(words[i] == "")
	    {
	      errors += "Bad line: "+line + "\n";
	      error_flag = 0;
	      break;
	    }
	  foreach(item in explode(words[i],"|"))
	    switch(item)
	      {
	      case "str":  flags |= STR; break;
	      case "file": flags |= FILE; break;
	      case "dir": flags |= DIR; break;
	      case "obj": flags |= OBJECT; break;
	      case "user": flags |= USER; break;
	      case "fname": flags |= FNAME; break;
	      case "num": flags |= NUM; break;
	      case "cfile": flags |= CFILE; break;
	      default:
		errors += "Bad line (invalid argument type): "+line + "\n";
		error_flag = 1;
		break;
	      }
	  if(error_flag)
	    break;
	  ((class proto_info)pathmap[cmd])->prototype[i] = flags;

	}
      if(error_flag)
	{
	  error_flag = 0;
	  continue;
	}
    }
    proto_info[dir] = pathmap;
    if(strlen(errors))
      NEWS_D->system_post(BUG_NEWSGROUP, "Bugs found by " + base_name(this_object()), errors);
}

private void
cache_dir(string dir)
{
  string* files;
  if(dir[<1] != '/')
    dir += "/";

  if(is_file(dir+"Cmd_rules"))
    parse_verb_defs(dir, read_file(dir+"Cmd_rules"));
  files = get_dir(dir + "?*.c");
  if(!arrayp(files) || !sizeof(files))
    return;
  cmd_info[dir] = map(files, (: $1[0..<3] :));
}

varargs
void
cache(string* paths)
{
  if(!paths)
    paths = keys(cmd_info);
  map(paths,(:cache_dir:));
}

// This one won't match commands not in your path.  For players, mainly...
varargs mixed find_cmd_in_path(string cmd,
			       string* path,
			       int just_give_me_the_dir)
{
    string dir, s;
    object o;

    path = map(path, (: evaluate_path :));
    foreach(dir in path)
    {
	if(dir[<1] != '/')
	    dir += "/";
	//Try adding this dir if we don't have it,
	if(!cmd_info[dir])
	    cache_dir(dir);
	//And if it's still not in the cache, this is a bogus path.
	if(!cmd_info[dir])
	    continue;
	if(member_array(cmd, cmd_info[dir]) != -1 && 
	   is_file(dir+cmd+".c") &&
	   o = load_object(dir+cmd))
	{ 
	    if(function_exists("call_main",o) != CMD)
		return -1;
	    return just_give_me_the_dir ?(mixed)dir :(mixed) o;
	}
    }

    return 0;
}


varargs mixed find_cmd(string cmd, string* path, int flag)
{
    object	o;
    string	dir, s;

    // First see if we were given an absolute path...
    s = evaluate_path(cmd);
    if(strsrch(cmd,"/") != -1 && o = load_object(s))
    {
	dir = base_path(s);
	if(function_exists("call_main",o) != CMD)
	{
	    if(!cmd_info[dir])
		cache_dir(dir);
	    return flag ? (mixed)({o, base_path(s)}) : (mixed)o;
	}
    }
    path = map(path, (: evaluate_path :));
    foreach(dir in path)
    {
	if(dir[<1] != '/')
	    dir += "/";
	//Try adding this dir if we don't have it,
	if(!cmd_info[dir])
	    cache_dir(dir);

	//And if it's still not in the cache, this is a bogus path.
	if(!cmd_info[dir])
	    continue;
	if(member_array(cmd, cmd_info[dir]) != -1 && 
	   o = load_object(dir+cmd))
	{ 
	    if(function_exists("call_main",o) != CMD)
		return -1;
	    return flag ? (mixed)({o, dir}) : (mixed)o;
	}
    }

    return 0;
} 


mixed
smart_arg_parsing(mixed argv, string* path){
  mixed			resv;
  mixed			info;
  string 		cmd_name;
  string		this_path;
  object		cmd_obj;
  class proto_info	pstuff;
  string		USAGE;
  string		opstr;
  mapping		ops;
  int			argcounter;
  int			i;
  mixed 		expanded_arg;
  mixed			this_arg;
  int			plural;

  switch(sizeof((info = map(filter_array(glob(argv[0]+".c"), 
					 (: is_file($1) && strlen($1) > 2 :)), 
			    (: $1[0..<3] :)))))
    {
    case 0:
      cmd_name = trim_spaces(argv[0]);
      break;
    case 1:
      cmd_name = split_path(info[0])[1];
      break;
    default:
      printf("Ambiguous expansion for %s.\n", argv[0]);
      return 1;
    }
  info = find_cmd(cmd_name, path, 1);
  if(intp(info))
    {	
      // hmm...
      return info;
    }
  this_path = info[1];
  cmd_obj = info[0];
  if(undefinedp(proto_info[this_path]) || undefinedp(pstuff=proto_info[this_path][cmd_name]))
    {
      // no prototypes, so don't do no globbing or nothin'.  In fact, just send back the raw string.
      if(sizeof(argv) > 1)
	{
	  // make it so that all non-strings are converted to strings, since whatever command
	  // is going to be expecting a string.
	  argv = map(argv, (: stringp($1) ? $1 : sprintf("%O",$1) :));
          return ({cmd_obj, ([]), implode(argv[1..], " ") });
	}
      else
          return ({ cmd_obj, ([]), 0});
    }

// Remove "'s for:  "word1 word2"
  argv = map(argv, (: (stringp($1) && $1[0] == '"' && $1[<1] == '"') ? 
		    $1[1..<2] : $1 :));
  USAGE = pstuff->proto_string;

  if(sizeof(argv) > 1)
    {
      argv = argv[1..];
      if((opstr = pstuff->options))
	{
	  info = getopt(argv, opstr);
	  if(!pointerp(info))
	    {
	      return -2;
	    }
	  argv = info[1];
	  ops = info[0];
	}
    }
  else
    {
      argv = ({});
    }
  if(!ops) ops = ([]);
  argcounter = 0;
  resv = allocate(sizeof(pstuff->prototype));
  for(i=0;i<sizeof(pstuff->prototype);i++)
    {
      if(argcounter == sizeof(argv))
	{
	  if(i >= pstuff->first_optional_arg && pstuff->first_optional_arg != -1)
	    break;
	  printf("Too few arguments.\nUsage: %s\n", USAGE);
	  return 1;
	}
      expanded_arg = parse_arg(pstuff->prototype[i],argv[argcounter++]);
      if(expanded_arg == -1)
	{
	  printf("Invalid argument: %O\nUsage: %s\n",argv[argcounter-1], USAGE);
	  return 1;
	}
      if(expanded_arg == -2)
	{
	  printf("Vague argument: %O\nUsage: %s\n", argv[argcounter-1], USAGE);
	  return 1;
	}
      if(expanded_arg == -3)
	{
	  printf("%s: No such file or directory.\n", argv[argcounter-1]);
	  return 1;
	}
      plural = pstuff->prototype[i] & PLURAL;
      this_arg = expanded_arg[0];
      resv[i] = expanded_arg[1];
      if(!plural)
	{
	  if(sizeof(resv[i]) > 1)
	    {
	      printf("Vague argument: %s\nUsage: %s\n", argv[argcounter-1], USAGE);
	      return 1;
	    }
	  resv[i] = resv[i][0];
	  continue;
	}
      while(1)
	{
	  if(argcounter == sizeof(argv))
	    break;	
	  expanded_arg = parse_arg(this_arg, argv[argcounter]);
	  if(!pointerp(expanded_arg))
	    break;
	  if(sizeof(expanded_arg[1]) == 1 &&
	     i+1 != sizeof(pstuff->prototype) &&
	     !(pstuff->prototype[i+1]&PLURAL) &&
	     (pstuff->prototype[i+1] & expanded_arg[0]) &&
	     (argcounter + 1 == sizeof(argv) ||
		 intp(parse_arg(pstuff->prototype[i+1], argv[argcounter+1]))))
	    break;
	  resv[i] += expanded_arg[1];
	  argcounter++;
	}
    }
  if(argcounter != sizeof(argv))
    {
      if(pstuff->prototype[i-1] & PLURAL)
	printf("%s: not found.\n", argv[argcounter]);
      else
	printf("Too many arguments.\nUsage: %s\n",USAGE);
      return 1;
    }
  return ({ cmd_obj, ops, resv });
}

private mixed parse_arg(int this_arg, mixed argv) {

  mixed 	glob_result, obj_result, num_result, str_result;
  mixed		cfile_result;
  int		hits;
  mixed		tmp;
  mixed		untrimmed_argv;

  untrimmed_argv = argv;
  if(stringp(argv)) 
    argv = trim_spaces(argv);


  if(stringp(argv) && (this_arg & CFILE))
    {
      cfile_result = (strlen(argv) > 2 && argv[<2..] == ".c") ?
	glob(evaluate_path(argv)) : glob(evaluate_path(argv)+".c");
      if(!sizeof(cfile_result) && argv == "here")
	{
	  cfile_result = ({ base_name(environment(this_body())) + ".c" });
	}
      cfile_result = filter_array(cfile_result, 
				  (: is_file :));
      if(!sizeof(cfile_result))
	cfile_result = 0;
      else
	hits++;
    }
  if((this_arg & (FILE|DIR|FNAME)) && stringp(argv))
    {
      glob_result = glob(evaluate_path(argv));
      if(!sizeof(glob_result) && argv == "here")
	{
	  glob_result = ({base_name(environment(this_body())) + ".c"});
	}
      if(!sizeof(glob_result) && (this_arg&FNAME))
	{
	  if(is_directory(base_path(evaluate_path(argv))))
	    glob_result = ({ evaluate_path(argv) });
	}

      if(!(this_arg & FILE))
	{
	  glob_result = filter_array(glob_result, (: !is_file($1) :));
	}
      if(!(this_arg & DIR))
	{
	  glob_result = filter_array(glob_result, (: !is_directory($1) :));
	}
      if(!sizeof(glob_result))
	glob_result = 0;
      else hits++;
    }
  if(this_arg & (OBJECT|USER))
    {
      if(stringp(argv))
	obj_result = get_object(argv);
      else if (objectp(argv))
	obj_result = argv;
      if(!(this_arg & OBJECT) && objectp(obj_result) &&
                              !(obj_result->query_link()))
	{	
	  obj_result = 0;
	}
      else if((!this_arg & USER) && objectp(obj_result) &&
                              obj_result->query_link())
	{
	  obj_result = 0;
	}
      if(obj_result)
	{
	  obj_result = ({ obj_result });
	  hits++;
	}
    }
  if(this_arg & NUM)
    {
      if(intp(argv))
	{
	  num_result = ({ argv });
	  hits++;
	}
      else if(sscanf(argv,"%d",tmp))
	{
	  num_result = ({ tmp });
	  hits++;
	}
    }
  if((this_arg & STR) && stringp(untrimmed_argv))
    {
      str_result = ({ untrimmed_argv });
      hits++;
    }
  if(!hits)
    { 
      if(this_arg & (CFILE|FILE|DIR))
	{
	  return -3;
	}
      return -1;
    }
  if(hits > 1)
    {
      if(this_arg & STR) hits--;
      if(hits > 1)
	return -2;
    }
  if(cfile_result)
    return ({ CFILE, cfile_result });
  if(glob_result)
    return ({ this_arg&(FILE|DIR|FNAME), glob_result });
  if(obj_result)
    return ({ this_arg&(OBJECT|USER), obj_result });
  if(num_result)
    return ({ NUM, num_result });
  return ({ STR, str_result });
}
