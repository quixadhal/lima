/* Do not remove the headers from this file! see /USAGE for more info. */

/* This isn't very clean, and I know it, but I'm in the mood to piss off people
  * by doing things in code they don't want me to do...   Eg, map_arrays =)
  */

#include <mudlib.h>

inherit M_GLOB;
inherit M_ANSI;
inherit CMD;

mapping		info;
mapping		is_dir;
mapping		sizes;

// for those who like their screens looking like Christmas trees
#undef XMAS

// We don't do this for everyone to save LOTS of processing.
private string
make_ansi_string(string file, string path)
{
  string fname = path + file;

  if(is_dir[fname])
    return ansi("%^BOLD%^%^MAGENTA%^" + file + "%^RESET%^%^WHITE%^");
  if(info[fname] && sizeof(info[fname]) == 3 &&
     intp(info[fname][2]) && info[fname][2])
    return ansi("%^BOLD%^%^YELLOW%^" + file + "%^RESET%^%^WHITE%^");
  return ansi("%^BOLD%^%^CYAN%^" + file + "%^RESET%^");
}

private string
make_l_flag_line(string basename, string fullname)
{
  if (is_dir[fullname])
    return sprintf("           %-20s %s", "Directory", basename);
		   
  if(!info[fullname])
    return sprintf("           %-20s %s", "Unreadable", basename);
  return sprintf("%-10d %20s %s", info[fullname][0], 
		 ctime(info[fullname][1])[4..], basename);
}

private
mixed
map_ls_arrays(function f, mixed arr1, mixed arr2)
{
  int 	i, s;
  mixed	res;

  s = sizeof(arr1);
  res = allocate(s);

  for(i=0;i<s;i++)
    res[i] = evaluate(f, arr1[i], arr2[i]);
  return res;
}

private void main(mixed argv, mapping flags)
{
  string 	path;
  mixed 	files;
  string	item;
  string 	*outarr, *fullpatharr;
  string	output = "";
  int           uses_ansi;

  if(!sizeof(argv) || !argv[0])
    argv[0] = glob(evaluate_path("./*"));
  else
    argv[0] = decompose(map(argv[0], (:is_directory($1) ?
			  glob($1 + ($1[<1] == '/' ? "*" : "/*")): $1:)));

#ifdef XMAX
  uses_ansi = i_use_ansi();
#else
  uses_ansi = 0;
#endif

  if(uses_ansi || flags["l"] || flags["s"] || flags["F"])
    {
      info = ([]); is_dir = ([]); sizes = ([]);
      foreach(item in argv[0])
	{
	  info[item] = stat(item);
	  is_dir[item] = is_directory(item);
	  sizes[item] = file_size(item);
	}
    }

  argv[0] = map_paths(argv[0]);
  if(!flags["a"])
    foreach(path, files in argv[0])
      {
	argv[0][path] = filter(files, (:$1[0] != '.':));
      }
  foreach(path in sort_array(keys(argv[0]),1))
    {
      outarr = argv[0][path];
      // only make this once.
      fullpatharr = map(outarr, (: $2 + ($2[<1] == '/' ? $1 :"/"+ $1) :), path);

      if(uses_ansi)
	{
	  outarr = map(outarr, (: make_ansi_string :), path);
	}
      if(flags["l"])
	outarr = map_ls_arrays((: make_l_flag_line :), outarr,
			       fullpatharr);
      else
	if(flags["s"])
	  outarr = map_ls_arrays((: (sizes[$2]+1024)/1024 + " " + $1 :),
				 outarr, fullpatharr);

      if(flags["F"])
	outarr = map_ls_arrays((: is_dir[$1] ? $2+"/" :
				info[$1] ? info[$1][2] ? $2+"*" : $2 :
				$2+"?":), fullpatharr,
			       outarr);

      output += ansi(sprintf("%%^BOLD%%^%%^WHITE%%^%s%%^RESET%%^:\n%-#79s\n\n", 
			     path, implode(outarr, "\n")));
    }


  new(MORE_OB)->more_string(output);
}

nomask int
valid_resend(string ob) {
    return ob == "/trans/cmds/dir";
}
