/* Do not remove the headers from this file! see /USAGE for more info. */

//   A new _call.c, written 5/2/94   by Rust
#include <mudlib.h>

inherit DAEMON;



mixed prepare_parts(string arg)
{
    function    is_wizard, is_admin, is_monster, is_player;
    mixed               tmp;

    is_wizard = (: wizardp :);
    is_admin = (: GROUP_D->is_admin($1) :);
    is_monster = (: !interactive($1) :);
    is_player = (: !wizardp($1) :);

    if(arg[0] == '\\' && strlen(arg)>1) return arg[1..];
    switch( arg )
    {
    case "users" :
        return users();
    case "wiz" :
        return filter_array( users(), is_wizard );
    case "adm" :
        return filter_array( users(), is_admin );
    case "link" :
        return ({ this_player()->query_link() });

    case "pla" :
        return filter_array( users(), is_player );
    }
    
    arg = eval( arg );
    
    if( stringp(arg) && tmp = get_object( arg ) ) 
        arg = tmp;
    
    return arg;
}



int main( string arg )
{
    mixed               parts;
    mixed               ret;
    string              fun;
    mixed               obs;
    object              ob;
    
    if ( !arg || arg == "" )
        return 0;
    
    parts = explode(arg, ";");
    if ( sizeof(parts) < 2 )
    {
        write("call requires arguments separates by a semicolon.\n");
        return 0;
    }
    
    fun = parts[1];
    obs = prepare_parts(parts[0]);
    if (!pointerp(obs)) obs = ({ obs });
    obs = filter(obs, (: objectp :));
    if (!sizeof(obs)) {
        write("call: no target(s) found.\n" );
        return 1;
    }
    parts = map_array( parts[2..], (: prepare_parts :) );

    foreach (ob in obs) {
        ret = call_other(ob, fun, parts...);
        printf("%O->%s() = %O\n", ob, fun, ret);
    }
    return 1;
}
