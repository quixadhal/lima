void do_tests() {
#ifndef __NO_ADD_ACTION__
    object save_tp = this_player();
    object o1, o2;
    
    ASSERT(!find_player("bar"));
    enable_commands();
    set_living_name("bar");
    o1 = find_player("bar");
    exec(this_object(), save_tp);
    o2 = find_player("bar");
    exec(save_tp, this_object());
    disable_commands();
    evaluate(bind( (: enable_commands :), save_tp));
    ASSERT(!o1);
    ASSERT(o2 == this_object());
#endif
}
