/* Do not remove the headers from this file! see /USAGE for more info. */

/*	
**	Player menu by John Viega (rust@virginia.edu) 5-Jul-95
**	For the Lima mudlib.
**
**	Still want to have:
**	o channel menu
**     o more reporter commands
**	o what's new or something like that.
**	o take args at line, ie, b yes for biff or somesuch
**     o disable newbie options
*/

#include <menu.h>
#include <mudlib.h>
#include <playerflags.h>
#include <commands.h>
#include <security.h>	/* ### for now */

inherit MENUS;
inherit M_ACCESS;	/* ### for now */

MENU toplevel;
// submenus of the toplevel (main) menu
MENU soulmenu;
MENU reportmenu;
MENU remotemenu;
MENU personalmenu;
// submenus of personalmenu
MENU biffmenu;  
MENU snoopablemenu; 
// Wizard stuff
MENU wizmenu;
MENU testmenu;


MENU_ITEM quit_item;	
MENU_ITEM goto_main_menu_item;
MENU_ITEM main_seperator;

// right now, we're just going to call the help command.
//private MENU helpmenu;


  private mapping dispatch = ([ "n" : "news", "w" : "who", 
			      "b" : "bug", "t" : "typo", "i" : "idea",
			      "?" : "help", "l" : "mudlist" ]);


private nomask void do_cmd(string cmd)
{
    unguarded(1, (: call_other, this_user(), "force_me", cmd :));
}

private nomask void 
simple_cmd(string cmd)
{
    do_cmd(dispatch[cmd]);
//  call_other(CMD_DIR_PLAYER "/" + dispatch[cmd],"main");
}


void
start_mail()
{
    this_body()->query_mailer()->begin_mail();
}

void 
handle_finger(string person)
{
    do_cmd("finger " + person);
//  CMD_OB_FINGER->main(person);
    prompt_then_return();
}

void
find_soul(string s)
{
  if(!s) {
    write("Invalid.\n");	
    return;
  }

  do_cmd("emoteapropos " + s);
//  CMD_OB_EMOTEAPROPOS->main(s);
}

void
show_souls(string s)
{
   do_cmd("feelings " + s);
//    CMD_OB_FEELINGS->main("^"+s);
}

 
void
show_adverbs(string s)
{
  write(
"Remember: You can get any of these adverbs by typing part of the adverb \n"
"then *, if the part you type is unique.  Eg, kick rust ene* would give you:\n"
"kick rust energetically, but kick rust en* won't because it also\n"
"matches endearingly and enthusiastically.\n");
  do_cmd("adverbs " + s);
//  CMD_OB_ADVERBS->main("^"+s);
}


void
change_email(string s)
{
  this_user()->set_email(s);
  write("Email info changed.\n");
}

void
change_title(string s)
{

  if(strsrch(s,"$N") == -1)
    s = "$N " + s;
  this_body()->set_title(s);
  
  write("Title changed.\n");
}
 
void
query_personal_info()
{
  printf("Your title is: %s\n"
	 "Your provided e-mail address is: %s\n"
	 "You %sget notified when new mail arrives.\n" 
	 "Wizards %s snoop you.\n",
	 this_body()->query_title(),
	 this_user()->query_email(),
	 this_body()->test_flag(F_BIFF) ? "" : "DON'T ",
	 this_body()->test_flag(F_SNOOPABLE) ? "CAN" : "can't"
	 );

  prompt_then_return();
}

void
prompt_change_title()
{
  get_input_then_call ((: change_title :), "Change title to what? " );
}

void
prompt_change_email()
{
  get_input_then_call ((: change_email :), "Change e-mail to what? ");
}

void 
set_biff(string s)
{
  if(s == "y")
    {
      this_body()->set_flag(F_BIFF);	
      write("Mail notification is now on.\n");

    }
  else
    {
      this_body()->clear_flag(F_BIFF);	
      write("Mail notification is now off.\n");
    }
  goto_previous_menu();
} 

 
void
set_ansi(string s)
{
  if(s && s=="on")
  {
  this_body()->query_shell_ob()->set_variable( "ansi" , 1);
  write("Ansi is now on.\n");
  } else {
  this_body()->query_shell_ob()->unset_variable( "ansi");
  write("Ansi is off.\n");
}
}
      
   
void
set_snoopable(string s)
{
  if(s == "n")
    {
      this_body()->clear_flag(F_SNOOPABLE);
      write("You now can not be snooped (except by admins)\n");
    }
  else
    {
      this_body()->set_flag(F_SNOOPABLE);
      write("Any wizard can now snoop you.\n");
    }
  goto_previous_menu();
}

void
finish_who(string mudname)
{
    do_cmd("finger @" + mudname);
//  CMD_OB_FINGER->main("@"+mudname);
  printf("%s queried.  It's up to that mud to reply to you.\n", mudname);
  prompt_then_return();
}

void
remote_who()
{
  write("Which mud do you want to query?\n");
  complete_choice(0, IMUD_D->query_mudnames(), (: finish_who :));
}

void
finish_mudinfo(string mudname)
{
do_cmd( "mudinfo " + mudname );
printf("%s queried.  It's up to that mud to reply to you.\n", mudname);
}
 
void
remote_mudinfo()
{
   write("Which mud do you want to query?\n");
   complete_choice(0, IMUD_D->query_mudnames(), (: finish_mudinfo :));
}

void
create()
{
    set_privilege(1);

  toplevel 	= new_menu(mud_name()+" Game Menu");
  soulmenu 	= new_menu("Soul Menu");
  reportmenu 	= new_menu("Reporter Menu");
  personalmenu  = new_menu("Personal Menu");
  biffmenu      = new_menu("Notify you when you receive new mail?");
  snoopablemenu = new_menu("Allow wizards to snoop you?");
  remotemenu    = new_menu("Other muds");
  wizmenu       = new_menu("Wizard menu");

  // Since we'll use these things more than once, we can just
  // call new_menu_item once, and insert them wherever we want
  // to use them.
  quit_item = new_menu_item("Quit", (:quit_menu_application:), "q");
  goto_main_menu_item =new_menu_item("Return to main menu", 
					 toplevel, "m");


  main_seperator = new_seperator("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");


  // Add items to the toplevel (main) menu.  
  add_menu_item (toplevel, main_seperator);
  add_menu_item (toplevel, new_menu_item("Read news (news)", 
					 (: simple_cmd :), "n"));
  add_menu_item (toplevel, new_menu_item("Send or read mail (mail)",
					 (: start_mail :), "m"));
  // The 1 at the end of this next one creates a prompt:
  // "[Hit enter to return to menu] "
  // You can also do this by calling prompt_then_return() from your
  // function.
  add_menu_item (toplevel, new_menu_item("See who's on (who)", 
					    (: simple_cmd :), "w", 1));
  add_menu_item (toplevel, new_menu_item("Get info about a person (finger)", 
  					    (: get_input_then_call, 
					      (: handle_finger :),
					     "Finger who? ":), "f"));
  add_menu_item (toplevel, new_menu_item("Soul/emote Menu", soulmenu, "s"));
  add_menu_item (toplevel, new_menu_item("Report a bug, typo or idea", 
					 reportmenu, "r"));
  add_menu_item (toplevel, new_menu_item("Change or view your personal information. ",
					 personalmenu, "i"));
  add_menu_item (toplevel, new_menu_item("Information on other muds",
					 remotemenu, "o"));
  add_menu_item (toplevel, new_menu_item("Wizard stuff", wizmenu, "*", 0, 
					 (: wizardp(this_player()) :)));
  add_menu_item (toplevel, quit_item);
  add_menu_item (toplevel, new_menu_item("Help", (: simple_cmd :), "?"));

  // Add items to the soul menu.
  add_menu_item (soulmenu, main_seperator);
  add_menu_item (soulmenu, 
		 new_menu_item("List souls", (: get_input_then_call,
					      (: show_souls :),
					      "List all souls starting with "
					      "(enter for ALL souls): ":),
			       "s"));
  add_menu_item (soulmenu, 
		 new_menu_item("List adverbs", (: get_input_then_call,
					      (: show_adverbs :),
					      "List all adverbs starting with "
					      "(enter for ALL adverbs): ":),
			       "a"));
  add_menu_item (soulmenu, 
		 new_menu_item("Find souls", (: get_input_then_call,
					      (: find_soul :),
					      "Find souls whoose output "
					      "contains string: ":),
			       "f"));

  //  add_menu_item (soulmenu, 
  //		 new_menu_item("Test a soul", (: get_input_then_call,
  //				      (: test_soul :),
  //				      "Soul to test: ":),
  //		       "t"));

  add_menu_item (soulmenu, quit_item);
  
  add_menu_item (soulmenu, goto_main_menu_item);
					 
  add_menu_item (reportmenu, main_seperator);
  add_menu_item (reportmenu, new_menu_item("Report a bug", (: simple_cmd :),
					   "b"));
  add_menu_item (reportmenu, new_menu_item("Report a typo", (: simple_cmd :),
					   "t"));
  add_menu_item (reportmenu, new_menu_item("Report an idea", (: simple_cmd :),
					   "i"));
  add_menu_item (reportmenu, quit_item);
  add_menu_item (reportmenu, goto_main_menu_item);

  add_menu_item (personalmenu, main_seperator);
  add_menu_item (personalmenu, new_menu_item("View your personal information",
					     (: query_personal_info :), "v"));
  add_menu_item (personalmenu, new_menu_item("Change your title", 
					     (: prompt_change_title :), "t"));
  add_menu_item (personalmenu, new_menu_item("Change your supplied "
					     "e-mail address",
					     (: prompt_change_email :), "e"));
  add_menu_item (personalmenu, new_menu_item("Set or unset mail notification",
					     biffmenu, "n"));
  add_menu_item (personalmenu, new_menu_item("Set whether or not you can be "
					     "snooped", snoopablemenu, "s"));
  //  add_menu_item (personalmenu, new_menu_item("Change your supplied real name",
  //					     (: prompt_change_real_name :), "n"));
add_menu_item(personalmenu, new_menu_item("Set ANSI on/off", (: get_input_then_call, (: set_ansi :), "Ansi 'on' or 'off' ? (default off): ":), "a"));
 
  add_menu_item (personalmenu, quit_item);
  add_menu_item (personalmenu, goto_main_menu_item);

  add_menu_item (biffmenu, main_seperator);
  add_menu_item (biffmenu, new_menu_item("Yes", (: set_biff :), "y"));
  add_menu_item (biffmenu, new_menu_item("No", (: set_biff :), "n"));
  add_menu_item (biffmenu, quit_item);
  add_menu_item (biffmenu, goto_main_menu_item);

  add_menu_item (snoopablemenu, main_seperator);
  add_menu_item (snoopablemenu, new_menu_item("Yes", 
					      (: set_snoopable :), "y"));
  add_menu_item (snoopablemenu, new_menu_item("No", 
					      (: set_snoopable :), "n"));
  add_menu_item (snoopablemenu, quit_item);
  add_menu_item (snoopablemenu, goto_main_menu_item);

  add_menu_item (remotemenu, main_seperator);
  add_menu_item (remotemenu, new_menu_item("List muds " + mud_name() + 
					   " knows about", 
					   (:simple_cmd :), "l", 1));
  add_menu_item (remotemenu, new_menu_item("See who's on another mud",
					   (:remote_who:), "w"));
  add_menu_item (remotemenu, quit_item);
  add_menu_item (remotemenu, goto_main_menu_item);
  add_menu_item (wizmenu, main_seperator);
add_menu_item (wizmenu, new_menu_item("Info on another mud",
                                    (: remote_mudinfo :), "i"));
  add_menu_item (wizmenu, quit_item);
  add_menu_item (wizmenu, goto_main_menu_item);

}


void
start_menu()
{
  init_menu_application(toplevel);
}
