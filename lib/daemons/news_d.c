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
 * ??????, Beek: modified the system to be automatic based on group prefixes.
 * 950811, Deathblade: converted to classes. added remove_post().
 * 9606xx, Ohara: added move_post() and reply_post.
 * 961205, Ohara: added change_subject at DB's grovel^H^H^H^H^H^Hrequest.

 * 
 */

#include <mudlib.h>
#include <security.h>
#include <commands.h>
#include <classes.h>

inherit M_ACCESS;

inherit CLASS_NEWSMSG;

#define SAVE_FILE "/data/news/news_d"
#define RECENT_FILE "/data/news/recent"

#define ARCHIVE_DIR "/data/news/archive"

void archive_posts();
nomask string * get_groups();

private mapping data = ([]);
private mapping removed = ([]);
private static mapping recent_changes = ([]);
private int new_format = 1;

// No info on a group means never archive.
private mapping archive_info = ([ ]);

private static mapping restrictions = 
([
  "wiz" : ({ (: wizardp :), (: wizardp :) }),
  "admin" : ({ (: adminp :), (: adminp :) }),
  "." : ({ 0, (: adminp :) }),
]);

#define is_group(x) (member_array(x,get_groups()) != -1)

nomask void save_recent() {
    unguarded(1, (: rm, RECENT_FILE :));
    unguarded(1, (: write_file, RECENT_FILE, save_variable(recent_changes) :));
}

nomask void save_me()
{
    unguarded(1, (: save_object, SAVE_FILE :));
    foreach (string key in keys(recent_changes))
    recent_changes[key] = ([]);
    save_recent();
}

/*
** Set the archive time on a group.
*/
nomask void set_archive_time(string group, int numDays)
{
    if (!check_previous_privilege(1))
    {
        error("Insufficient privs");
    }
    if (!is_group(group))
    {
        error("First arg not a valid newsgroup");
    }
    if(!intp(numDays))
    {
        error("Number of days not an integer.");
    }
    if(numDays < 0)
    {
        error("Number of days must be greater than or equal to 0.");
    }
    if(!numDays) 
    {
        /* Never expire */
        map_delete(archive_info, group);
    }
    else
    {
        archive_info[group] = numDays;
    }
    save_me();
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

#define MSG_TIME        1
#define MSG_THREAD      2
#define MSG_SUBJECT     3
#define MSG_MESSAGE     4
#define MSG_POSTER      5

            msg = new(class news_msg,
              time : post[MSG_TIME],
              thread_id : post[MSG_THREAD],
              subject : post[MSG_SUBJECT],
              poster : post[MSG_POSTER],
              body : post[MSG_MESSAGE]);
            msg->userid         = lower_case(msg->poster);

            contents[id] = msg;
        }
    }

    new_format = 1;
    save_me();
}

nomask void create()
{
    string rec;

    set_privilege(1);
    if ( clonep(this_object()) )
    {
        destruct(this_object());
        return;
    }

    restore_object(SAVE_FILE, 1);
    if (rec = read_file(RECENT_FILE))
        recent_changes = restore_variable(rec);
    else
        recent_changes = 0;

    if ( !new_format )
        convert_news();
    if (!recent_changes) {
        recent_changes = ([]);
        foreach (string key in keys(data))
	    recent_changes[key] = ([]);
    } else {
        foreach (string key, mixed value in recent_changes) {
            if (value == "#removed#") {
                map_delete(data, key);
                map_delete(recent_changes, key);
		map_delete(removed, key);
            } else {
                /* possible for new groups */
                if (!data[key])
		    data[key] = ([]);
		if (!removed[key])
		    removed[key] = ({});

		recent_changes[key] = ([]);
                foreach (string key2 in keys(value)) {
                    data[key][key2] = value[key2];
		    if (classp(value[key2]) &&
			((class news_msg)value[key2])->body == 0)
			removed[key] += ({ key2 });
		}
            }
        }
    }
    foreach (string key, mixed value in data) {
        if (mapp(value)) {
            foreach (string key2, class news_msg msg in value) {
                if (classp(msg) && !msg->body)
                    map_delete(value, key2);
            }
        }
    }
    save_me();
    archive_posts();
}

private nomask int get_new_id(string group)
{
    int id = data[group]["next_id"]++;

    recent_changes[group]["next_id"] = data[group]["next_id"];

    return id;
}

private nomask void notify_users(string group, class news_msg msg)
{
    CHANNEL_D->deliver_channel("news",
      sprintf("%s: %s [%s]",
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
    msg = new(class news_msg,
	      time : 		time(),
	      thread_id : 	post_id,
	      subject : 	subject,
	      userid : 		this_user()->query_userid(),
	      body : 		message);
    msg->poster          = capitalize( msg->userid );

    data[group][post_id] = msg;
    recent_changes[group][post_id] = msg;
    save_recent();

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

//### need to think on this. I don't think we want to require priv 1
//### ... especially since even *this* object doesn't have that.  And
//### just checking the previous object is a no-no.
//### 
//### check for Mudlib:daemons maybe?
//    if ( get_privilege(previous_object()) != 1 )
//  return 0;
    if ( !data[group] )
        return 0;
    post_id = get_new_id(group);
    msg = new(class news_msg,
      time : time(),
      thread_id : post_id,
      subject : subject);
    if ( poster )
    {
        msg->poster = poster;
        msg->userid = base_name(previous_object());
    }
    else if ( this_body() )
    {
        msg->userid = this_user()->query_userid();
        msg->poster = capitalize( msg->userid );
    }
    else if ( this_user() )
    {
        msg->userid = this_user()->query_userid();
        msg->poster = capitalize( msg->userid);
    }
    else
    {
        msg->poster = msg->userid = mud_name();
    }
    msg->body           = message;

    data[group][post_id] = msg;
    recent_changes[group][post_id] = msg;
    save_recent();

    notify_users(group, msg);

    return post_id;
}

varargs nomask void add_group(string group)
{
    if (!check_privilege("Mudlib:daemons")) error("Permission denied.\n");

    data[group] = (["next_id":1]);
    recent_changes[group] = (["next_id":1]);
    save_recent();
}

nomask void remove_group(string group)
{
    if (!check_privilege("Mudlib:daemons")) error("Permission denied.\n");
    map_delete(data, group);
    recent_changes[group] = "#removed#";
    save_recent();
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

    msg = new(class news_msg,
      time : time(),
      thread_id : id, /* link to original thread_id */
      subject : subject,
      userid : this_user()->query_userid(),
      body : message);
    msg->poster       = capitalize( msg->userid );

    data[group][post_id] = msg;
    recent_changes[group][post_id] = msg;
    save_recent();

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
//### oops... can't do this yet... leave this unsafe for now
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

    if ((this_user() && !adminp(this_user()) && msg->userid != this_user()->query_userid()) &&
      (msg->userid != base_name(previous_object())))
    {
        error("* illegal attempt to remove post\n");
    }

    msg->body = 0;
    recent_changes[group][id] = msg;
    save_recent();
}

nomask int * get_messages(string group, int no_removed)
{
    array ret = keys(data[group]) - ({ "next_id" });

    if (no_removed)
	ret -= removed[group];
    
    return ret;
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
    ret = filter(keys(data), function(string group)
      {
          array a;
          string prefix;
          function f;
          int i = member_array('.', group, 1) - 1;
          if( i == -2) i = sizeof(group);

          prefix = group[0..i];
          a = restrictions[prefix];

          if( !sizeof(a)) return 1;
          f = a[0];
    if(!f) return 1;
          return evaluate(f, this_user());
      }
    );
    return sort_array(ret, 1);
}

nomask int query_write_to_group( string group )
{
    function f;
    array a;

    string prefix;
    int i = member_array('.', group, 1) - 1;
    if( i == -2) i = sizeof(group);

    prefix = group[0..i];

    a = restrictions[prefix];
    if( !sizeof(a)) return 1;
    f = a[1];
    if( !f) return 1;
    return evaluate(f, this_user());
}


nomask int get_group_last_id(string group)
{
    return data[group]["next_id"] - 1;
}

nomask void dump_to_file(string group, string fname)
{
    mapping contents = data[group];
    int id;

    foreach ( id in sort_array(keys(contents) - ({ "next_id" }), 1) )
    {
        class news_msg msg = contents[id];

        if ( !msg->body )
            continue;

        write_file(fname,
          sprintf("---\nposter: %s\nsubject: %s\ndate: %s\n\n%s\n",
            msg->poster, msg->subject,
            intp(msg->time) ? ctime(msg->time) : msg->time,
            msg->body));
    }
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

    /* This doesn't give w/ the newsreader. */
#if 0
    map_delete(data[group], id);
#else
    /* Do this instead. */
    remove_post(group, id);
#endif
}

nomask void archive_posts()
{
    int archive_days;
    int archive_time;
    string group;
    mapping contents;



    foreach ( group, contents in data )
    {
        mixed id;
        class news_msg msg;

        archive_days = archive_info[group];
        if(!archive_days)
        {
            continue;
        }
        archive_time = time() - archive_days * 24 * 60 * 60;
        foreach ( id, msg in contents )
        {
            if ( id == "next_id" )
                continue;
            if ( intp(msg->time) && msg->time < archive_time ) 
                archive_post(group, id);
        }
    }
}


// Blame --OH. for this code :)

nomask void move_post( string curr_group, int curr_id, string to_group )
{
    class  news_msg msg;
    int new_id;

    msg = copy( data[curr_group][curr_id]);
    if( !adminp(this_user()) && msg->userid != this_user()->query_userid())
    {
        write( "You cannot move posts which don't belong to you.\n");
        return;
    }
    if( curr_group == to_group )
    {
        write( "Same group. Post not moved.\n");
        return;
    }
    if( !query_write_to_group( to_group ))
    {
        write( "You don't have permission to move posts to " + to_group + "\n");
        return;
    }
    new_id = get_new_id(to_group);
    msg->body = "(Originally in " + curr_group + ")\n" + msg->body;
    data[to_group][new_id] = msg;
    recent_changes[to_group][new_id] = msg;
    remove_post( curr_group, curr_id );
    write( "Post moved.\n");
    save_recent();
}

void remove() {
    save_me();
}


nomask void change_header( string group, int id, string header )
{
    class news_msg msg;
    object tu = this_user();

    msg = data[group][id];
    if( !adminp( tu) && tu->query_userid() != msg->userid)
    {
        write( "You cannot change the subjects of posts that don't belong to you\n");
    }
    else if( sizeof( header ))
        ((class news_msg)data[group][id])->subject = header;
    else
        write( "Subject not changed.\n");
    save_recent();
    return;
}

void dump(string path) {
    string ret = "";
    
    foreach (string group in get_groups()) {
	foreach (int id in get_messages(group, 1)) {
	    class news_msg post = get_message(group, id);

	    ret += "Group: " + group + "\n" +
		   "Subject: " + post->subject + "\n" +
		   "Date: " + ctime(post->time) + "\n" +
		   "Poster: " + post->poster + "\n\n" +
		   post->body + "\n---------------------------------------------------------------------------\n\n";
	}
    }
    write_file(path, ret);
}

array search_for(string what) {
    array ret = ({});
    
    foreach (string group, mapping contents in data) {
	foreach (mixed id, class news_msg post in contents) {
	    if (id == "next_id" || !post->body)
		continue;

	    if (regexp(post->body, what) && query_write_to_group(group))
		ret += ({ ({ group, id }) });
	}
    }

    return ret;
}

array search_for_author(string who) {
    array ret = ({});
    
    foreach (string group, mapping contents in data) {
	foreach (mixed id, class news_msg post in contents) {
	    if (id == "next_id" || !post->body)
		continue;

	    if (lower_case(post->poster) == lower_case(who) && query_write_to_group(group))
		ret += ({ ({ group, id }) });
	}
    }

    return ret;
}
