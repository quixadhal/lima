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

private string array
wiz_dir_parts = explode(WIZ_DIR, "/") - ({ "", "." });

string evaluate_path(string path) {
    string *tree;
    int idx;

    if (!path || path[0] != '/') {
	if (this_body())
	    path = this_body()->query_shell_ob()->get_variable("pwd")
		+ "/" + path;
	else {
	    string lname = file_name(previous_object());
	    int tmp = strsrch(path, "/", -1);
	    path = lname[0..tmp-1] + "/" + path;
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


