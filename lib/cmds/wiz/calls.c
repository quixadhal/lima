/* Do not remove the headers from this file! see /USAGE for more info. */

// call_out info display by Rust.

#define DIVIDER "\
------------------------------------------------------------------------\
-----\n"


int main()
{

    mixed         *call_out_stuff;
    int           i;

    call_out_stuff = filter_array( call_out_info(), (: sizeof :) );

    printf( "%-50s%-20s%-10s\n","Object","Function","Delay" );
    write( DIVIDER );

    i = sizeof( call_out_stuff );

    while( i-- )
    {
	if ( functionp(call_out_stuff[i][1]) )
	    printf( "%-70O%-10d\n", call_out_stuff[i][1], 
		   call_out_stuff[i][2] );
	else
	    printf( "%-50s%-20s%-10d\n", file_name( call_out_stuff[i][0] ),
		   call_out_stuff[i][1], call_out_stuff[i][2] );
    }


    printf("\nThere are %d call_outs active.\n", sizeof( call_out_stuff ) );
    return 1;
}
