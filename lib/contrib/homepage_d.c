/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**  7-Jan-97    Valentino.     Created.
**
** Hiware. Say hi if you ever happen to use this code. I am @Deepest on I3.
** No strings attached :)
*/

inherit M_DAEMON_DATA;

#include <http_d.h>
#include <ports.h>
#define DEEPEST

void build_homepages()
{
  string *dirs=get_dir(WIZ_DIR+"/"+"*");
  string page="<h1>Homepages for ";
#ifdef DEEPEST
  page+="coders";
#endif
#ifndef DEEPEST
  page+="Wizards";
#endif
  page+=" on "+MUD_NAME+"!</h1>\n<UL>\n";
  foreach (string name in dirs)
    {
      if (is_file(WIZ_DIR+"/"+name+HTTP_USER_HOME+"/index.html"))
	page+="  <LI> <A HREF=http:/"+"/"+__HOST__+":"+(PORT_HTTP)+"/~"+name+
	  ">\n        "+capitalize(name)+"'s Homepage.</A>\n";
    }
  page+="</UL>\n";
  set_privilege(1);
  unguarded(1, (: write_file, HTTP_ROOT+"/"+DEFAULT_PAGE,page,1 :));
}
