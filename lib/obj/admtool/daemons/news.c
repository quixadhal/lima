/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "news";
}

nomask string module_key() {
    return "n";
}

nomask string module_user() {
    return "[Mudlib:daemons]";
}

nomask string module_priv() {
    return "Mudlib:daemons";
}

private nomask void list_newsgroups()
{
    string* grouplist = NEWS_D->get_groups();

    if ( sizeof(grouplist) == 0 )
    {
	write("    <none>\n");
	return;
    }
    grouplist = sort_array(grouplist, 1);
    more(grouplist);

}

private nomask void rcv_add_newsgroup(string str)
{
    str = lower_case(trim_spaces(str));
    if ( str == "" )
	return;

    if ( member_array(str, NEWS_D->get_groups()) != -1 )
    {
	write("** That group already exists.\n");
	return;
    }
    NEWS_D->add_group(str);
    printf("** Group '%s' added.\n", str);
}

private nomask void rcv_remove_newsgroup(string group_name)
{
    string* grouplist = NEWS_D->get_groups();
    
    if ( !group_name )
    {
	write("** no group name supplied.\n");
	return;
    }

    group_name = lower_case(trim_spaces(group_name));
    if ( member_array(group_name, grouplist) == -1 )
    {
	write("** That newsgroup does not exist.\n");
	return;
    }

    NEWS_D->remove_group(group_name);
    printf("** Group '%s' removed.\n", group_name);
}

private void rcv_archive_group(string group,int days)
{
  if ( member_array(group,NEWS_D->get_groups()) == -1 )
    {
      write("** That newsgroup does not exist.\n");
      return;
    }
  NEWS_D->set_archive_time(group,to_int(days));
}

private void rcv_show_archive_times()
{
  string array output=({});
  mapping archive_info=NEWS_D->get_archive_info();
  if(!sizeof(keys(archive_info)))
    {
      write("No archived newsgroups.\n");
      return;
    }
  foreach(string group,int day in archive_info)
    output+=({sprintf("%s archives after %d days",group,day)});
  more(output);
}

private void rcv_forward_mail(string group,string destination)
{
  if(member_array(group,NEWS_D->get_groups())==-1)
    {
      write ("** That newsgroup does not exist\n");
      return;
    }
  NEWS_D->add_mail_forward(group,destination);
  printf("Group %s forwarded to %s\n",group,destination);
}

private void rcv_unforward_mail(string group,string destination)
{
  if(member_array(group,NEWS_D->get_groups())==-1)
    {
      write("** That newsgroup does not exist\n");
      return;
    }
  if(destination=="0")
    {
      NEWS_D->remove_mail_forward(group);
      printf("Group %s is no longer forwarded anywhere\n",group);
      return;
    }
  if(member_array(destination,NEWS_D->query_mail_forward())>-1)
    NEWS_D->remove_mail_forward(group);
  printf("Group %s is not forwarded to %s",group,destination);
}

private nomask void rcv_list_forward_newsgroup(string group)
{
  string array addies;
  if(member_array(group,NEWS_D->get_groups())==-1)
    {
      write("** That newsgroup does not exist\n");
      return;
    }
  addies=NEWS_D->query_mail_forward(group);
  printf("Group %s is forwarded to the following addresses\n",group);
  more(addies);
}

private nomask void show_restrictions()
{
  string output;
  mapping restrictions=NEWS_D->list_restrictions();
  output=sprintf("Prefix restrictions\n%s",
		 sprintf("%=|19s%=|30s%=|30s\n","Prefix","Write","Read") );
  foreach(string prefix,mixed restrict in restrictions)
    output+=sprintf("%=19s%=30O%=30O\n",prefix,restrict[0],restrict[1]);
  more(output);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "List newsgroups.",
	    action : (: list_newsgroups :)),
	new(class command_info,
	    key : "a",
	    proto : "[group]",
	    desc : "Add a new newsgroup",
	    args : ({ "New newsgroup name? " }),
	    action : (: rcv_add_newsgroup :)),
	new(class command_info,
	    key : "r",
	    proto : "[group]",
	    desc : "Remove a newsgroup",
	    args : ({ "Remove which newsgroup? " }),
	    action : (: rcv_remove_newsgroup :)),
	new(class command_info,
	    key : "A",
	    proto : "[group] [#days]",
	    desc : "Archive a group", 
	    args : ({ "Which newsgroup? ", "How many days? " }),
	    action : (: rcv_archive_group :)),
	new(class command_info,
	    key : "T",
	    desc : "Show archive times of groups",
	    action : (: rcv_show_archive_times :)),
	new(class command_info,
	    key : "f",
	    proto : "[group] [email]",
	    desc : "Forward a group to an email address",
	    args : ({ "Which newsgroup? ", "Email destination? " }),
	    action : (: rcv_forward_mail :)),
	new(class command_info,
	    key : "u",
	    proto : "[group] [email]",
	    desc : "Remove an email a group is forwarded to",
	    args : ({ "Which newsgroup? ", "Email address (0 for all) " }),
	    action : (: rcv_unforward_mail :)),
	new(class command_info,
	    key: "L",
	    proto : "[group]",
	    desc : "List emails a group is forwarded to",
	    args : ({ "Which newsgroup? " }),
	    action : (: rcv_list_forward_newsgroup :)),
	new(class command_info,
	    key : "R",
	    proto : "[prefix] [restrict]",
	    desc : "Set the read restrictions of a prefix",
	    args : ({ "Which prefix? ", "restriction? " }),
	    action : (: call_other, NEWS_D, "set_read_restriction" :)),
	new(class command_info,
	    key : "W",
	    proto : "[prefix] [restrict]",
	    desc : "Set the write restrictions of a prefix",
	    args : ({ "Which prefix? ", "Restriction? " }),
	    action : (: call_other, NEWS_D, "set_write_restriction" :)),
	new(class command_info,
	    key : "S",
	    desc : "Show the prefix restrictions",
	    action : (: show_restrictions :)),
    });
}
