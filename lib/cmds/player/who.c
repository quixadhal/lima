/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust 2-6-94

//:PLAYERCOMMAND
//USAGE who [ -w -p ]
//
//The who command will give you a list of all user's presently logged
//onto the MUD, and whether or not they are idle.
//
//Used with the given arguments you can alter the way who works:
//
//who -w
//Will show wizards only.
//
//who -p
//Will show players only.

#include <playerflags.h>

inherit CMD;

#define DIVIDER \
"-------------------------------------------------------------------------"

#ifdef ZORKMUD
# define USER_DESC	"(ZORKERS ONLY)"
# define WHO_FORMAT	"%s:  (GUE Time is: %s) %-28s%d\n%s\n"
#else
# define USER_DESC	"(PLAYERS ONLY)"
# define WHO_FORMAT	"%s:  (Local Time is: %s) %s%d users listed.\n%s\n"
#endif


string get_who_string(string arg)
{
  object *u;
  int    i;
  string name, extra, retval;

  extra = retval = "\n";
  if ( this_user() )
  {
    switch ( arg )
    {
      case "-p":
      case "-z":
        u = filter_array(bodies(), (: !wizardp($1) :));
        extra = USER_DESC + "\n";
        break;
      case "-w":
      case "-i":
        u = filter_array(bodies(), (: wizardp :));
        extra = "(IMPLEMENTORS ONLY)\n";
        break;
      case "-m":
        if ( wizardp(this_user()) )
        {
          u = filter_array(children(USER_OB),
              (: !interactive($1) :))->query_body();
              extra = "(NON-INTERACTIVES)\n";
              break;
        }
/* FALLTHRU */
      default:
        if ( arg )
          retval += "Who: Unknown flag.\n";
        u = bodies();
        break;
    }
    if ( !wizardp(this_user()) )
      u = filter_array(u, (: $1->is_visible() :));
  }
  else
    u = filter_array(bodies(), (: $1->is_visible() :));

  u -= ({ 0 });
  i = sizeof(u);
  retval += sprintf(WHO_FORMAT, mud_name(), ctime(time()), extra,
      i, DIVIDER);
  if ( !i )
    retval += sprintf("%|70s\n","Sorry, no one fits that bill.");
  foreach ( object body in u )
  {
    name = body->query_formatted_desc(78);
    if ( !name )
      name = capitalize(body->query_userid());

    if ( !body->is_visible() )
      name = "("+name+")";
    if ( body->test_flag(F_IN_EDIT) )
      name = "*"+ name;
    retval += sprintf("%-68s\n",name);
  }
  return retval + DIVIDER;
}

private void main(string arg)
{
  if ( arg == "" )
    arg = 0;

  out(get_who_string(arg));
}

void player_menu_entry()
{
  bare_init();
  main(0);
  done_outputing();
}