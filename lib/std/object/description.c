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
private static string proper_name;
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

string extra_short() { return 0; }
string extra_long() { return ""; }


nomask void set_proper_name(string str)
{
    proper_name = str;
}

void set_unique(int x)
{
    unique = x;
}

int query_unique()
{
    return 0;
}


string short()
{
    string extra;

    if (extra = extra_short())
        extra = " (" + extra + ")";
    else
        extra = "";

    if (!proper_name)
    {
	if (!sizeof(ids))
	    return "nondescript thing";

	if (!sizeof(adj))
	    return ids[0] + extra;

	return adj[0] + " " + ids[0] + extra;
    }
    
    return proper_name + extra;
}

string plural_short() {
    return pluralize(short());
}

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

string the_short() {
    if (!proper_name) return "the "+short();
    return proper_name;
}

string a_short() {
    if (unique) return the_short();
    if (!proper_name) return add_article(short());
    return proper_name;
}

nomask void set_long(mixed str)
{
    long = str;
    if ( functionp(long) ) return;
    if ( long == "" || long[<1] != '\n' ) long += "\n";
}

string long()
{
    string extra;
    string res;

    res = evaluate(long);
    if (!res)
	return "You see nothing special about " + the_short() + "\n";

    if ( extra = extra_long() )
	return res + extra;

    return res;
}

int
id(string arg){
    string	my_name;

    return member_array(arg,ids) != -1;
}

nomask string *parse_command_id_list();

string *query_id(){
    return parse_command_id_list();
}

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
    string my_name;

    fake_ids = this_object()->fake_item_id_list();
    if ( !fake_ids )
	fake_ids = ({ });

    if ( !this_user() )
	return ids + fake_ids;

    my_name = this_user()->query_real_name();
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

void add_plural( string plural )
{
    if( !stringp( plural ) )
        return;

    if( !pointerp( plurals ) )
        plurals = ({});

    plurals += ({ plural });
    parse_refresh();
}

void add_id_no_plural( string id ) {
    if (!stringp(id))
        return;
    if (!pointerp(ids))
	ids = ({ id });
    else ids += ({ id });
    parse_refresh();
}

void add_id( string id )
{
    add_id_no_plural(id);
    add_plural( pluralize( id ) );
}

static
void remove_id( string id )
{
    ids -= ({ id });
    parse_refresh();
}

void set_adj( mixed new_adjs ) 
{
    if( stringp( new_adjs ) )
	adj = ({new_adjs});

    else if( pointerp( new_adjs ) )
	adj = new_adjs;
    parse_refresh();
}

string* query_adj()
{
	return copy(adj);
}

void set_adjectives( mixed new_adjs ){
    set_adj( new_adjs );
    parse_refresh();
}

void
add_adj( string new_adj )
{
  if( !pointerp(adj) )
    adj = ({});

  if( stringp(new_adj) )
    adj += ({ new_adj });
    parse_refresh();
}

static string *discarded_message = ({
  "Someone has left %s lying on the ground.",
  "It seems that someone has left %s lying here.",
  "%s lies here, discarded.",
  "%s lies at your feet.",
  "Some luckless fool has left %s here.",
  "%s lies here, abandoned.",
});

static string *plural_discarded_message = ({
  "Someone has left %s lying on the ground.",
  "It seems that someone has left %s lying here.",
  "%s lie here, discarded.",
  "%s lie at your feet.",
  "Some luckless fool has left %s here.",
  "%s lie here, abandoned.",
});

string untouched_long() {
    return untouched_long;
}

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

	return sprintf( plural_discarded_message[random(sizeof(plural_discarded_message))],
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

    return sprintf( discarded_message[random(sizeof(discarded_message))],
      str );
}

static void set_in_room_desc( string arg )
{
  in_room_desc = arg;
}


static void set_primary_id( string arg ) { primary_id = arg; }

string query_primary_id()
{
    if( !primary_id ) return sizeof(ids) ? ids[0] : "thing";
    return primary_id;
}

void set_plural_in_room_desc( string arg ){ plural_in_room_desc = arg; }

string query_posessive(){  return "its"; }

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
    return proper_name;                                     
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
