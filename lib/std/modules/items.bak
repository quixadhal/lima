/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#pragma show_error_context

/*
** Implemented in OBJ::description
*/
void add_id(string id);


static int DEF_ITEM = 1;
mapping items;

void items_init() {
    items = ([]);
}

void add_items(mapping m)
{
  int	i,j;
  mixed i_keys;

  i = sizeof( m );
  i_keys = keys( m );
  
  while( i-- )
  {
    if( pointerp( i_keys[i] ) )
    {
	j = sizeof( i_keys[i] );
        while( j-- )
	{
	  add_id( i_keys[i][j] );
	  items[ i_keys[i][j] ] = values(m)[i];
	}
    }
    else
    {
	add_id( i_keys[i] );
	items[ i_keys[i] ] = m[ i_keys[i] ];
    }
  }
    parse_refresh();
}


void set_items(mapping m)
{
    items = ([ ]);
    add_items(m);
}


string get_item_desc(string str){
    return stringp(str) ? evaluate(items[str]) : 0;
}

string * fake_item_id_list()
{
    return keys(items);
}
