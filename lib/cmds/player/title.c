/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE:	title [new title]
//
//without giving it a new title, shows you what your title is.
//When you give it a new title, use $N for where you want your name to be.
//
//For example:
//  title $N rocks!
//
//And then when I type 'title' I see:
//  Rust rocks!

inherit CMD;
inherit M_ANSI;

void create()
{
  ::create();
  no_redirection();
}

private void main(string title)
{
  if(!sizeof(title))
  {
    outf("Your title is: %s\n", this_body()->query_title());
    return;
  }
  if(strsrch(title,"$N") == -1)
    title = "$N " + title;
  if (colour_strlen(title) > 65)
  {
    outf("That title is to long.\n");
    return;
  }
  this_body()->set_title(title);

  out("Title changed.\n");
}
