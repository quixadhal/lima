/* Do not remove the headers from this file! see /USAGE for more info. */

// John
// Sep 7 94

private static string	getmsg;
private static int	gettable;
private static string	dropmsg;
private static int	non_dropable;


void
set_getmsg( string s )
{
  getmsg = s;
}

string
query_getmsg()
{
  return getmsg;
}

void
set_gettable( int g )
{
  gettable = g;
}

mixed
get()
{
  object env;
  int tmp;

  if (gettable < 0) {
    write(getmsg);
    return -1;
  }

  if (!gettable) return 0;

  env = environment();
  while (env) {
    if (tmp = environment()->can_take_from())
      return tmp;
    env = environment(env);
  }
  return 1;
}

void
set_dropmsg( string s )
{
  dropmsg = s;
}

string
query_dropmsg()
{
  return dropmsg;
}

void
set_dropable( int g )
{
  if( g >= 0 )
	g = !g;

  non_dropable = g;
}

mixed
drop()
{
  if (dropmsg) {
    write(dropmsg);
    return -1;
  }

  return !non_dropable;
}

/* verb interaction */
mixed direct_get_obj(object ob) {
    if (gettable < 0)
      return getmsg;
    if (gettable == 0)
      return 0;
    return 1;
}

mixed direct_drop_obj(object ob) {
    if (dropmsg)
      return dropmsg;
    return !non_dropable;
}
