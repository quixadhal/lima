/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name()
{
   return "verb methods";
}

nomask string module_key()
{
   return "v";
}

nomask string module_user()
{
   return "[Mudlib:daemons]";
}

nomask mixed module_priv()
{
   return "Mudlib:daemons";
}

private void receive_remove_method(string method)
{
   METHOD_D->remove_method(method);
   write("Done.\n");
}

private void receive_remove_method_equivalent(string method, string equiv)
{
   METHOD_D->remove_method_equivalents(method, equiv);
   write("Done.\n");
}

private void receive_add_method(string method, string equivs)
{
   string array tmp = map(explode(equivs, ","), (: trim_spaces :));

   METHOD_D->add_method(method);
   if(sizeof(tmp))
   {
      foreach(string s in tmp)
      {
         METHOD_D->add_method_equivalents(method, s);
      }
   }
   write("Done.\n");
}

private void receive_add_method_equivalent(string method, string equiv)
{
   METHOD_D->add_method_equivalents(method, equiv);
   write("Done.\n");
}

private void list_methods()
{
   string array tmp = METHOD_D->list_methods();
   write(implode(tmp, ", ") + "\n\n");
}

private void list_method_equivalents(string method)
{
   string array tmp = METHOD_D->list_method_equivalents(method);
   if(!tmp)
   {
      write("That method does not exist. Use 'a' to add it.\n");
   }
   else
   {
      write(implode(tmp, ", ") + "\n\n");
   }
}

nomask class command_info array module_commands()
{
   return ({
      new(class command_info,
         key : "l",
         desc : "list methods",
         action : (: list_methods :)),
      new(class command_info,
         key : "e",
         proto : "[method]",
         desc : "list method equivalents",
         args : ({ "Method: " }),
         action : (: list_method_equivalents :)),
      new(class command_info,
         key : "a",
         proto : "[method] [equiv, equiv, ...]",
         desc : "add method",
         args : ({ "Method: ", "Equivs (seperate by commas): " }),
         action : (: receive_add_method :)),
      new(class command_info,
         key : "A",
         proto : "[method] [equiv]",
         desc : "add an equivalent to a method",
         args : ({ "Add to which method? ", "Add what equivalent? " }),
         action : (: receive_add_method_equivalent :)),
      new(class command_info,
         key : "r",
         proto : "[method]",
         desc : "remove method",
         args : ({ "Remove which method? " }),
         action : (: receive_remove_method :)),
      new(class command_info,
         key : "R",
         proto : "[method] [equiv]",
         desc : "remove equivalent from method",
         args : ({ "Remove from which method? ", "Remove which equivalent? " }),
         action : (: receive_remove_method_equivalent :)),
   });
}
