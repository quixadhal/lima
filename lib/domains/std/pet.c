/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** pet.c
**
** Written by Beek, during a dangerous attack of chrome and cheeze.
*/

/*

.petrc documentation:

legal fields: name, gender, proper_name, in_room_desc, ids, adjs, long,
              command illegal action

each line should be field:value

use quotes to force something to be taken as a string; this is useful if
    it can be mistaken for a number or an array
use commas for arrays (arrays are always arrays of strings)

The defaults correspond to the following file (as an example):
---
name:Foo's pet
gender:0
proper_name:Foo's pet
in_room_desc:Foo's pet
ids:pet
adjs:foos
long:"It doesn't have a description, unfortunately."
command illegal_cmd:growl
---
*/

inherit LIVING;
inherit M_ACTIONS;
inherit M_TRIGGERS;

object master;
mapping commands = ([]);

void do_command(string who, string str) {
    mixed comms;

    if (str[<1] == '.')
	str = str[0..<2];
    
    if (lower_case(who) != master->query_name()) {
	comms = commands["illegal_cmd"] || "growl";
    } else
    if (!(comms = commands[str])) {
	comms = str;
    }
    
    if (pointerp(comms))
	comms = choice(comms);
    
    respond(comms);
}

mapping parse_file(string file) {
    string line;
    mapping ret = ([]);
    
    foreach (line in explode(file, "\n")) {
	mixed key, value;
	mixed tmp;
	
	if (sscanf(line, "%s:%s", key, value) != 2) {
	    write(".petrc: Syntax error\n");
	} else {
	    if (value[0] == '"' && value[<1] == '"')
		value = value[1..<2];
	    else
	    if (to_int(value) || value == "0")
		value = to_int(value);
            else
	    if (sizeof(tmp = explode(value, ",")) > 1) {
		value = tmp;
	    }
	    if (key[0..7] == "command ")
		commands[key[8..]] = value;
	    else
		ret[key] = value;
	}
    }
    return ret;
}

void setup() {
    string file;
    mapping m;
    string name;
    
    master = this_user();
    if (file = read_file("/wiz/" + master->query_name() + "/.petrc"))
	m = parse_file(file);
    else 
	m = ([]);
    
    this_body()->add_hook("move", (: move(environment(master->query_body())) :));

    name = m["name"] || capitalize(master->query_name()) + "'s Pet";

    set_name(name);
    set_gender(m["gender"]);
    set_proper_name(m["proper_name"] || name);
    set_in_room_desc(m["in_room_desc"] || name);
    if (!m["ids"]) m["ids"] = "pet";
    if (stringp(m["ids"]))
	add_id(m["ids"]);
    else
	add_id(m["ids"]...);
    if (!m["adjs"]) m["adjs"] = master->query_name() + "s";
    if (stringp(m["adjs"]))
	add_id(m["adjs"]);
    else
	add_id(m["adjs"]...);
    set_long(m["long"] || "It doesn't have a description, unfortunately.");

    add_pattern("%s says: " + name + ", %s", (: do_command :));
}
