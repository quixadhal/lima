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

 //:EXAMPLE
 //This is an example to illustrate some code.

Data is updated nightly and dumped to the /help/autodoc directory
*/

#include <mudlib.h>
#include <security.h>
#include <log.h>

inherit M_REGEX;
inherit M_DAEMON_DATA;

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
private int last_time;
private array files_to_do, dirs_to_do;

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

static private string * filtered_dirs = ({
  "/data/", "/ftp/", "/help/", "/include/",
  "/log/", "/open/", "/tmp/", "/user/"
});

string mod_name(string foo) {
    sscanf(foo, "%s.c", foo);
    return foo[strsrch(foo, "/", -1) + 1..];
}

void process_file(string fname) 
{
    string file = read_file(fname);
    string line;
    array lines;
    string outfile = 0;
    int empty;
    int last_line = -20;
    int idx = 0;

    if (!file) return;
    lines = regexp(explode(file, "\n"), "^//", 1);

    rm("/help/autodoc/FIXME/" + mod_name(fname));

    while (idx < sizeof(lines)) {
        if (lines[idx][2..4] == "###") {
            write_file("/help/autodoc/FIXME/" + mod_name(fname), lines[idx][5..] + "\n");
        }
	if (last_line != lines[idx+1] - 1 && sizeof(lines[idx]) > 2 &&
            lines[idx][2] == ':') {
	    line = lines[idx][3..];
	    if (line == "MODULE") {
                if (empty) write_file(outfile, "\n[No description available]\n");
                empty = 1;
		outfile = "/help/autodoc/modules/" + mod_name(fname);
                rm(outfile);
	    } else
	    if (line == "COMMAND") {
                if (empty) write_file(outfile, "\n[No description available]\n");
                empty = 1;
		outfile = "/help/autodoc/command/" + mod_name(fname);
                rm(outfile);
	    } else
            if (sscanf(line, "HOOK %s", line) == 1) {
                if (empty) write_file(outfile, "\n[No description available]\n");
                empty = 0;
                outfile = "/help/autodoc/hook/" + line;
                rm(outfile);
                write_file(outfile, "Called by: " + fname + "\n\n");
            } else
	    if (sscanf(line, "FUNCTION %s", line) == 1) {
                mkdir("/help/autodoc/functions/" + mod_name(fname));
                if (empty) write_file(outfile, "\n[No description available]\n");
                empty = 0;
		outfile = "/help/autodoc/functions/" + mod_name(fname) + 
		"/" + line;
                rm(outfile);
                write_file(outfile, "Found in: " + fname + "\n\n");
            } else
            if (sscanf(line, "EXAMPLE %s", line) == 1) {
                if (empty) write_file(outfile, "\n[No description available]\n");
                empty = 0;
		outfile = "/help/autodoc/examples/" + line;
                rm(outfile);
                write_file(outfile, "Found in: " + fname + "\n\n");
	    } else {
                if (empty) write_file(outfile, "\n[No description available]\n");
                empty = 0;
		outfile = 0;
		LOG_D->log(LOG_AUTODOC, "Bad header tag: " + line + "\n");
	    }
            printf("Writing to: %O\n", outfile);
	} else {
	    if (last_line == lines[idx+1] - 1) {
		if (outfile) write_file(outfile, lines[idx][2..] + "\n");
                empty = 0;
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
		string dir = dirs_to_do[0] + item[0] + "/";
		if ( member_array(dir, filtered_dirs) != -1 )
		    continue;
                dirs_to_do += ({ dir });
            } else
	    if (item[2] > last_time && item[0][<2..<1] == ".c") {
		files_to_do += ({ dirs_to_do[0] + item[0] });
	    }
	}
	dirs_to_do[0..0] = ({ });
    } else
    if (sizeof(files_to_do)) {
        printf("Updating docs for %s ...\n", files_to_do[0]);
	/*
	** We need an unguarded() for any writes that may occur... there
	** is no user object, so protection checks will always fail.  This
	** will terminate the checking at this daemon rather than fall
	** off the stack and fail.  Note that we don't actually hit priv
	** 1, but the maximum allowed.
	*/
	unguarded(1, (: process_file, files_to_do[0] :));
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
    ::create();
    if ( !last_time )
	do_sweep();
    else
	call_out( (: do_sweep :), 86400);
}
