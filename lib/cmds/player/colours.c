/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//$$ see: ansi
//USAGE colours
//      colours <name> <value>
//      colours remove <value>
//
//This command handles customising the colours displayed in the mud -
//ie changing them to suit your tastes
//Some of types items displayed in the mud are allocated a "colour name",
//which is then given a default "colour value"
//eg "say" could be given a value "blue", in which case all "says" would
//appear in blue
//If you wish to have them appear in red, you would use :
//colours say red
//You can also use "bold" as a colour, with comma if necessary - eg
//colour say bold,red
//You can clear your personalised setting, and revert to the mud default, with
//colours remove say

inherit CMD;
inherit M_ANSI;

void do_print(string which)
{
  string array colours;
  string array illegal;
  string array irrelevant;
  mapping null = ANSI_D->query_translations()[1];
  int longest;

  if(wizardp(this_user()))
    irrelevant = ({});
  else
    irrelevant = ANSI_D->query_restrictions();

  if (which)
    colours = ({ which });
  else
    colours = this_user()->query_colours() - irrelevant;

  if (!colours)
  {
    write("No colours set.\n");
    return;
  }

  illegal = filter(colours, (: $(null)[$1] == 0 :));
  colours -= illegal;

  foreach (string item in colours)
    if (strlen(item) > longest)
      longest = strlen(item);

  illegal = map(illegal, (: lower_case :));
  if (sizeof(illegal))
    write("Unsupported settings (use 'colours remove whatever'): " + implode(illegal, "\n") + "\n\n");

  colours = map(sort_array(colours, 1), function(string item, int longest)
    {
      string what = this_user()->query_colour(item);
      if (what)
        return sprintf("%-"+longest+"s : %%^%s%%^%s%%^RESET%%^", lower_case(item), upper_case(item), what);
      else
        return sprintf("%-"+longest+"s : (not set)", item);
    },
    longest);
  write(colour_table(colours, this_user()->query_screen_width()) + "\n");
  write("USAGE:\n");
  write("\"colours <name> <value>\" to change a value\n");
  write("eg \"colours say magenta\" for all says to appear in magenta\n");
  write("\"colours remove <name>\" to remove a value\n");
  write("eg \"colours remove say\" to remove the setting for say\n");
  write("and all says will then revert to the global default colour\n");
}

nomask private void main(string str)
{
  string which, what;

  if (!str)
  {
    do_print(0);
    return;
  }

  if (sscanf(str, "%s %s", which, what) != 2)
  {
    do_print(str);
    return;
  }

  if (which == "remove")
  {
    this_user()->remove_colour(what);
    printf("Setting for '%s' removed.\n", what);
    return;
  }

  this_user()->set_colour(which, what);
  printf("'%s' set to '%%^%s%%^%s%%^RESET%%^'.\n",
    which, upper_case(which), what);
}
