/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/secret_door";

void setup(string dir);
mixed do_on_open();
mixed do_on_close();
mixed do_on_close();
mixed do_on_open();

void update_sibling()
{
    object sibling = get_sibling();
    // Needed for initial load ...\
    if( !sibling )
	return;

    // Magic ...
    if( is_open())
	sibling->do_on_open();
    else sibling->do_on_close();
    ::update_sibling();
}
void setup(string dir) {
function f;
set_id("door");
set_adj("large oak");
add_adj("large", "oak");
set_long("It is about 10 feet tall, and very beautiful.");
set_closed(1);
{
add_hook( "open", (: do_on_open :));
add_hook( "close", (: do_on_close :));
setup_door("large oak door", dir)
;
}
;
}


mixed do_on_open() {
{
call_other(environment(this_body()), "set_room_state","oak_door");
;
}
;
}


mixed do_on_close() {
{
call_other(environment(this_body()), "clear_room_state","oak_door");
;
}
;
}

