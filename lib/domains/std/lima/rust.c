/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIVING;
inherit M_CONVERSATION;

void handle_help() {
    new(__DIR__ "directory")->move(this_body());
    targetted_action("$N $vgive $t a directory.\n", this_body());
    set_start( ({ "forever" }), this_body() );
}

void setup() {
    set_name("Rust");
    set_gender(1);
    set_proper_name("Rust");
    set_in_room_desc("Rust is here.  He appears to be busy trying to write rooms.\n");
    set_long("You can't really tell much about him, except that he is male (or at least CLAIMS to be male), and likes to be called Rust.  Funny how MUDs work like that, eh?\n");
    
    set_options( ([
	"hello" : "What's up?",

	"idea" : "What is your idea?",
	"mudlib" : "Want to write a mudlib for me?",

	"work" : "Why will it be so much work?",

	"zork_no" : "What's Zork?",
	"zork_yes" : "Of course!  I love that game!",

	"help" : "Can I help?",

	"forever" : "This is going to take forever.",
    ]) );
    set_responses( ([
	"hello" : "I've got this great idea for a new MUD!@@idea,mudlib",

	"idea" : "I'm working on a MUD based on Zork.  Unfortunately, it will be a lot of work.@@work",
	"mudlib" : ({ "!cha", "You didn't think it would be that easy, did you?", "!wink" }),

        "work" : "I want to create all the Zork rooms, but that will take forever!  Ever played Zork?@@zork_yes,zork_no",

	"zork_no" : "!emote looks at you like you are a complete idiot.  \"Never mind,\" he says.@@@@idea,zork_yes",
	"zork_yes" : ({ "!smile", "Me too!@@help@@zork_no" }),

	"help" : ({ "!kewl", "Here is a directory to work in.@@forever", (: handle_help :) }),
	
	"forever" : "Yeah, I wish I knew a better way to do this.",
    ]) );
    set_start( ({ "hello" }) );
}
