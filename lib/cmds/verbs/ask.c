/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed do_ask_liv_about_wrd(object liv, string item)
{
    if (liv->query_items(item, 1))
      return 1;
    else return 0;
}

mixed do_ask_liv_about_str(object liv, string item)
{
    if (liv->query_items(item, 1))
      return 1;
    else return 0;
}

void create()
{
    add_rules( ({ "LIV about WRD", "LIV about STR" }), ({ }) );
}
