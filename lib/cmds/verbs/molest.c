/* Do not remove the headers from this file! see /USAGE for more info. */
/* Included for historical reasons - ie: practically every Infocom game
 * with the notable exception of Nord and Bert included support for it.

*/

inherit VERB_OB;

mixed direct_molest_liv( object liv )
{
    return "Now, now.  Play nice.\n";
}

mixed direct_molest_obj( object ob )
{
    return "What an <ahem> strange idea.\n";
}

array query_verb_info()
{
    return ({ ({ "OBJ", "LIV" }) });
}
