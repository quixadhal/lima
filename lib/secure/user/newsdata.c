/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** newsdata.c
**
** Provides the player data for the newsreader.
**
** 24-Dec-94. Deathblade. Created
** 03-Dec-97  Explanded to use M_SET (thus getting rid of highwater tracking)
**            Tigran
** 13-Mar-98  Added toggleable threading and the ability to mark articles in 
**            a newsgroup unread -- Tigran.
*/

inherit M_SET;

void save_me();
private string array subscribed_groups();
private int check_subscribed(string group);

private nomask mapping news_data;
private nomask int threading_toggled=1;

nomask int set_threading(int i)
{
  if(i)
    threading_toggled=1;
  else
    threading_toggled=0;
}

nomask int query_threading()
{
  return threading_toggled;
}

/* Function that should be used to catch up a group */
nomask void catch_up_newsgroup(string group)
{
  news_data[group]=sprintf("%c%s",
			   news_data[group][0],
			   set_add_range("",
					 1,
					 NEWS_D->get_group_last_id(group) ) );
  save_me();
}

nomask void mark_newsgroup_unread(string group)
{
  news_data[group]=sprintf("%c",check_subscribed(group)?':':'#');
  save_me();
}

/* Function for querying whigh articles have been read */
nomask string get_news_id_read(string group)
{
  if(!valid_set(news_data[group][1..]))
    {
      catch_up_newsgroup(group);
      printf("Your news read data has been corrupted, marking everything as read.\n"
	     "Please notify Tigran@Lima Bean that this happened");
    }
      if(strlen(news_data[group])<=1)
        return "";
  return news_data[group][1..];
}


/* Function for adding a news article to the list of read articles */
nomask void add_news_id_read(string group,int id)
{
  if(set_min(news_data[group][1..])==0)
    {
      news_data[group]=sprintf("%c%s",
			       news_data[group][0],
			       set_remove(news_data[group][1..],
					  0) );
    }
  news_data[group]=sprintf("%c%s",
			   news_data[group][0],
			   set_add(news_data[group][1..],
				   id) );
  save_me();
}

/* If the group is regisetered mark it unsubscribed, if it is not yet 
 * registered we must take care of that as well */
nomask void unsubscribe_group(string group)
{
  if(undefinedp(news_data[group])) {
    news_data[group]="#";
  }
  else {
    news_data[group]=sprintf("#%s",
			     news_data[group][1..]);
  }
  save_me();
}

/* If the group is already registered, subscribe to it, if not registered
 * we need to do that too. */
nomask void subscribe_group(string group)
{
  if(undefinedp(news_data[group])) {
    news_data[group]=":";
  }
  else {
    news_data[group]=sprintf(":%s",
			     news_data[group][1..]);
  }
  save_me();
}

/* Check to see if the group is subscribed to */
nomask int check_subscribed(string group)
{
  return news_data[group][0]=='#'?0:1;
}

/* Return a list of all subscribed groups.  Possibly this should be removed
 * as it may be redundant w/ the existance of registered_groups() */
nomask string array subscribed_groups()
{
  string array groups;
  if(!sizeof(news_data))
    return 0;
  groups=sort_array(keys(news_data),1);
  return filter(groups,
		(: check_subscribed :) );
}

/* Returns all groups that are registered either as subscribed or 
 * unsubscribed */
nomask string array registered_groups()
{
  if(!sizeof(news_data))
    return 0;
  return keys(news_data);
}

/* Function that removes groups from your news_data that aren't in NEWS_D */
/* Added by Tigran.  This function also converts news data into the proper
 * format if it has not already been done */
nomask void validate_groups()
{
  string array groups;
  if(!news_data) {
    news_data=([]);
  }
  if(!sizeof(news_data))
    return;
  groups = keys(news_data) - NEWS_D->get_groups();
  map_array(groups, (: map_delete(news_data, $1) :));
  foreach(string group,mixed data in news_data) {
    if(intp(data)) {
      data=sprintf(":%s",set_add_range("",1,data) );
    }
  }
  save_me();
}

nomask mapping get_news_data()
{
  return news_data;
}
