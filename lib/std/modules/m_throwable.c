/* Do not remove the headers from this file! see /USAGE for more info. */

// John
// Sep 7 94

private static mixed	throw_response = 1;
private static function	my_throw_hook;

void add_hook(string, function);
mixed move (object);

private mixed prevent_throw() 
{
    return throw_response;
}

void
set_throwmsg( string s )
{
    throw_response = s;
    if (!my_throw_hook) {
	my_throw_hook = (: prevent_throw :);
	add_hook("prevent_throw", my_throw_hook);
    }
}

string
query_throwmsg()
{
    if (stringp(throw_response)) return throw_response;
}

void
set_throwable( int g )
{
    if (g == -1 || !g)
	throw_response = 0;
    else
	throw_response = 1;

    if (!my_throw_hook) {
	my_throw_hook = (: prevent_throw :);
	add_hook("prevent_throw", my_throw_hook);
    }
}

mixed throw(object target)
{
    object env;
    mixed tmp;
    
    if (throw_response != 1)
	return throw_response;
    
    env = environment();
    tmp = move(environment(this_body()));
    if (tmp == 1)
    {
	if(!target)
	{
	    this_body()->simple_action("$N $vthrow a $o.", this_object());
	    return 1;
	}
	else
	{
	    // Add skill stuff here...
	    this_body()->targetted_action("$N $vthrow a $o at $t.  It bounces off of $p1 head, and lands in front of $p1 feet.\n", target, this_object());

	    return 1;
	}
    }
}

int is_throwable()
{
	return throw_response == 1;
}






