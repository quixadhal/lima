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

Rewritten by Beek, MonicaS; the equivalent of the above is now:

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

 //:AUTODOC_MUDNAME
 // This adds AUTODOC_MUDNAME code change to the mudlib
 // AUTODOC_MUDNAME is set in config.h
 // (keep this line blank after comment to make comment file readable)

 //:TODO 
 //What we'd like to do with this in the future

 //### Something has to be fixed
 //### This doesn't need to start at the left margin

Data is updated nightly and dumped to the /help/autodoc directory
*/

#include <security.h>
#include <log.h>

inherit M_REGEX;
inherit M_DAEMON_DATA;

//:MODULE
//The doc daemon handles finding source files which have been modified and
//updating the appropriate documentation in /help/autodoc.

// Public functions --------------------------------------------------------


private void continue_scan();
private int last_time;
private array files_to_do, dirs_to_do;

private void delete_directory(string directory)
{
  if(file_size(directory)==-1)
    return;
  foreach(mixed array file in get_dir(sprintf("%s/",directory),-1))
    {
      string target=sprintf("%s/%s",directory,file[0]);
      if(file[1]==-2)
	delete_directory(target);
      else
	rm(target);
    }
  rmdir(directory);
}

private void make_directories()
{
  /* Assume that if the filesize is -1 that a directory needs to be created */
  if(file_size("/help/autodoc")==-1)
    mkdir("/help/autodoc");
  if(file_size("/help/autodoc/FIXME")==-1)
    mkdir("/help/autodoc/FIXME");
  if(file_size("/help/autodoc/command")==-1)
    mkdir("/help/autodoc/command");
  if(file_size("/help/autodoc/examples")==-1)
    mkdir("/help/autodoc/examples");
  if(file_size("/help/autodoc/functions")==-1)
    mkdir("/help/autodoc/functions");
  if(file_size("/help/autodoc/hook")==-1)
    mkdir("/help/autodoc/hook");
  if(file_size("/help/autodoc/modules")==-1)
    mkdir("/help/autodoc/modules");
  if(file_size("/help/autodoc/todo")==-1)
    mkdir("/help/autodoc/todo");
  if(file_size(sprintf("/help/autodoc/%s",MUD_AUTODOC_DIR))==-1)
    mkdir(sprintf("/help/autodoc/%s",MUD_AUTODOC_DIR));
}  
  
//:FUNCTION scan_mudlib
//
// Recursively searches the mudlib for files which have been changed
// since the last time the docs were updated, and recreates the documentation
// for those files.
void scan_mudlib() {
    printf("Starting scan ...\n");
    files_to_do = ({ });
    dirs_to_do = ({ "/" });
    if(!last_time)
      {
	delete_directory("/help/autodoc");
	make_directories();
      }
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

nosave private string * filtered_dirs = ({
  "/data/", "/ftp/", "/help/", "/include/",
  "/log/", "/open/", "/tmp/", "/user/", "/wiz/",
  "/contrib/",
});

string mod_name(string foo) {
    sscanf(foo, "%s.c", foo);
    return foo[strsrch(foo, "/", -1) + 1..];
}

string func_name(string bar) {
    sscanf(bar, "%s(", bar);
    return bar;
}

void process_file(string fname) {
    string file = read_file(fname);
    string line, prototype;
    array lines, match;
    string outfile = 0;
    int i;

    /* If the file has not been modified since the last time that DOC_D
     * scanned, there is no reason for it to be checked again -- Tigran */
    if(last_time &&
       get_dir(fname,-1)[0][2]<last_time)
      return;

    rm("/help/autodoc/FIXME/" + mod_name(fname));
    rm("/help/autodoc/todo/" + mod_name(fname));
    rm("/help/autodoc/modules/" + mod_name(fname));
    rm("/help/autodoc/command/" + mod_name(fname));
    rm("/help/autodoc/hook/" + mod_name(fname));
    rm("/help/autodoc/" + MUD_AUTODOC_DIR + "/" + mod_name(fname));
    delete_directory("/help/autodoc/functions/" + mod_name(fname));

    if (!file) return;
    lines = explode(file, "\n");

    while (i < sizeof(lines)) {
	if (regexp(lines[i],"^[ \t]*//###")) {
	    outfile = "/help/autodoc/FIXME/" + mod_name(fname);
	    write_file(outfile, "FIXME in file "+fname+" line "+(i+1)+":\n\n");
	    while (sscanf(lines[i], "%*(^[ \t]*//###)%s", line)) {
		write_file(outfile, line+"\n");
		// line = "";
		i++;
	    }
	    write_file(outfile, "\nCode:\n"+implode(lines[i..i+3], "\n")+"\n");
	    printf("Writing to: %O\n", outfile);
	}
	else if (lines[i][0..2] == "//:") {
	    line = lines[i][3..];
	    i++;
	    if (line == "TODO") {
		outfile = "/help/autodoc/todo/" + mod_name(fname);
		write_file(outfile, "TODO in file "+fname+" line "+i+":\n\n");
		while (lines[i][0..1] == "//") {
		    write_file(outfile, lines[i][2..]+"\n");
		    i++;
		}
	    }
	    else if (line == "MODULE") {
		outfile = "/help/autodoc/modules/" + mod_name(fname);
		write_file(outfile, "Module "+mod_name(fname)+" (file: "+fname+"):\n\n");
		while (lines[i][0..1] == "//") {
		    write_file(outfile, lines[i][2..]+"\n");
		    i++;
		}
	    }
	    else if (line == "COMMAND") {
		outfile = "/help/autodoc/command/" + mod_name(fname);
		write_file(outfile,"Command "+mod_name(fname)+" (file: "+fname+"):\n\n");
		while (lines[i][0..1] == "//") {
		    write_file(outfile, lines[i][2..]+"\n");
		    i++;
		}
	    }
	    else if (sscanf(line, "HOOK %s", line) == 1) {
		outfile = "/help/autodoc/hook/" + line;
		//		rm(outfile);
		write_file(outfile, "Hook "+line+":\nCalled by module "
		  +mod_name(fname)+" (file: "+fname+")\n\n");
		while (lines[i][0..1] == "//") {
		    write_file(outfile, lines[i][2..]+"\n");
		    i++;
		}
	    }
	    else if (sscanf(line, "FUNCTION %s", line) == 1) {
		if (func_name(line) != line)
		    LOG_D->log(LOG_AUTODOC, "Bad function name: "+fname+" line " + i
		      + ": " + line + "\n");
		mkdir("/help/autodoc/functions/" + mod_name(fname));
		outfile = "/help/autodoc/functions/" + mod_name(fname) + "/" + func_name(line);
		write_file(outfile, "Function "+line+":\nDefined in module "
		  +mod_name(fname)+" (file: "+fname+")\n\n");
		while (lines[i][0..1] == "//") {
		    write_file(outfile, lines[i][2..]+"\n");
		    i++;
		}
		//### regexp() doesn't match any ";", had to replace_string() them
		match = regexp(map(lines[i..i+19], (: replace_string($1, ";", "#") :)),
		  "\\<"+line+"\\>", 1);
		if (sizeof(match) > 0) {
		    if (sscanf(implode(lines[i+match[1]-1..i+match[1]+3], "\n"),  
			"%([ \t]*([a-zA-Z_][a-zA-Z0-9_* \t\n]*|)\\<"+line
			+"\\>[ \t\n]*\\([ \t\na-zA-Z_0-9*,.]*(\\)|))", 
			prototype)) {
			write_file(outfile, "\nPrototype:\n"+prototype+";\n");
		    }
		}
	    }
	    else if (line == AUTODOC_MUDNAME) {
		outfile = "/help/autodoc/"+MUD_AUTODOC_DIR+"/" + mod_name(fname);
		write_file(outfile,"**** "+fname+" ****\n\n");
		while (lines[i][0..1] == "//") {
		    write_file(outfile, lines[i][2..]+"\n");
		    i++;
		}
	    }
	    else {
		LOG_D->log(LOG_AUTODOC, "Bad header tag: "+fname+" line "+i
		  +": " + line + "\n");
	    }
	    printf("Writing to: %O\n", outfile);
	}
	else
	    i++;
    }
}

void continue_scan()
{
  array files;
  array item;

  for (int i = 0; i < 10; i++)
  {
    if (sizeof(dirs_to_do))
    {
      printf("Scanning %s ...\n", dirs_to_do[0]);
      files = get_dir(dirs_to_do[0], -1);
      foreach (item in files)
      {
        if (item[1] == -2)
        {
          string dir = dirs_to_do[0] + item[0] + "/";
          if ( member_array(dir, filtered_dirs) != -1 )
            continue;
          dirs_to_do += ({ dir });
        } else if (item[2] > last_time && item[0][<2..<1] == ".c") {
          files_to_do += ({ dirs_to_do[0] + item[0] });
        }
      }
	    dirs_to_do[0..0] = ({ });
    } else if (sizeof(files_to_do))	{
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
      HELP_D->rebuild_data();
      return;
    }
  }	    
  call_out( (: continue_scan :), 1);
}

void do_sweep()
{
  scan_mudlib();
  call_out( (: do_sweep :), 86400);
}

void create()
{
  if(clonep())
  {
    destruct(this_object());
    return;
  }	    
  ::create();
  if ( !last_time )
    do_sweep();
  else
    call_out( (: do_sweep :), 86400);
}
