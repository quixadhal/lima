/* Do not remove the headers from this file! see /USAGE for more info. */

#include <edit.h>

inherit __DIR__ "scr_command";
inherit M_INPUT;

string fname;

void finish_editing(string array alltext)
{
  string fname=get_file_name();
  if(!alltext) {
    write("Description unchanged.\n");
    return;
  }

  if(change_attribute(fname,"long",implode(alltext,"\n"))) {
    write("Description changed.\n");
    update(fname);
  }
}

private void main() {
    string fname;

    if (!(fname = get_file_name()))
	return;

    write("Begin typing description.\n");
    new(EDIT_OB,EDIT_TEXT,0,(:finish_editing:));
}
