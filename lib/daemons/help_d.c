/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** File: help_d.c
**
** Created 25-OCT-94 by Deathblade for the new help system.
**
** This daemon keeps track of topic->pathname mappings.
** It will also manage a topic name appearing in multiple
** locations.
*/

#include <mudlib.h>
#include <dirs.h>

inherit M_DAEMON_DATA;

// mapping topic_refs, to record directories and topic names
//       which should be used as additional topic references for all files
//       within those dirs - eg all files within /help/player/bin as to be
//       available via "help player commands"

private nosave mapping topic_refs =
  ([ "/help/player/bin/" : "player_commands",
  "/help/player/command/" : "player_commands",
  "/help/player/quests/" : "quests",
  "/help/player/" : "topics",
  "/help/wizard/bin/" : "wizard_commands",
  "/help/admin/command/" : "admin_commands",
  ]);

/*
** This mapping contains all the topics in the system.  It
** maps topic names to an array of pathnames.  The level-
** restriction on the topic is then computed by examining
** the lead part of the pathname.
*/
private mapping topics;

/*
** This mapping contains the level-restrictions on the help
** sub-directories.  It is read from the _restrict file in
** the /help directory.
**
** It maps subdir names to levels.
*/
private mapping restrict;

private nosave int	pending_count;
private nosave object	initiator;

private nosave array	ignore = ({ DIR_HELP "/autodoc/FIXME/" });

nomask void process_dir(string path);


private int f_restrict(string s)
{
  string * r;

  if ( s[0] == '#' )
    return 0;
  if ( member_array(':', s) == -1 )
    return 0;
  r = explode(s, ":");
  restrict[trim_spaces(r[0])] = to_int(r[1]);
}

private void process_file(string path, string file)
{
  string pathname;
  string *player_dirs = keys(topic_refs);
  string rfile = "";

  if ( file == "." || file == ".." )
    return;

  if ( file[0] == '_' )
    return;

  pathname = path + file;

  if ( file_size(pathname) == 0 )
  {
    write("Warning: '" + pathname + "' contains nothing.\n");
    return;
  }
  if ( file_size(pathname) == -2 )
  {
    pathname += "/";
/* Ack. Avoid execution cost errors. */
    call_out((: process_dir :), 1, pathname);
    ++pending_count;
  }

  file = lower_case(file);
  if ( topics[file] )
    topics[file] += ({ pathname });
  else
    topics[file] = ({ pathname });

  if(member_array(path,player_dirs) > -1)
  {
    rfile = topic_refs[path];
    if(topics[rfile])
      topics[rfile] += ({ pathname });
    else
      topics[rfile] = ({ pathname });
  }
}

nomask void process_dir(string path)
{
  if ( member_array(path, ignore) == -1 )
    map_array(get_dir(path + "*"), (: process_file, path :));

  if ( !--pending_count )
  {
    save_me();

    if ( initiator )
    {
      tell(initiator, "HELP_D has finished the rebuild.\n");
      initiator = 0;
    }
  }
}

nomask void rebuild_data()
{
  string * lines;
  string * dirs;

  if ( pending_count )
  {
    write("HELP_D is currently rebuilding!\n");
    return;
  }

  write("HELP_D is beginning the rebuild...\n");
  initiator = this_user();

  topics = ([ ]);
  restrict = ([ ]);

  lines = explode(read_file(DIR_HELP "/_restrict"), "\n");
  map_array(lines, (: f_restrict :));

  dirs = filter(get_dir(DIR_HELP "/*"), (: $(restrict)[$1] != 99 :));

  pending_count = 0;
  map_array(dirs, (: process_file, DIR_HELP "/" :));
}

nomask void create()
{
  ::create();

  if ( topics == 0 )
    rebuild_data();
}

/*
** find_topic()
**
** Will return 0 if the topic does not exist (within the current
** player's level).  Otherwise, an array of pathnames will be
** returned.
*/
nomask string * find_topic(string name)
{
  mixed *result;
  int lvl;

  if ( !this_body() )
    return 0;
  result = topics[lower_case(name)];
  if ( !result )
    return 0;

//### simulate the old levels
  lvl = adminp(this_user()) ? 5 : wizardp(this_user()) ? 1 : 0;

  return filter_array(result, function(string file, int lvl){
	array parts = explode(file, "/");
	if (sizeof(parts) < 3) return 1;
	return (lvl >= restrict[parts[1]]);
    }, lvl);
}

/*
** conflict_report()
**
** Print out a report of the topics that have conflicts.
*/
nomask void conflict_report()
{
  mixed * values;

  values = values(topics);
  values = filter_array(values, (: sizeof($1) > 1 :));
  more(sprintf("%O", values));
}

/*
** Return the topic and restrict list.  Note that these are
** the originals -- the topic mapping is awfully large to
** copy, so we'll just assume we have nice clients.
*/
mapping query_topics(){ return topics; }

mapping query_restrict(){ return restrict; }
