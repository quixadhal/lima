/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** reporter.c
**
** Generic reporting facility (for bugs, ideas, typos, etc.).
** Lifted from Rust's "bug" command.
**
** Created: 4-Sep-94  Deathblade
*/

#include <config.h>
#include <mudlib.h>

inherit DAEMON;

#define DIVIDER "\
\n***********************************************************************\n"

/*
** For security reasons (since we write these using unguarded()), the
** file names should be "constants" rather than passed into us.  Same
** thing goes for posting to newsgroups.
**
** This does imply that the Reporter is limited by virtue of need to
** change this file, but that is a necessity if this will be writing
** to restricted directories.
*/
private static mapping log_files = ([
				     "Bug" : "/log/BUGS",
				     "Typo" : "/log/TYPOS",
				     "Idea" : "/log/IDEAS",
				     "Todo" : "/log/TODOS",
				     ]);
private static mapping news_groups = ([
				       "Bug" : BUG_NEWSGROUP,
				       "Typo" : TYPO_NEWSGROUP,
				       "Idea" : IDEA_NEWSGROUP,
				       "Todo" : TODO_NEWSGROUP,
				       ]);

private nomask void issue_report(string type, string subject, string text)
{
  string where_am_i;
  object env;

  env = environment(this_body());
  where_am_i = (env ? file_name(env) : "<nowhere>");

  if ( subject )
    {
      text = sprintf("%s reported from %s--\n\n%s\n",
		     type,
		     where_am_i,
		     text);

      unguarded(1, (: NEWS_D->system_post($(news_groups[type]),
					  $(subject),
					  $(text)) :));
    }
  else
    {
      text = sprintf("%s: %s reports from %s on %s--\n\n%s" DIVIDER,
		     type,
		     this_body()->query_name(),
		     where_am_i,
		     ctime(time()),
		     text);

      unguarded(1, (: write_file, log_files[type], text :) );
    }

  write("Thanks for the report!\n");
}

/*
** begin_report()
**
** Entry point for a full text-entry bug report.  When the user completes
** their entry of the report, it will be handled appropriately.
*/
varargs void begin_report(string type, string subject)
{
  if ( !log_files[type] )
    error("Illegal report type.\n");

  printf("** %s report **\n", type);
  clone_object(EDIT_OB)->edit_text(0, "done_ed", ({ type, subject }));
}

/* handle the completion of the report */
void done_ed(mixed *ctx, string *lines)
{
  if ( !lines )
    {
      printf("%s entry aborted.\n", ctx[1]);
      return;
    }

  issue_report(ctx[0], ctx[1], implode(lines, "\n") + "\n");
}

/*
** short_report()
**
** The text has already been entered/derived/whatever.  This will just
** go straight ahead and issue the appropriate report.
*/
varargs void short_report(string type, string subject, string text)
{
  if ( !log_files[type] )
    error("Illegal report type.\n");

  issue_report(type, subject, text);
}
