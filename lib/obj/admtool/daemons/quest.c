/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "quest";
}

nomask string module_key() {
    return "Q";
}

private nomask void list_quests()
{
    more(QUEST_D->quest_dump());
}

private nomask void get_milestone(string quest_name, int value, string file,
				  string s)
{
    if ( !stringp(s) || !strlen(s) )
    {
	s = 0;
    }
    QUEST_D->add_quest(quest_name, value, file, s);

    printf("Quest %s added.\n", quest_name);
}

private nomask void get_master(string quest_name, int value, string master)
{
    string file = evaluate_path(master);

    if ( !is_file(file) && !is_file(file+".c") )
    {
	write("That file doesn't exist.\n");
	return;
    }

    modal_simple((: get_milestone, quest_name, value, file :),
		 "If this is a quest endpoint, give a one line description of the quest for\nplayers, or else hit enter (and nothing else!): ");
	     
}


private nomask void get_value(string quest_name, string pts_as_string)
{
    int value = to_int(pts_as_string);
    printf("Quest %s' value is set at %d points.\n", quest_name, value);
    modal_simple((: get_master, quest_name, value :),
		 "What file can dole out these points? ");
}

private nomask void add_quest(string name)
{
    if ( !stringp(name) || !strlen(name) )
    {
	write("Not a valid quest name.\n");
	return;
    }
    if ( QUEST_D->quest_exists(name) )
    {
	write("That quest already exists.\n");
	return;
    }

    modal_simple((: get_value, name :),
		 "How many points is this quest item worth? ");
}


private nomask void remove_quest(string name)
{
    if ( !QUEST_D->delete_quest(name) )
    {
	write("No such quest.\n");
	return;
    }
    else
    {
	write("Deleted.\n");
    }
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list all places to get points",
	    action : (: list_quests :)),
	new(class command_info,
	    key : "L",
	    desc : "list all quests",
	    action : (: more(QUEST_D->dump_final_goals()) :)),
        new(class command_info,
	    key : "a",
	    proto : "[name]",
	    desc : "add quest points for a goal",
	    args : ({ "Which quest? " }),
	    action : (: add_quest :)),
	new(class command_info,
	    key : "r",
	    proto : "[name]",
	    desc : "remove quest points for a goal",
	    args : ({ "Which quest? " }),
	    action : (: remove_quest :))
    });
}
