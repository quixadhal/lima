/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// Wizard command "scan"
// list the filenames of objects in an inventory.
// optional flag -d for "deep" scan (ie recursive)
// usage : "scan [-d]" to scan environment
//         "scan [-d] ob" to scan object "ob"

// Peregrin@ZorkMUD
// Updated 1995/10/9 by Ohara,
// Updated 2003 by Loriel

inherit CMD;

#define PAD "    "

// scan the inventory of an object.
string scan_object(object ob, int depth, int deep_scan)
{
  object *inv;
  string tempstr, retstr, pad;
  int    i, l;

  pad = PAD;
  i = depth;
  while (i--)
  {
    pad += PAD;
  }

  retstr = "";
  inv = all_inventory(ob);
  l = sizeof(inv);
  while (l--)
  {
    retstr += pad+file_name(inv[l])+": "
        +((tempstr=inv[l]->short()) ? "\""+tempstr+"\" " : "[no short]")
        +"\n"
        +(deep_scan ? scan_object(inv[l],depth+1, deep_scan) : "");
  }
  return retstr;
}

// the actual scan command.
private void main(mixed *arg, mapping flags)
{
  string outstr;
  string objs;
  int deep_scan;

  deep_scan = !!flags["d"];

  if (!arg[0])
    arg[0] = environment(this_body());
  outstr = "Scanning " + file_name( arg[0] ) + ":\n";
  objs = scan_object( arg[0], 0, deep_scan );
  outstr += (objs != "")? objs : "    None\n";
  out( outstr );
}