/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** master.c -- the MudOS Master object
*/

#include <config.h>
#include <daemons.h>
#include <mudlib.h>
#include <security.h>


private mapping errors = ([]);

object compile_object(string path)
{
    /* ### virtual object stuff here */
    return 0;
}

private void crash()
{
    shout("Game Driver shouts: Ack! I think the game is crashing!\n");
    users()->quit();
}

nomask string get_player_fname()
{
    string err;
    object ob;

    if ( err = catch(ob = load_object(USER_OB)) || !ob )
    {
	if ( !ob )
	    err = "File '" + USER_OB + "' does not exist!";
	write("Main login ob not loadable!\n" + err + "\n");
	if ( err = catch(ob = load_object(USER_OB_BAK)) )
            write("Spare login obj not loadable!\n" + err + "\n");
        else if ( !ob )
	    write("Spare login ob (" + USER_OB_BAK + ") does not exist!\n");
	else
            return USER_OB_BAK;
    }
    else
        return USER_OB;

    return 0;
}

object connect()
{
    object ob;
    string ret;

    ret = catch(ob = clone_object(get_player_fname()));
    write("\n");
    if ( ret )
    {
        write(ret + "\n");
        return 0;
    }
    return ob;
}

int different(string fn, string pr) {
    sscanf(fn, "%s#%*d", fn);
    fn += ".c";
    return (fn != pr) && (fn != ("/" + pr));
}

string trace_line(object obj, string prog, string file, int line) {
    string ret;
    string objfn = obj ? file_name(obj) : "<none>";

    ret = objfn;
    if (different(objfn, prog)) ret += sprintf(" (%s)", prog);
    if (file != prog) ret += sprintf(" at %s:%d\n", file, line);
    else ret += sprintf(" at line %d\n", line);
    return ret;
}

varargs string standard_trace(mapping mp, int flag) {
    string obj, ret;
    mapping *trace;
    int i,n;

    ret = mp["error"] + "Object: " + trace_line(mp["object"], mp["program"],
					       mp["file"], mp["line"]);
    ret += "\n";
    trace = mp["trace"];

    n = sizeof(trace);
    
    for (i=0; i<n; i++) {
	if (flag) ret += sprintf("#%d: ", i);

	ret += sprintf("'%s' at %s", trace[i]["function"],
		       trace_line(trace[i]["object"], trace[i]["program"],
				  trace[i]["file"], trace[i]["line"]));
    }
    return ret;
}

string error_handler(mapping mp, int caught)
{
    string ret;
    string logfile = (caught ? "/log/catch" : "/log/runtime");
    string what = mp["error"];
    
    ret = "---\n" + standard_trace(mp);
    write_file(logfile, ret);

    /* If an object didn't load, they get compile errors.  Don't spam
       or confuse them */
    if (what[0..23] == "*Error in loading object")
	return;
    
    if ( this_user() )
    {
	printf("%sTrace written to %s\n", what, logfile);
	errors[this_user()->query_userid()] = mp;
    }
    errors["last"] = mp;
    
    // Strip trailing \n, and indent nicely
    what = replace_string(what[0..<2], "\n", "\n         *");
    NCHANNEL_D->deliver_string("wiz_errors",
		       sprintf("[errors] Error logged to %s\n[errors] %s\n[errors] %s\n",
			       logfile,
			       what,
			       trace_line(mp["object"], mp["program"],
					  mp["file"], mp["line"])));

    return 0;
}

mapping query_error(string name)
{
/* This MUST be secure */
    if (!check_privilege(1)) return 0;
    return errors[name];
}

string ws = " \t\n";

/* don't use the string module, it's dangerous to do that here :) */
string trim_spaces(string str)
{
    int i=0,j=strlen(str)-1;
    while (i<=j && member_array(str[i], " \t\n") != -1) i++;
    while (i<=j && member_array(str[j], " \t\n") != -1) j--;
    return str[i..j];
}

mixed name_of_file_owner(string object_name)
{
    string wiz_name;
    string start, trailer;

    if ( sscanf(object_name, WIZ_DIR"/%s", wiz_name) == 1 )
    {
        if ( sscanf(wiz_name, "%s/%s", start, trailer) == 2 )
            wiz_name = start;
        return wiz_name;
    }
    else if (object_name[0..3] != "ftp/" && object_name[0..4] != "open/")
    {
        /* no creator, but legal. */
        return 1;
    }
}

// Handle tabs and non-printables; unfortunately UNIX tabs are indent
// sensitive; this means that strlen(str) depends where we are on the line ...
private int funky_strlen(string str, int start) {
    int l, n, pos = start;
    l = n = strlen(str);
    for (int i = 0; i < n; i++) {
	if (str[i] == '\t') {
	    int z = 8 - (pos % 8);
	    l += z;
	    pos += z;
	}
	if (str[i] < 32 || str[i] > 126) {
	    l--;
	    continue;
	}
	pos++;
    }
    return l;
}

#define WIDTH 75
#define INDENT "## "

private void report_context(string src, int line, string context) {
    string txt;
    int len, clen;
    int pos = strlen(INDENT);

    if (!context || context == "") return;
    if (context[<1] == '\n') context = context[0..<2];
    
    src = read_file((src[0] == '/' ? src : "/" + src), line, 1);
    if (src == "") return;
    if (src[<1] != '\n')
	src += "\n";
    // HACK WARNING:
    // If there are nonprintables, we might not slash off enough, hence
    // the while.  If there are tabs, we might slice off too much.
    txt = src;
    while ((len = funky_strlen(txt, pos)) > WIDTH)
	txt = txt[0..<len-(WIDTH - 5)] + "...\n";
    // END HACK.  RESUME NORMAL DRIVING
    write(INDENT + txt);
    if (context == "the end of line")
	context = "\n";

    if (sscanf(src, "%s" + context + "%s", txt, src) != 2)
	return;
    len = funky_strlen(txt, pos);
    pos += len;

    if (pos <= WIDTH) {
	printf(INDENT "%-*' 's", len, "");
	clen = funky_strlen(context, pos);
	pos += clen;
	printf("^%-*' 's", clen - 1, "");
	while (sscanf(src, "%s" + context + "%s", txt, src) == 2) {
	    len = funky_strlen(txt, pos);
	    pos += len;
	    clen = funky_strlen(context, pos);
	    pos += clen;
	    if (pos > WIDTH) {
		if (pos - len - clen + 1 <= WIDTH)
		    write("^");
		break;
	    }
	    printf("%-*' 's^%-*' 's", len, "", clen - 1, "");
	}
    }
    write("\n");
}

void log_error(string file, string message)
{
    string name;
    string where, err, context;
    string src, txt;
    int line;

    name = name_of_file_owner(file);

    if( file[0..3] != "tmp/" && this_user() && wizardp(this_user()))
    {
	if (sscanf(message, "%s: %s before %s", where, err, context) == 3)
	{
#ifdef OLD_STYLE_COMPILATION_ERRORS
	    printf(INDENT "Compilation error: %s\n   %s, before [%s]\n",
		   err, where, trim_spaces(context));
#else
	    /* for safety; it'll look wierd but have the right info */
	    src = where; 
	    sscanf(where, "%s line %d", src, line);
	    printf(INDENT "------- %s:%i\n## %s\n", src, line, err);
	    report_context(src, line, context);
#endif
	}
	else
	{
#ifdef OLD_STYLE_COMPILATION_ERRORS
	    sscanf(message, "%s: %s", where, err);
	    printf(INDENT "Compilation error: %s   %s\n", err, where);
#else
	    /* for safety; it'll look wierd but have the right info */
	    src = where; 
	    sscanf(where, "%s line %d", src, line);
	    printf(INDENT "------- %s:%i\n## %s\n", src, line, err);
#endif
	}
    }

    if ( intp(name) )
	write_file(LOG_DIR"/log",message);
    else
	write_file(WIZ_DIR "/" + name + "/log", message);
}

int save_ed_setup(object who, int code) {
    string file;

    if (!intp(code))
        return 0;
    file = wiz_dir(who) + "/.edrc";
    rm(file);
    return write_file(file, code + "");
}

int retrieve_ed_setup(object who) {
    string file;
    int code;

    file = wiz_dir(who) + "/.edrc";
    if (file_size(file) <= 0)
        return 0;
    sscanf(read_file(file), "%d", code);
    return code;
}

void destruct_env_of(object ob)
{
    mixed error;

    /* ### needs work to improve speed? */
    if ( !ob->query_link() )
	return;

    if ( error = catch(ob->move(VOID_ROOM)) )
    {
        write(error);
    }
}

int valid_shadow(object ob)
{
    return (int)ob->query_allow_shadow(previous_object());
}

/*
 * Default language functions used by parse_command() 
 */
string *parse_command_id_list()
{
    return ({ "one", "thing" });
}

string *parse_command_plural_id_list()
{
    return ({ "ones","stuff", "things", });
}

string *parse_command_adjectiv_id_list()
{
    return ({});
}

string *parse_command_prepos_list()
{
    return ({ "in","from","on","under","behind","beside","of","for","to",
	      "with", "at", "off", "out", "down", "up", "around", "over",
	      "into", "about", });
}

string parse_command_all_word()
{
    return "all";
}

object *parse_command_users()
{
    return users()->query_body();
}

string get_save_file_name(string file)
{
    file = "/" + file + "%";
    rm( file );
    return file;
}

string make_path_absolute(string path)
{
    return evaluate_path(path);
}

int valid_override(string file, string efun_name)
{
    // M_GRAMMAR overrides this
    if (efun_name == "pluralize") return 1;
    /*
    ** The simul efun ob can use efun:: all it wants.  Other objects are
    ** completely restricted.
    */
    return file == "/secure/simul_efun/overrides";
}
    
int valid_socket(object ob, string what, mixed * info)
{
    string fname;

    sscanf(file_name(ob), "%s#%*s", fname);
    if ( fname == SOCKET )
    {
	return 1;
    }
    return 0;
}


void remove()
{
    destruct(this_object());
}

private nomask void preload( string file )
{
    object o;

    write("Preloading: "+file+"...  ");
    catch(o = load_object(file));
    if ( objectp(o) )
	write("success\n");
    else
	write("FAILED\n");
}

string* epilog( int eflag )
{
    return filter_array(explode(read_file(PRELOAD_FILE), "\n") - ({ "" }),
			(: $1[0] != '#' :));
}

string object_name( object ob )
{
    if ( !ob )
	return "<destructed>";

    if ( interactive(ob) )
	return ob->query_userid();

    if ( ob->is_living() )
	if(ob = ob->query_link()) return ob->query_userid()+"'s body";
	else
        return "NPC";
}



#if 0
mixed valid_write(string path, string eff_user, string call_fun, object caller)
{
    return 1;
}

mixed valid_read(string path, string eff_user, string call_fun, object caller)
{
    return 1;
}
#else
#pragma strict_types

mixed valid_write(mixed path, object caller, string call_fun)
{
    mixed foo;

    if (caller == this_object())
        return 1;
    path = evaluate_path(path);

    if (SECURE_D->check_privilege(SECURE_D->query_protection(path,1),1))
        return path;

    write_file(ACCESS_LOG,
	       (this_user() ? this_user()->query_userid() : "<Master>") +
	       ": attempted to write " + path + "\n");

    printf("Bad file name: %s.\n",path);
    return 0;
}

mixed valid_read(string path, object caller, string call_fun)
{
    mixed foo;

//write_file("/log/tmplog",sprintf("%O: %s for %s\n",caller,path,call_fun));

    if (caller == this_object())
        return 1;
    if (file_name(caller)==SECURE_D && call_fun == "restore_object")
        return 1;

    path = evaluate_path(path);
    if (SECURE_D->check_privilege(SECURE_D->query_protection(path,0),0))
        return path;

    write_file(ACCESS_LOG,
	       (this_user() ? this_user()->query_userid() : "<Master>") +
	       ": attempted to read " + path + "\n");

    printf("Bad file name: %s.\n",path);
    return 0;
}

#endif

int valid_bind(object binder, object old_owner, object new_owner)
{
    return check_privilege(get_protection(base_name(new_owner), "wf"));
}

int valid_save_binary()
{
    return 1;
}
