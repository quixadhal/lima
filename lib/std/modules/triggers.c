/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

void respond(string);

class pattern {
    string pattern;
    mixed action;
    string left, right;
}

private int counter = 1;
private mapping patterns = ([ 0 : ({}) ]);

varargs void add_pattern(string pattern, mixed action, mixed left, mixed right) {
    class pattern pat = new(class pattern);
    
    pat->pattern = pattern;
    pat->action = action;
    pat->left = left;
    pat->right = right;

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

void catch_tell(string str) {
    class pattern pat;
    mixed left, right;
    mixed ret;
    
    // strip trailing newline and whitespace
    while (member_array(str[<1], " \t\n") != -1)
	str = str[0..<2];
    
    foreach (pat in patterns[0]) {
	if (!pat->pattern || 
	    str == pat->pattern || sscanf(str, pat->pattern, left, right)) {
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
