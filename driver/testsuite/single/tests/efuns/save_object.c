int x = 0;
static int z = 5;
int y = 1;

void do_tests() {
    save_object("/sf");
    ASSERT(read_file("/sf.o") == "#" + __FILE__ + "\ny 1\n");
    save_object("/sf", 1);
    ASSERT(read_file("/sf.o") == "#" + __FILE__ + "\nx 0\ny 1\n");
}
