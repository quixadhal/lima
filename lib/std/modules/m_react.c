/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

void add_hook(string, function);

private object		script;
private function	my_hook;
private string *	response_queue = ({});

class parse_info {
    int cur;
    string array lines;
    string array vars;
    int trigger_num;
    int term;
}

//:FUNCTION do_game_command
//Emulates handling of emotes and player commands for NPCs that inherit this
//module.  E.g. do_game_command("wield sword").  do_game_command("smile hap*").
 static void do_game_command(string str) {
    object save_tp;
    array winner;
    string verb, argument;

    save_tp = this_player();
    set_this_player(this_object());

    verb = str;
    sscanf(verb, "%s %s", verb, argument);

    winner = CMD_D->find_cmd_in_path(verb, ({ CMD_DIR_PLAYER "/" }));
    if (arrayp(winner)) {
        winner[0]->call_main(0, 0, 0, 0, 0, 0, argument);
    }
    else if (environment()) {

   mixed result = parse_sentence(str);

	if (stringp(result))
	    write(result);
    }

    set_this_player(save_tp);
}

private void do_respond() {
    mixed cmd = response_queue[0];
    if (stringp(cmd))
	do_game_command(cmd);
    else
	evaluate(cmd);
    response_queue = response_queue[1..];
}

//:FUNCTION respond
//Does a command [using do_game_command] after a small delay.  This is the
//best way to have NPCs react to events, as a slight delay makes it a bit
//more believable, and also if you do things immediately, the action may
//PRECEDE the thing you are responding to.  Example: Troll which attacks
//anyone who says 'foo'.  I say 'foo', you and the troll are in the room.
//If the message is delivered to the troll before you, the troll will attack
//me BEFORE the message gets to you.
void respond(mixed str) {
    response_queue += ({ str });
    call_out( (: do_respond :), random(3));
    if (!my_hook) {
	my_hook = (: remove_call_out :);
	add_hook("remove", my_hook);
    }
}

private array
parse_file(class parse_info pi, string array term) {
    array program = ({});
    string keyword;
    int tmp;
    array tmparr;
    string arg1, arg2;
    
    while (pi->cur < sizeof(pi->lines)) {
	string line = trim_spaces(pi->lines[pi->cur++]);
	if (line[0] == '#')
	    continue;

	if (term && (tmp = member_array(line, term)) != -1) {
	    pi->term = tmp;
	    return program;
	}
	if (line[0] == '@') {
	    keyword = line[1..];
	    line = 0;
	    sscanf(keyword, "%s %s", keyword, line);
	    switch (keyword) {
	    case "function":
		if (!line) error("Missing function name.\n");
		program += ({ ({ "function", line }) + 
				  parse_file(pi, ({ "@endfunction" })) });
		break;
	    case "move":
	    case "callscript":
		if (sscanf(line, "%s %s", arg1, arg2) < 2)
		    error("Too few arguments to " + keyword);
		program += ({ ({ keyword, arg1, arg2 }) });
		break;
	    case "trigger":
		program += ({ ({ "trigger", line, pi->trigger_num }) });
		program += ({ ({ "function", "trigger_" + pi->trigger_num++ }) + 
				  parse_file(pi, ({ "@endtrigger" })) });
		break;
	    case "doevery":
	    {
		int first, second;
		if (sscanf(line, "%d to %d", first, second) != 2) {
		    first = to_int(line);
		    second = first;
		}

		program += ({ ({ "doevery", first, second }) + 
				  parse_file(pi, ({ "@enddoevery" })) });
		break;
	    }
	    case "oneof":
		program += ({ ({ "oneof" }) + parse_file(pi, ({ "@endoneof" })) });
		break;
	    case "set":
	    {
		string lhs, rhs;
		if (sscanf(line, "%s=%s", lhs, rhs) != 2)
		    error("Missing = in @set\n");
		lhs = trim_spaces(lhs);
		rhs = trim_spaces(rhs);
		
		if (lhs[0] != '$') error("Variable must begin with $\n");
		if (member_array(lhs[1..], pi->vars) == -1)
		    pi->vars += ({ lhs[1..] });
		program += ({ ({ "set", lhs, rhs }) });
		break;
	    }
	    case "action":
		program += ({ ({ keyword, line }) });
		break;
	    case "if":
		tmparr = ({ parse_file(pi, ({ "@else", "@endif" }) ) });
		if (pi->term == 0)
		    tmparr += ({ parse_file(pi, ({ "@endif" })) });
		program += ({ ({ "if", line }) + tmparr });
		break;
	    case "delay":
		tmp = to_int(line || "2");
		if (tmp <= 0) error("Bad interval " + line + ".\n");
		program += ({ ({ "delay", tmp }) +
				  parse_file(pi, term) });
		return program;
		break;
	    default:
		error("Unknown keyword " + keyword + ".\n");
	    }
	    continue;
	}
	if (line != "")
	    program += ({ line });
    }
    if (term) error("missing " + term[<1]);
    return program;
}

object query_body() {
    return this_object();
}

object query_shell_ob() {
    return this_object();
}

string handle_prefix1(string token, string expr) {
    switch (token) {
    case "new":
	return "new(" + expr + ")";
    case "not":
	return "!(" + expr + ")";
    }
}

string handle_prefix2(string token, string expr1, string expr2) {
    switch (token) {
    case "find":
	return "present(\"" + expr1 + "\", " + expr2 + ")";
    }
}

string handle_infix2(string token, string expr1, string expr2) {
    switch (token) {
    case "find":
	return "present(\"" + expr1 + "\", " + expr2 + ")";
    }
}

string parse_literal(string expr) {
    int tmp;
    string tmpstr;
    
    if (expr[0] == '$')
	return "var_" + expr[1..];
    if (expr == "here")
	return "environment(actor)";
    if (sscanf(expr, "%d%s", tmp, tmpstr) == 2 && tmpstr == "")
	return "" + tmp;
    return expr;
}

string parse_expr1(array parts) {
    string expr1, expr2;
    string token = parts[parts[0]++];
    
    switch (token) {
    case "new":
    case "not":
	expr1 = parse_expr1(parts);
	return handle_prefix1(token, expr1);
    case "find":
	expr1 = parse_expr1(parts);
	expr2 = parse_expr1(parts);
	return handle_prefix2(token, expr1, expr2);
    default:
	return parse_literal(token);
    }
}

/*
 * Simple RDP parser.  The grammar is:
 *
 * expr1: literal |
 *        prefix1 expr1 |
 *        prefix2 expr1 expr1
 *
 * expr: expr1 |
 *       expr1 infix expr1
 */
string parse_expr(array parts) {
    string expr1, expr2;
    string token;
    
    expr1 = parse_expr1(parts);
    if (parts[0] == sizeof(parts)) return expr1;
    token = parts[parts[0]++];
    if (token != "infix2")
	error("Syntax error.\n");
    expr2 = parse_expr1(parts);

    return handle_infix2(token, expr1, expr2);
}

string handle_expression(string e) {
    array parts = explode(e, " ");
    int idx = 0;
    int tmp;
    string tmpstr;
    
    parts = ({ 1 }) + map(parts, (: trim_spaces :)) - ({ "" });

    if (sizeof(parts) == 1) error("Missing expression.\n");

    return parse_expr(parts);
}

string handle_set(string lhs, string rhs) {
    return "  var_" + lhs[1..] + " = " + handle_expression(rhs) + ";\n";
}

void compile_func(mapping funcs, string sname, array prog) {
    int i,j;
    string ssname;
    mixed prog_stack = ({ "", prog });
    
    funcs[sname] = "";
    while (sizeof(prog_stack)) {
	/* pop */
	funcs[sname] += prog_stack[0];
	prog = prog_stack[1];
	prog_stack = prog_stack[2..];
	
	for (i = 0; i < sizeof(prog); i++) {
	    mixed item = prog[i];
	
	    if (stringp(item))
		funcs[sname] += "  actor->respond(\"" + item + "\");\n";
	    else {
		switch (item[0]) {
		case "function":
		    compile_func(funcs, item[1], item[2..]);
		    break;
		case "action":
		    funcs[sname] += "  call_out( (: $(actor)->simple_action(\"" + item[1] + "\") :), 0);\n";
		    break;
		case "trigger":
		    //### Do a better job of making a regexp
			      item[1] = replace_string(item[1], "*", ".*");
		    if (!funcs["triggers"]) funcs["triggers"] = "";
		    funcs["triggers"] += "  if (regexp(str, \"" + item[1] + "\"))\n    trigger_" + item[2] + "();\n";
		    break;
		case "move":
		    funcs[sname] += "  " + handle_expression(item[1]) + "->move(" + handle_expression(item[2]) + ");\n";
		    break;
		case "callscript": 
		    funcs[sname] += "  present(\"" + item[1] + "\", environment(actor))->run_script(\"" + item[2] + "\");\n";
		    break;
		case "delay":
		    ssname = sname + "_";
		    funcs[sname] += "  call_out( (: " + ssname + " :), " + item[1] + ");\n";
		    compile_func(funcs, ssname, item[2..]);
		    break;
		case "set":
		    funcs[sname] += handle_set(item[1], item[2]);
		    break;
		case "if":
		    funcs[sname] += "  if (" + handle_expression(item[1]) + ") {\n";
		    if (sizeof(item) == 4) {
			prog_stack = ({ "  } else {\n", item[3], "}\n", prog[i+1..] })
			    + prog_stack;
			prog = item[2];
			i = -1;
		    } else {
			prog_stack = ({ "  }\n", prog[i+1..] }) + prog_stack;
			prog = item[2];
			i = -1;
		    }
		    break;
		case "doevery":
		    xxx;
		default:
		    error("Unknown opcode " + item[0] + "\n");
		}
	    }
	}
    }
}

string compile_program(class parse_info pi, array prog) {
    mapping funcs = ([]);
    string ret;
    
    compile_func(funcs, "main", prog);

    ret = @END
object actor;

void create(object a) {
    actor = a;
}

END;

    foreach (string var_name in pi->vars) {
	ret += "mixed var_" + var_name + ";\n";
    }

    foreach (string name, string statements in funcs)
	if (name != "triggers")
	    ret += "void " + name + "();\n";
    ret += "\n\n";
    
    if (funcs["triggers"]) {
        ret += "void triggers(string str) {\n" + funcs["triggers"] + "}\n\n";
        map_delete(funcs, "triggers");
    }

    foreach (string name, string statements in funcs)
	ret += "void " + name + "() {\n" + statements + "}\n\n";

    return ret;
}

void receive_outside_msg(string str) {
    if (str[<1] == '\n') str = str[0..<2];
    if (script)
	script->triggers(str);
}
	
void receive_private_msg(string str) {
    if (str[<1] == '\n') str = str[0..<2];
    if (script)
	script->triggers(str);
}
	
void compile_script(string fname) {
    class parse_info pi;
    array program;
    object ob;
    
    if (script) destruct(script);
    if (fname[0] != '/') {
	string lname = file_name();
	int tmp = strsrch(lname, "/", -1);
	fname = lname[0..tmp] + fname;
    }
    
    pi = new(class parse_info,
	     lines : explode(read_file(fname), "\n"),
	     vars : ({ }) );

    program = parse_file(pi, 0);
    write_file("/tmp/m_react.c", compile_program(pi, program), 1);

    if (ob = find_object("/tmp/m_react"))
	destruct(ob);
    script = new("/tmp/m_react", this_object());
    script->main();
}

void run_script(string name) {
    call_other(script, name);
}
