/* Do not remove the headers from this file! see /USAGE for more info. */

// call_out info display by Rust.

//:COMMAND
//USAGE:  calls
//
//This command shows you the number of call_outs that are active
//
//Produces display like :
//     calls
//
//object                                            Function            Delay
//-----------------------------------------------------------------------------
///secure/master                                    <function>          1
///domains/std/harry#474                            <function>          3
///domains/std/harry                                <function>          3
//
//There are 3 call_outs active..

inherit CMD;

private void main()
{
    mixed	*call_out_stuff;
    mixed	*data;
    int		i;

    call_out_stuff = filter_array(call_out_info(), (: sizeof :));

    outf("%-50s%-20s%-10s\n","Object","Function","Delay" );
    outf("%77'-'s\n", "");

    foreach ( data in call_out_stuff )
    {
	if ( functionp(data[1]) )
	    outf("%-70O%-10d\n", data[1], data[2]);
	else if ( data[0] )
	    outf("%-50s%-20s%-10d\n", file_name(data[0]),
		   data[1], data[2]);
	else
	    outf("%-50s%-20s%-10d\n", "<destructed>", data[1], data[2]);
    }

    outf("\nThere are %d call_outs active.\n", sizeof(call_out_stuff));
}
