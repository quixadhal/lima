void do_tests() {
    ASSERT(query_ip_number(this_object()) == 0);
    ASSERT(stringp(query_ip_number(this_player())));
}
