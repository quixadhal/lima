/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() 
{
  return "damage";
}

nomask string module_key()
{
  return "D";
}

private nomask void list_damage_types()
{
  string array types = sort_array(DAMAGE_D->query_damage_types(), 1);

  if(sizeof(types) == 0)
    write("There are no damage types defined.\n");
  else
    write(implode(types, "\n") + "\n");
}

private nomask void add_damage_type(string name)
{
  DAMAGE_D->add_damage_type(name);
  printf("Damage type \"%s\" has been added.\n", name);
}

private nomask void remove_damage_type(string name)
{
  DAMAGE_D->remove_damage_type(name);
  printf("Damage type \"%s\" has been removed.\n", name);
}

nomask class command_info array module_commands() 
{
  return ({
    new(class command_info,
      key : "l",
      desc : "list damage types",
      action : (: list_damage_types :)),
    new(class command_info,
      key : "a",
      proto : "[name]",
      desc : "Add a damage type",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({ "What name? " }),
      action : (: add_damage_type :)),
    new(class command_info,
      key : "r",
      proto : "[name]",
      desc : "Remove a damage type",
      who : "[Mudlib]",
      priv : "Mudlib",
      args : ({ "What name? " }),
      action : (: remove_damage_type :)),
  });
}
