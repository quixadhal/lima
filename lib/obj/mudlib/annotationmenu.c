/* Do not remove the headers from this file! see /USAGE for more info. */

/*	
**	Annotation menu For the Lima mudlib.
**      Rust @ Lima Bean Dec 3, 1995.
**
**	Todo: This relies on the fact that 2 people aren't editing the same
**      annotation at once.  If you view one that got removed while you were
**      on the view annotation menu, you're hosed.  Need to add some locking
**      code, even though this shouldn't be a problem often.
**
*/

#include <menu.h>
#include <classes.h>
#include <edit.h>

inherit MENUS;
inherit CLASS_ANNOTATION;

MENU toplevel;
MENU view_annotations;
MENU rm_annotations;
MENU_ITEM quit_item;
MENU_ITEM goto_main_menu_item;
MENU_ITEM seperator;
private static object annotation_target;
private static class annotation new_annotation;
private void remove_annotation();



private void rcv_can_ed(string y_or_n)
{
  switch(lower_case(y_or_n))
    {
    case "y":
    case "yes":
      new_annotation->read_only = 0;
      break;
    case "n":
    case "no":
      new_annotation->read_only = 1;
      break;
    default:
      get_input_then_call ((: rcv_can_ed :), "yes or no? ");
      return;
    }
  ANNOTATION_D->add_annotation(annotation_target, new_annotation);
  write("**Annotation added**\n\n");
  goto_menu_silently(toplevel);
}

private void end_edit(string array annotation)
{
  new_annotation->text = implode(annotation,"\n");
  get_input_then_call((: rcv_can_ed :), "Allow anyone to remove this "
		       "annotation? ");
}

private void rcv_title(string title)
{
  new_annotation->title = title;
  new(EDIT_OB, EDIT_TEXT, 0, (: end_edit :));
}

private void add_annotation()
{
  new_annotation = new(class annotation);
  new_annotation->author = this_body()->query_userid();
  get_input_then_call((: rcv_title :), "Subject: ");
}

private void rm_it(int a)
{
  if(ANNOTATION_D->remove_annotation(annotation_target,a) != 1)
    {
      write("**You don't have permission to remove that annotation.**\n");
    }
  else
    {
      write("**Removed.**\n");
    }
  remove_annotation();
}

private void remove_annotation()
{
  class annotation array ann;
  class annotation a;


  ann = ANNOTATION_D->retrieve_annotations(annotation_target);
  if(!ann)
    {
      write("**No Annotations to remove.**\n\n");
      return;
    }
  rm_annotations = new_menu("Remove Annotations");
  for(int i = 0; i < sizeof(ann); i++)
    {
      a = ann[i];
      add_menu_item (rm_annotations, new_menu_item(
		  sprintf("%-30s (%s on %s)",a->title,capitalize(a->author),
			  a->date), (:rm_it($(i)):)));
    }
  add_menu_item(rm_annotations, goto_main_menu_item);
  add_menu_item(rm_annotations, quit_item);

  goto_menu_silently(rm_annotations);
}


private void done_seeing_it()
{
  write("\n\n");
}


private void see_it(int index)
{
  class annotation a;
  string output;

  a = (ANNOTATION_D->retrieve_annotations(annotation_target)[index]);

  if(!a)
    {
      write("**No annotations to view.**\n");
      return;
    }
  output = sprintf("Title: %s\nAuthor: %s\n Date: %s\n"
	   "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n%s\n",
		   a->title, capitalize(a->author), a->date, a->text);

  more (output, 0, (:done_seeing_it:));
}

private void see_annotations()
{
  class annotation array ann;
  class annotation a;

  ann = ANNOTATION_D->retrieve_annotations(annotation_target);
  if(!ann)
    {
      write("**No Annotations to view.**\n\n");
      return;
    }
  view_annotations = new_menu("View Annotations -- Which to view?");
  for(int i = 0; i < sizeof(ann); i++)
    {
      a = ann[i];
      add_menu_item (view_annotations, 
		     new_menu_item(
		       sprintf("%-30s (%s on %s)",a->title,
			       capitalize(a->author),a->date),
		       (:see_it($(i)):)));
    }

  add_menu_item(view_annotations, goto_main_menu_item);
  add_menu_item(view_annotations, quit_item);

  goto_menu_silently(view_annotations);
}

void create(object o)
{

  annotation_target = o;

  toplevel 	= new_menu(mud_name()+" Annotation Menu");

  quit_item = new_menu_item("Quit", (:quit_menu_application:), "q");
  goto_main_menu_item =new_menu_item("Return to main menu", 
					 toplevel, "m");
  seperator = 
    new_seperator("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");

  // Add items to the toplevel (main) menu.  
  add_menu_item (toplevel, new_menu_item("Add annotation", (:add_annotation:),
					 "a"));
  add_menu_item (toplevel, new_menu_item("View annotations", 
					 (:see_annotations:), "v"));
  add_menu_item (toplevel, new_menu_item("Remove an annotation",
					 (:remove_annotation:), "r"));
  add_menu_item (toplevel, quit_item);

  allow_empty_selection(toplevel);
  set_no_match_function(toplevel,(:quit_if_cr:));
  }

void start_menu()
{
  init_menu_application(toplevel);
}












