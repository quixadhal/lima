/* Do not remove the headers from this file! see /USAGE for more info. */

// call_out info display by Rust.
#include <mudlib.h>

inherit CMD;

private void main()
{
    mixed	*call_out_stuff;
    mixed	*data;
    int		i;

    call_out_stuff = filter_array(call_out_info(), (: sizeof :));

    printf("%-50s%-20s%-10s\n","Object","Function","Delay" );
    printf("%77'-'s\n", "");

    foreach ( data in call_out_stuff )
    {
	if ( functionp(data[1]) )
	    printf("%-70O%-10d\n", data[1], data[2]);
	else if ( data[0] )
	    printf("%-50s%-20s%-10d\n", file_name(data[0]),
		   data[1], data[2]);
	else
	    printf("%-50s%-20s%-10d\n", "<destructed>", data[1], data[2]);
    }

    printf("\nThere are %d call_outs active.\n", sizeof(call_out_stuff));
}
