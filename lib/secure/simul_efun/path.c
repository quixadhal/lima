/* Do not remove the headers from this file! see /USAGE for more info. */

object this_body();

int 
path_exists( string x ){
    return file_size(x) != -1; 
}

int 
is_directory( string x ){
    return x != "" && file_size( x ) == -2; 
}

int 
is_file( string x ){
    return file_size(x) > -1;
}


string* 
split_path( string p ){
    int pos;
    while(p[<1] == '/' && strlen(p) > 1) p = p[0..<2];
    pos = strsrch(p, '/', -1); /* find the last '/' */
    return ({ p[0..pos], p[pos+1..] });
}

string 
base_path( string p ){
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



/*
 * string evaluate_path(string path);
 *
 * Given a path name with the usual . or .. operands, it will parse and
 * return a path based of callers current working directory.
 *
* Justice@Pandora, ZorkMUD
 *
 * Sep 4, 1993
 */

string evaluate_path(string path) {
    string *tree, *new_tree;
    if (!path || path[0] != '/') {
	string *route;
	route = explode(file_name(previous_object()), "/");
	path = (this_body() ? this_body()->query_shell_ob()->get_variable("pwd") : "/" +
	  (sizeof(route) > 2 ? implode(route[1.. sizeof(route)-2], "/") : ""))
	+ "/" + path;
    }
    tree = explode(path, "/") - ({ "" });
    new_tree = ({ });
    while(sizeof(tree)) {
	string temp;
	temp = tree[0];
	tree = (sizeof(tree) > 1 ? tree[1..] : ({ }));
	if (temp[0] == '~') {
	    if ( this_user() )
		new_tree = explode(WIZ_DIR, "/") + ({ (strlen(temp) > 1 ?
		  temp[1..] : this_user()->query_userid()) });
	    continue;
	}
	switch(temp) {
	case "..":
	    new_tree = (sizeof(new_tree) > 1 ?
	      new_tree[0 .. sizeof(new_tree)-2] : ({ }));
	    break;
	case ".":
	    break;
	default:
	    new_tree += ({ temp });
	}
    }
    return "/" + implode(new_tree, "/");
}

string
join_path( string dir, string file ){
    if(dir[<1] != '/') return dir+"/"+file;
    return dir+file;
}

int
is_source( string path )
{
    return is_file(path) && strlen(path)>1 && path[<2..] == ".c";
}

mapping
map_paths( mixed paths ){
    mapping res;
    int i;

    res = ([]);
    paths = map_array( paths, (: split_path :) );
    i = sizeof(paths);
    while(i--)
    {
	if(undefinedp(res[paths[i][0]])) res[paths[i][0]] = ({ paths[i][1] });
	else res[paths[i][0]] += ({paths[i][1]});
    }

    return res;
}


