/* Do not remove the headers from this file! see /USAGE for more info. */

/* grammar related stuff */
private static string array ids;
private static string array plurals;
private static string array adjs;

private string primary_id, primary_adj;

/* calculated internally */
private static mixed internal_short;
/* unique objects are refered to as 'the' instead of 'a' */
private static int unique;
/*
 * proper_name: Proper_name should only be set for objects who should not
 *     be refered to as "a xxx" or "the xxx"
 */
private static mixed proper_name;


/*
** Can be implemented by subclasses to provide additional stuff dynamically
*/
string array fake_item_id_list();
int is_visible();
string invis_name();
int test_flag(mixed);

varargs mixed call_hooks(string, mixed, mixed);
private void resync();

void create() 
{
    parse_init();
    ids = ({});
    plurals = ({});
    adjs = ({});
    resync();
}

//:FUNCTION set_proper_name
//Set the proper name of an object.  Objects with proper names never have
//adjectives added in front of their names.
nomask void set_proper_name(string str)
{
    proper_name = str;
    resync();
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

private void resync() {
    if (!proper_name) {
	if (!primary_id && sizeof(ids))
	    primary_id = ids[0];

	if (primary_id) {
	    if (primary_adj)
		internal_short = primary_adj + " " + primary_id;
	    else
		internal_short = primary_id;
	} else {
	    internal_short = "nondescript thing";
	}
    } else
	internal_short = proper_name;
    parse_refresh();
}

//:FUNCTION short
//Return the 'short' description of a object, which is the name by which
//it should be refered to
string short()
{
    if(!is_visible())
	return invis_name();
    return evaluate(internal_short);
}

//### obsolete?
//### I really don't think so.  Isn't it used by multiple copies of the same object ?
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
    if(!is_visible())
	return invis_name();

    if (!proper_name) return "the "+short();
    return evaluate(proper_name);
}

//:FUNCTION a_short
//return the short descriptions, with the word 'a' in front if appropriate
string a_short() {
  if(!is_visible())
      return invis_name();

  if (unique) return the_short();
  if (!proper_name) return add_article(short());
  return evaluate(proper_name);
}

/****** the id() functions ******/
int
id(string arg) {
    string	my_name;

    return member_array(arg,ids) != -1;
}

int plural_id( mixed arg ) {
    return member_array(arg, plurals) != -1;
}

// In the following, id handles _both_ id and plural, except in the _no_plural
// cases.
/****** add_ ******/

//:FUNCTION add_adj
//Add an adjective.  The first adjective becomes the primary adjective.
//See add_secondary_id if you don't want that.
void
add_adj(string array adj... )
{
    if(!arrayp(adjs))
      adjs = adj;
    else
      adjs += adj;
    resync();
}

//:FUNCTION add_plural
//Add a plural id
void add_plural( string array plural... )
{
    if(!arrayp(plurals))
      plurals = plural;
    else 
      plurals += plural;
    resync();
}

//:FUNCTION add_id_no_plural
//Add an id without adding the corresponding plural
void add_id_no_plural( string array id... ) {
    // set new primary
    if(!arrayp(ids))
      ids = id;
    else
      ids += id;
    resync();
}

//:FUNCTION add_id
//Add an id and it's corresponding plural
void add_id( string array id... )
{
    if(!arrayp(ids))
      ids = id;
    else
      ids += id;
    plurals += map(id, (: pluralize :));
    resync();
}

/****** set_ ******/
//These actually add, but the first argument becomes the primary id/adjective
void set_id( string array id... ) {
    ids += id;
    plurals += map(id, (: pluralize :));
    primary_id = id[0];
    resync();
}

void set_adj( string array adj... ) {
    if(!arrayp(adjs))
      adjs = adj;
    else
      adjs += adj;
    primary_adj = adj[0];
    resync();
}

/****** remove_ ******/
//:FUNCTION remove_id
//Remove the given id
static
void remove_id( string array id... )
{
    if(!arrayp(ids))
      return;
    ids -= id;
    plurals -= map(id, (: pluralize :));
    resync();
}

void remove_adj( string array adj ... ) {
    if(!arrayp(ids))
      return;
    adjs -= adj;
    resync();
}

/****** clear_ ******/

//:FUNCTION clear_id
//removes all the ids of an object.
void clear_id() {
    ids = ({ });
    plurals = ({ });
    resync();
}

//:FUNCTION clear_adj
//Remove all the adjectives from an object
void clear_adj() 
{
    adjs = ({ });
    resync();
}

/****** query_ ******/

//:FUNCTION query_id
//Returns an array containing the ids of an object
string array query_id() {
    string array fake = this_object()->fake_item_id_list();
    
    if (fake) return fake + ids;
    else return ids;
}

//:FUNCTION query_adj
//return the adjectives
string array query_adj()
{
    return adjs;
}

/****** parser interaction ******/

string array parse_command_id_list()
{
    if (test_flag(INVIS)) return ({ });
//### should strip non-alphanumerics here; might need an efun to do it
//### efficiently
    return query_id();
}

nomask string array parse_command_plural_id_list() {
    if (test_flag(INVIS)) return ({ });
    return plurals;
}

nomask string array parse_command_adjectiv_id_list() {
    if (test_flag(INVIS)) return ({ });
    return adjs;
}
