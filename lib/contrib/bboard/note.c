/* Do not remove the headers from this file! see /USAGE for more info. */


inherit VERB_OB;

mixed do_note_str_on_obj(string str, object ob) {
	ob->note(str);
}

array query_verb_info() {
	return ({({"STR on OBJ"}),({})});
}
