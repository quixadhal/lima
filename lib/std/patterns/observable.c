/* Do not remove the headers from this file! see /USAGE for more info. */

private nosave mapping observers = ([]);
private nosave int     next_handle  = 0;

int add_observer(function f)
{
  int handle = next_handle++;
  observers[handle] = f;
  return handle;
}

int remove_observer(int handle)
{
  // Probably should check:
  // member_array(handle, keys(observers))!=-1
  map_delete(observers, handle);
}

protected int notify_observers(mixed args...)
{
  foreach(object observer in values(observers))
    evaluate(observer, args...);
}
