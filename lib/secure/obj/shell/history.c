/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** history.c
**
** 6-21-95 -- Created (John Viega, rust@virginia.edu)
**
** Wants code to handle cycling through your history if you're
** in a char mode shell.  That already exists in the current readline
** library, but I think this is probably a better place for it.
*/

#include <mudlib.h>
#include <log.h>

#define DEFAULT_HISTORY_BUFFER_SIZE	20


object query_owner();

private static string* history = ({});
private static int buffer_size = DEFAULT_HISTORY_BUFFER_SIZE;
private static int array_index;
private static int command_number;
    

// Returns a string or -1 if the item is no longer in your history

//:FUNCTION get_nth_item
//returns the nth command if it's still in the history buffer.
nomask static mixed 
get_nth_item(int n)
{
    string item;
    int array_start;

    if(!intp(n))
    {
	error("Bad type arg to get_nth_item");
    }
    if(n<1 || n>command_number)
    {
	error("bad arg range for get_nth_item");
    }
    // Make corrections just in case someone keeps readjusting the
    // size of the history buffer and doesn't really have their full history
    if(buffer_size == -1)
    {
	int proper_index = n - (command_number - (array_index - 1));
	if(n <= 0)
	    return -1;
	return history[(proper_index-1)];
    }

    if(n > command_number || n <= command_number - buffer_size)
	return -1;


    array_start = command_number - (array_index - 1);
    if(n >= array_start)
    {
	item = history[(n-array_start)];
    }
    else
    {
	item = history[(buffer_size + (n-array_start))];
    }
    if(!stringp(item))
    {
	return -1;
    }
    return item;
}

//:FUNCTION add_history_item
//add a history item to a history buffer.
static void 
add_history_item(string item)
{
    if(!buffer_size)
    {
	return;
    }
    if(!stringp(item))
    {
	error("bad type arg to add_history_item");
    }

    history[array_index] = item;
    command_number++;
    array_index++;
    if(buffer_size == -1)
    {
	return;
    }
    if(array_index==sizeof(history))
    {
	array_index = 0;
    }
}

//:FUNCTION get_buffer_size
//returns the size of a history buffer.
static int 
get_buffer_size()
{
  return buffer_size;
}

//:FUNCTION get_command_number
//returns how many commands have been added to the history.
static int
get_command_number()
{
    return command_number;
}

//:FUNCTION get_ordered_history
//returns the history buffer in order of least to most recent items
static string*
get_ordered_history()
{
    if (buffer_size == -1)
    {
	return copy(history);
    }
    if (command_number == array_index)
    {
	if (!command_number)
	{
	    return ({});
	}

	return history[0..(array_index-1)];
    }

    return history[array_index..] + history[0..(array_index-1)];
}
	

private void
set_history(string* h)
{
    history[0..(sizeof(h))] = h;
}

// Allocates a new history buffer, and copies over as much of the old
// one as possible in the process.
private
void
allocate_history_buffer()
{
  string* old_buffer;
  old_buffer = get_ordered_history();
  if(buffer_size != -1)
  {
      history = allocate(buffer_size);
  }
//  if(old_buffer)
//     set_history(old_buffer);
}


//:FUNCTION set_history_buffer_size
//sets the size of a history buffer.  -1 means no size limit.
static int
set_history_buffer_size(int s)
{
  if(!intp(s) || s < -1)
	error("bad argument to set_history_buffer_size");
  buffer_size = s;
  allocate_history_buffer();
}

void 
create()
{
  if (!sizeof(history))
	allocate_history_buffer();
}


//:FUNCTION pattern_history_match
//returns the most recently added history item that matches
//the regexp.  An implicit ^ is added to the beginning of the regexp.

static string
pattern_history_match(string rgx)
{
    mixed matches;
    matches = regexp(get_ordered_history(), "^" + M_GLOB->translate(rgx,1));
    if(!sizeof(matches))
	return 0;
    return matches[<1];
}


/*
**  This is the user interface section,
**  for use by shells or whatever finds this stuff useful.
*/

static void 
display_history()
{
  int i,j;
  mixed h = get_ordered_history();
  for(i=command_number-sizeof(h)+1, j=0; i <= command_number; i++,j++)
    printf("%d\t%s\n", i, h[j]);
}
      
  
static string
history_command(string input)
{
  int cmdnumber;
  int lastcmdnum = command_number;
  string rest;

  switch(strlen(input))
    {
    case 1:
      display_history();
      return 0;
    case 2:
      if(input[1] == HISTORY_CHAR)
	{
	  if(!lastcmdnum)
	    {
	      write("Invalid history item.\n");
	      return 0;
	    }
	return get_nth_item(lastcmdnum);
	}
    default:
      if(sscanf(input[1..],"%d", cmdnumber))
	{
	  if(cmdnumber < 1 || cmdnumber >= command_number ||
	     (input = get_nth_item(cmdnumber)) == -1)
	    {
	      write("History index out of range.\n");
	      return 0;
	    }
	  return input;
	}

      if(!input=pattern_history_match(input[1..]))
	{
	  write("No pattern matches found.\n");
	  return 0;
	}

      return input;
    }
}
       
nomask mixed
query_history()
{
    if(check_previous_privilege(1))
    {
	object ob = query_owner();

	if ( ob && (ob = ob->query_link()) )
	{
	    string msg = sprintf("%s read the history of %s\n",
				 this_user()->query_userid(),
				 ob->query_userid());

	    LOG_D->log(LOG_SNOOP, msg);

	    if ( adminp(ob) )
		tell_object(ob, sprintf("%s has just read your history!\n",
					this_body()->query_name()));
	}

	return get_ordered_history();
    }
}
