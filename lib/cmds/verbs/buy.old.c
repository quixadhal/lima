/* Do not remove the headers from this file! see /USAGE for more info. */

/* Renamed and placed out of commission by Vette on April 18, 1997
   This file is replaced by /cmds/player/buy.c
   The parsing in the new buy cannot be done by a verb, or
   so stated by Darklord
*/

inherit VERB_OB;

void do_buy_obj_from_liv(object ob, object liv) {
    liv->sell_object(ob);
}
void do_buy_str_from_liv(string str, object liv)
{
    string *sentence;
    int i;
    int number;

    sentence = explode(str, " ");
    i = sizeof(sentence);
    if( i == 1 )
	liv->sell_stored_objects(sentence[0]);
    else
    {
	if ( i == 2 )
	{
	    if( sscanf(sentence[1], "%d", number) == 1)
	    {
		liv->sell_stored_objects(sentence[0], number);
	    }
	    else
	    {
		this_body()->my_action("the shopkeeper says, \"I don't understand that request.\"");
	    }
	}
	else
	{
	    this_body()->my_action("The shopkeeper says, \"I don't understand that request.");
	}
    }
}



array query_verb_info() {
    return ({ ({ "OBJ from LIV", "STR from LIV" }) });
}
