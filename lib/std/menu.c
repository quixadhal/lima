/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Stanadard Menuing facilities.
** Jul-4-95. John Viega (rust@virginia.edu)  -- Created
**
**
** To-do for this:
**      make it so you can have a single item that you can choose that
**      doesn't get displayed in the menu.
**	Convert strings like titles, etc... to allow functionals.
**      move completion callback into a closure.
**	finish new_prompt() support / dont_complete... remove old completion
**      code.
**      add a security check in the input callback.
*/


#include <menu.h>
#include <mudlib.h>

class menu_item {
  string 	description;
  mixed		action;
  int		disabled;
  mixed		choice_name;  // should be a string if the user sets it.
  int		prompt_after_action;
  function	constraint;
  // A seperator is just a line of text, and not something that
  // can be chosen.  Therefore, action, choice_name, and 
  // prompt_after_selection are meaningless if this is non-zero.
  int 		seperator;
}


class menu {
  // Items should only ever be MENU_ITEM*'s or string*'s, and
  // should be of uniform type.  If this is of type string,
  // The menu won't display options.
  // And if items is a string*, you'd better have a no_match_function,
  // because there won't be any MENU_ITEMS to match against.
  mixed		*items;
  mixed		title;
  mixed		prompt;
  int		allow_enter;
  function	no_match_function;
  int   	num_columns;	
  int		dont_complete;
  string   	*current_choices;
}


static void goto_menu(MENU);
static void display_current_menu();
static void return_to_current_menu();
static void prompt_then_return();
static void finish_completion(int);


MENU 	current_menu, previous_menu;
int 	need_refreshing;

static void remove()
{
  destruct(this_object());
}

varargs static MENU 
new_menu(string title, string prompt, int allow_enter,
	    function no_match_function)
{
  MENU 	new_menu;

  new_menu = new(MENU);
  new_menu->items = ({});
  new_menu->title = title;
  new_menu->prompt = prompt;
  new_menu->allow_enter = allow_enter;
  new_menu->no_match_function = no_match_function;

  return new_menu;
}

varargs static MENU
new_prompt(string prompt, function callback, string* completions)
{
  MENU new_menu;

  new_menu = new(MENU);
  new_menu->prompt = prompt;
  new_menu->no_match_function = callback;
  new_menu->items = completions ? completions : ({});
}


varargs static MENU_ITEM
new_seperator (string description, function constraint)
{
  MENU_ITEM new_menu_item;

  new_menu_item = new(MENU_ITEM);
  new_menu_item->description = description;
  new_menu_item->constraint = constraint;
  new_menu_item->seperator = 1;
  
  return new_menu_item;
}


varargs static MENU_ITEM
new_menu_item(string description, mixed action, string choice_name, 
	      int prompt, function constraint)
{
  MENU_ITEM	new_menu_item;

  new_menu_item = new(MENU_ITEM);
  new_menu_item->description = description;
  new_menu_item->action = action;
  new_menu_item->choice_name = choice_name;
  new_menu_item->prompt_after_action = prompt;
  new_menu_item->constraint = constraint;

  return new_menu_item;
}


static void
add_menu_item(MENU menu, MENU_ITEM menu_item)
{
  menu->items += ({ menu_item });
}

static void
set_menu_items(MENU menu, MENU_ITEM* menu_items)
{
  menu->items = menu_items;
}


static void
set_menu_title(MENU menu, string title)
{
  menu->title = title;
}

static void
set_menu_prompt(MENU menu, mixed prompt)
{
  if(!(stringp(prompt) || functionp(prompt)))
    {
      error("Bad type arg 2 to set_menu_prompt");
      return;
    }

  menu->prompt = prompt;
}

static void 
allow_empty_selection(MENU menu)
{
  menu->allow_enter = 1;
}

static void 
disallow_empty_selection(MENU menu)
{
  menu->allow_enter = 0;
}

static void
set_no_match_function(MENU menu, function f)
{
  menu->no_match_function = f;
}

static void 
set_number_of_columns(MENU menu, int n)
{
  menu->num_columns = n;
}

static void
disable_menu_item(MENU_ITEM item)
{
  item->disabled = 1;
}

static void
enable_menu_item(MENU_ITEM item)
{
  item->disabled = 0;
}

static void
set_menu_item_description(MENU_ITEM item, string description)
{
  item->description = description;
}

static void
set_menu_item_action(MENU_ITEM item, mixed action)
{
  //  Should type check here, but I can't figure out how
  //  to typecheck the class.
  item->action = action;
}

static void
set_menu_item_choice_name (MENU_ITEM item, string choice_name)
{
  item->choice_name = choice_name;
}

static void
constrain_menu_item (MENU_ITEM item, function f)
{
  item->constraint = f;
}

// This variable is kind of a hack... when an action is taken 
// (assuming that action is a function) this menu becomes the
// current menu.  This was done so that I could integrate completion
// menus as a real menu without having to go through and modify
// every single action 
private MENU menu_after_selection;

static void
new_parse_menu_input(string input)
{
  string* 	matches;
  int		i;
  MENU_ITEM	matched_item;
  MENU		completion_menu;

  input = trim_spaces(input);
  if(input == "" && !current_menu->allow_enter)
    {
      return;
    }
  if((i=member_array(input, current_menu->current_choices)) != -1)
    {
      matches = ({ input });
    }
  else
    {
      if(current_menu->dont_complete)
	{
	  if(functionp(current_menu->no_match_function))
	    {
	      evaluate (current_menu->no_match_function, input);	
	    }
	  else
	    {
	      write("Invalid selection.\n");
	    }
	  return;
	}
      matches = M_REGEX->insensitive_regexp(current_menu->current_choices,
					    M_GLOB->translate(input, 1));
    }
  switch(sizeof(matches)){
    case 0:
      write("Invalid selection.\n");
      return;
    case 1:
      if(!sizeof(current_menu->items) || stringp(current_menu->items[0]))
	{
	  evaluate(current_menu->no_match_function, matches[0]);
	  if(menu_after_selection)
	    {	
	      goto_menu(menu_after_selection);
	      menu_after_selection = 0;
	    }
	  return;
	}
      matched_item = filter_array(current_menu->items,
				  (: intp(((MENU_ITEM)$1)->choice_name) ?
				   sprintf("%d",((MENU_ITEM)$1)->choice_name) 
				   == $2 : ((MENU_ITEM)$1)->choice_name ==  
				   $2 :), matches[0])[0];
      if (functionp(matched_item->action))
	{
	  if(menu_after_selection)
	    {	
	      goto_menu(menu_after_selection);
	      menu_after_selection = 0;
	    }
	  evaluate(matched_item->action,input);
	  need_refreshing = 1;
	  if(matched_item->prompt_after_action)
	    prompt_then_return();
	  return;
	}
      goto_menu (matched_item->action);
      return;
    default:
      completion_menu = new_menu("Choose one by number:\n"
				 "---------------------\n");
      set_menu_items(completion_menu,    
             filter_array(current_menu->items,
		   (: intp(((MENU_ITEM)$1)->choice_name) ?
		    member_array(sprintf("%d",
				 ((MENU_ITEM)$1)->choice_name),$2) != -1 :
		    member_array(((MENU_ITEM)$1)->choice_name, $2) != -1 :),
			  matches));
      add_menu_item(completion_menu, new_menu_item("Return to previous menu",
						   current_menu));
      goto_menu(completion_menu);
      menu_after_selection = current_menu;
  }
}



static void
parse_menu_input(mixed input)
{
  string* 	choices;
  int		counter;
  MENU_ITEM	item;
  mixed		action;

  if(input == -1)
    remove();
  input = trim_spaces(input);
  if(input == "" && !current_menu->allow_enter)
    return; 	

  foreach (item in current_menu->items)
    {
      // Quick and sleazy way of knowing we're a prompt...
      if(stringp(item))
	break;
      if(item->disabled || item->seperator || 
	 (item->constraint && !evaluate(item->constraint)))
	continue;
      if ((!stringp(item->choice_name) && sprintf("%d",++counter) == input) ||
	 input == item->choice_name)
	{	
	  action = item->action;
	  if(functionp(action))
	    {
	      evaluate(action, input);
	      need_refreshing = 1;
	      if(item->prompt_after_action)
		prompt_then_return();
	      return;
	    }
	  goto_menu (action);
	  return;
	}
    }
    if( functionp(current_menu->no_match_function) )
      evaluate (current_menu->no_match_function, input);
    else	
      write("Invalid selection.\n");

}

static string
get_current_prompt()
{
  mixed	prompt;
  
  prompt = current_menu->prompt;
  if(need_refreshing)
    display_current_menu();
  if(!prompt) {
    // Build a smart default prompt.  This info and constraint info
    // could easily be cached....
    // the only thing not smart about this prompt is that it assumes
    // your choices are one character if you provide them yourself.
    // I did that because comma seperating choices is ugly.
      string s = "";
      string c;
      MENU_ITEM item;
      int counter;
      foreach(item in current_menu->items)
	{
	  if(item->disabled || item->seperator ||
	     (item->constraint && !evaluate(item->constraint)))
	    continue;
	  if(!c=item->choice_name)
	    {
	      counter++;
	      continue;
	    }
	  s += c;
	}
	switch(counter)
	  {
	  case 0:
	    s = sprintf("[%s] ",s);
	    break;
	  case 1:
	    s = (s == "" ? "[1] " : sprintf("[1,%s] ", s));	
	    break;
	  default:
	    s = (s == "" ? sprintf("[1-%d] ", counter) :
		 sprintf("[1-%d,%s] ", counter, s));
	    break;
	  }
	return s;
    }
	  
 
  return stringp(prompt) ? prompt : evaluate(prompt);
}
    
static void
init_menu_application(MENU toplevel)
{
  this_user()->modal_push ((: parse_menu_input :), (: get_current_prompt :));
  current_menu = toplevel;
  goto_menu (toplevel);
}

static void 
quit_menu_application()
{
  this_user()->model_pop();
  destruct(this_object());
}
  
static void
goto_menu(MENU m)
{
  previous_menu = current_menu;
  current_menu = m;
  display_current_menu();
}

static void
return_to_current_menu()
{
  this_user()->modal_func ((: parse_menu_input :), (: get_current_prompt :));
}

static void
goto_previous_menu()
{
  MENU swap;
  swap = current_menu;
  current_menu = previous_menu;
  previous_menu = swap;
}

static void
display_current_menu()
{
  int leftwidth;
  int rightwidth;
  int num_columns, i, j;
  int counter;
  string output;
  MENU_ITEM this_item;

  need_refreshing = 0;
  if(!sizeof(current_menu->items) && !current_menu->no_match_function)
    {
      write("###Not implemented yet.\n");	
      current_menu = previous_menu;
      need_refreshing = 1;
      prompt_then_return();
      return;
    }

  if(!sizeof(current_menu->items) || stringp((current_menu->items)[0]))
    {
      current_menu->current_choices = current_menu->items;
      return;
    }
  rightwidth = max(map(filter_array(current_menu->items,
				    (: !(((MENU_ITEM)$1)->seperator) :)),
			      (: strlen(((MENU_ITEM)$1)->description) :)));
  // This stuff is getting as ugly as Amylaar closures =P
  leftwidth = max(map(filter_array(current_menu->items,
					    (: stringp(((MENU_ITEM)$1) ->
						       choice_name) :)),
			       (: strlen(((MENU_ITEM)$1)->choice_name) :)) +
			   ({3}));

  output = current_menu->title + "\n";
    
    if(!(num_columns=current_menu->num_columns))
    num_columns = 78 / (leftwidth + rightwidth + 6);
    if(!num_columns)
      num_columns = 1;
  // Build this each time, and pass it on to the input handler,
  // because the menu may change....
  current_menu->current_choices = ({});
  for(i = 0, j = 0; i < sizeof(current_menu->items); i++, j++)
    {
      this_item = current_menu->items[i];	
      if(this_item->disabled || (this_item->constraint && 
			    !evaluate(this_item->constraint)))
	continue;
      if(this_item->seperator)
	{
	  j = -1;  // we want it to be 0 on the next loop, and it's
	           // going to get incremented at the start of the
	           // next loop
	  output += sprintf("%s\n", this_item->description);
	  continue;
	}
      if(!stringp(this_item->choice_name))
	{
	  output += sprintf("%="+leftwidth+"d)  %-"+rightwidth+"s",
			    ++counter, this_item->description);
          current_menu->current_choices += ({ sprintf("%d",counter) });
	  // Note, this will still get recalculated every time, since
	  // we're setting it to an int, and not a string, but that's
	  // what we want since we want the menus to be dynamic...
	  // We set this for convenience of finding this menu item
	  // without having to recalculate all this crap when it comes
	  // time to process the choice.
	  this_item->choice_name = counter;
	}
      else 
	{
	  output += sprintf("%="+leftwidth+"s)  %-"+rightwidth+"s",
			    this_item->choice_name, 
			    this_item->description);
          current_menu->current_choices += ({ this_item->choice_name });
	}
      if(j%num_columns == (num_columns-1))
	output += "\n";
      else
	output += "   ";
    }
  output += "\n";
  new(MORE_OB)->more_string(output);
}




string* 	cur_choices;
function 	completion_callback;

varargs static void 
complete_choice(string input, string* choices, function f)
{
  string* 	matches;
  int		i;
  string	output;

  if(input)
    matches = M_REGEX->insensitive_regexp(cur_choices, 
					M_GLOB->translate(input,1));
  else matches = choices;

  switch(sizeof(matches))
    {
    case 0:
      write("Invalid selection.\n");
      return;
    case 1:
      evaluate(f, matches[0]);
      return;
    default:
      output = "Select choice by number\n"
	       "-----------------------\n";
      for(i=1; i<= sizeof(matches); i++)
	  output += sprintf("%=3d)  %s\n", i, matches[i-1]);
      this_user()->modal_func((: finish_completion :),
			      "[Enter number or r to return to menu] ");
      // Don't do this before the modal_func I'll bet...
      new(MORE_OB)->more_string(output);
      completion_callback = f;
      cur_choices = matches;	
      return;
    }
}


private int
finish_completion(string input)
{
  int i;
  if(input == "")
    return;
  if(input == "r")
    {
      this_user()->modal_func((: parse_menu_input :), 
			      (: get_current_prompt :));
      goto_menu(current_menu);
      return;
    }
  if((i = to_int(input)) < 1 || i > sizeof(cur_choices))
    {
      write("Invalid choice.\n");
      return;
    }
  // Put this before the evaluate so that the callback can change things.
  this_user()->modal_func((: parse_menu_input :), (: get_current_prompt :));
  evaluate(completion_callback, cur_choices[i-1]);
  cur_choices = 0;
  completion_callback = 0;
  need_refreshing = 1;
}

void
receive_string(function thencall, string input)
{
  // order here is important!  prompt_then_return() won't work
  // from your callback if the evaluate() goes first.
  this_user()->modal_func ((: parse_menu_input :), (: get_current_prompt :));
  evaluate(thencall, input);
}

static void
get_input_then_call(function thencall, string prompt)
{
    this_user()->modal_func((: receive_string, thencall :), prompt);
}
  
static void 
prompt_then_return()
{
    this_user()->modal_func((: return_to_current_menu :),
			    "[Hit enter to return to menu] ");
}

