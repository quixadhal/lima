/* Do not remove the headers from this file! see /USAGE for more info. */

// -Beek

inherit CMD;
inherit M_GRAMMAR;

//:COMMAND
//USAGE codesay <string>
//
//Displays the string formatted as LPC code.
//Useful when discussing/explaining snippets of code.
//
//>codesay void create() { ::create(); }
//
//You say:
//        void create(){
//            ::create();
//        }

void create()
{
  ::create();
  no_redirection();
}

private void main(string s) {
    if (!s || s == "") {
	write("Say what?\n");
	return;
    }

    s = replace_string(s, ";", ";\n");
    s = replace_string(s, "{", "{\n");
    s = replace_string(s, "}", "}\n");

    // Indent
    write_file("/tmp/codesay", s, 1);
    ed_start("/tmp/codesay");
    ed_cmd("I");
    ed_cmd("x");
    s = read_file("/tmp/codesay");
    // fix the fact that indentation uses tabs
    s = "\n\t" + replace_string(s, "\n", "\n\t") + "\n";

    tell(this_user(), "You say:" + s, NO_ANSI | MSG_INDENT);
    tell_environment(this_body(),
		     this_body()->query_name() + " says:" + s,
		     NO_ANSI | MSG_INDENT,
		     ({ this_body() }));
}
