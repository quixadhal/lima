/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";


nomask string module_name() {
    return "colour";
}

nomask string module_key() {
    return "C";
}

nomask string module_user() {
    return "[Mudlib:daemons]";
}

nomask mixed module_priv() {
    return "Mudlib:daemons";
}

private void receive_remove_colour(string key) {
    ANSI_D->remove_default_colour(key);
    write("Done.\n");
}

private void receive_add_colour(string key, string value, int restricted) {
    ANSI_D->add_default_colour(key, value, restricted);
    write("Done.\n");
}

//Returns lower_cased if unrestricted
string restrict_modify(string val)
{
  if(member_array(val, ANSI_D->query_restrictions())<0)
    return lower_case(val);
  return val;
}

private void list_colours() {
  string * restrict = ANSI_D->query_restrictions();
  mixed tmp;
  tmp = ANSI_D->defaults();
  tmp = sort_array(keys(tmp), 1);
  tmp = map(tmp, (: restrict_modify :));
  tmp = implode(tmp, ", ");
  write("Default colours (wiz-only in CAPs)\n" + tmp + "\n\n");
}

private void restrict(string key)
{
  ANSI_D->add_restriction(key);
}

private void unrestrict(string key)
{
  ANSI_D->remove_restriction(key);
}

nomask class command_info array module_commands() {
  return ({
    new(class command_info,
      key : "l",
      desc : "list default colours",
      action : (: list_colours :)),
    new(class command_info,
      key : "a",
      proto : "[colour] [value] [restricted]",
      desc : "add default colour",
      args : ({ "Colour code? ", "Value? ", "1 for wiz-only? " }),
     action : (: receive_add_colour :)),
    new(class command_info,
      key : "r",
      proto : "[colour]",
      desc : "remove default colour",
      args : ({ "Remove which colour? " }),
      action : (: receive_remove_colour :)),
    new(class command_info,
      key : "w",
      proto : "[colour]",
      desc : "make colour wiz-only",
      args : ({ "Make which colour wiz-only? " }),
      action : (: restrict :)),
    new(class command_info,
      key : "p",
      proto : "[colour]",
      desc : "make colour player-usable",
      args : ({ "Make which colour player-usable? " }),
      action : (: unrestrict :))
    });
}
