/* Do not remove the headers from this file! see /USAGE for more info. */

// This could print a tree, but that would take effort. -- Rust

//:ADMINCOMMAND
//USAGE: snoops
//
//Displays a list of wizards currently snooping, together with their targets.
//
//Admin only

inherit CMD;

private void main()
{
  mapping raw_data;
  mapping str_data = ([]);
  string array snoopers;


  if(!adminp(this_body()))
    {
      out("This command is for admins only, sorry :(\n");
      return;
    }

  raw_data = SNOOP_D->get_snoop_info();

  if(!sizeof(raw_data))
    {
	out("Nobody is being snooped.\n");
	return;
    }
  foreach(object snoop_ob in keys(raw_data))
    {
      // Haha! Obfuscation rox!
      // This code: 
      // 1) converts the object in the current key into a proper name
      // 2) converts the people that guy is snooping into a _sorted_ comma
      //    seperated string (removing dested people by subtracting ({0}) )
      // 3) stuffs 1 mapped to 2 into a mapping (str_data).
      str_data[capitalize(snoop_ob->query_userid())] = 
	implode(sort_array(map(raw_data[snoop_ob]-({0}), 
		       (: capitalize($1->query_userid()) :)), 0), ", ");
      
    }

  snoopers = sort_array(keys(str_data), 0);
  foreach(string snooper in snoopers)
    {
      outf("%s: %s\n", snooper, str_data[snooper]);
    }
}
