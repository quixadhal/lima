/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz (rust@virginia.edu)
// Oct 18, 1994

private mixed list = ({ 0, });
private int size = 0;
private function rule = (: $1 > $2 :);

void sort_in(int);


void 
set_rule( function r )
{
  if( functionp(r) )
  {
    rule = r;
  }
}

void
add_element(mixed e)
{
  list += ({ e });
  sort_in( ++size );
}

private
void
sort_in( int index )
{
  int lowerIx, Truth;
  mixed swap;

  if( index == 1 )
  {
    return;
  }

  lowerIx = index >> 1;
  Truth = evaluate( rule, list[index], list[lowerIx] );

  if( Truth )
  {
    swap = list[lowerIx];
    list[index] = swap;
  }
  sort_in( lowerIx );
}

mixed
remove_element( int elementIndex )
{
  mixed retval;
  int upperIx, Truth;

  retval = list[elementIndex];
  upperIx = elementIndex << 1;
  if( upperIx > size )
  {
    return retval;
  }
  if( upperIx != size )
  {
  Truth = evaluate( rule, list[upperIx+1], list[upperIx] );
    if( Truth )
    {
	upperIx++;
    }
  }
  Truth = evaluate( rule, list[elementIndex], list[upperIx] );
  if( Truth )
  {
    return retval;
  }

  list[elementIndex] = list[upperIx];
  
  if( ( upperIx << 1 ) > size )
  {
    list = list[0..<2];
    size--;
    if( !upperIx & 1 )
    {
      sort_in( upperIx-1 );
    }
  }
  else
  {
    remove_element( upperIx );
  }
}

mixed
get_sorted_list()
{
  mixed listState, newList;
  int listSize;

  listSize = size;
  listState = list;

  while( size )
  {
    newList += ({ remove_element(1) });
  }

  size = listSize;
  list = listState;

  return newList;
}
