/* Do not remove the headers from this file! see /USAGE for more info. */

//  Yaynu@Lima Bean
#include <mudlib.h>
inherit CMD;
inherit M_ANSI;
 
private void main(string arg) {
object forwho;   
if(!arg){
    if(i_use_ansi())  write("Ansi is "+
        ansi("%^BOLD%^%^MAGENTA%^on.%^RESET%^%^WHITE%^")+"\n");
    else write("Ansi if off.\n");
    return;
        }     
    if(!forwho)
      forwho = this_user();
    if(arg != "on" && arg != "off"){
        write("Usage: ansi <on/off>.\n");
        return;}
    if(arg == "on") {
        this_body()->query_shell_ob()->set_variable( "ansi" , 1);      
        write("Turning ansi on.\n");
        return;}
        this_body()->query_shell_ob()->unset_variable( "ansi"); 
        write("Turning ansi off.\n");
        return;
}                                    
 

