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
 * 941224, Deathblade: GUElib conversion
 * 950707, Rust: added system_post on July 7, 1995
 * 950724, Rust: added the restrict_group() interface on July 24, 1995.
 * ??????, Beek: modified the system to be automatic based on group prefixes.
 * 950811, Deathblade: converted to classes. added remove_post().
 * 
 */

#include <mudlib.h>
#include <security.h>
#include <commands.h>
#include <classes.h>

inherit M_ACCESS;

inherit CLASS_NEWSMSG;

#define SAVE_FILE "/data/news/news_d"

#define ARCHIVE_DAYS 14
#define ARCHIVE_DIR "/data/news/archive"

void archive_posts();

private mapping data = ([]);
private int new_format;

private static mapping restrictions = 
([
	"wiz" : 	(: wizardp(this_user()) :),
	"admin" : 	(: adminp(this_user()) :)
]);

nomask void save_me()
{
    unguarded(1, (: save_object, SAVE_FILE :));
}

/*
** Convert the storage format from (old-style) mappings to classes
*/
private nomask void convert_news()
{
    string group;
    mapping contents;

    foreach ( group, contents in data )
    {
	mixed id;
	mapping post;

	foreach ( id, post in contents )
	{
	    class news_msg msg;

	    if ( id == "next_id" )
		continue;

#define MSG_TIME	1
#define MSG_THREAD	2
#define MSG_SUBJECT	3
#define MSG_MESSAGE	4
#define MSG_POSTER	5

	    msg = new(class news_msg);
	    msg->time		= post[MSG_TIME];
	    msg->thread_id	= post[MSG_THREAD];
	    msg->subject	= post[MSG_SUBJECT];
	    msg->poster		= post[MSG_POSTER];
	    msg->userid		= lower_case(msg->poster);
	    msg->body		= post[MSG_MESSAGE];

	    contents[id] = msg;
	}
    }

    new_format = 1;
    save_me();
}

nomask void create()
{
    set_privilege(1);
    if ( clonep(this_object()) )
    {
	destruct(this_object());
	return;
    }

    restore_object(SAVE_FILE, 1);
    if ( !new_format )
	convert_news();
  
    archive_posts();
}

private nomask int get_new_id(string group)
{
    return data[group]["next_id"]++;
}

private nomask void notify_users(string group, class news_msg msg)
{
    NCHANNEL_D->deliver_string("plyr_news",
			       sprintf("[news] %s: %s [%s]\n",
				       group,
				       msg->subject[0..39],
				       msg->poster));
}

nomask int post(string group, string subject, string message)
{
    int post_id;
    class news_msg msg;

    if (!data[group]) return 0;
    post_id = get_new_id(group);
    msg = new(class news_msg);
    msg->time		= time();
    msg->thread_id	= post_id;
    msg->subject	= subject;
    msg->poster		= this_body()->query_name();
    msg->userid		= this_user()->query_userid();
    msg->body		= message;

    data[group][post_id] = msg;
    save_me();

    notify_users(group, msg);

    return post_id;
}

varargs nomask int system_post(string group,
			       string subject,
			       string message,
			       string poster)
{
    int post_id;
    class news_msg msg;

    if ( get_privilege(previous_object()) != 1 )
	return 0;
    if ( !data[group] )
	return 0;
    post_id = get_new_id(group);
    msg = new(class news_msg);
    msg->time		= time();
    msg->thread_id	= post_id;
    msg->subject	= subject;
    if ( poster )
    {
	msg->poster = msg->userid = poster;
    }
    else if ( this_body() )
    {
	msg->poster = this_body()->query_name();
	msg->userid = this_user()->query_userid();
    }
    else if ( this_user() )
    {
	msg->poster = msg->userid = this_user()->query_userid();
    }
    else
    {
	msg->poster = msg->userid = mud_name();
    }
    msg->body		= message;

    data[group][post_id] = msg;
    save_me();

    notify_users(group, msg);

    return post_id;
}

varargs nomask void add_group(string group)
{
    string fn;

    fn = file_name(previous_object());
    if (fn != CMD_OB_ADMTOOL) return;

    data[group] = (["next_id":1]);
    save_me();
}

nomask void remove_group(string group)
{
    string fn;

    fn = file_name(previous_object());
    if (fn != CMD_OB_ADMTOOL) return;

    map_delete(data, group);
    save_me();
}

nomask int followup(string group, int id, string message)
{
    string subject;
    int post_id;
    class news_msg msg;

    if (!data[group]) return 0;
    if (!data[group][id]) return 0;
    post_id = get_new_id(group);
    subject = ">" + ((class news_msg)data[group][id])->subject;

    id = ((class news_msg)data[group][id])->thread_id;

    msg = new(class news_msg);
    msg->time		= time();
    msg->thread_id	= id;		/* link to original thread_id */
    msg->subject	= subject;
    msg->poster		= this_body()->query_name();
    msg->userid		= this_user()->query_userid();
    msg->body		= message;

    data[group][post_id] = msg;
    save_me();

    notify_users(group, msg);

    return post_id;
}

nomask class news_msg get_message(string group, int id)
{
    class news_msg msg;

    if ( !data[group] )
	return 0;

    msg = data[group][id];
    if ( !msg )
	return 0;

    /* sigh */
    /* ### oops... can't do this yet... leave this unsafe for now */
//    return copy(msg);
    return msg;
}

nomask void remove_post(string group, int id)
{
    class news_msg msg;

    if ( !data[group] )
	return;

    msg = data[group][id];
    if ( !msg || !msg->body )
	return;

    if ( !adminp(this_user()) &&
	 msg->userid != this_user()->query_userid() )
    {
	error("* illegal attempt to remove post\n");
    }

    msg->body = 0;
}

nomask int * get_messages(string group)
{
    return keys(data[group]) - ({ "next_id" });
}

nomask int * get_thread(string group, int thread)
{
    return filter_array(keys(data[group]),
			(: $1 != "next_id" &&
			 ((class news_msg)data[$(group)][$1])->thread_id
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
    class news_msg msg = data[group][id];

    unguarded(1, (: write_file, 
		  sprintf("%s/%s", ARCHIVE_DIR, group),
		  sprintf("---\nposter: %s\nsubject: %s\ndate: %s\n%s\n\n",
			  msg->poster, msg->subject,
			  intp(msg->time) ? ctime(msg->time) : msg->time,
			  msg->body)
		  :) );

    map_delete(data[group], id);
}

nomask void archive_posts()
{
#ifdef NOT_YET
    int archive_time;
    string group;
    mapping contents;

    archive_time = time() - ARCHIVE_DAYS * 24 * 60 * 60;

    foreach ( group, contents in data )
    {
	mixed id;
	class news_msg msg;

	foreach ( id, msg in contents )
	{
	    if ( id == "next_id" )
		continue;

	    /* ### how to archive posts with strings as times? */
	    if ( intp(msg->time) && msg->time < archive_time )
		archive_post(group, id);
        }
    }
#endif /* NOT_YET */
}
