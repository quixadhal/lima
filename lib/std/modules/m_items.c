/* Do not remove the headers from this file! see /USAGE for more info. */


private nosave object my_item_mgr;

void add_item(mixed array stuff ...)
{
    if ( mapp(stuff[<1]) )
    {
	new(SIMPLE_OB, stuff[<1], stuff[0..<2]);
	return;
    }

    if ( stuff[<1][<1] != '\n' )
    {
	stuff[<1] += "\n";
    }

    if ( my_item_mgr )
    {
	my_item_mgr->add_simple_fake_item(stuff[<1], stuff[0..<2]);
    }
    else
    {
	my_item_mgr = new(FAKE_ITEM_MGR, stuff[<1], stuff[0..<2]);
    }
}
