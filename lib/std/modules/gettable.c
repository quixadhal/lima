/* Do not remove the headers from this file! see /USAGE for more info. */

// John
// Sep 7 94

private static string	getmsg;
private static int	gettable;
private static string	dropmsg;
private static int	non_dropable;
private static function	my_drop_hook, my_get_hook;

void add_hook(string, function);

private mixed prevent_drop() {
    if (dropmsg)
      return dropmsg;
    return !non_dropable;
}

private mixed prevent_get() {
    if (gettable < 0)
	return getmsg;
    if (gettable == 0)
	return 0;
    return 1;
}

void
set_getmsg( string s )
{
    getmsg = s;
    if (!my_get_hook) {
	my_get_hook = (: prevent_get :);
	add_hook("prevent_get", my_get_hook);
    }
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
    if (!my_get_hook) {
	my_get_hook = (: prevent_get :);
	add_hook("prevent_get", my_get_hook);
    }
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
    if (!my_drop_hook) {
	my_drop_hook = (: prevent_drop :);
	add_hook("prevent_drop", my_drop_hook);
    }
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
    if (!my_drop_hook) {
	my_drop_hook = (: prevent_drop :);
	add_hook("prevent_drop", my_drop_hook);
    }
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

int is_gettable()
{
	return gettable;
}
