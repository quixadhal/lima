/* Do not remove the headers from this file! see /USAGE for more info. */

/* This isn't very clean, and I know it, but I'm in the mood to piss off people
  * by doing things in code they don't want me to do...   Eg, map_arrays =)
  */

//:COMMAND
//USAGE ls [ -a -s -F ] [path]
//
//This command will show you the files and directories in the
//current directory.  You can change the default behavior of ls by
//suppplying a valid path, and the optional - arguments.
//
// -l  --  shows file size in bytes and date last modified
// -s  --  shows the file size (KB) to the left of the file
// -p  --  suppresses / to the right of the directory name (for non-ansi users)
// -n  ..  suppresses "invisible" files (names starting with .)
//
//The flags and path are both optional, if you leave off the path
//ls assumes you meant the current directory.
//You can use wildcards in the path - eg *.h.

inherit M_GLOB;
inherit M_ANSI;
inherit CMD;

mapping		info;

/* these used to be mappings too, but they duplicate info in 'info' */
#define size(x) (info[x] ? info[x][0] : 0)
#define is_dir(x) (size(x) == -2)

// We don't do this for everyone to save LOTS of processing.
private string
make_ansi_string(string file, string path)
{
    string fname = path + file;

    if(is_dir(fname))
        return "%^LS_DIR%^" + file + "%^RESET%^/";
    if(info[fname] && sizeof(info[fname]) == 3 &&
      intp(info[fname][2]) && info[fname][2])
        return "%^LS_LOADED%^" + file + "%^RESET%^*";
    return "%^LS_DEFAULT%^" + file + "%^RESET%^";
}

private string
make_l_flag_line(string basename, string fullname)
{
    if (is_dir(fullname))
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

private string path_join(string s1, string s2) {
    if (s1[<1] == '/')
	return s1 + s2;
    else
	return s1 + "/" + s2;
}

int do_ls(mixed argv, mapping flags)
{
    string 	path;
    mixed 	files;
    string	item;
    string 	*outarr, *fullpatharr;
    int           uses_ansi;

    if(!argv[0])
	argv[0] = glob(evaluate_path("./*")); /* C-mode */
    else
	argv[0] = decompose(map(argv[0], (:is_directory($1) ?
	      glob(path_join($1, "*")) : $1 :)));

    // Changed because this is non-intuitive compared to the rest of the
    // uses of colour -- Tigran.
    //uses_ansi = get_user_variable("colour-ls");
    uses_ansi=i_use_ansi();

    if (uses_ansi || flags["l"] || flags["s"] || !flags["p"])
    {
	info = ([]);
	foreach(item in argv[0]) {
	    info[item] = stat(item);
	    if (sizeof(info[item]) == 1) {
		/* directory */
		info[item] = ({ -2, time(), 0 });
	    }
	}
    }

    argv[0] = map_paths(argv[0]);
    if(flags["n"])
	foreach(path, files in argv[0])
    {
	argv[0][path] = filter(files, (:$1[0] != '.':));
    }
    foreach(path in sort_array(keys(argv[0]),1))
    {
	outarr = argv[0][path];
	// only make this once.
	fullpatharr = map(outarr, (: path_join($(path), $1) :));

	if(uses_ansi)
	{
	    outarr = map(outarr, (: make_ansi_string :), path);
	}
	if(flags["l"])
	    outarr = map_ls_arrays((: make_l_flag_line :), outarr,
	      fullpatharr);
	else
	if(flags["s"])
	    outarr = map_ls_arrays((: (size($2)+1024)/1024 + " " + $1 :),
	      outarr, fullpatharr);

	if(!flags["p"] && !uses_ansi)
	    outarr = map_ls_arrays((: is_dir($1) ? $2+"/" :
		info[$1] ? info[$1][2] ? $2+"*" : $2 :
		$2+"?":), fullpatharr,
	      outarr);

	if(end_of_pipeline())
	    out("%^LS_HEADING%^" + path + "%^RESET%^:\n" +
	      colour_table(outarr, this_user()->query_screen_width()) + "\n" );
	else
	    out(implode(outarr, "\n"));
    }
    if (!sizeof(get_output()))
    {
	out("No matching files.");
	return 0;
    }
    return 1;
}

nomask int
valid_resend(string ob) {
    return ob == "/trans/cmds/dir";
}

private void main(mixed argv, mapping flags)
{
    do_ls(argv, flags);
}

string external_ls(string array files, mapping flags)
{
    mixed		info;

    hello_stdio(0,0,0);
    info = map(files, (:M_GLOB->glob($1):));
    if(!do_ls(({files}), flags))
	return 0;
    return get_output();
}