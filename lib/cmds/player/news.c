/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

int main(string arg)
{
    clone_object(NEWSREADER)->begin_reading(arg);
    return 1;
}
