/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust: I intend to make a version where you can't use regexps in completion.
// Quick working thing tho.

#include <mudlib.h>

//:MODULE
//Various functions for expanding partial matches

//:FUNCTION complete
//Given a string and a list of possible completions of that string,
//return an array of all strings that would be valid completions.

string* complete(string partial, string* potentials)
{ 
    int n = sizeof(partial) - 1;
    return filter(potentials, (: $1[0..$(n)] == $(partial) :));
}

//:FUNCTION case_insensitive_complete
//same as complete, but upper and lower case are ignored.

string* case_insensitive_complete(string partial, string* potentials)
{
    int n = strlen(partial) - 1;
    partial = lower_case(partial);
    return filter(potentials, (: lower_case($1[0..$(n)]) == $(partial) :));
}

//:FUNCTION complete_user
//given a username that might be partial, returns a
//user name, or an array of strings on a partial match, or 0 on no match.

mixed complete_user(string name)
{
    object* matches = filter(users(),
			     (: strsrch($1->query_userid(),$2) != -1 :),
			     lower_case(name));

    switch(sizeof(matches))
    {
    case 0:
	return 0;
    case 1:
	return matches[0]->query_userid();
    default:
	return map(matches, (: $1->query_userid() :));
    }
}
