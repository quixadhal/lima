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

mapping translations, null_translations, identity_translations;

#define RESET "\e[0m"
#define BOLD "\e[1m"
#define FLASH "\e[5m"
#define BLACK "\e[30m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define ORANGE "\e[33m"
#define YELLOW BOLD ORANGE
#define BLUE "\e[34m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"
#define WHITE "\e[37m"
#define B_RED "\e[41m"
#define B_GREEN "\e[42m"
#define B_ORANGE "\e[43m"
#define B_YELLOW BOLD B_ORANGE
#define B_BLUE "\e[44m"
#define B_CYAN "\e[46m"
#define B_BLACK "\e[40m"
#define B_WHITE "\e[47m"
#define CLEARLINE "\e[L\e[G"
#define B_MAGENTA "\e[45m"
#define INITTERM "\e[H\e[2J"
#define ENDTERM ""
#define SAVE "\e7"
#define RESTORE "\e8"
#define HOME "\e[H"

#if 0
/* # is stringize operator */
#define E(x) #x : x
#endif

private void initialize() {
    translations = ([
    "NONE" : "",
#if 0
    E(RESET), E(BOLD), E(FLASH), E(RED), E(GREEN), E(ORANGE), E(YELLOW),
    E(BLUE), E(CYAN), E(MAGENTA), E(WHITE), E(B_RED), E(B_GREEN), E(B_ORANGE),
    E(B_YELLOW), E(B_BLUE), E(B_CYAN), E(B_BLACK), E(B_WHITE), E(CLEARLINE),
    E(B_MAGENTA), E(INITTERM), E(ENDTERM), E(SAVE), E(RESTORE), E(HOME),
#else
    "RESET" : RESET, "BOLD" : BOLD, "FLASH" : FLASH, "RED" : RED, 
    "GREEN" : GREEN, "ORANGE" : ORANGE, "YELLOW" : YELLOW, "BLUE" : BLUE,
    "CYAN" : CYAN, "MAGENTA" : MAGENTA, "WHITE" : WHITE, "B_RED" : B_RED,
    "B_GREEN" : B_GREEN, "B_ORANGE" : B_ORANGE, "B_YELLOW" : B_YELLOW,
    "B_BLUE" : B_BLUE, "B_CYAN" : B_CYAN, "B_BLACK" : B_BLACK, 
    "B_WHITE" : B_WHITE, "CLEARLINE" : CLEARLINE, "B_MAGENTA" : B_MAGENTA,
    "INITTERM" : INITTERM, "ENDTERM" : ENDTERM, "SAVE" : SAVE, 
    "RESTORE" : RESTORE, "HOME" : HOME,
#endif
#ifndef CONFIGURABLE_COLOUR
    "ROOM_EXIT" : MAGENTA,
    "TELL" : "bold",
    "CHANNEL" : "green",
    "MORE" : "bold",
#endif
  ]);
    null_translations = map(translations, function(){ return "";} );
    identity_translations = map(translations, (: "%^" + $1 + "%^" :));
}

mapping *query_translations() {
    if ( file_name(this_object()) != M_ANSI )
	return 0;
    if (!translations)
        initialize();
    return ({ translations, null_translations, identity_translations });
}

private nomask void share() {
    if (!translations) {
	mapping *tmp = M_ANSI->query_translations();
	translations = tmp[0];
	null_translations = tmp[1];
	identity_translations = tmp[2];
    }
}

varargs string ansi(string str, object forwho) {
    // Maintain a shared structure to save memory
    share();

    if (!forwho)
        forwho = this_body();
    if (forwho)
	forwho = forwho->query_shell_ob();
    if (forwho && forwho->get_variable("ansi"))
	return terminal_colour(str, translations);
    else
	return terminal_colour(str, null_translations);
}


nomask int i_use_ansi()
{
  return (this_body() &&
	  this_body()->query_shell_ob()->get_variable("ansi") != 0);
}

//:FUNCTION colour_strlen
//colour_strlen(s) gives the length of the visible portion of s.  Colour
//codes (e.g. %^GREEN%^) are ignored.
int colour_strlen(string str) {
    share();

    return strlen(terminal_colour(str, null_translations));
}

//:FUNCTION colour_truncate
//colour_truncate(str, len) returns a string which consists of len or
//fewer visible characters, not counting colour codes.
string colour_truncate(string str, int len) {
    string result;
    int idx;

    share();

    result = terminal_colour(str, identity_translations, len);
    
    if ((idx = member_array('\n', result)) == -1)
	return result;
    
    return result[0..idx-1];
}
