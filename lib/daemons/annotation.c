/* Do not remove the headers from this file! see /USAGE for more info. */

#include <classes.h>
#include <security.h>
#define SAVE_FILE "/data/daemons/annotations"

inherit CLASS_ANNOTATION;
inherit M_ACCESS;

private mapping annotations = ([]);


void create()
{
  restore_object(SAVE_FILE) || write("No annotations restored.\n");
}

void add_annotation(mixed o, class annotation a)
{
  class annotation array arrs;

  a->date = ctime(time());
  a->text = wrap(a->text);
  //## Add code for owner of an annotation... check against WIZ_DIR

  arrs = annotations[(objectp(o) ? base_name(o) : o)]; 

  if(arrs)
    {
      annotations[(objectp(o) ? base_name(o) : o)] += ({a});
    }
  else
    {
      annotations[(objectp(o) ? base_name(o) : o)] = ({a});
    }
  unguarded(1, (: save_object(SAVE_FILE) :));
}

class annotation array retrieve_annotations(mixed o)
{
  return copy(annotations[(objectp(o) ? base_name(o) : o)]);
}

int remove_annotation(mixed o, int index)
{
  class annotation array arr = annotations[(objectp(o) ? base_name(o) : o)];
  class annotation a;
  string thisguy;

  if(!arrayp(arr))
    {
      return -1;
    }
  else if (index >= sizeof(arr))
    {
      return -2;
    }
  else if ((a = arr[index]) && (a->read_only) && (!adminp(this_body())))
    {
      thisguy = this_body()->query_userid();
      if ((thisguy != a->author) && (thisguy != a->owner))
	{
	  return -3;
	}
    }
  arr[index..index] = ({});
  annotations[(objectp(o) ? base_name(o) : o)] = arr;

  unguarded(1, (: save_object(SAVE_FILE) :));
  return 1;
}

