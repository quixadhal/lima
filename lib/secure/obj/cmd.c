/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;
inherit __DIR__ "cmd/stdio";

varargs void main(mixed arg1, mixed arg2);

varargs nomask 
mixed call_main(mixed arg1, mixed arg2, mixed arg3, mixed arg4,
		mixed implode_info, mixed remaining_implode_info,
		string original_input) {

  mixed 	hello_res;

    if (previous_object() != this_user()->query_shell_ob())
	error("Illegal attempt to fake a command.\n");
    if(!restrict_redirection())
      {
	if((hello_res = hello_stdio(arg3,arg4, remaining_implode_info)) == 0)
	  {
	    return 0;
	  };
	if(stringp(hello_res))
	  main(arg1,arg2,hello_res,implode_info);
	else
	  main(arg1, arg2,0,implode_info);
      }
    else
      {
	bare_init();
	main(original_input,arg1,arg2,implode_info);
      }
    return done_outputing();
}

static nomask varargs
void resend(string ob, mixed arg1, mixed arg2) {
    if (ob->valid_resend(base_name()))
	ob->do_resend(arg1, arg2);
    else
	error("Illegal resend()\n");
    out(ob->get_output());
}

nomask
void do_resend(mixed arg1, mixed arg2) {
    string prog = call_stack(0)[1];
    string fun = call_stack(2)[1];
    
    if (prog != "secure/obj/cmd.c" || fun != "resend")
	error(sprintf("Illegal resend() from %s::%s()\n", prog, fun));
    if(!restrict_redirection())
      hello_stdio(0,0,0);
    else
      	bare_init();
    main(arg1, arg2);
}






