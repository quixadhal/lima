/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit M_GRAMMAR;
inherit __DIR__ "names";

/* Our descriptions:
 * long:   should be a complete sentence or a closure.
 * in_room_desc: A longer string for rooms.  Titles for players.
 * plural_in_room_desc: Message for more than 1.
 */
/* if these are zero, sane defaults are used based on 'names'. */
private static mixed long;
private static mixed in_room_desc;
private static string plural_in_room_desc;
/* our description when we haven't been touched.  Really an
 * in_room_desc, not a long */
private string untouched_long;

/* these are somewhere else... */
int test_flag(int);
int is_visible();
int is_attached();

//:FUNCTION set_long
//Set the long description of an object
nomask void set_long(mixed str)
{
    long = str;
    if ( functionp(long) ) return;
    if ( long == "" || long[<1] != '\n' ) long += "\n";
}

//:FUNCTION long
//Return the verbose description of an object that you see when you look
//at it.
string long()
{
    string extra;
    string res;

    if(!is_visible())
      return "Funny, you don't see anything at all.";

    res = evaluate(long);
    if (!res)
	return "You see nothing special about " + the_short() + "\n";

//:HOOK extra_long
//The returned strings are added on to the end of the long description.
    return res + (call_hooks("extra_long", (: $1 + $2 :), "") || "");
}

static string array discarded_message, plural_discarded_message;

string untouched_long() {
    return untouched_long;
}

//:FUNCTION show_in_room
//Return a string appropriate for inclusing in a room long description.
//Note that duplicatep() objects return nothing.
string show_in_room()
{
    string str;
    int our_count;
    if(!is_visible()) return 0;
    /* If an object is attached, it is considered part of its
     * parent and it's the parent's responsibility to print an
     * appropriate description.  For example, wielded weapons are
     * attached and should be included in the monster's description.
     * c.f. Zork's:
     *    A nasty-looking troll, brandishing a bloody axe, blocks all
     *         passages out of the room.
     */
    if (is_attached()) return 0;
    our_count = count();
    if (our_count > 4) {
	if (plural_in_room_desc)
	    return sprintf( plural_in_room_desc, "many");

	str = this_object()->short();
	if (!str) return 0;

	return "There are many "+plural_short()+" here.\n";
    }
    if (our_count > 1 )
    {
	if( plural_in_room_desc ) 
	    return sprintf( plural_in_room_desc, our_count+"");

	str = this_object()->short();
	if( !str ) 
	    return 0;

	//	if( query_ob_flag( AUTO_IN_ROOM_DESC ) )

	if (!plural_discarded_message)
	    plural_discarded_message = MESSAGES_D->get_messages("discarded-plural");
	return capitalize(sprintf( choice(plural_discarded_message),
		       sprintf("%d %s", our_count, plural_short())));
    }

    if (!test_flag(TOUCHED) && (str = untouched_long()))
	return str;

    if( in_room_desc )
	return evaluate(in_room_desc);

    str = this_object()->a_short();

    if( !str )
	return 0;

    //    if( query_ob_flag( AUTO_IN_ROOM_DESC ) )

    if (!discarded_message)
	discarded_message = MESSAGES_D->get_messages("discarded");
    
    return capitalize(sprintf( choice(discarded_message), str ));
}

//:FUNCTION set_in_room_desc
//Set the description an object has when it is sitting in a room
static void set_in_room_desc( string arg )
{
  in_room_desc = arg;
}

void set_plural_in_room_desc( string arg ){ plural_in_room_desc = arg; }

string query_posessive(){  return "its"; }

//:FUNCTION
//set the untouched description of the object.  This is the way the object
//is described originally, but not after is has been taken or moved.
void set_untouched_desc(string arg){
    if(stringp(arg)) untouched_long = arg;
}

/* todo: this won't work with extra_long, long closures, etc */
//### should be removed
static
string shitty_hack_to_get_object_long()
{
    if (!long) return "You see nothing special about " + the_short() + "\n";   
    return long;                                                               
}

void
create()
{
     names::create();
}

string query_in_room_desc()
{
    if(!is_visible()) 
      return "";
    return (string)evaluate(in_room_desc);
}
