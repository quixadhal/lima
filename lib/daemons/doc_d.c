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

#include <mudlib.h>
#include <security.h>


#define PATH_TO_DRIVER_DOCS "/help/wizard/MudOS/efuns"

inherit M_REGEX;
inherit M_ACCESS;

DOC_MODULE("The doc daemon is the central repository for documentation strings.")

mapping function_docs = ([]);
mapping command_docs  = ([]);
mapping module_docs   = ([]);
mixed   function_list = ({});


#define SAVE_FILE	"/data/daemons/doc_d"

private void process_func(object ob, string file, string funcname)
{
    string 	this_doc_string;
    string*	path_info;

    /*
    ** Only process functions defined in the module itself (not inherited)
    */
    if ( function_exists(funcname, ob) != file )
	return;

    switch(funcname[0..5])
    {
    case "__DOC_":
	this_doc_string = call_other(ob, funcname);
	funcname = funcname[6..];
	if(!function_docs[funcname])
	{
	    function_docs[funcname] = ([]);
	}
	function_docs[funcname][file] = this_doc_string;
	break;

    case "__DOCM":
	this_doc_string = call_other(ob, "__DOCM");
	path_info = split_path(file);
	if(!module_docs[path_info[1]])
	{
	    module_docs[path_info[1]] = ([]);
	}
	module_docs[path_info[1]][path_info[0]] = this_doc_string;
	break;

    case "__DOCC":
	this_doc_string = call_other(ob, "__DOCC");
	path_info = split_path(file);
	if(!command_docs[path_info[1]])
	{
	    command_docs[path_info[1]] = ([]);
	}
	command_docs[path_info[1]][path_info[0]] = this_doc_string;
	break;
    }
}


DOC(update_docs_of, "updates the doc strings for a given object.")

void update_docs_of(object loaded_obj)
{
    string   	file;


    /* the object may have disappeared between create() and now */
    if ( !loaded_obj )
        return;
    if ( catch(file = file_name(loaded_obj)) )
        return;
  
    map_array(regexp(functions(loaded_obj), "^__DOC."),
	      (: process_func, loaded_obj, file :));

    unguarded(1, (: save_object, SAVE_FILE :));
}


void snarf_one_file(string name, string dir)
{
    string file, cmd;
    string *lines;
    int i;
    if(sscanf(name,"%s.3",cmd) != 1)
	return;
    file = dir + "/" + name;
    lines = explode(read_file(file),"\n");
    for(i=0;i<sizeof(lines);i++)
    {
	if(search(lines[i],".SH NAME") > -1)
	{
	    string s = lines[++i];
	    s = s[strsrch(s,"-")+1..<1];
	    if(!function_docs[cmd])
		function_docs[cmd] = (["efun" : s]);
	    else
		function_docs[cmd]["efun"] = s;
	}
    }
}
	    
    
void
snarf_driver_docs(string dirname, string* files, mixed blah)
{
    if(pointerp(files))
        map_array(files, (:snarf_one_file:), dirname);
}

void
update_driver_doc_entries()
{
    walk_dir(PATH_TO_DRIVER_DOCS, (:snarf_driver_docs:), 0);
    unguarded(1, (: save_object, SAVE_FILE :));
}


void
create()
{
  if(clonep(this_object()))
	destruct(this_object());
  set_privilege(1);
  restore_object(SAVE_FILE);
}



DOC(get_module_doc, "returns the doc strings for all modules "
		   "whoose name matches the passed argument.")

mapping
get_module_doc(string name)
{
    return copy(module_docs[name]);
}


DOC(get_command_doc, "returns the doc strings for all commands whose name "
		    "matches the passed argument.")

mapping 
get_command_doc(string command)
{
    return copy(command_docs[command]);
}


DOC(get_function_doc, "returns the doc strings for all functions whose name "
		    "matches the passed argument.")

mapping
get_function_doc(string func)
{
    return copy(function_docs[func]);
}


DOC(function_apropos, "returns a data structure with all functions that have "
    "anything to do with the string you give it.")

mapping
function_apropos(string s)
{
    mixed funclist, objlist;
 
}


// of data.
private
void
get_apropos_for_func(string funcname, string pat, mapping retval)
{
    mapping l = function_docs[funcname];

    if(sizeof(regexp(({funcname}),pat)))
    {
	retval[funcname] = l;
	return;
    }
    
    map_array(keys(l),
		 function(string k, mapping m, string p, mapping r, string f)
		 {
		     if(sizeof(regexp(({m[k]}),p)))
		     {
			 if(!r[f])
			     r[f] = ([ k : m[k] ]);
			 else
                             r[f][k] = m[k];
		     }
		 }, l, pat, retval, funcname);
}


mapping
apropos_function(string pattern)
{
    mapping ret = ([]);
    map_array(keys(function_docs), (: get_apropos_for_func :), pattern, ret);
    return ret;
}   
		  
		  

    

    
