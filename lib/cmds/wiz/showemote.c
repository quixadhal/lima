/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: feelings, m_messages, addemote, rmemote, stupidemote, targetemote
//USAGE:  showemote <soul>
//
//     This will show all the rules and messages for the given soul
//
//>showemote kick
//
//"OBJ" -> $N $vkick the $o.
//"LIV" -> $N $vkick $t.
//"LIV LIV" -> "$N $vjump up in the air and $vkick $n1 and $n2 //simultaneously.
//"LIV STR" -> $N $vkick $t $o.

#include <daemons.h>

inherit CMD;

private void main(string str)
{
  mixed data;
  mixed *m;
  int i;

  if (!str)
  {
    out("showemote verb\n");
    return;
  }
  data = SOUL_D->query_emote(str);
  if (!data)
  {
    out("No such emote.\n");
    return;
  }
  m = keys(data);
  for (i=0; i<sizeof(m); i++)
  {
    if (stringp(data[m[i]])) {
      outf("%O -> %s\n", m[i], data[m[i]]);
    } else {
      outf("%O -> %s\n", m[i], implode(data[m[i]], "\n\t-> "));
    }
  }
}