/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** reporter.c
**
** Generic reporting facility (for bugs, ideas, typos, etc.).
** Lifted from Rust's "bug" command.
**
** Created: 4-Sep-94  Deathblade
*/

#include <mudlib.h>

inherit DAEMON;

#define DIVIDER "\
\n***********************************************************************\n"


varargs 
void begin_report(string log_or_newsgroup, string type, string title)
{
    printf("** %s report **\n", type);
    clone_object(EDIT_OB)->edit_text(0, "done_ed", ({ log_or_newsgroup,
							type, title }));
}

varargs
void short_report(string log_file, string type, string text, string subject) {
    string where_am_i;
    string wizname;
    string xtra;
    object env;

    env = environment(this_body());
    where_am_i = (env ? file_name(env) : "<nowhere>");
    text = sprintf("%s: %s reports from %s on %s--\n\n",
		   type, this_body()->query_name(), where_am_i,
		   ctime(time())) + text + (subject ? DIVIDER : "\n");

#ifdef NOPE
    if ( sscanf(where_am_i, WIZ_DIR"/%s/%s", wizname, xtra) == 2 )
    {
	object m;
	m = clone_object(MAIL_OBJECT);
	m->mailnote(wizname, this_body()->query_name(),0,
	  type+" report",text);
    }
#endif
    if(subject)
      	unguarded(1, (:NEWS_D->system_post($(log_file),$(subject),$(text)):));
    else
      unguarded(1, (: write_file, "/log/"+log_file, text :) );

    write("Thanks for the report!\n");
}

void done_ed(mixed *ctx, string *text)
{
    string where_am_i;
    string Write;
    string wizname;
    string xtra;
    object env;

    if ( !text )
    {
	printf("%s entry aborted.\n", ctx[1]);
	return;
    }

    Write = implode(text, "\n") + "\n";
    env = environment(this_body());
    where_am_i = (env ? file_name(env) : "<nowhere>");
    where_am_i = file_name(environment(this_body()));
    Write = sprintf("%s: %s reports from %s on %s--\n\n",
		    ctx[1], this_body()->query_name(),
		    where_am_i, ctime(time())) + Write + (ctx[2] ? DIVIDER
							  : "\n");

#ifdef NOPE
    if ( sscanf(where_am_i, WIZ_DIR"/%s/%s", wizname, xtra) == 2 )
    {
	object m;
	m = clone_object(MAIL_OBJECT);
	m->mailnote(wizname, this_body()->query_name(),0,
	  ctx[1]+" report",Write);
    }
#endif

    if(ctx[2])
      {
	unguarded(1, (:NEWS_D->system_post($(ctx[0]),$(ctx[2]),$(Write)):));
      }
    else
      {
	unguarded(1, (: write_file, "/log/"+ctx[0], Write :));
      }

    write("Thanks for the report!\n");
}
