/* Do not remove the headers from this file! see /USAGE for more info. */

/* Wraith@Lima Bean / Hatter@Wonderland   infodawn@lima.mudlib.org
// Total Reimp by --OH.

   Verb to search for things that may be hidden.
*/

inherit VERB_OB;


void do_search_obj(object ob) {
    ob->do_search();
}

void do_search_obj_with_obj(object ob1, object ob2) {
    ob1->do_search( ob2 );
}

void do_search_for_str(string str) { 
    environment(this_body())->do_search( 0, str );
}

void do_search_for_str_in_obj(string str, object ob) { 
    ob->do_search( 0, str );
}

void do_search_obj_for_str( object ob, string str )
{
    ob->do_search( 0, str );
}

void do_search_obj_with_obj_for_str( object ob, object with, string str)
{
    ob->do_search( with, str );
}

void do_search_for_str_in_obj_with_obj(string str, object ob1, object ob2)
{
    ob1->do_search( ob2, str );
}

void do_search_obj_for_str_with_obj( object ob1, string str, object ob2 )
{
    ob1->do_search( ob2, str );
}

void do_search()
{
    environment( this_body() )->do_search();
}

array query_verb_info() {
    return ({ ({ "OBJ", "OBJ with OBJ", "for STR", "for STR in OBJ", 
	"OBJ for STR", "OBJ with OBJ for STR", "OBJ for STR with OBJ",
	"for STR in OBJ with OBJ", "" }) });
}
