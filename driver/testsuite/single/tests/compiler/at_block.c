void do_tests() {
    ASSERT( @END
xxx
yyy
END == "xxxyyy");
    ASSERT( sizeof(@@END
xxx
yyy
END == 2));
}
