/* Do not remove the headers from this file! see /USAGE for more info. */

class flow_set
{
    function cond;
    string to;
    string msg_here;
    string msg_there;
    int delay;
}

class flow_set array flow_arr = ({});

void add_hook( string tag, function hook );

void object_action( object obj, string msg, object where )
{
    string array msgs = M_MESSAGES->action(({}), msg, obj);
    msg = msgs[0];
    tell_from_inside( where, msg );
}

void flow( object obj, string msg_here, string msg_there, string go_to )
{
    object to = load_object( go_to );
    if( !obj ) return;
    if( environment( obj ) != this_object() ) return;
    object_action( obj, msg_here, environment(obj));
    object_action( obj, msg_there, to );
    obj->move(to);
}

void fall_through( object obj)
{
    // Not the most efficient of implementations .. a smarter one would
    // sort by delay or something.
    foreach( class flow_set fs in flow_arr )
    {
	if( !evaluate( fs->cond, obj )) continue;

	if( fs->delay )
	    call_out((: flow( $(obj), $(fs->msg_here), $(fs->msg_there), $(fs->to)) :), fs->delay );
	else flow( obj, fs->msg_here, fs->msg_there, fs->to );
    }
}

void set_flow( mapping array flows )
{
    foreach( mapping fs in flows )
    {
	class flow_set fl;
	fl = new(class flow_set);
	if( undefinedp( fl->cond = fs["cond"] ) )
	    error( "cond undefined" );
	if( undefinedp( fl->to = fs["to"] ))
	    error( "to undefined" );
	if( undefinedp( fl->msg_here = fs[ "msg_here"]))
	    error( "msg_here undefined");
	if( undefinedp( fl->msg_there = fs["msg_there"]))
	    error( "msg_there undefined");
	if( undefinedp( fl->delay = fs["delay"]))
	    fl->delay = 0;
	flow_arr += ({ fl });
    }
    add_hook( "object_arrived", (: fall_through :));
}
