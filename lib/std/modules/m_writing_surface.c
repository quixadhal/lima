/* Do not remove the headers from this file! see /USAGE for more info. */

private int max_char = 100;
private int char_left = 100;
string array msgs;
string extra;

void add_hook(string, mixed);
void remove_hook(string, mixed);

//:FUNCTION set_space
//Set the maximum number of characters that can fit on the writing surface
void set_space(int max) {
    max_char = max;
}

//:FUNCTION query_space
//Return the maximum number of characters that can fit on the writing surface
int query_space() {
    return max_char;
}

//:FUNCTION query_space_left
//Return the amount of space left (number of characters) on a writing surface
int query_space_left() {
    return char_left;
}

//:FUNCTION add_text
//Add some text to a writing surface.  Make sure it fits (query_space_left)
//first.
void add_text(string str) {
    remove_hook("extra_long", extra);
    
    char_left -= strlen(str);
    if (msgs) {
	msgs += ({ str });
	extra = "There are " + sizeof(msgs) + " things written on it:\n\t'" +
	    implode(msgs, "'\n\t'") + "'\n";
    } else {
	msgs = ({ str });
	extra = "There is 1 thing written on it:\n\t'" + str + "'\n";
    }
    add_hook("extra_long", extra);
}

/* parser interaction */
mixed direct_write_str_on_obj(string str, object ob) {
    if (strlen(str) > query_space_left())
	return "There isn't enough room.\n";
    return 1;
}

mixed direct_write_obj_with_str(object ob, string str) {
    if (strlen(str) > query_space_left())
	return "There isn't enough room.\n";
    return 1;
}
