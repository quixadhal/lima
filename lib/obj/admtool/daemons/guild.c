/* Do not remove the headers from this file! see /USAGE for more info. */

/*************************************************************
 *
 *      guild.c
 *
 *  Admtool interface for the guild daemon
 *
 *  Coded by Aktah 1-28-00
 *
 *************************************************************/

#include <edit.h>

inherit "/obj/admtool/internal/base.c";

nomask string module_name() 
{
  return "guild";
}

nomask string module_key()
{
  return "G";
}

private nomask void list_guilds()
{
  string array guilds = sort_array(GUILD_D->query_guilds(), 1);

  if(sizeof(guilds) == 0)
  {
    write("There are no guilds defined.\n");
    return;
  }

  printf("%-18s%-40s\n%*'-'s\n", "Name", "Title", 58, "");
  foreach(string guild in guilds)
  {
    printf("%-18s%-40s\n", capitalize(guild), capitalize(GUILD_D->query_guild_title(guild)));
  }
}

private nomask void show_guild(string name)
{
  string str, temp;
  string array ary;
  int i;

  str = "Name: " + capitalize(name) + "\n";
  str += "Title: " + GUILD_D->query_guild_title(name) + "\n";
  str += "Prospectus: " + GUILD_D->query_guild_prospectus(name) + "\n";
  ary = GUILD_D->query_guild_banned(name);
  if(sizeof(ary))
  {
    temp = "Banned guilds: " + ary[0] + "\n";
    for(i = 1; i < sizeof(ary); i++)
      temp += "               " + ary[i] + "\n";
    str += temp;
  }
  ary = GUILD_D->query_guild_prereq(name);
  if(sizeof(ary))
  {
    temp = "Prereq guilds: " + ary[0] + "\n";
    for(i = 1; i < sizeof(ary); i++)
      temp += "               " + ary[i] + "\n";
    str += temp;
  }
  ary = GUILD_D->query_guild_allies(name);
  if(sizeof(ary))
  {
    temp = "Allied guilds: " + ary[0] + "\n";
    for(i = 1; i < sizeof(ary); i++)
      temp += "               " + ary[i] + "\n";
    str += temp;
  }
  ary = GUILD_D->query_guild_exclusive(name);
  if(sizeof(ary))
  {
    temp = "Excluded Attributes: " + ary[0] + "\n";
    for(i = 1; i < sizeof(ary); i++)
      temp += "                    " + ary[i] + "\n";
    str += temp;
  }
  ary = GUILD_D->query_guild_attributes(name);
  if(sizeof(ary))
  {
    temp = "Attributes: " + ary[0] + "\n";
    for(i = 1; i < sizeof(ary); i++)
      temp += "            " + ary[i] + "\n";
    str += temp;
  }
  str += "Begone level: " + GUILD_D->query_guild_begone(name) + "\n";
  str += "Suspend level: " + GUILD_D->query_guild_suspend(name) + "\n";
  if(GUILD_D->query_guild_sees_secret(name))
    str += "Sees secret.\n";
  else
    str += "Does not see secret.\n";
  if(GUILD_D->query_guild_is_secret(name))
    str += "Is secret.\n";
  else
    str += "Is not secret.\n";
  if(GUILD_D->query_guild_need_all(name))
    str += "Needs all prereqs.\n";
  else
    str += "Does not need all prereqs.\n";
  write(str);
}

private nomask void add_guild(string name)
{
  GUILD_D->guild_add(name);
  printf("Guild %s has been added.\n", name);
}

private nomask void remove_guild(string name)
{
  GUILD_D->guild_remove(name);
  printf("Guild %s has been removed.\n", name);
}

private nomask void set_title2(string name, string title)
{
  GUILD_D->set_guild_title(name, title);
}

private nomask void set_title(string name)
{
  modal_simple( (: set_title2, name :), "Title? ");
}

private nomask void send_prospectus(string name, string array lines)
{
  GUILD_D->set_guild_prospectus(name, implode(lines, " "));
}

private nomask void prospectus(string name)
{
  write("Prospectus for guild " + name + ":\n");
  new(EDIT_OB, EDIT_TEXT, 0, (: send_prospectus, name :));
}

private nomask void ban(string name, string guild)
{
  if(!GUILD_D->query_guild_banned(name))
  {
    GUILD_D->set_guild_banned(name, guild);
    return;
  }
  GUILD_D->set_guild_banned(name,
    GUILD_D->query_guild_banned(name) + ({ guild }));
}

private nomask void prereq(string name, string guild)
{
  if(!GUILD_D->query_guild_prereq(name))
  {
    GUILD_D->set_guild_prereq(name, guild);
    return;
  }
  GUILD_D->set_guild_prereq(name,
    GUILD_D->query_guild_prereq(name) + ({ guild }));
}

private nomask void ally(string name, string guild)
{
  if(!GUILD_D->query_guild_allies(name))
  {
    GUILD_D->set_guild_allies(name, guild);
    return;
  }
  GUILD_D->set_guild_allies(name, 
    GUILD_D->query_guild_allies(name) + ({ guild }));
}

private nomask void exclude(string name, string attr)
{
  if(!GUILD_D->query_guild_exclusive(name))
  {
    GUILD_D->set_guild_exclusive(name, attr);
    return;
  }
  GUILD_D->set_guild_exclusive(name, 
    GUILD_D->query_guild_exclusive(name) + ({ attr }));
}

private nomask void attrib(string name, string attr)
{
  if(!GUILD_D->query_guild_attributes(name))
  {
    GUILD_D->set_guild_attributes(name, attr);
    return;
  }
  GUILD_D->set_guild_attributes(name, 
    GUILD_D->query_guild_attributes(name) + ({ attr }));
}

private nomask void begone(string name, string level)
{
  GUILD_D->set_guild_begone(name, to_int(level));
}

private nomask void suspend(string name, string level)
{
  GUILD_D->set_guild_suspend(name, to_int(level));
}

private nomask void see_secret(string name)
{
  if(GUILD_D->query_guild_sees_secret(name))
  {
    GUILD_D->set_guild_sees_secret(name, 0);
    write(capitalize(name) + " no longer sees secret.\n");
  }
  else
  {
    GUILD_D->set_guild_sees_secret(name, 1);
    write(capitalize(name) + " sees secret.\n");
  }
}

private nomask void is_secret(string name)
{
  if(GUILD_D->query_guild_is_secret(name))
  {
    GUILD_D->set_guild_is_secret(name, 0);
    write(capitalize(name) + " is no longer secret.\n");
  }
  else
  {
    GUILD_D->set_guild_is_secret(name, 1);
    write(capitalize(name) + " is secret.\n");
  }
}

private nomask void need_all(string name)
{
  if(GUILD_D->query_guild_need_all(name))
  {
    GUILD_D->set_guild_need_all(name, 0);
    write(capitalize(name) + " no longer needs all prereqs.\n");
  }
  else
  {
    GUILD_D->set_guild_need_all(name, 1);
    write(capitalize(name) + " needs all prereqs.\n");
  }
}

nomask class command_info array module_commands() 
{
  return ({
    new(class command_info,
      key : "l",
      desc : "list guilds",
      action : (: list_guilds :)),
    new(class command_info,
      key : "v",
      proto : "[name]",
      desc : "View a guild",
      args : ({ "Which guild? " }),
      action : (: show_guild :)),
    new(class command_info,
      key : "g",
      proto : "[name]",
      desc : "Add a guild",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({ "What name? " }),
      action : (: add_guild :)),
    new(class command_info,
      key : "r",
      proto : "[name]",
      desc : "Remove a guild",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({ "What name? " }),
      action : (: remove_guild :)),
    new(class command_info,
      key : "t",
      proto : "[name]",
      desc : "Set a guild's title",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({ "What name? " }),
      action : (: set_title :)),
    new(class command_info,
      key : "p",
      proto : "[name]",
      desc : "Set a guild's prospectus",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({ "What name? " }),
      action : (: prospectus :)),
    new(class command_info,
      key : "b",
      proto : "[name] [guild to be banned]",
      desc : "Add to a guild's banned list",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Which guild? " }),
      action : (: ban :)),
    new(class command_info,
      key : "P",
      proto : "[name] [guild to be needed]",
      desc : "Add to a guild's prereq list",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Which guild? " }),
      action : (: prereq :)),
    new(class command_info,
      key : "A",
      proto : "[name] [ally]",
      desc : "Add to a guild's ally list",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Which guild? " }),
      action : (: ally :)),
    new(class command_info,
      key : "e",
      proto : "[name] [attribute]",
      desc : "Add to guild's exclude list",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Which attribute? " }),
      action : (: exclude :)),
    new(class command_info,
      key : "a",
      proto : "[name] [attribute]",
      desc : "Add to guild's attributes",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Which attribute? " }),
      action : (: attrib :)),
    new(class command_info,
      key : "B",
      proto : "[name] [level]",
      desc : "Set a guild's begone level",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Begone level? " }),
      action : (: begone :)),
    new(class command_info,
      key : "s",
      proto : "[name] [level]",
      desc : "Set a guild's suspend level",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? ", "Suspend level? " }),
      action : (: suspend :)),
    new(class command_info,
      key : "S",
      proto : "[name]",
      desc : "Toggle a guild's sees_secret flag",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? "}),
      action : (: see_secret :)),
    new(class command_info,
      key : "i",
      proto : "[name]",
      desc : "Toggle a guild's is_secret flag",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? "}),
      action : (: is_secret :)),
    new(class command_info,
      key : "n",
      proto : "[name]",
      desc : "Toggle a guild's need_all flag",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({"What name? "}),
      action : (: need_all :)),
  });
}
