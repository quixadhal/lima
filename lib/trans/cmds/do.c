/* Do not remove the headers from this file! see /USAGE for more info. */

/* Another really funky command from Beek. */

#include <mudlib.h>
#include <commands.h>

inherit DAEMON;

int loopvars;

string do_loop(int i) {
    string str;

    str = " ";
    str[0]=105 + loopvars++;
    return sprintf("for (%s=0; %s<%i; %s++)", str, str, i, str);
}

string parse_ref(string ref) {
    string fname;
    int i;

    if (ref == "me") return "this_body()";
    if (ref == "here") return "environment(this_body())";
    if (find_body(ref)) return "find_body(\""+ref+"\")";
    if (ref=="cmd") return "this_user()->force_me";
    if (present(ref,environment(this_body()))) return "present(\""+ref+"\", environment(this_body()))";
    if (present(ref,this_body())) return "present(\""+ref+"\",this_body())";
    if (find_object(ref)) return "find_object(\""+ref+"\")";
//    if (sscanf(ref,"%d",i)) return do_loop(i);
    fname = evaluate_path(ref);
    if (find_object(fname)) return "find_object(\""+fname+"\")";

    return "."+ref;
}

int main(string str) {
    string *args;
    int i,n;
    string tmp;
    mixed *rtmp;
    

    loopvars = 0;
    if(!str) return write("Do wot?\n"),0;
    rtmp = reg_assoc(str, ({ "\\.[a-z0-9/#@_]+" }), ({ 0 }) );
    args = rtmp[0];

    for (i=0, n = sizeof(args); i<n; i++) {
	if (args[i][0]=='.') {
	    args[i] = parse_ref(args[i][1..<0]);
	}
    }

    str = implode(args,"");
    if (strsrch(str,";")==-1) str = "return "+str;

    tmp = " ";
    while (loopvars--) {
	tmp[0] = 105+loopvars;
	str = "int "+tmp+"; "+str;
    }
    write("Doing: "+str+"\n");
    return CMD_OB_EVAL->main(str);
}
