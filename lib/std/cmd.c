#include <mudlib.h>

inherit DAEMON;

varargs void main(mixed arg1, mixed arg2);

varargs nomask 
void call_main(mixed arg1, mixed arg2) {
    if (previous_object() != this_body()->query_shell_ob())
	error("Illegal attempt to fake a command.\n");
    main(arg1, arg2);
}

static nomask varargs
void resend(string ob, mixed arg1, mixed arg2) {
    if (ob->valid_resend(file_name()))
	ob->do_resend(arg1, arg2);
    else
	error("Illegal resend()\n");
}

nomask
void do_resend(mixed arg1, mixed arg2) {
    string prog = call_stack(0)[1];
    string fun = call_stack(2)[1];
    
    if (prog != "std/cmd.c" || fun != "resend")
	error(sprintf("Illegal resend() from %s::%s()\n", prog, fun));

    main(arg1, arg2);
}
