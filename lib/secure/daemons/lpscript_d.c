/* Do not remove the headers from this file! see /USAGE for more info. */

inherit M_ACCESS;

private mapping inheritable = ([
    "living" : LIVING,
    "actor": M_ACTIONS,
]);

int cur, intrigger;

string handle_oneof(string arg, array block);
string handle_expression(string arg);
string handle_block(array lines);
void add_error(int, string);

private mapping keywords = ([
    "oneof" : (: handle_oneof :),
    "if" : (: "if (" + handle_expression($1) +
	    ") {\n" + handle_block($2) + "}\n" :),
    "nexttrigger" : (: intrigger ? "continue;" : (add_error(cur, "nexttrigger illegal outside of trigger"), "") :),
]);

mixed handle_periodic(string arg, array lines);
mixed handle_trigger(string arg, array lines);

private mapping funcs = ([
    "periodic" : (: handle_periodic :),
    "trigger" : (: handle_trigger :),
]);

#define CREATE_STR(x) ({ "setup", x + "(\"" + $1[0] + "\")" })

mixed handle_is(array args);
mixed handle_gender(array args);
mixed handle_list(string str, array args);

// TODO: add requirements for these (living for gender, etc)
private mapping attributes = ([
    "is" : (: handle_is :),
    "id" : (: handle_list, "add_id" :),
    "name" : (: CREATE_STR("set_name") :),
    "proper_name" : (: CREATE_STR("set_proper_name") :),
    "in_room_desc" : (: CREATE_STR("set_in_room_desc") :),
    "long" : (: CREATE_STR("set_long") :),
    "gender" : (: handle_gender :),
]);

int dest, indent, linesync;
array lines;
array errors;
string fname;

array inherits;
array triggers;

void create() {
    set_privilege(1);
}

void add_error(int line, string err) {
    if (sizeof(errors) < 3)
	errors += ({ fname + ":" + line + "-> " + err });
}

void do_errors() {
    if (sizeof(errors))
	error("Script compilation failed:\n" + implode(errors, "\n") + "\n");
}

array line_info() {
    if (linesync) {
	linesync = 0;
	return ({ cur });
    }
    return ({});
}

array parse_block(int oldind, int indent) {
    int needend = -1;
    array ret = ({});
    
    while (cur < sizeof(lines)) {
	int newind;
	string line = lines[cur++];

	if (line == "" || line[0] == '#') {
	    linesync = 1;
	    continue;
	}

	if (line[0] == ' ') {
	    newind = 1;
	    while (line[newind] == ' ')
		newind++;
	    line = line[newind..];
	} else newind = 0;

	if (indent == -1) {
	    if (newind == 0)
		add_error(cur, "Bad indentation.");
	    indent = newind;
	}
	
	if (newind < indent) {
	    if (line == "end") {
		if (newind == oldind) {
		    needend = 0;
		    break;
		} else if (needend)
		    add_error(cur, "Bad indentation for 'end'.");
	    }
	    cur--;
	    break;
	} else
	if (newind > indent) {
	    if (arrayp(ret[<1]))
		add_error(cur, "Bad indentation.");
	    cur--;
	    ret[<1] = ({ ret[<1] }) + line_info() + parse_block(indent, newind);
	} else {
	    ret += line_info() + ({ line });
	    needend++;
	}
    }
    
    if (needend > 0)
	add_error(cur-1, "Missing 'end'.");
    return ret;
}

void assert_alphanum(int line, string what) {
    if (!regexp(what, "^[A-Za-z][A-Za-z0-9_]*$"))
	add_error(line, "Illegal name '" + what + "'.");
}

void handle_grouping() {
    int newind;
    
    linesync = cur = dest = indent = 0;
    
    while (cur < sizeof(lines)) {
	mixed part1;
	string part2, part3;
	string line = lines[cur++];

	if (line == "" || line[0] == '#') {
	    linesync = 1;
	    continue;
	}
	
	if (line[0] == ' ')
	    add_error(cur, "Bad indentation.");
	if (sscanf(line, "%s[%s]:%s", part1, part2, part3) == 3) {
	    assert_alphanum(cur, part1);
	    part1 = ({ "[", part1, part2 });
	    part2 = part3;
	} else if (sscanf(line, "%s:%s", part1, part2) == 2) {
	    assert_alphanum(cur, part1);
	    part1 = ({ ":", part1 });
	} else part1 = 0;
	
	if (part1) {
	    if (part2 == "") {
		if (part1[0] == ":") part1[0] = "=";
		lines[dest++] = line_info() + part1 + parse_block(0, -1);
	    } else {
		lines[dest++] = line_info() + part1 + ({ trim_spaces(part2) });
	    }
	} else
	    add_error(cur, "Syntax error");
    }
    lines = lines[0..dest-1];
}

mixed handle_expr1(string arg) {
    string rest = "";
    mixed tmp;
    
    arg = trim_spaces(arg);
    if (sscanf(arg, "%d%s", tmp, rest) == 2) {
	return ({ tmp+"", rest });
    }
    sscanf(arg, "%([A-Za-z]*)%s", arg, rest);

    switch (arg) {
    case "who":
	return ({ "this_body()", rest });
    case "me":
	return ({ "this_object()", rest });
    case "chance":
	tmp = handle_expr1(rest);
	return ({ "random(100) < " + tmp[0], tmp[1] });
    }
    add_error(cur, "'" + arg + "' unrecognized.");
    return ({ "", rest });
}

mixed handle_infix(string arg) {
    string rest = "";
    
    arg = trim_spaces(arg);
    sscanf(arg, "%s %s", arg, rest);

    switch (arg) {
    case "notequal":
	return ({ (: $1 + " != " + $2 :), rest });
    }
    
    return 0;
}

mixed handle_subexpression(string arg) {
    mixed tmp, tmp2, tmp3;
    int i = 0;
    
    while (1) {
	switch (arg[i++]) {
	case 0:
	    add_error(cur, "expression expected.");
	    return ({ "0", "" });
	case ' ':
	    i++;
	    break;
	case '(':
	    tmp = handle_subexpression(arg[i..]);
	    if (tmp[1][0] != ')')
		add_error(cur, "expected ')'");
	    return ({ tmp[0], tmp[1][1..] });
	default:
	    tmp = handle_expr1(arg[i-1..]);
	    if (tmp2 = handle_infix(tmp[1])) {
		tmp3 = handle_subexpression(tmp2[1]);
		tmp[0] = evaluate(tmp2[0], tmp[0], tmp3[0]);
		tmp[1] = tmp3[1];
	    }
	    return tmp;
	}
    }
}

string handle_expression(string arg) {
    mixed tmp;

    tmp = handle_subexpression(arg);
    if (tmp[1] != "")
	add_error(cur, "junk found after valid expression");
    return tmp[0];
}

string handle_block(array lines) {
    string ret = "";
    
    foreach (mixed line in lines) {
	if (arrayp(line)) {
	    string keyword, args;
	    function f;
	  
	    keyword = line[0];
	    sscanf(keyword, "%s %s", keyword, args);
	    if (f = keywords[keyword]) {
		ret += evaluate(f, args, line[1..]);
	    } else
		add_error(cur, "Unknown keyword '" + keyword + "'.");
	}
	else if (line[0] == '!')
	    ret += "respond(\"" + line[1..] + "\");\n";
	else {
	    string keyword, args;
	    function f;
	  
	    keyword = line;
	    sscanf(keyword, "%s %s", keyword, args);
	    if (f = keywords[keyword]) {
		ret += evaluate(f, args, ({}));
	    } else
		add_error(cur, "Unknown keyword '" + keyword + "'.");
	}
    }
    return ret;
}

string handle_oneof(string arg, array block) {
    string ret;

    ret = "switch (random(" + sizeof(block) + ")) {\n";

    for (int i = 0; i < sizeof(block); i++) {
	ret += "case " + i + ":\n";
	ret += handle_block( ({ block[i] }) );
	ret += "break;\n";
    }
    return ret + "}\n";
}

mixed handle_periodic(string arg, array lines) {
    int min, max;
    string time;
    
    if (sscanf(arg, "%d to %d", min, max) == 2) {
	time = min + " + random(" + (max-min) + ")";
    } else {
	sscanf(arg, "%d", min);
	time = "" + (min || 2);
    }
    
    return ({ "setup", "f = function(function f) { " + handle_block(lines) + " call_out(f, " + time + ", f); }; call_out(f, " + time + ", f)" });
}

mixed handle_trigger(string arg, array lines) {
    array ret;
    int num;
    int oldlen;
    
    intrigger = 1;
    oldlen = strlen(arg);
    arg = replace_string(arg, "*", "%s");
    num = strlen(arg) - oldlen;

    triggers += ({ ({ arg, num, handle_block(lines) }) });
		     
    intrigger = 0;
    return 0;
}

mixed handle_list(string func, array args) {
    args = map(explode(args[0], ","), (: trim_spaces :));
    
    return ({ "setup", func + "(\"" + implode(args, "\", \"") + "\")" });
}

mixed handle_is(array args) {
    string ret = "";
    args = map(explode(args[0], ","), (: trim_spaces :));

    foreach (string file in args) {
	if (file[0] == '"' && file[<1] == '"')
	    inherits += ({ file });
	else if (inheritable[file])
	    inherits += ({ inheritable[file] });
	else
	    add_error(cur, "'" + file + "' unknown.");
    }
    return 0;
}

mixed handle_gender(array args) {
    int gen;
    
    switch (args[0]) {
    case "male":
	gen = 1;
	break;
    case "female":
	gen = 2;
	break;
    case "neuter":
	gen = 0;
	break;
    }
    
    return ({ "setup", "set_gender(" + gen + ")" });
}

void handle_parsing() {
    cur = 0;
    inherits = ({});
    triggers = ({});
    
    for (int i = 0; i < sizeof(lines); i++) {
	int j;
	mixed arr = lines[i];
	function func;
	
	if (intp(arr[0])) {
	    cur = arr[0];
	    j = 1;
	} else 
	    j = 0;
	
	if (arr[j] == ":") {
	    if (func = attributes[arr[j+1]]) {
		cur++;
		lines[i] = evaluate(func, arr[j+2..]);
	    } else
		add_error(cur++, "Unknown attribute '" + arr[j+1] + "'.");
	} else if (arr[j] == "[") {
	    if (func = funcs[arr[j+1]]) {
		cur++;
		lines[i] = evaluate(func, arr[j+2], arr[j+3..]);
	    } else
		add_error(cur++, "Unknown function '" + arr[j+1] + "'.");
	} else if (arr[j] == "=") {
	    error("Functions not supported.\n");
	} else
	    error("Internal error.\n");
    }
}

private nomask void handle_generation(string outname) {
    string ret = implode(inherits, (: $1 + "inherit \"" + $2 + "\";\n" :), "");
    array tmp;
    string actions = "";
    
    if (sizeof(triggers)) {
	for (int i = 0; i < sizeof(triggers); i++)
	    actions += "case " + i + ":\n" + triggers[i][2] + "return;\n";
	
	ret += "array patterns = ({ ";
	for (int i = 0; i < sizeof(triggers); i++) {
	    if (i) ret += ", ";
	    ret += "\"" + triggers[i][0] + "\"";
	}
	ret += "});\n";
	ret += "array num = ({ ";
	for (int i = 0; i < sizeof(triggers); i++) {
	    if (i) ret += ", ";
	    ret += triggers[i][1];
	}
	ret += "});\n";
/*******************************/
	ret += @END
	
void receive_outside_msg(string str) {
string _1, _2, _3, _4, _5, _6;
if (str[<1] == '\n') str = str[0..<2];
for (int i = 0; i < sizeof(patterns); i++) {
if (sscanf(str, patterns[i], _1, _2, _3, _4, _5, _6) == num[i]) {
switch (i) {
END + actions + "}}}}";
/*******************************/
    }
    
    tmp = unique_array(lines - ({ 0 }), (: $1[0] :));
    foreach (array item in tmp) {
        switch (item[0][0]) {
        case "setup":
            ret += "\nvoid setup(string str) {\nfunction f;\n";
            break;
        default:
	    ret += "\nmixed " + item[0][0] + "() {\n";
            break;
        }
	foreach (array block in item)
	    ret += implode(block[1..], ";\n") + ";\n";
	ret += "}\n\n";
    }

    unguarded(1, (: write_file, outname, ret, 1 :));
}

object compile(string scrname) {
    mixed tmp = split_path(scrname);
    mixed path = tmp[0];
    mixed name = tmp[1][0..<5]; /* no .scr */
    mixed outname = path + "tmp_" + name + ".c";

    fname = scrname;
    lines = explode(read_file(scrname), "\n");
    errors = ({});
    handle_grouping();
    do_errors();
    handle_parsing();
    do_errors();
    handle_generation(outname);
    
    return load_object(outname);
}
