/* Do not remove the headers from this file! see /USAGE for more info. */

//simple who cgi script.
//Cowl

string main() {
    object *people = bodies();
    string ret = "<pre>";
    foreach(object person in people) {
        ret += sprintf("%s\n", person->query_name());
    }
    return ret;
}
