/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <driver/type.h>

/*
** From OBJ::description
*/
string the_short();


private mapping 	entries;
private string		read_text;


void
set_text( string t )
{
  read_text = t;
}

string
read()
{
    this_body()->simple_action("$N $vread the $o.\n", this_object());
    return read_text;
}

string
read_entry( string entry )
{

    entry = entries[entry];

    if (functionp(entry))
	return evaluate(entry);

    if (stringp(entry) && file_size(entry) > 0)
	return read_file(entry);

    return 0;
}

int
has_entries()
{
  return mapp( entries );
}

/* Parser interaction */
mixed direct_read_obj(object ob) {
    if (!read_text) {
      if (entries)
          return "It has a number of entries about various topics.\n";
      return 0;
    }
    return 1;
}

mixed direct_read_prep_obj(string p, object ob) {
    return direct_read_obj(ob);
}

mixed direct_read_str_prep_obj(string str, string p, object ob) {
    if (!entries) {
      if (read_text)
          return "It's fairly short.  Just read it all.\n";
      return 0;
    }
    return 1;
}

mixed direct_read_prep_str_prep_obj(string p1, string str, string p2,
                                  object ob) {
    return direct_read_str_prep_obj(str, p2, ob);
}
