void do_tests() {
    ASSERT(arrayp(users()));
    ASSERT(member_array(this_player(), users()) != -1);
}
