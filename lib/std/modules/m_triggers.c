/* Do not remove the headers from this file! see /USAGE for more info. */

#include <triggers.h>

inherit "/std/object/msg_recipient";

void respond(string);

class pattern {
    string pattern;
    mixed action;
    string left, right;
    int type_to_listen;
}

private int counter = 1;
private mapping patterns = ([ 0 : ({}) ]);

varargs void add_pattern(string pattern, mixed action, mixed left, mixed right,int type) {
    class pattern pat = new(class pattern);
    
    pat->pattern = pattern;
    pat->action = action;
    pat->left = left;
    pat->right = right;
    pat->type_to_listen=type;
    
    patterns[0] += ({ pat });
}

varargs void add_sub_pattern(mixed rule, string pattern, mixed action,
			     mixed left, mixed right) {
    class pattern pat = new(class pattern);
    
    pat->pattern = pattern;
    pat->action = action;
    pat->left = left;
    pat->right = right;
    
    if (patterns[rule])
	patterns[rule] += ({ pat });
    else
	patterns[rule] = ({ pat });
}

mixed try_to_match(string str, string rule) {
    class pattern pat;
    mixed left, right;

    if (!str || !rule || !patterns[rule])
	return str;

    foreach (pat in patterns[rule]) {
	if (!pat->pattern 
	    || str == pat->pattern || sscanf(str, pat->pattern, left, right)) {
	    left = try_to_match(left, pat->left);
	    right = try_to_match(right, pat->right);
	    return evaluate(pat->action, left, right);
	}
    }
    return str;
}

varargs void check_msg(string str,int type) {
    class pattern pat;
    mixed left, right;
    mixed ret;
    
    str = terminal_colour(str, ANSI_D->query_translations()[1], sizeof(str));
    // strip trailing newline and whitespace
    while (str != "" && member_array(str[<1], " \t\n") != -1)
	str = str[0..<2];
    foreach (pat in patterns[0]) {
      if(pat->type_to_listen&&
	 !(type&pat->type_to_listen))
	continue;
      if (!pat->pattern || 
	  str == pat->pattern ||
	  sscanf(str, pat->pattern, left, right))
	{
	  left = try_to_match(left, pat->left);
	  right = try_to_match(right, pat->right);
	  if (stringp(pat->action))
	    ret = pat->action;
	  else {
	    ret = evaluate(pat->action, left, right);
	    if (!stringp(ret)) continue;
	  }
	  respond(ret);
	  return;
	}
    }
}

varargs void
receive_inside_msg(string msg,object array exclude,int message_type,mixed other)
{
  ::receive_inside_msg(msg,exclude,message_type,other);
  check_msg(msg,LISTEN_INSIDE);
}

varargs void
receive_outside_msg(string msg, object array exclude,int message_type,mixed other)
{
  ::receive_outside_msg(msg,exclude,message_type,other);
  check_msg(msg,LISTEN_OUTSIDE);
}

varargs void
receive_private_msg(string msg, int message_type, mixed other)
{
  ::receive_private_msg(msg,message_type,other);
  check_msg(msg,LISTEN_PRIVATE);
}
