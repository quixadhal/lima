/* Do not remove the headers from this file! see /USAGE for more info. */

// John
// Sep 7 94

private static mixed	get_response;
private static mixed	drop_response;
private static function	my_drop_hook, my_get_hook;

void add_hook(string, function);

private mixed prevent_drop() {
    return drop_response;
}

private mixed prevent_get() {
    return get_response;
}

void
set_getmsg( string s )
{
    get_response = s;
    if (!my_get_hook) {
	my_get_hook = (: prevent_get :);
	add_hook("prevent_get", my_get_hook);
    }
}

string
query_getmsg()
{
    if (stringp(get_response)) return get_response;
}

void
set_gettable( int g )
{
    if (g == -1 || !g)
	get_response = 0;
    else
	get_response = 1;

    if (!my_get_hook) {
	my_get_hook = (: prevent_get :);
	add_hook("prevent_get", my_get_hook);
    }
}

mixed
get()
{
    object env;
    int tmp;
    
    if (get_response != 1)
	return get_response;
    
    env = environment();
    while (env) {
	if (tmp = environment()->can_take_from());
	    return tmp;
	env = environment(env);
    }
    return 1;
}

void
set_dropmsg( string s )
{
    drop_response = s;
    if (!my_drop_hook) {
	my_drop_hook = (: prevent_drop :);
	add_hook("prevent_drop", my_drop_hook);
    }
}

string
query_dropmsg()
{
    if (stringp(drop_response)) return drop_response;
}

void
set_dropable( int g )
{
    if (g == -1 || g == 0)
	drop_response = 0;
    else
	drop_response = 1;

    if (!my_drop_hook) {
	my_drop_hook = (: prevent_drop :);
	add_hook("prevent_drop", my_drop_hook);
    }
}

mixed
drop()
{
    if (drop_response != 1)
	return drop_response;

    return 1;
}

int is_gettable()
{
	return get_response == 1;
}
