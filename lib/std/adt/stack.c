/* Do not remove the headers from this file! see /USAGE for more info. */

private mapping stack = ([]);
private int     counter;

int
size()
{
  return counter;
}

void
push( mixed elem )
{
  stack[++counter] = elem;
}

mixed
pop()
{
  mixed retval;

  
  if( !counter )
    error("Can not pop() from empty stack");

  retval = stack[counter];

  stack = map_delete( stack, counter-- );
  return retval;
}
