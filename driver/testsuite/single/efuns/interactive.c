void do_tests() {
    ASSERT(interactive(this_player()));
    ASSERT(!interactive(this_object()));
    ASSERT(sizeof(users()) == sizeof(filter(users(), (: interactive :))));
}

