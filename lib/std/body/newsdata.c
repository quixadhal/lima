/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** newsdata.c
**
** Provides the player data for the newsreader.
**
** 24-Dec-94. Deathblade. Created
*/

#include <daemons.h>

private mapping news_data = ([]);

nomask int get_news_group_id(string group)
{
    return news_data[group];
}

nomask void set_news_group_id(string group, int id)
{
    news_data[group] = id;
}

nomask string * subscribed_groups()
{
    return sort_array(keys(news_data), 1);
}

nomask void validate_groups()
{
    string * groups;

    groups = keys(news_data) - NEWS_D->get_groups();
    map_array(groups, (: map_delete(news_data, $1) :));
}
