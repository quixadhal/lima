/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";


nomask string module_name() {
    return "skill";
}

nomask string module_key() {
    return "S";
}

private nomask void list_skills()
{
    string * skills = sort_array(SKILL_D->query_skills(), 1);
    string * result;

    if ( sizeof(skills) == 0 )
    {
	write("No skills are defined yet.\n");
	return;
    }

    result = ({ sprintf("%-20s %s", "Skill", "Internal name"),
		repeat_string("-", 40) }) +
	map(skills, function (string name)
	    {
		string * parts = explode(name, "/");
		return sprintf("%-20s %s",
			       repeat_string("   ",
					     sizeof(parts)-1) + parts[<1],
			       name);
	    }
	    );

    more(result);
}

private nomask void rcv_add_skill(string skill_name)
{
    string * result;

    skill_name = lower_case(trim_spaces(skill_name));
    if ( skill_name == "" )
	return;

    result = SKILL_D->register_skill(skill_name);
    if ( !result )
	write("ERROR: skill already exists.\n");
    else
	printf("Added: %s.\n", format_list(result));
}

private nomask void rcv_remove_skill(string skill_name)
{
    string * result;

    skill_name = lower_case(trim_spaces(skill_name));

    result = SKILL_D->remove_skill(skill_name);
    if ( !result )
	write("ERROR: no such skill exists.\n");
    else
	printf("Removed: %s.\n", format_list(result));
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "l",
	    desc : "list skills",
	    action : (: list_skills :)),
	new(class command_info,
	    key : "a",
	    proto : "[name]",
	    desc : "add a skill",
	    args : ({ "New skill name? " }),
	    action : (: rcv_add_skill :)),
	new(class command_info,
	    key : "r",
	    proto : "[name]",
	    desc : "remove a skill",
	    args : ({ "Remove which skill? " }),
	    action : (: rcv_remove_skill :))
    });
}
