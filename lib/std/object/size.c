/* Do not remove the headers from this file! see /USAGE for more info. */

private nosave int size = MEDIUM;

//:FUNCTION query_size
//query_size() returns the size of an object
int query_size()
{
  return size;
}

//:FUNCTION set_size
//set_size(s) sets the size of an object to 's'
void set_size(int s)
{
  size = s;
}
