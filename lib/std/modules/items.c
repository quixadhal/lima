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
    return stringp(str) ? items[str] : 0;
}


string * fake_item_id_list()
{
    return keys(items);
}


int parse_item_desc( string arg )
{
    mixed		value;
    int		start_index;
    int		cur_index;
    int		halt_execution;
    int		value_size;
    mixed		element;

    if( member_array( arg, keys( items ) ) ==  -1 )
	return 0;

    if(!stringp(arg))
	return 0;

    arg = un_article( arg );

    value = items[arg];
    if( value == DEF_ITEM )
    {
	printf( "You see nothing special about the %s.\n", arg );
	return 1;
    }

    if( stringp(  value ) )
    {
	write( value );
	return 1;
    }

    if(  !pointerp( arg ) )
	return 0;

    value_size = sizeof(  value );

    while( 1 )
    {
	if(  cur_index < 0 || cur_index >= value_size )
	    break;

	start_index = cur_index;

	element = value[cur_index];

	while( element ){

	    if( intp( element ) )
	    {
		cur_index += element;
		break;
	    }

	    if( stringp( element ) )
	    {
		write( element );
		element = 0;
		break;
	    }

	    if( !functionp( element ) )
		return 0;

	    element = evaluate( element );
	}

	if( start_index == cur_index )
	    break;
    }

    return 1;
}

// ### This is deprecated, and possibly unused
// get_fakeobj() called by the parser.
object
get_fakeobj( string id )
{
  object	o;

  if( !items )
	return 0;
  if( items[id] )
    return 0;

  o = clone_object(FAKE_OB);

  o->set_short( id );
  o->set_long( items[id] );

  // Temporary, I'm a bit lazy tonight.
  //  Rustozz
  call_out(  (: call_other, o, "remove" :), 1  );

  return o;
}
