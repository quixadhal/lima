/* Do not remove the headers from this file! see /USAGE for more info. */

object this_body();
mixed get_user_variable(string varname);

//:FUNCTION cannonical_form
//Change object path names to standard form, stripping the trailing .c, if
//any, the clone number, if any, and making sure the leading / exists.
//
//This function is useful for making sure that alternate forms of the
//pathname match correctly, since:
//cannonical_form("foo/bar") == cannonical_form("/foo/bar.c")
string
cannonical_form(mixed fname) {
    if (objectp(fname)) fname = file_name(fname);
    sscanf(fname, "%s#%*d", fname);
    sscanf(fname, "%s.c", fname);
    if (fname[0] != '/') fname = "/" + fname;
    return fname;
}

int 
path_exists( string x ) {
    return file_size(x) != -1; 
}

int 
is_directory( string x ) {
    return x != "" && file_size( x ) == -2; 
}

int 
is_file( string x ) {
    return file_size(x) > -1;
}


string* 
split_path( string p ) {
    int pos;
    while(p[<1] == '/' && strlen(p) > 1) p = p[0..<2];
    pos = strsrch(p, '/', -1); /* find the last '/' */
    return ({ p[0..pos], p[pos+1..] });
}

string 
base_path( string p ) {
    return split_path(p)[0];   
}

string 
depath( string p ){
    return split_path(p)[1];
}

varargs
void 
walk_dir( string path, function func, mixed arg )
{
    mixed tmp, names, res;
    int i;


    if( !is_directory( path ) ){
	tmp = split_path( path );
	evaluate(func, arg, tmp[0], ({ tmp[1] }) );
	return;
    }
    names = get_dir( path+"/*" );
    names -= ({ ".", ".."});
    res = evaluate(func, path, names, arg);

    if(!res) res = names;
    if(path[<1] != '/') path += "/";
    res = map_array( res, (: $(path) + $1 :) );
    res = filter_array(res, (: file_size($1) == -2 :) );
    i = sizeof( res );
    while(i--) walk_dir( res[i], func, arg );
}


//:FUNCTION canonical_path
//Strip out all "." and ".." forms from a path.  Remove double slashes.
//Ensure the path has a leading slash.
string canonical_path(string path)
{
    string *parts = explode(path, "/") - ({ "", "." });
    int idx;

    if (!sizeof(parts)) return "/";

    while ( (idx = member_array("..", parts)) != -1 )
    {
	if ( idx > 1 )
	    parts = parts[0..idx-2] + parts[idx+1..];
	else if ( idx == 0 )
	    parts = parts[1..];
	else
	    parts = parts[2..];
    }

    return "/" + implode(parts, "/") + (path[<1] == '/' ? "/" : "");
}


/*
 * string evaluate_path(string path);
 *
 * Given a path name with the usual . or .. operands, it will parse and
 * return a path based of callers current working directory.
 *
 * Justice@Pandora, ZorkMUD
 *
 * Sep 4, 1993
 *
 * Rewritten by Beek to be more efficient.
 */

private string array
wiz_dir_parts = explode(WIZ_DIR, "/") - ({ "", "." });

varargs
string evaluate_path(string path, string prepend)
{
    string *tree;
    int idx;

    if (!path || path[0] != '/') {
	if (this_body())
	    path = get_user_variable("pwd")
	    + "/" + path;
	else if(prepend) 
	    path = prepend + "/" + path;
	else {
	    string lname = file_name(previous_object());
	    int tmp = strsrch(lname, "/", -1);
	    path = lname[0..tmp] + path;
	}
    }
    tree = explode(path, "/") - ({ "", "." });
    while (idx < sizeof(tree)) {
	string tmp = tree[idx];
	if (tmp == "..") {
	    if (idx) {
		tree[idx-1..idx] = ({ });
		idx--;
	    } else
		tree[idx..idx] = ({ });
	    continue;
	}
	if (tmp[0] == '~' && this_user()) {
	    if (sizeof(tmp) == 1)
		tmp = this_user()->query_userid();
	    else
		tmp = tmp[1..];
	    tree[0..idx] = wiz_dir_parts + ({ tmp });
	    continue;
	}
	idx++;
    }
    return "/" + implode(tree, "/");
}

string
join_path( string dir, string file ){
    if(dir[<1] != '/') return dir+"/"+file;
    return dir+file;
}

mapping
map_paths( mixed paths ){
    mapping res;
    int i;

    res = ([]);
    paths = map_array( paths, (: split_path :) );
    for(i=0;i<sizeof(paths);i++)
    {
	if(undefinedp(res[paths[i][0]])) res[paths[i][0]] = ({ paths[i][1] });
	else res[paths[i][0]] += ({paths[i][1]});
    }

    return res;
}



varargs string absolute_path( string relative_path, mixed relative_to )
{
    if( !relative_to ) relative_to = previous_object();
    if( relative_path[0] != '/' )
	if( objectp( relative_to ))
	    relative_path = base_path( file_name( relative_to )) + relative_path;
	else if ( stringp( relative_to ))
	    relative_path = relative_to + "/" + relative_path;
	else error( "Invalid relative_to path passed" );
    relative_path = cannonical_form( relative_path );
    relative_path = evaluate_path( relative_path );
    return relative_path;
}

nomask string wiz_dir(mixed what)
{
  if(stringp(what))
    {
#ifdef EXPANDED_WIZ_DIR
      return sprintf("%s/%c/%s",WIZ_DIR,what[0],what);
#else
      return sprintf("%s/%s",WIZ_DIR,what);
#endif
    }
  if(objectp(what))
    {
      string who=what->query_userid();
#ifdef EXPANDED_WIZ_DIR
      return sprintf("%s/%c/%s",WIZ_DIR,who[0],who);
#else
      return sprintf("%s/%s",WIZ_DIR,who);
#endif
    }
}
