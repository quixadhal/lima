/* Do not remove the headers from this file! see /USAGE for more info. */

/* Copyright 1994 - Tim Hollebeek
 *
 * Permission is granted to copy and use this code elsewhere, provided
 * that all code derived from this code retain this header and credit
 * the author (Tim Hollebeek) for the source, that all improvements
 * to this code be communicated to the above mentioned author for possible
 * inclusion in this code, that all derived works are made publicly
 * available to whoever wants them, and no profit is made off of this
 * code or any other derived works or any other package or system this
 * is used in without express written permission of the author.
 */
/* 
 *  General news daemon, by Beek
 *  Oct 10, 1994
 *
 * Interface:
 *
 * id NEWS_D->post(group, subject, message)
 * id NEWS_D->followup(group, id, message)
 * message NEWS_D->get_message(group, id)
 * id array NEWS_D->get_messages(group)
 * id array NEWS_D->get_thread(group, thread)
 * NEWS_D->get_groups()
 * varargs id NEWS_D->system_post(group, subject, message, poster)  (poster optional)
 *
 *
 * GUElib conversion by Deathblade on 24-Dec-94.
 * Rust added system_post on July 7, 1995
 * Rust added the restrict_group() interface on July 24, 1995.
 * Beek modified the system to be automatic based on group prefixes.
 * 
 */

#include <mudlib.h>
#include <security.h>
#include <news.h>
#include <commands.h>

inherit M_ACCESS;

#define SAVE_FILE "/data/news/news_d"
#define ARCHIVE_DAYS 14
#define ARCHIVE_DIR "/data/news/archive"

void archive_posts();

private mapping data = ([]);
private static mapping restrictions = 
([
	"wiz" : 	(: wizardp(this_user()) :),
	"admin" : 	(: GROUP_D->adminp(this_user()) :)
]);

nomask void create()
{
  string group;
  int rest;
  set_privilege(1);
  if (clonep(this_object())) destruct(this_object());

  restore_object(SAVE_FILE, 1);
  
  archive_posts();
}

nomask void save_me()
{
    unguarded(1, (: save_object, SAVE_FILE :));
}

private nomask int get_new_id(string group)
{
    return data[group]["next_id"]++;
}

private nomask void notify_users(string group, mapping msg)
{
    NCHANNEL_D->deliver_string("plyr_news",
			       sprintf("[news] %s: %s [%s]\n",
				       group,
				       msg[MSG_SUBJECT][0..39],
				       msg[MSG_POSTER]));
}

nomask int post(string group, string subject, string message)
{
    int post_id;
    mapping map;

    map = allocate_mapping(5);
    if (!data[group]) return 0;
    post_id = get_new_id(group);
    map[MSG_TIME] = time();
    map[MSG_THREAD] = post_id;
    map[MSG_SUBJECT] = subject;
    map[MSG_MESSAGE] = message;
    map[MSG_POSTER] = this_body()->query_name();
    data[group][post_id] = map;
    save_me();
    notify_users(group, map);
    return post_id;
}

varargs nomask int system_post(string group, string subject, string message, string poster)
{
  int post_id;
  mapping map = allocate_mapping(5);
  if(get_privilege(previous_object()) != 1)
    return 0;
  if(!data[group])
    return 0;
  post_id = get_new_id(group);
  map[MSG_TIME] = time();
  map[MSG_THREAD] = post_id;
  map[MSG_SUBJECT] = subject;
  map[MSG_MESSAGE] = message;
  map[MSG_POSTER] = poster ? poster : this_body() ? capitalize(this_body()->
			       query_link()->query_userid()) : mud_name();
  data[group][post_id] = map;
  save_me();
  notify_users(group, map);
  return post_id;
}

varargs nomask void add_group(string group)
{
    string fn;

    fn = file_name(previous_object());
    if (fn != ADMTOOL) return;

    data[group] = (["next_id":1]);
    save_me();
}

nomask void remove_group(string group)
{
    string fn;

    fn = file_name(previous_object());
    if (fn != ADMTOOL) return;

    map_delete(data, group);
    save_me();
}

nomask int followup(string group, int id, string message)
{
    int post_id;
    int tmp;
    mapping map;
    string subject;

    map = allocate_mapping(6);
    if (!data[group]) return 0;
    if (!data[group][id]) return 0;
    post_id = get_new_id(group);
    subject = ">" + data[group][id][MSG_SUBJECT];

    /* the original article is the thread id */
    while (tmp = data[group][id][MSG_FOLLOW])
	id = tmp;
    map[MSG_TIME] = time();
    map[MSG_THREAD] = id;
    map[MSG_SUBJECT] = subject;
    map[MSG_MESSAGE] = message;
    map[MSG_POSTER] = this_body()->query_name();
    data[group][post_id] = map;
    save_me();
    notify_users(group, map);
    return post_id;
}

nomask mapping get_message(string group, int id)
{
    mapping msg;

    msg = data[group][id];
    if ( !msg )
	return 0;

    /* sigh */
    return copy(msg);
}

nomask int * get_messages(string group)
{
    return keys(data[group]) - ({ "next_id" });
}

nomask int * get_thread(string group, int thread)
{
    return filter_array(keys(data[group]), (: $1 != "next_id" &&
					      data[$(group)][$1][MSG_THREAD]
					      == $(thread) :) );
}

nomask string * get_groups()
{
    string array ret;
    // filter before sorting; the func is typically pretty cheap, and
    // and calling them all is O(n).  Sorting the list first is more
    // expensive
    ret = filter(keys(data), function(string group) {
	// this will be "" if there is no '.'
	string prefix = group[0..member_array('.', group) - 1];
	function f = restrictions[prefix];
	if (!f) return 1;
	return evaluate(f);
    });
    return sort_array(ret, 1);
}

nomask int get_group_last_id(string group)
{
    return data[group]["next_id"] - 1;
}

private nomask void archive_post(string group, int id)
{
    unguarded(1, (: write_file, 
	sprintf("%s/%s", ARCHIVE_DIR, group),
	sprintf("---\nposter: %s\nsubject: %s\ndate: %s\n%s\n\n",
		data[group][id][MSG_POSTER],
		data[group][id][MSG_SUBJECT],
		ctime(data[group][id][MSG_TIME]),
		data[group][id][MSG_MESSAGE])
		  :) );
    map_delete(data[group], id);
}

nomask void archive_posts()
{
#ifdef NOT_YET

    string *groups;
    int *ids;
    int i,j,ni,nj;
    mapping group;
    int archive_time;

    archive_time = time() - ARCHIVE_DAYS * 24 * 60 * 60;

    groups = keys(data);
    ni = sizeof(groups);

    for (i=0; i<ni; i++)
    {
        group = data[groups[i]];
        ids = keys(group) - ({ "next_id" });
        nj = sizeof(ids);

        for (j=0; j<nj; j++)
	{
            if ( group[ids[j]][MSG_TIME] < archive_time )
                archive_post(groups[i], ids[j]);
        }
    }
#endif /* NOT_YET */
}
