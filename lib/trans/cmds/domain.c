/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <security.h>

inherit CMD;

void create()
{
  set_privilege(1);
}

void write_error(string error)
{
  if (!error)
    write("Ok.\n");
  else
    write("Error: " + error +".\n");
}

private void main(string arg)
{
  string *args;
  mixed temp, temp2;
  int i;
  string err;
  if (!arg)
    return 0;
  args = explode(arg," ")-({""});
  arg = args && args[0];
  args = args[1..];
  switch (arg)
  {
    case "create":
      if (sizeof(args)!=1)
        return 0;
      args[0] = lower_case(args[0]);
      err = SECURE_D->create_domain(args[0]);
      if (err)
      {
	write_error(err);
	return;
      }
      write("Domain created.\n");
      mkdir("/domains/"+args[0]);
      write("Created /domains/"+args[0]+"\n");
      write("Setting protection level of /domains/"+args[0]+"\n");
      err =
      SECURE_D -> set_protection("/domains/"+args[0],1,capitalize(args[0]+":"));
      if (err)
      {
	write_error(err);
	return;
      }
      return;
    case "delete":
      if (sizeof(args)!=1)
        return 0;
      SECURE_D->set_protection("/domains/"+args[0],1,-1);
      err = SECURE_D->delete_domain(args[0]);
      if (err)
      {
	write_error(err);
	return;
      }
      write("Domain deleted.\n");
      write("Please remove files and entry in /room/init_file manually.\n");
      return;
    case "add":
      if (sizeof(args)<3 || args[<2]!="to")
        return 0;
      temp = args[<1];
      args = args[0..<3];
      for (i=0; i<sizeof(args); i++)
      {
	if (args[i]=="-lord" && i+1 <sizeof(args))
	{
	  temp2 = args[++i];
	  err = SECURE_D->add_domain_member(temp,temp2,1);
	  if (!err)
	    write("Added lord "+temp2+" to "+temp+".\n");
	}
	else
	{
	  temp2 = args[i];
	  err = SECURE_D->add_domain_member(temp,temp2,0);
	  if (!err)
	    write("Added "+temp2+" to "+temp+".\n");
	}
	if (err)
	  write_error(err);
      }
      return;
    case "remove":
      if (sizeof(args)<3 || args[<2]!="from")
        return 0;
      temp = args[<1];
      args = args[0..<3];
      for (i=0; i<sizeof(args); i++)
      {
	err = SECURE_D -> remove_domain_member(temp,args[i]);
	if (err)
	  write_error(err);
	else
	  write("Removed "+args[i]+" from "+temp+".\n");
      }
      return;
    case "list":
      if (!sizeof(args))
      {
	temp = SECURE_D -> query_domains();
	temp = sort_array(temp,1);
	if (!sizeof(temp))
	  write("There are no domains.\n");
	else
	{
	  write("The following domains exist:\n");
	  printf("%-#78s\n",implode(temp,"\n")+"\n");
	}
	return;
      }
      for (i=0; i<sizeof(args); i++)
      {
	temp = sort_array(SECURE_D->query_domains(args[i])||({ }),1);
	if (!sizeof(temp))
	  write("Wizard "+args[i]+" doesn't belong to any domain.\n");
	else
	  write("Wizard "+args[i]+" belongs to "+
	    implode(temp,", ")+".\n");
      }
      return;
    case "show":
      if (!sizeof(args))
	return 0;
      for (i=0; i<sizeof(args); i++)
      {
	temp = SECURE_D ->query_domain_members(args[i]);
	if (!temp)
	  write("There is no domain "+args[i]+".\n");
	else
	{
	  temp2 = SECURE_D -> query_domain_lords(args[i]) || ({ });
	  temp -= temp2;
	  write("Domain "+args[i]+":\n");
	  switch (sizeof(temp2))
	  {
	    case 0:
	      write("Lords: (none)\n");
	      break;
	    case 1:
	      write("Lord: "+temp2[0]+"\n");
	      break;
	    default:
	      write("Lords: "+implode(temp2,", ")+"\n");
	  }
	  switch (sizeof(temp))
	  {
	    case 0:
	      write("Members: (none)\n");
	      break;
	    case 1:
	      write("Member: "+temp[0]+"\n");
	      break;
	    default:
	      write("Members: "+implode(temp,", ")+"\n");
	  }
	}
      }
      return;
  }
}

