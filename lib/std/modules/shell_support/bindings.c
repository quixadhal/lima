/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** bindings.c
**
** Handles key bindings in character mode.
**
** 95-May-20.  Rust.        Created.
*/

nomask void modal_push_char(function callback);	// M_INPUT


DOC_MODULE("Implements character mode key bindings.");

private static int escape_hit;
private static string matchstr = "";
private static mapping bindings;
private static function default_func;

private nomask void handle_one_char(int);

nomask void handle_input(string s)
{
  map(s, (: handle_one_char :));
}

private nomask int match_filter(string binding)
{
  return binding == matchstr;
}

private nomask int partial_match_filter(string binding)
{ 
  return strsrch(binding, matchstr) == 0;
}
  
private nomask void handle_one_char(int c)
{
  string* binding_matches;

  switch(c)
  {
     case '\t':
       matchstr += "<TAB>";
       break;
     case '\n':
     case '\r':
       matchstr += "<RET>";
       break;
     case 27:
       matchstr += "<ESC>";
       break;
     case 0:
       matchstr += "<DEL>";
       break;
     case 1..8:
     case 11..12:
     case 14..26:
       matchstr = sprintf("%s<CTRL-%c>",matchstr,(c-1+'a'));
       break;
     default:
       matchstr = sprintf("%s%c",matchstr,c);
       break;
  }
  binding_matches = filter_array(keys(bindings), (: match_filter :));
   if(sizeof(binding_matches))
  {
    evaluate(bindings[matchstr], matchstr);
    matchstr = "";
    return;
  }
// Now check for a partial match, in which case we hold off...

  binding_matches = filter_array(keys(bindings), (:partial_match_filter:));
  if(sizeof(binding_matches))
     return;

  evaluate(default_func, matchstr);
  matchstr = "";

}

DOC(init_charmode, "start up character mode, passing a map of bindings and a "
"function to handle keystrokes not covered in the binding map.");

nomask void init_charmode(mapping new_bindings, function d)
{
// add a binding for < so that a < won't hold waiting for the ESC>
// of course, you can bind over this.
    bindings = ([ "<" : d ]) + new_bindings;
    default_func = d;
    modal_push_char((: handle_input :));
}

DOC(bind, "Add a key binding to a charmode shell.");
nomask void bind(string sequence, function callback)
{
    bindings[sequence] = callback;
}

DOC(unbind, "Remove a key binding from a charmode shell.");
nomask void unbind(string sequence)
{
    map_delete(bindings, sequence);
}
