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
	return regexp(potentials, "^"+partial);
}

//:FUNCTION case_insensitive_complete
//same as complete, but upper and lower case are ignored.

string* case_insensitive_complete(string partial, string* potentials)
{
  return M_REGEX->insensitive_regexp(potentials, "^"+partial);
}

//:FUNCTION complete_user
//given a username that might be partial, returns a
//user name, or an array of strings on a partial match, or 0 on no match.

mixed complete_user(string name)
{
  object* matches = filter(users(),(:strsrch($1->query_real_name(),lower_case($2)) != -1 :), name);

  switch(sizeof(matches))
    {
    case 0:
      return 0;
    case 1:
      return matches[0]->query_real_name();
    default:
      return map(matches, (:$1->query_real_name():));
    }
}
