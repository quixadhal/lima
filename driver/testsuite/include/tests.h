#ifndef TESTS_H
#define TESTS_H

#define ASSERT(x) if (!(x)) { error(file_name() + ": Check failed.\n"); }
#define ASSERT2(x, r) if (!(x)) { error(file_name() + ":" + r + ", Check failed.\n"); }

#define SAVETP tp = this_player()
#define RESTORETP evaluate(bind( (: enable_commands :), tp))

#endif
