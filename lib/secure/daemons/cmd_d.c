/* Do not remove the headers from this file! see /USAGE for more info. */

/* This will be converted to an inherited part of shells when:
 * 1) We have programatic security
 * 2) We have shared variables
 * (I guess that should hold for all daemons, though...)
 *  Someday might want to have cfile file and dir all accept string*'s in case people want to 
 *  use variables for them.  Right now, you can only use strings....
 */

//  cmd_d.c written by Rust (John Viega, rust@virginia.edu) for the Lima mudlib, July 1995.

#include <security.h>

#define PLURAL		1
#define	STR		2
#define	NUM		4

#define IS_PATH		8
#define IS_FILE 	16
#define IS_DIR		32
#define	IS_FNAME	64
#define IS_CFILE	128
#define IS_OBFILE	256

#define	FILE		(IS_PATH | IS_FILE)
#define DIR		(IS_PATH | IS_DIR)
#define CFILE		(IS_PATH | IS_FILE | IS_CFILE)
#define OBFILE		(IS_PATH | IS_FILE | IS_CFILE | IS_OBFILE)
#define FNAME		(IS_PATH | IS_FNAME)

#define	IS_OBJECT	512
#define	ONLY_USER	1024

#define OBJECT		(IS_OBJECT)
#define USER		(IS_OBJECT | ONLY_USER)

#define FILE_FLAGS (IS_PATH | IS_FILE | IS_DIR | IS_OBFILE | IS_FNAME)

inherit M_GLOB;
inherit M_REGEX;
inherit M_GETOPT;
inherit M_ACCESS;


private mixed parse_arg(int,string);

class proto_info
{
    string 	options;
    int *	prototype;
    int		first_optional_arg;
    string	proto_string;
}


private nosave mapping cmd_info = ([]);
private nosave mapping proto_info = ([]);

void create()
{
    set_privilege(1);
}

private void parse_verb_defs(string dir, string filecontents) 
{
    mapping	pathmap = ([]);
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
//### gross.
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
		case "obfile": flags |= OBFILE; break;
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

private void cache_dir(string dir)
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

varargs void cache(string* paths)
{
    if(!paths)
	paths = keys(cmd_info);
    map(paths,(:cache_dir:));
}

int is_command(object o) {
    mixed ret;
    
    if(function_exists("call_main", o) != CMD)
	return 0;
    if ((ret = o->not_a_cmd()) && (ret == 1 || ret == file_name(o)))
	return 0;
    return 1;
}

// This one won't match commands not in your path.  For players, mainly...
varargs mixed find_cmd_in_path(string cmd, string* path)
{
    string dir;
    object o;

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
            if(!is_command(o))
		return -1;
	    return ({ o, dir, cmd });
	}
    }

    return 0;
}


varargs mixed find_cmd(string cmd, string* path)
{
    object	o;
    string	dir, s;
    
    if (member_array('/', cmd) != -1) {
	s = evaluate_path(cmd);
	if(o = load_object(s)) {
	    if (is_command(o)) {
		mixed tmp = split_path(s);
		dir = tmp[0];
		s = tmp[1];
		sscanf(s, "%s.c", s);
		if(!cmd_info[dir])
		    cache_dir(dir);
		return ({o, dir, s });
	    }
	}
    }
    return find_cmd_in_path(cmd, path);
}


mixed smart_arg_parsing(mixed argv, string* path, string *implode_info)
{
    mixed		resv;
    mixed		info;
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
    mixed		this_arg;
    int			plural;

    if (sizeof(argv) == 0)
	return -1;
    
    cmd_name = trim_spaces(argv[0]);
    if (member_array('/', cmd_name) != -1) {
	array matches = filter_array(glob(cmd_name + ".c"), (: is_file :));
	switch (sizeof(matches)) {
	case 1:
	    if ((cmd_obj = load_object(matches[0])) &&
		is_command(cmd_obj)) {
		mixed tmp = split_path(matches[0]);
		this_path = tmp[0];
		cmd_name = tmp[1][0..<3];
	    } else
		return 0;
	    break;
	case 0:
	    break;
	default:
	    printf("Ambiguous expansion for %s.\n", cmd_name);
	    return 1;
	}
    }
    if (!this_path) {
	info = find_cmd(cmd_name, path);
	if (intp(info))
	    return info;
	cmd_obj = info[0];
	this_path = info[1];
	cmd_name = info[2];
    }

    if(undefinedp(proto_info[this_path]) || 
       undefinedp(pstuff=proto_info[this_path][cmd_name]))
    {
	// no prototypes, so don't do no globbing or nothin'.
	// In fact, just send back the raw string.
	if(sizeof(argv) > 1)
	{
	    // make it so that all non-strings are converted to strings, 
	    // since whatever command is going to be expecting a string.
	    argv = map(argv, (: stringp($1) ? $1 : sprintf("%O",$1) :));
	    return ({cmd_obj, ([]), implode_by_arr(argv[1..], implode_info) });
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
	    if(!arrayp(info))
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
    for(i=0; i<sizeof(pstuff->prototype); i++)
    {
	if(argcounter == sizeof(argv))
	{
	    if(i >= pstuff->first_optional_arg && pstuff->first_optional_arg != -1)
		break;
	    printf("Too few arguments.\nUsage: %s\n", USAGE);
	    return 1;
	}
	expanded_arg = parse_arg(pstuff->prototype[i],argv[argcounter++]);
	if (intp(expanded_arg)) {
	    // error
	    switch (expanded_arg) {
	    case -1:
		printf("Invalid argument: %O\nUsage: %s\n",
		       argv[argcounter-1], USAGE);
		break;
	    case -2:
		printf("Vague argument: %O\nUsage: %s\n",
		       argv[argcounter-1], USAGE);
		break;
	    case -3:
		printf("%s: No such file or directory.\n", argv[argcounter-1]);
		break;
	    }
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
	while (1)
	{
	    if(argcounter == sizeof(argv))
		break;	
	    expanded_arg = parse_arg(this_arg, argv[argcounter]);
	    if(!arrayp(expanded_arg))
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

private mixed parse_arg(int this_arg, mixed argv)
{
  int		hits;
  mixed	untrimmed_argv;
  array	result = ({});
  array       string_result;
    
  untrimmed_argv = argv;

  if(stringp(argv)) {
    argv = trim_spaces(argv);
    if (this_arg & IS_PATH) {
      string path = evaluate_path(argv);
      result = glob(path);
      if((this_arg & IS_CFILE))
	result = filter(result, (:!is_directory($1):));
      if (!sizeof(result) && (this_arg & IS_OBFILE)) {
	object ob = get_object(argv);
	if (ob)
	  {
	    string bname = base_name(ob);
	    if(is_file(bname + ".c"))
	      result = ({ bname + ".c" });
	    else
	      {
		if(is_file(bname + ".scr"))
		  result = ({bname + ".scr"});
	      }
	  }
      }
      if (!sizeof(result) && (this_arg & IS_CFILE))
	{
	  int ix = strsrch(path, ".", -1);
	  string extension = path[ix+1..];

	  if(extension != "c" && extension != "scr")
	    {
	      result = glob(path + ".c") + glob(path + ".scr");
	    }
	}

      if ((this_arg & IS_FILE) && !(this_arg & IS_DIR))
	result = filter(result, (: is_file :));

      if ((this_arg & IS_DIR) && !(this_arg & IS_FILE))
	result = filter(result, (: is_directory :));

      if (!sizeof(result) && (this_arg & IS_FNAME)) {
	if (is_directory(base_path(path)))
	  result = ({ path });
      }
	    
      if (!sizeof(result))
	result = 0;
      else {
	result = ({ this_arg & FILE_FLAGS, result });
	hits++;
      }
    }

    if (this_arg & STR) {
      string_result = ({ STR, ({ untrimmed_argv }) });
      hits++;
    }
  }

  if (this_arg & IS_OBJECT)
    {
      object ob;

      if(stringp(argv))
	ob = get_object(argv);
      else if (objectp(argv))
	ob = argv;
      else
	ob = 0;

      if((this_arg & ONLY_USER) && ob && !ob->query_link())
	ob = 0;

      if(ob) {
	result = ({ this_arg & (IS_OBJECT | ONLY_USER), ({ ob }) });
	hits++;
      }
    }

  if (this_arg & NUM)
    {
      int tmp;
	
      if (intp(argv))
	{
	  result = ({ NUM, ({ argv }) });
	  hits++;
	}
      else if (sscanf(argv,"%d",tmp))
	{
	  result = ({ NUM, ({ tmp }) });
	  hits++;
	}
    }

  if(!hits)
    { 
      if (this_arg & IS_PATH)
	return -3;
      else
	return -1;
    }

  if (this_arg & STR) {
    if (hits == 1)
      result = string_result; // use string hit
    else
      hits--; // discard string hit
  }

  if(hits > 1)
    return -2;

  return result;
}



