/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek - June 20, 1995
// Implements the defacto standard %^ color escapes.  Currently,
// only ANSI is supported, although the extension to other terminal
// types should be trivial.  Ideally, this would interface with
// termcap etc

inherit M_DAEMON_DATA;

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

#ifdef CONFIGURABLE_COLOUR
static
#endif
mapping translations, null_translations, identity_translations;

mapping array query_translations() {
    return ({ translations, null_translations, identity_translations });
}

#ifdef CONFIGURABLE_COLOUR
mapping defaults;

mapping defaults() { return defaults; }
#endif

void create() {
    ::create();

    if (!translations)
	translations = ([
	    "NONE" : "",
	    "RESET" : RESET, "BOLD" : BOLD, "FLASH" : FLASH, "RED" : RED, 
	    "GREEN" : GREEN, "ORANGE" : ORANGE, "YELLOW" : YELLOW, "BLUE" : BLUE,
	    "CYAN" : CYAN, "MAGENTA" : MAGENTA, "WHITE" : WHITE, "B_RED" : B_RED,
	    "B_GREEN" : B_GREEN, "B_ORANGE" : B_ORANGE, "B_YELLOW" : B_YELLOW,
	    "B_BLUE" : B_BLUE, "B_CYAN" : B_CYAN, "B_BLACK" : B_BLACK, 
	    "B_WHITE" : B_WHITE, "CLEARLINE" : CLEARLINE, "B_MAGENTA" : B_MAGENTA,
	    "INITTERM" : INITTERM, "ENDTERM" : ENDTERM, "SAVE" : SAVE, 
	    "RESTORE" : RESTORE, "HOME" : HOME,
	    ]);
			
#ifdef CONFIGURABLE_COLOUR
    if (!defaults)
	defaults = ([
	    "ROOM_EXIT" : "magenta",
	    "TELL" : "bold",
	    "CHANNEL" : "green",
	    "MORE" : "bold",
	    "ROOM_SHORT" : "none",
	    "LS_DIR" : "magenta",
	    "LS_LOADED" : "yellow",
	    "LS_DEFAULT" : "cyan",
	    "LS_HEADING" : "bold"
	    ]);
#endif

    translations = translations + defaults;

    null_translations = map(translations, function(){ return "";} );
    identity_translations = map(translations, (: "%^" + $1 + "%^" :));
}

void resync() {
    translations = translations + defaults;
    null_translations = map(translations, function(){ return "";} );
    identity_translations = map(translations, (: "%^" + $1 + "%^" :));

    save_me();
    users()->update_translations();
}

void add_default_colour(string key, string value) {
    defaults[upper_case(key)] = lower_case(value);
    resync();
}

void remove_default_colour(string key) {
    map_delete(defaults, upper_case(key));
    resync();
}
    
