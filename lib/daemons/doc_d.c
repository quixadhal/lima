/* Do not remove the headers from this file! see /USAGE for more info. */

// DOC_D by Rust (rust@virginia.edu)  4-10-95
// Inspired by emacs on-line documentation, which is
// awesome once you know how to use it....  =)
// Some languages like Lisp and Python have doc strings. 
// since it wasn't about to be built into the language, 
// documentation is done via the preprocessor.
//
// at the top of a module, do:
// DOC_MODULE("this is the doc string explaining my module.");
//
// On top of every function in the mudlib, do (right before the function):
// DOC(funcname, "this is the doc for function funcname");
//
// On top of every command anywhere in the lib do:
// DOC_COMMAND("this is the documentation for the foo command.");

/* --------------------------------------------------------------

Rewritten by Beek; the equivalent of the above is now:

[Start the comment at the left margin; these are indented so this daemon
 doesn't see these examples.]

 //:MODULE
 //This is the description of this module.
 //$$ Note: helpsys style directives can be included
 //see: another_module

 //:FUNCTION funcname
 //This is the doc for function funcname

 //:COMMAND
 //This is the doc for the command

 //:HOOK
 //This documents a hook called with call_hooks

Data is updated nightly and dumped to the /help/autodoc directory
*/

#include <mudlib.h>
#include <security.h>

inherit M_REGEX;
inherit M_ACCESS;

#define SAVE_FILE     "/data/daemons/doc_d"

//:MODULE
//The doc daemon handles finding source files which have been modified and
//updating the appropriate documentation in /help/autodoc.

// Public functions --------------------------------------------------------

//:FUNCTION scan_mudlib
//
// Recursively searches the mudlib for files which have been changed
// since the last time the docs were updated, and recreates the documentation
// for those files.

private void continue_scan();
private static array files_to_do, dirs_to_do;
private int last_time;

void scan_mudlib() {
    printf("Starting scan ...\n");
    files_to_do = ({ });
    dirs_to_do = ({ "/" });
    continue_scan();
}

//:FUNCTION complete_rebuild
//
// Rebuild all the data, regardless of modification time
void complete_rebuild() {
    last_time = 0;
    scan_mudlib();
}

// Everything below here is
private:
// ---------------------------------------------------------------------

void save_me() {
    unguarded(1, (: save_object, SAVE_FILE :));
}

string mod_name(string foo) {
    sscanf(foo, "%s.c", foo);
    return foo[strsrch(foo, "/", -1) + 1..];
}

void process_file(string fname) {
    array lines = regexp(explode(read_file(fname), "\n"), "^//", 1);
    string line;
    string outfile = 0;
    int last_line = -20;
    int idx = 0;

    rm("/help/autodoc/FIXME/" + mod_name(fname));
    
    while (idx < sizeof(lines)) {
        if (lines[idx][2..4] == "###") {
            write_file("/help/autodoc/FIXME/" + mod_name(fname), lines[idx][5..]);
        }
	if (last_line != lines[idx+1] - 1 && sizeof(lines[idx]) > 2 &&
            lines[idx][2] == ':') {
	    line = lines[idx][3..];
	    if (line == "MODULE") {
		outfile = "/help/autodoc/modules/" + mod_name(fname);
                rm(outfile);
	    } else
	    if (line == "COMMAND") {
		outfile = "/help/autodoc/command/" + mod_name(fname);
                rm(outfile);
	    } else
            if (sscanf(line, "HOOK %s", line) == 1) {
                outfile = "/help/autodoc/hook/" + line;
                rm(outfile);
                write_file(outfile, "Called by: " + fname + "\n\n");
            } else
	    if (sscanf(line, "FUNCTION %s", line) == 1) {
                mkdir("/help/autodoc/functions/" + mod_name(fname));
		outfile = "/help/autodoc/functions/" + mod_name(fname) + "/" + line;
                rm(outfile);
                write_file(outfile, "Found in: " + fname + "\n\n");
	    } else {
		outfile = 0;
		write_file("/log/AUTODOC", "Bad header tag: " + line + "\n");
	    }
            printf("Writing to: %O\n", outfile);
	} else {
	    if (last_line == lines[idx+1] - 1) {
		if (outfile) write_file(outfile, lines[idx][2..]);
	    } else
		outfile = 0;
	}
	last_line = lines[idx+1];
	idx += 2;
    }
}

void continue_scan() {
    array files;
    array item;

  for (int i = 0; i < 10; i++) {
    if (sizeof(dirs_to_do)) {
        printf("Scanning %s ...\n", dirs_to_do[0]);
        files = get_dir(dirs_to_do[0], -1);
        foreach (item in files) {
            if (item[1] == -2) {
                dirs_to_do += ({ dirs_to_do[0] + item[0] + "/" });
            } else
	    if (item[2] > last_time && item[0][<2..<1] == ".c") {
		files_to_do += ({ dirs_to_do[0] + item[0] });
	    }
	}
	dirs_to_do[0..0] = ({ });
    } else
    if (sizeof(files_to_do)) {
        printf("Updating docs for %s ...\n", files_to_do[0]);
	process_file(files_to_do[0]);
	files_to_do[0..0] = ({ });
    } else {
        printf("Done.\n");
	last_time = time();
	save_me();
	return;
    }
  }	    
  call_out( (: continue_scan :), 0);
}

void
do_sweep() {
    scan_mudlib();
    call_out( (: do_sweep :), 86400);
}

void
create()
{
    if(clonep()) {
	destruct(this_object());
	return;
    }	    
    set_privilege(1);
    restore_object(SAVE_FILE);
    if ( !last_time )
	do_sweep();
    else
	call_out( (: do_sweep :), 86400);
}
