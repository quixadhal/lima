#include <limbs.h>

inherit "/obj/admtool/internal/base.c";
inherit CLASS_LIMB;

nomask string module_name() {
  return "body";
}

nomask string module_key() {
  return "B";
}

nomask string module_user() {
  return "[Mudlib:daemons]";
}

nomask string module_priv() {
  return "Mudlib:daemons";
}

private nomask void print_body(string cmd, string bodytype)
{
  mapping body_info = BODY_D->get_body(bodytype);
  string output = sprintf("%-20s %-15s %-15s %-15s\n", "Limb", "Health", "Parent Limb", "Type");

  output += "--------------------------------------------------------------------\n";
  foreach ( string key, class limb val in body_info )
    {
      string *type = ({});
      if ( LIMB_VITAL &  val->flags )  type += ({ "vital" });
      if ( LIMB_WIELDING &  val->flags ) type += ({ "wielding" });
      if ( LIMB_MOBILE &  val->flags ) type += ({ "mobile" });
      if ( LIMB_SYSTEM &  val->flags ) type += ({ "system" });
      if ( LIMB_ATTACKING & val->flags ) type += ({ "attacking" });
      output += sprintf("%-20s %-15d %-15s %-15s\n",
			key,
			val->health,
			val->parent?val->parent:"None",
			implode(type, ", "));
    }
  more(output);
}

private nomask void add_body(string cmd, string bodytype)
{
  if ( sizeof( BODY_D->get_body(bodytype) ) == 0 )
    {
      BODY_D->add_body(bodytype, ([]));
      write("Added.\n");
    }
  else
    write("Already exists.\n");
}

private nomask void finish_add_limb(string bodytype, string limb, int health, string parent, int flags)
{
  BODY_D->add_limb_to_body(bodytype, limb, to_int(health), to_int(health), parent, to_int(flags));
  write("Limb added.\n");
}

private nomask void set_limb_flags(string bodytype, string limb, int health, string parent, string flags) {
  int limbflags;
  if(limbflags=to_int(flags)) {
    if(limbflags > 31) {
      input_one_arg("Limb flags cannot sum to more than 31: ",
                    (: set_limb_flags, bodytype, limb, health, parent :) );
      return;
    }
  }
  else {
    string array parts=explode(flags,",")-({});
    foreach(string part in parts)
      {
	switch(trim_spaces(lower_case(part)))
	  {
	  case "vital":
	    limbflags |= LIMB_VITAL;
	    continue;
	  case "wielding":
	    limbflags |= LIMB_WIELDING;
	    continue;
	  case "mobile":
	    limbflags |= LIMB_MOBILE;
	    continue;
	  case "system":
	    limbflags |= LIMB_SYSTEM;
	    continue;
          case "attacking":
             limbflags |= LIMB_ATTACKING;
             continue;
	  default:
	    input_one_arg("Only 'vital', 'wielding', 'mobile', 'attacking' and 'system' flags are allowed\n"
			  "Please reenter: ", 
			  (:set_limb_flags, bodytype, limb, health, parent :) );
	    return;
	  }
      }
  }
  finish_add_limb(bodytype,limb,health,parent,limbflags);
}

private nomask void set_limb_parent(string bodytype, string limb, int health, string parent) {
  
  input_one_arg("Limb Flags: ", (:set_limb_flags, bodytype, limb, health, parent :) );
}

private nomask void set_limb_health(string bodytype, string limb, string health) {
  if(!to_int(health))
    {
      input_one_arg("You must enter an integer value: ",
		    (: set_limb_health, bodytype, limb :) );
      return;
    }
  input_one_arg("Parent Limb: ", 
		(:set_limb_parent, bodytype, limb, to_int(health) :) );
}

private nomask void add_limb(string cmd, string bodytype, string limb)
{
  if ( BODY_D->get_body(bodytype) == 0 )
    {
      write("That body does not exist.\n");
      return;
    }
  input_one_arg("Health: ",(: set_limb_health , bodytype, limb :) );
}

private nomask void remove_limb(string cmd, string bodytype, string limb)
{
  BODY_D->remove_limb_from_body(bodytype, limb);
  write("Removed limb.\n");
}

private nomask void remove_body(string cmd, string bodytype)
{
  BODY_D->remove_body(bodytype);
  write("Removed.\n");
}

private nomask void list_body_types() {
  string *types = sort_array(BODY_D->list_body_types(),1);

  if ( sizeof(types) == 0 ) {
    write("No body types are defined.\n");
    return;
  }
  printf("\nBODY TYPES:\n");
  foreach ( string type in types ) {
    write(capitalize(type) + "\n");
  }
}

nomask class command_info array module_commands() {
  return ({
      new(class command_info,
          key : "p",
          proto : "[bodytype]",
          args : ({ "Bodytype: " }),
          action : (: print_body, "p" :),
          desc : "print body type"),
      new(class command_info,
          key : "l",
          desc : "list body types",
          action : (: list_body_types :)),
      new(class command_info),
          new(class command_info,
          key : "a",
          proto : "[bodytype] [limb]",
          args : ({ "Bodytype: ", "Limb: " }),
          action : (: add_limb, "a" :),
          desc : "add limb to body type"),
      new(class command_info,
          key : "r",
          proto : "[bodytype] [limb]",
          args : ({ "Bodytype: ", "Limb: " }),
          action : (: remove_limb, "r" :),
          desc : "remove limb from body type"),
      new(class command_info,
          key : "A",
          proto : "[bodytype]",
          args : ({ "Bodytype: " }),
          action : (: add_body, "R" :),
          desc : "add body type"),
      new(class command_info,
          key : "R",
          proto : "[bodytype]",
          args : ({ "Bodytype: " }),
          action : (: remove_body, "R" :),
          desc : "remove body type")
      });
}
