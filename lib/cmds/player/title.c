/* Do not remove the headers from this file! see /USAGE for more info. */

void main(string title)
{
  if(!title)
    {
      printf("Your title is: %s\n", this_body()->query_title());
      return;
    }
  if(strsrch(title,"$N") == -1)
    title = "$N " + title;
  this_body()->set_title(title);
  
  write("Title changed.\n");
}
