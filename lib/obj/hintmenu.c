/* Do not remove the headers from this file! see /USAGE for more info. */

/*	
**	For the Lima mudlib.
**
*/

#include <menu.h>
#include <security.h>

inherit MENUS;
inherit M_ACCESS;

MENU toplevel;


MENU_ITEM quit_item;
MENU_ITEM goto_main_menu_item;
MENU_ITEM main_seperator;
MENU_ITEM blank_seperator;
string array hints;
int	hints_read;


void show_next_hint(string input)
{
  MENU m;

  switch(input)
    {
    case "b":
      goto_menu(previous_menu);
      return;
    case "":
      break;
    case "p":
      if(hints_read==1)
	{
	  write("No previous hints.\n");
	  return;
	}
      write(iwrap(sprintf("%d) %s\n", --hints_read,hints[hints_read-1])));
      return;
    case "q":
      quit_menu_application();
      return;
    default:
      return;
    }

      
  if(hints_read >= sizeof(hints))
    {
      write("[no more hints]\n\n");
      goto_menu(previous_menu);
      return;
    }
  write(iwrap(sprintf("%d) %s\n", hints_read+1, hints[hints_read++])));
}

void view_question(string area, string file, string question)
{
  MENU question_menu = new_menu("");
  set_menu_prompt(question_menu,
		  "[<enter>, (p)revious hint, (b)ack, (q)uit] ");
  set_no_match_function(question_menu,(:show_next_hint:));
  hints = explode(read_file("/help/hints/"+area+"/"+file), "\n")[1..]-({""});
  hints_read = 0;
  show_next_hint("");
  allow_empty_selection(question_menu);
  previous_menu = current_menu;
  current_menu = question_menu;
}

void view_dir(string s)
{
  string array files = get_dir("/help/hints/"+s+"/*") - ({".","..","README"});
  string array questions = map(files, (: read_file("/help/hints/"+$(s)
						   +"/"+$1,1,1) :));
  MENU m = new_menu(replace_string(s,"_"," "));
  MENU_ITEM item;
  function f;


  add_menu_item(m, main_seperator);
  for(int i = 0; i < sizeof(questions); i++)
    {
      f = (: view_question($(s),$(files[i]),$(questions[i][0..<2])) :);
      item = new_menu_item(questions[i][0..<2], f);
      add_menu_item(m, item);
    }
  add_menu_item(m, blank_seperator);
  add_menu_item(m, quit_item);
  add_menu_item(m, goto_main_menu_item);
  add_menu_item(m, blank_seperator);
  allow_empty_selection(m);
  set_no_match_function(m,(:quit_if_cr:));
  set_menu_prompt (m, "[#mq] ");
  previous_menu = current_menu;
  current_menu = m;
}

void create()
{
  string array sections;

  set_privilege(1);

  toplevel 	= new_menu(mud_name()+" Hint Menu");


  // Since we'll use these things more than once, we can just
  // call new_menu_item once, and insert them wherever we want
  // to use them.
  quit_item = new_menu_item("Quit", (:quit_menu_application:), "q");
  goto_main_menu_item =new_menu_item("Return to main menu", 
					 toplevel, "m");


  main_seperator = 
    new_seperator("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"
		  "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
  blank_seperator = new_seperator("");


  // Add items to the toplevel (main) menu.  
  add_menu_item (toplevel, 
    new_seperator(
      "What area of the mud are you looking for hints for?"));
  add_menu_item (toplevel, main_seperator);

  sections = sort_array(get_dir("/help/hints/*") - ({".","..","README"}),1);

  foreach(string s in sections)
    {
      add_menu_item (toplevel, new_menu_item(replace_string(s,"_"," "),
					     (: view_dir($(s)) :)));
    }	
    //    if(sizeof(sections)%2) 
    add_menu_item(toplevel,blank_seperator);
  add_menu_item (toplevel, main_seperator);
  add_menu_item (toplevel, quit_item);
  add_menu_item (toplevel, blank_seperator);
  add_menu_item (toplevel, main_seperator);
  set_menu_prompt (toplevel, "[#q] ");
  allow_empty_selection(toplevel);
  set_no_match_function(toplevel,(:quit_if_cr:));
  }

void start_menu()
{
  init_menu_application(toplevel);
}












