/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** objfind.c
**
** Find the locations (environments) of instances of the given class
**
** 07-Nov-94. Created. Deathblade.
*/

//:COMMAND
//$$ see: objdump, objcount, idfind
//USAGE: objfind <fname>
//
//This command is used to find all the instances of objects with a given filename.
//This is very handy in tracking down where certain items are,
//or if they have no environment.
//
//Example:
//
//> objfind /gue/zork1/trophy_case

inherit CMD;

mapping envs;

private void main(string arg)
{
  object * obs;

  obs = objects( (: base_name($1) == $(arg) :) );

  envs = ([ ]);
  map_array(obs, (: envs[$1] = environment($1) :));

  out(sprintf("%O\n", envs));
}
