/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

// Beek - June 20, 1995
// Implements the defacto standard %^ color escapes.  Currently,
// only ANSI is supported, although the extension to other terminal
// types should be trivial.  Ideally, this would interface with
// termcap etc
// The %^ codes are the standard ones, however the implementation
// here is somewhat novel.  Basically, we don't parse every line
// sent to the user.  This saves us CPU and prevents the mud from
// looking like a Christmas tree.

mapping translations, null_translations;

// Subtle point: by making this private, we allow the driver to call
// this, but if we are inherited this one disappears.
private void create() {
    // Don't count on this getting called when we are inherited.
    // It's too easy for the coder to forget to call modules::create(),
    // so we spare him the burden by doing our initialization in a
    // lazy fashion.
    if (file_name(this_object()) != "/std/modules/ansi")
	return;
    if (clonep()) destruct(this_object());
    translations = ([
    "RESET" : "\e[0m",
    "BOLD" : "\e[1m",
    "FLASH" : "\e[5m",
    "BLACK" : "\e[30m",
    "RED" : "\e[31m",
    "GREEN" : "\e[32m",
    "ORANGE" : "\e[33m",
    "YELLOW" : "\e[1m\e[33m",
    "BLUE" : "\e[34m",
    "CYAN" : "\e[36m",
    "MAGENTA" : "\e[35m",
    "WHITE" : "\e[37m",
    "B_RED" : "\e[41m",
    "B_GREEN" : "\e[42m",
    "B_ORANGE" : "\e[43m",
    "B_YELLOW" : "\e[1m\e[43m",
    "B_BLUE" : "\e[44m",
    "B_CYAN" : "\e[46m",
    "B_BLACK" : "\e[40m",
    "B_WHITE" : "\e[47m",
    "CLEARLINE" : "\e[L\e[G",
    "B_MAGENTA" : "\e[45m",
    "INITTERM" : "\e[H\e[2J",
    "ENDTERM" : "",
  ]);
    null_translations = map(translations, (: "" :) );
}

mapping *query_translations() {
    return ({ translations, null_translations });
}

string ansi(string str) {
    // Maintain a shared structure to save memory
    if (!translations) {
	mapping *tmp = M_ANSI->query_translations();
	translations = tmp[0];
	null_translations = tmp[1];
    }
    if (this_body()->query_shell_ob()->get_variable("ansi"))
	return terminal_colour(str, translations);
    else
	return terminal_colour(str, null_translations);
}


nomask int i_use_ansi()
{
  return this_body()->query_shell_ob()->get_variable("ansi") != 0;
}
