/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//$$ see: feelings, random, semote
//
//USAGE:	adverbs [pattern]
//
//Without an argument, shows all adverbs that you can use in soul commands.
//With an argument, shows all adverbs matching the given pattern.
//
//Example:
//    adverbs f*ly
//
// will show you all adverbs that start w/ 'f' and end in 'ly'
//
//To use an adverb in a soul command, use as much of the adverb as necessary
//to uniquely identify it, followed by a *.
//
//For example:
//    smile fla*
//
//will translate to: 'smile flatly'.
//
//But: 'smile fl*' will not, because 'flirtingly' also starts with fl.

inherit CMD;
inherit M_GLOB;

#define HEADER  "--------------------------------<  Adverbs  >-------------------------------\n"
#define TRAILER "------------------------<  Found %4d/%4d  (%3d%%)  >------------------------\n"

private void main(string arg)
{
    string * list;
    int count;

    list = SOUL_D->get_adverbs();
    count = sizeof(list);

    if (count == 0) {
	out("There are no adverbs!\n");
	return;
    }

    if( !sizeof(arg))
	arg = "*";
    else if ( !has_magic(arg) )
	arg = arg + "*";
    arg = "^"+translate(arg)+"$";
    list = regexp(list, arg);
    if ( !list )
    {
	outf("No adverbs matched '%s'.\n", arg);
	return;
    }

    if(end_of_pipeline())
	outf(HEADER "%-#79s\n" TRAILER, 
	  implode(sort_array(list, 1), "\n"),
	  sizeof(list), count, sizeof(list) * 100 / count);
    else
	outf(HEADER "%s\n" TRAILER, implode(sort_array(list, 1), "\n"),
	  sizeof(list), count, sizeof(list) * 100 / count);

}

void player_menu_entry(string str)
{
    bare_init();
    ZBUG(str);
    main(str);
    done_outputing();
}
