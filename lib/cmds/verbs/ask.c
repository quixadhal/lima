/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed do_ask_liv_about_wrd(object liv, string item)
{
    if(liv->query_items(item))
     return 1;
    else return 0;
}

array query_verb_info()
{
     return ({ ({"LIV about WRD" }) });
}
