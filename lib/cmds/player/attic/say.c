/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//
// Syntax: <say MESSAGE>
//         <say /last>
//
// This command is used to talk with others in the same room as you.
// It is done in a common language that everybody can understand.
// The /last and /history syntaxes will display your say history.

#include <mudlib.h>

inherit CMD;
inherit M_GRAMMAR;

#define MAX_HISTORY 20
#undef PER_BODY_sAY_HISTORY

void query_history(object o);
void add_history(object o, string msg);

private mapping history = ([ ]);

void create()
{
  ::create();
  no_redirection();
}

private void main(string s)
{
  if (!s || s == "")
  {
    out("Say what?\n");
    return;
  }

  if (s == "/last")
#ifdef PER_BODY_SAY_HISTORY
    return query_history(this_body());
#else
    return query_history(environment(this_body()));
#endif /* PER_BODY_SAY_HISTORY */

#ifdef ZORKMUD
    this_body()->simple_action("$N $vsay, \"$O\"", punctuate(s));
#ifdef PER_BODY_SAY_HISTORY
    add_history(this_body(), sprintf("You say, \"%s\"", punctuate(s)));
    foreach(object ob in filter(all_inventory(environment(this_body())) - ({ this_body() }), (: $1->query_link() :)))
      add_history(ob,
#else
    add_history(environment(this_body()),
#endif /* PER_BODY_SAY_HISTORY */
                  sprintf("%s says, \"%s\"", this_body()->query_name(), punctuate(s)));
#else
    this_body()->simple_action("%^SAY%^$N $vsay:%^RESET%^ $o", punctuate(s));
#ifdef PER_BODY_SAY_HISTORY
    add_history(this_body(),
                sprintf("%%^SAY%%^You say:%%^RESET%%^ %s", punctuate(s)));
    foreach(object ob in filter(all_inventory(environment(this_body())) - ({
this_body() }), (: $1->query_link() :)))
      add_history(ob,
#else
    add_history(environment(this_body()),
                  sprintf("%%^SAY%%^%s says:%%^RESET%%^ %s", this_body()->query_name(), punctuate(s)));
#endif /* PER_BODY_SAY_HISTORY */

#endif /* ZORKMUD */
}

private void add_history(object o, string msg)
{
  if (!history[o])
    history[o] = ({ msg });
  else
    history[o] += ({ msg });

  if (sizeof(history[o]) > MAX_HISTORY)
    history[o] = history[o][1..<1];
}

private void query_history(string o)
{
  string hist;

  if ( !history[o] )
    return out("No say history.\n");
  hist = implode(history[o], "\n");

  if ( hist == "" )
    hist = "<none>\n";

  more(sprintf("History of 'says':\n%s\n", hist));
}

nomask int valid_resend(string ob) {
    return ob == "/cmds/player/converse";
}
