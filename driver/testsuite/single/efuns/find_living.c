void do_tests() {
#ifndef __NO_ADD_ACTION__
    object ob;
    object save_tp = this_player();
    
    ASSERT(!find_living("bar"));
    enable_commands();
    set_living_name("bar");
    ob = find_living("bar");
    disable_commands();
    evaluate(bind( (: enable_commands :), save_tp));
    ASSERT(ob == this_object());
    ASSERT(!find_living("bar"));
#endif
}
