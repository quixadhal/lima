/* Do not remove the headers from this file! see /USAGE for more info. */


int direct_knock_at_obj()
{
    return 1;
}

int direct_knock_on_obj()
{
    return 1;
}

void do_knock()
{
    this_body()->targetted_action("$N $vrap on the $t.", this_object());
}
