void do_tests() {
    string tmp = "this is a test";
    mixed *ret;
    
    ret = explode(tmp, "");
    ASSERT(sizeof(ret) == sizeof(filter(ret, (: strlen($1) == 1 :))));
    ret = explode(tmp, " ");
    ASSERT(sizeof(ret) == 4);
    ASSERT(ret[1] == "is");
    ASSERT(ret[3] == "test");

    ret = explode(" " + tmp, " ");
    ASSERT(sizeof(ret) == 4);
    ASSERT(ret[1] == "is");
    ASSERT(ret[3] == "test");

#ifndef __SANE_EXPLODE_STRING__
    ret = explode("     " + tmp, " ");
    ASSERT(sizeof(ret) == 4);
    ASSERT(ret[1] == "is");
    ASSERT(ret[3] == "test");
#endif

    tmp = "this  is  a  test  ";
    ret = explode(tmp, "  ");
    ASSERT(sizeof(ret) == 4);
    ASSERT(ret[1] == "is");
    ASSERT(ret[3] == "test");

    ret = explode("  " + tmp, "  ");
    ASSERT(sizeof(ret) == 4);
    ASSERT(ret[1] == "is");
    ASSERT(ret[3] == "test");

#ifndef __SANE_EXPLODE_STRING__
    ret = explode("      " + tmp, "  ");
    ASSERT(sizeof(ret) == 4);
    ASSERT(ret[1] == "is");
    ASSERT(ret[3] == "test");
#endif

}
