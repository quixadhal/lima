static int stat() {
}

private int priv() {
}

void do_tests() {
    // These next two might be considered a bug
    ASSERT(function_exists("stat", this_object()));
    ASSERT(function_exists("priv", this_object()));

    ASSERT(!function_exists("bar", this_object()));
    ASSERT(function_exists("do_tests", this_object()) + ".c" == __FILE__);
}
     
