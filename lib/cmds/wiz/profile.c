/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE profile
//      profile <filename>
//      profile -total <filename>
//      profile all
//
//This displays time taken processing specified functions.
//To use this, the driver must be compiled with the PROFILE_FUNCTIONS option
//defined (in local_options).
//
//Using profile without any argument displays the "top 10" objects,
//together with the total time taken executing functions within them.
//
//Using profile <name> displays the time taken in each individual function
//within the specified object, split betwen "self" and "children", together
//with the number of calls involved.
//
//Using profile -total <name> produces a similar display, but sorted on the
//"Total" column rather than "Self"
//
//The "profile all" option is likely to produce a "Too long evaluation" error,
//unless you increase the max eval cost, but it should provide details of
//time taken by functions in the 20 "worst" objects.

inherit CMD;

#ifdef __PROFILE_FUNCTIONS__
void profile_all(function compare)
{
  mapping *prof;
  mapping *worst = ({});
  object ob;
  mapping item;

  foreach (ob in objects())
  {
    prof = function_profile(ob);
    prof = filter(prof, (: $1["calls"] :));
    prof = sort_array(prof, compare);
    prof = prof[0..9];
    map(prof, (: $1["name"] = $(file_name(ob)) + "::" + $1["name"] :) );
    worst += prof;
  }
  worst = sort_array(worst, compare);
  worst = worst[0..19];

  outf("%-40s %5s %8s %8s %8s\n", "Name", "Calls", "Self", "Children", "Total");
  foreach (item in worst)
  {
    outf("%-40s %5i %8i %8i %8i\n",
        item["name"], item["calls"], item["self"], item["children"],
        item["self"] + item["children"]);
  }
}

int total(mapping *info)
{
  int ret;
  mapping item;

  foreach (item in info)
    ret += item["self"];
  return ret;
}

protected void top_ten()
{
  mixed *tmp;
  int i, j;

  tmp = map(objects((: !clonep($1) :)),
      (: ({ $1, total(function_profile($1)) }) :));
// We could use sort_array() here, but that's an inefficient way
// to get the top 10 when there are lots of objects.

  for (i = 0; i < 10; i++)
  {
    mixed foo;

    for (j = i; j < sizeof(tmp); j++)
      if (tmp[i][1] < tmp[j][1])
      {
        foo = tmp[i];
        tmp[i] = tmp[j];
        tmp[j] = foo;
      }
    outf("%60-O %i\n", tmp[i][0], tmp[i][1]);
  }
}
#endif

private void main(string str)
{
#ifndef __PROFILE_FUNCTIONS__
  out("Function profiling not enabled.\n");
  return;
#else
  mapping *prof;
  mapping item;
  string obname;
  object ob;
  function compare;

  if (!str)
  {
    top_ten();
    return;
  }

  if (sscanf(str, "-total %s", str))
  {
    compare = (: $2["self"] + $2["children"] - $1["self"] - $1["children"] :
);
  } else {
    compare = (: $2["self"] - $1["self"] :);
  }

  if (str == "all")
  {
    profile_all(compare);
    return;
  }

  obname = evaluate_path(str);

  if (!(ob = find_object(obname)))
  {
    out("Can't find object " + obname + "\n");
    return;
  }

  prof = function_profile(ob);
  prof = filter(prof, (: $1["calls"] :));
  prof = sort_array(prof, compare);

  outf("%-30s %5s %8s %8s %8s\n", "Name", "Calls", "Self", "Children", "Total");
  foreach (item in prof)
  {
    outf("%-30s %5i %8i %8i %8i\n",
        item["name"], item["calls"], item["self"], item["children"],
        item["self"] + item["children"]);
  }
#endif
}