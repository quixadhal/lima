/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** objcount.c
**
** Provide counts of instantiated objects on a per-class basis.
**
** 07-Nov-94. Created. Deathblade.
*/

//:COMMAND
//$$ see: objdump, objfind
//USAGE: objcount
//       objcount <minimum>
//
//This command is used to find objects that have more than one instance.
//The number of instances (including blueprint) for each qualifying item
//is printed along with that object.
//
//Note: the "blueprint" is counted as an instance in display,
//but not for determining whether the item qualifies.
//
//When the optional "minimum" parameter is used, this ignores any items
//where the number of instances (excluding blueprint) is less than the number
//Thus "objcounts 2" lists items with 3 or more non-blueprint instances,

inherit CMD;

private void main(mixed args)
{
  mapping counts = ([ ]);

  objects((: $(counts)[base_name($1)]++ :));

/* filter out those without the requested minimum number of instances.
   defaults to 2 or more (meaning cloned obs) */
  if ( !args[0] )
    args[0] = 1;
  counts = filter(counts, (: $2 > $(args[0]) :));

  out(sprintf("%O\n", counts));
}
