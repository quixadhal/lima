/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

#define ITERS_PER_CALL	2

#define DATA_FILE	"/data/find.codes"
#define TMP_DATA_FILE	"/data/find.tmp"

mixed array stack;
string path;
int building_database = 0;

private void end_building()
{
    cp(TMP_DATA_FILE,DATA_FILE);
    rm(TMP_DATA_FILE);
    printf("Locate DB build done.\n");
    building_database = 0;
}

private void do_building()
{
    int i = ITERS_PER_CALL;
    string array	this_dir;

    while(i--)
    {
	string new_path;

	if(!sizeof(stack))
	{
	    end_building();
	    return;
	}

	if(intp(stack[0]))
	{
	    /* hit the sentinel. pop up one directory level */
	    stack = stack[1..];
	    path = split_path(path)[0];
	    i++;
	    continue;
	}

	new_path = path + stack[0]+"/";
	stack = stack[1..];
	this_dir = get_dir(new_path+"*",-1);
	if(arrayp(this_dir))
	    this_dir = this_dir[2..];
	else
	    continue;

	/* prepend the dir and write into the file */
	map(this_dir,(:write_file(TMP_DATA_FILE,
	      join_path($(new_path),$1[0])+"\n"):));

	/* filter out just the dirs and extract the names */
	this_dir = map(filter(this_dir, (:$1[1] == -2:)), (:$1[0]:));

	/* push onto stack w/ a sentinel to go up the dir tree */
	if(sizeof(this_dir))
	{
	    stack = this_dir + ({0}) + stack;
	    path = new_path;
	}
    }
    call_out((:do_building:), 2);
}

private void begin_database_build()
{
    stack = ({""});
    path = "";
    if(is_file(TMP_DATA_FILE) && !rm(TMP_DATA_FILE))
    {
	out("You don't have permission to do that.\n");
	return;
    }
    do_building();
    building_database = 1;
}


private void main(mixed * arg, mapping flags) 
{
    string  find;
    string outstr;

    if (!arg[0] && !flags["u"])
    {
	out( "Syntax: findfile <string>\n");
	return;
    }
    if(flags["u"])
    {
	if(building_database)
	{
	    out("Already building database.\n");
	    return;
	}
	out("Building database... This takes a while.\n");
	begin_database_build();
	return;
    }
    else
    {
	/* arg[0] is an array of strings. use just the first */
	find = arg[0];
	ed_start(DATA_FILE);
	outstr = ed_cmd(sprintf("1,$g#%s#p", find));
if( !sizeof(outstr))
        out( "No matching files found.\n");
else
    out( outstr );
	ed_cmd("q");
	return;
    }
}
