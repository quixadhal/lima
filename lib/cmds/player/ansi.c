/* Do not remove the headers from this file! see /USAGE for more info. */

//  Yaynu@Lima Bean
#include <mudlib.h>
inherit CMD;
inherit M_ANSI;
 
private void main(string arg) {
object forwho;   
if(!arg){
    if(i_use_ansi())  out("Ansi is "+
        ansi("%^BOLD%^%^MAGENTA%^on.%^RESET%^%^WHITE%^")+"\n");
    else out("Ansi is off.\n");
    return;
        }     
    if(!forwho)
      forwho = this_user();
    if(arg != "on" && arg != "off"){
        out("Usage: ansi <on/off>.\n");
        return;}
    if(arg == "on") {
        this_body()->query_shell_ob()->set_variable( "ansi" , 1);      
        out("Turning ansi on.\n");
     this_user()->update_translations();
        return;}
        this_body()->query_shell_ob()->unset_variable( "ansi"); 
     this_user()->update_translations();
        out("Turning ansi off.\n");
        return;
}                                    
 

