/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit M_GRAMMAR;

/* grammar related stuff */
private static string * ids;
private static string * plurals;
private static string * adj;
/* unique objects are refered to as 'the' instead of 'a' */
private static int unique;
/* Our descriptions:
 * proper_name: Propper_name should only be set for objects who should not
 *     be refered to as "a xxx" or "the xxx"
 * long:   should be a complete sentence or a closure.
 * in_room_desc: A longer string for rooms.  Titles for players.
 * plural_in_room_desc: Message for more than 1.
 */
/* if these are zero, sane defaults are used based on grammar above. */
private static mixed proper_name;
private static mixed long;
private static mixed in_room_desc;
private static string plural_in_room_desc;
/* This is sorta redundant.  If you don't set it, ids[0] is used. */
private string primary_id;
/* our description when we haven't been touched.  Really an
 * in_room_desc, not a long */
private string untouched_long;


/*
** Can be implemented by subclasses to provide additional stuff
*/
string * fake_item_id_list();
/* this is somewhere els.  Dunno where ... */
int test_flag(int);

//:FUNCTION set_proper_name
//Set the proper name of an object.  Objects with proper names never have
//adjectives added in front of their names.
nomask void set_proper_name(string str)
{
    proper_name = str;
}

//:FUNCTION set_unique
//Unique objects are always refered to as 'the ...' and never 'a ...'
void set_unique(int x)
{
    unique = x;
}

//:FUNCTION
//Return the value of 'unique'
int query_unique()
{
    return unique;
}

varargs mixed call_hooks(string, mixed, mixed);

string calculate_extra() {
//:HOOK extra_short
//The non-zero return values are added on to the end of the short descriptions
//when inv_list() is used (surrounded by parenthesis)
    return  call_hooks("extra_short", 
		       function(string sofar, mixed this) {
			   if (!this) return sofar;
			   return sofar + " (" + this + ")";
		       }
		       , "");
}

//:FUNCTION short
//Return the 'short' description of a object, which is the name by which
//it should be refered to
string short()
{
    if (!proper_name)
    {
	if (!sizeof(ids))
	    return "nondescript thing";

	if (!sizeof(adj))
	    return ids[0];

	return adj[0] + " " + ids[0];
    }
    
    return evaluate(proper_name);
}

//### obsolete?
string plural_short() {
    return pluralize(short());
}

//### should be somewhere else?
string add_article(string str) {
    switch (str[0]) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
	return "an "+str;
    }
    return "a "+str;
}

//:FUNCTION the_short
//return the short descriptions, with the word 'the' in front if appropriate
string the_short() {
    if (!proper_name) return "the "+short();
    return evaluate(proper_name);
}

//:FUNCTION a_short
//return the short descriptions, with the word 'a' in front if appropriate
string a_short() {
    if (unique) return the_short();
    if (!proper_name) return add_article(short());
    return evaluate(proper_name);
}

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

    res = evaluate(long);
    if (!res)
	return "You see nothing special about " + the_short() + "\n";

//:HOOK extra_long
//The returned strings are added on to the end of the long description.
    return res + call_hooks("extra_long", (: $1 + $2 :), "");
}

int
id(string arg){
    string	my_name;

    return member_array(arg,ids) != -1;
}

nomask string *parse_command_id_list();

//:FUNCTION query_id
//Returns an array containing the ids of an object
string *query_id(){
    return parse_command_id_list();
}

//:FUNCTION set_id
//Set the ids of an object to the given string or array of strings, and
//also set the plurals
void set_id(mixed arg){
    if(!stringp(arg) && !pointerp(arg)) return;
    if (stringp(arg))  ids = ({ arg });
    else ids = arg;
    plurals = map_array( ids, (: pluralize :) );
    parse_refresh();
}

int plural_id( mixed arg ){
    return member_array(arg, plurals) != -1;
}

string *parse_command_id_list()
{
    string * fake_ids;

    fake_ids = this_object()->fake_item_id_list();
    if ( !fake_ids )
	fake_ids = ({ });

    if ( !this_user() )
	return ids + fake_ids;

    if ( !pointerp(ids) )
	ids = ({ });
    return ids + fake_ids;
}

nomask string *parse_command_plural_id_list(){
    return plurals;
}

nomask string *parse_command_adjectiv_id_list(){
    return adj;
}

//:FUNCTION add_plural
//Add a plural id
void add_plural( string plural )
{
    if( !stringp( plural ) )
        return;

    if( !pointerp( plurals ) )
        plurals = ({});

    plurals += ({ plural });
    parse_refresh();
}

//:FUNCTION add_id_no_plural
//Add an id without adding the corresponding plural
void add_id_no_plural( string id ) {
    if (!stringp(id))
        return;
    if (!pointerp(ids))
	ids = ({ id });
    else ids = ({ id }) + ids;
    parse_refresh();
}

//:FUNCTION add_id
//Add an id and it's corresponding plural
void add_id( string id )
{
    add_id_no_plural(id);
    add_plural( pluralize( id ) );
}

//:FUNCTION remove_id
//Remove the given id
static
void remove_id( string id )
{
    ids -= ({ id });
    parse_refresh();
}

//:FUNCTION set_adj
//Set the adjectives to the given string or array of strings
void set_adj( mixed new_adjs ) 
{
    if( stringp( new_adjs ) )
	adj = ({new_adjs});

    else if( pointerp( new_adjs ) )
	adj = new_adjs;
    parse_refresh();
}

//:FUNCTION query_adj
//return the adjectives
string* query_adj()
{
    return adj;
}

//:FUNCTION set_adjectives
//calls set_adj
void set_adjectives( mixed new_adjs ){
    set_adj( new_adjs );
}

//:FUNCTION add_adj
//Add an adjective
void
add_adj( string new_adj )
{
  if( !pointerp(adj) )
    adj = ({});

  if( stringp(new_adj) )
    adj += ({ new_adj });
    parse_refresh();
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
    if(test_flag(INVIS)) return 0;
    /* If an object is attached, it is considered part of its
     * parent and it's the parent's responsibility to print an
     * appropriate description.  For example, wielded weapons are
     * attached and should be included in the monster's description.
     * c.f. Zork's:
     *    A nasty-looking troll, brandishing a bloody axe, blocks all
     *         passages out of the room.
     */
    if (test_flag(ATTACHED)) return 0;
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
	return sprintf( choice(plural_discarded_message),
		       sprintf("%d %s", our_count, plural_short()));
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
    
    return sprintf( choice(discarded_message), str );
}

//:FUNCTION set_in_room_desc
//Set the description an object has when it is sitting in a room
static void set_in_room_desc( string arg )
{
  in_room_desc = arg;
}

//:FUNCTION set_primary_id
//Set the primary id of an object
static void set_primary_id( string arg ) { primary_id = arg; }

//:FUNCTION query_primary_id
//Return the primary id of an object
string query_primary_id()
{
    if( !primary_id ) return sizeof(ids) ? ids[0] : "thing";
    return primary_id;
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
static
string shitty_hack_to_get_object_long()
{
    if (!long) return "You see nothing special about " + the_short() + "\n";   
    return long;                                                               
}

static
string shitty_hack_to_get_object_short()
{
    if (!proper_name) {                                     
        if (!sizeof(ids)) return "nondescript thing"; 
        if (!sizeof(adj)) return ids[0];              
        return adj[0] + " " + ids[0];                 
    }                                                 
    return evaluate(proper_name);                                     
}

void
create()
{
  ids = ({});
  plurals = ({});
  adj = ({});
}

string query_in_room_desc()
{
  return (string)evaluate(in_room_desc);
}
