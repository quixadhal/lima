/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek - July 30, 1995
//
// Interface to the parse_sentence() debug info

//:COMMAND
//USAGE: parse <verb>
//
//Interface to the parse_sentence() debug info.
//Requires the debugging options to be compiled into the driver
//(eg by "./build.MudOS debug").
//
//Displays results of all the parser checks on the verb - which
//rule(s) it matches, and results of the can_, direct_ and indirect_
//checks as appropriate, which helps determine where it fails and
//hence why "You can't do that (with that)"

inherit CMD;

private void main(string str)
{
    int flag = 1;
    if (!str) {
        out("Parse what?\n");
        return;
    }

    if ( sizeof(str) > 3 && str[0..2] == "-v ")
    {
        flag = 2;
        str = str[3..];
    }

    out(this_body()->do_game_command(str, flag));
}
