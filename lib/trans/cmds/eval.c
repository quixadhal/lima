/* Do not remove the headers from this file! see /USAGE for more info. */

//_eval.c
//By Rust and Phaedrus

#include <mudlib.h>


inherit DAEMON;

#define SYNTAX "USAGE:  eval <lpc expression>\n"

//prototype
string doith(string arg);

int main(string str)
{
    string tmp_file;
    string error;
    string initial_write_to_file;
    mixed return_value;
    object o;



    initial_write_to_file = 
	"#include <mudlib.h> \n"
	"#include <daemons.h>\n"
	"#include <config.h>\n"
	"mixed eval_function(){\n";
    tmp_file = wiz_dir(this_user()) + "/EVAL.c";
    rm(tmp_file);
    if(o = find_object(tmp_file)) destruct(o);
    if(!write_file(tmp_file,initial_write_to_file)){
	write("eval: Unable to write to "+tmp_file+". \n");  //shouldn't happen.
	    return 0;
    }
    str = doith(str);
    write_file(tmp_file,str+"\n");
    write_file(tmp_file,"}\n");
    
    //ok, written a file.  now call it's function.
	error = catch(return_value = tmp_file -> eval_function());
    if(error && error != 0){
	write(error+"\nFile is saved as "+tmp_file+"\n");
	return 1;
    }
    write("Return = ");
    printf("%O\n\n", return_value);
    rm(tmp_file);
    destruct(find_object(tmp_file));
    return 1;
}

/* alog to take a string of LPC and explode it (safely) on its ;'s. */

string doith(string inpu) {
    int i;    // pointer to string within array containing an exploded bit 
	int j;    // pointer to character within a given string
	    int cnt;  // current count of (+1) ('s and (-1) )'s
		int poq;  // (waiting on) parethesisisis or quote; dual state
		    string *inp; // sais what it does; does what it sais.
			
			inp = explode(inpu, ";");
    inp[sizeof(inp)-1] += ";";
    for(i=0;i<sizeof(inp); i++) {
	for(j=0;j<strlen(inp[i]); j++) {
	    if(!poq && inp[i][j] == ')') cnt --;
	if(!poq && inp[i][j] == '(') cnt ++;
	   if(inp[i][j] == 92 && inp[i][j+1] == 34) j+=2;
	   if(inp[i][j] == 34) poq = (!poq);  // toggle poq state 
	   if(strlen(inp[i])==j+1) {
	       if(sizeof(inp) == i+1) {
		   // were at the end. there are no more lines where the closing )
					   // could possibly be. barf here.
					       // OR closing "  =)
		    if (cnt>0)   write("eval: You have "+cnt+" too many left parenthesis. (().\n");
		    else  if(cnt<0) write("eval: You have "+(-cnt)+" too many right parenthesis. ()).\n");
		    else if(poq) write("eval: CR in middle of stinking string.\n");
		    // error or no error we return here at the end
		    return implode(inp, ";\n");
		}
		else if (poq || cnt)
		    inp[i] = inp[i] + ";"+ inp[i+1], inp -= ({inp[i+1]});
	    }
	}
    }
}


int help(){
    write(SYNTAX);
    write(
      "The eval command places an lpc expression inside a function on the \n"
      "fly, and tries to execute it.  Any return value is given to the user.\n"
      "example:\n"
      "eval return this_body()->query_name();\n"
      "\nWill give your cap name, or Someone if you are invis.\n\n"
      "See also: call\n"
    );
    return 1;
}
