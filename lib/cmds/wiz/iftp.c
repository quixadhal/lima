/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** iftp.c -- I3 file transfer utility
**
** 960205, Deathblade: created
*/

//:COMMAND
//USAGE iftp <mudname>
//
//Invokes the intermud ftp client.

inherit CMD;
inherit M_COMPLETE;
inherit M_GLOB;

private void main(string arg)
{
  string * matches;
  string mudname;
  string test;

  if ( !arg || arg == "" )
  {
    out("Usage: iftp mudname\n");
    return;
  }

  matches = case_insensitive_complete(translate(arg),
      IMUD_D->query_up_muds());

  switch ( sizeof(matches) )
  {
    case 0:
      out("No matching mud currently up.\n");
      return;

    case 1:
      mudname = matches[0];
      break;

    default:
      mudname = lower_case(arg);
      foreach ( test in matches )
        if ( lower_case(test) == mudname )
        {
          matches = 0;
          break;
        }
      if ( !matches )
        break;
      outf("Ambigous.  The following muds might apply: %s\n",
          implode(matches, ", "));
      return;
  }

  new(IFTP_OB, mudname);
}
