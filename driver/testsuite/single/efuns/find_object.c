void do_tests() {
    object ob;
    
    ASSERT(find_object(__FILE__) == this_object());
    ob = find_object("/single/efuns/unloaded");
    if (ob)
	destruct(ob);
    ASSERT(!find_object("/single/efuns/unloaded"));
    ASSERT(ob = find_object("/single/efuns/unloaded", 1));
    ASSERT(file_name(ob) == "/single/efuns/unloaded");
    destruct(ob);
    ASSERT(ob = load_object("/single/efuns/unloaded", 1));
    ASSERT(file_name(ob) == "/single/efuns/unloaded");
    ASSERT(!load_object("/foo"));
}
