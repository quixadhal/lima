/* Do not remove the headers from this file! see /USAGE for more info. */

private string upper;

void create() {
    string ascii = "";
    for(int i=0; i < 256; i++) {
	if (i)
	    ascii += chr(i);
	else
	    ascii += " ";
    }
    
    upper = ascii[0..('a'-1)] + "ABCDEFGHIJKLMNOPQRSTUVWXYZ" + ascii[('z'+1)..];
}
     
string handle_command(string part) {
    string ret;
    
    if (part == 0) {
	string array commands = get_dir("/help/autodoc/command/");

	return @END
<title>Automatic Command Documentation</title>
<H1>Automatic Command Documentation</H1>
<HR>
<UL>
END
+ implode(map(commands, (: "<LI> <A HREF=\"autodoc.c?=command-" + $1 + "\"> " + $1 + " </A>" :)), "\n") + "</UL>\n";
    }

    ret = "<title>Automatic Documentation for: " + part + "</title>\n";
    ret += "<H1>Automatic Documentation for: " + part + "</H1>\n";
    ret += "<HR>\n" + read_file("/help/autodoc/command/" + part);
    
    return ret;
}

string handle_function(string part) {
    string ret, func, tmp;
    int i;
    
    if (part == 0) {
	string array functions = get_dir("/help/autodoc/functions/");

	return @END
<title>Automatic Function Documentation</title>
<H1>Automatic Function Documentation</H1>
<HR>
<UL>
END
+ implode(map(functions, (: "<LI> <A HREF=\"autodoc.c?=functions-" + $1 + "\"> " + $1 + " </A>" :)), "\n") + "</UL>\n";
    }
    sscanf(part, "%s-%s", part, func);
    if (func == 0) {
	string array functions = get_dir("/help/autodoc/functions/" + part + "/");

	return @END
<title>Automatic Function Documentation</title>
<H1>Automatic Function Documentation</H1>
<HR>
<UL>
END
+ implode(map(functions, (: "<LI> <A HREF=\"autodoc.c?=functions-" +$(part) + "-" + $1 + "\"> " + $1 + " </A>" :)), "\n") + "</UL>\n";
    }
    ret = "<title>Automatic Documentation for: " + func + "</title>\n";
    ret += "<H1>Automatic Documentation for: " + func + "</H1>\n";

    tmp = read_file("/help/autodoc/functions/" + part + "/" + func);
    
    i = member_array('\n', tmp);
    
    ret += "<HR>\n<H3>" + tmp[0..i] + "</H3> <p>" + tmp[i..];
    
    return ret;
}

string handle_hook(string part) {
    string ret, tmp;
    int i;
    
    if (part == 0) {
	string array hooks = get_dir("/help/autodoc/hook/");

	return @END
<title>Automatic Hook Documentation</title>
<H1>Automatic Hook Documentation</H1>
<HR>
<UL>
END
+ implode(map(hooks, (: "<LI> <A HREF=\"autodoc.c?=hook-" + $1 + "\"> " + $1 + " </A>" :)), "\n") + "</UL>\n";
    }

    ret = "<title>Automatic Documentation for: " + part + "</title>\n";
    ret += "<H1>Automatic Documentation for: " + part + "</H1>\n";

    tmp = read_file("/help/autodoc/hook/" + part);
    i = member_array('\n', tmp);
    
    ret += "<HR>\n<H3>" + tmp[0..i] + "</H3> <p>" + tmp[i..];
    
    return ret;
}

string handle_module(string part) {
    string ret;
    
    if (part == 0) {
	string array modules = get_dir("/help/autodoc/modules/");

	return @END
<title>Automatic Module Documentation</title>
<H1>Automatic Module Documentation</H1>
<HR>
<UL>
END
+ implode(map(modules, (: "<LI> <A HREF=\"autodoc.c?=modules-" + $1 + "\"> " + $1 + " </A>" :)), "\n") + "</UL>\n";
    }

    ret = "<title>Automatic Documentation for: " + part + "</title>\n";
    ret += "<H1>Automatic Documentation for: " + part + "</H1>\n";
    ret += "<HR>\n" + read_file("/help/autodoc/modules/" + part);
    
    return ret;
}

string module_summary_line(string arg) {
    string cannonical_name;

    arg = arg[0..<3];
    cannonical_name = "M_" + map(arg, (: upper[$1] :));
    
    if (file_size("/help/autodoc/functions/" + arg) == -2) {
	return "<LI> <A HREF=\"autodoc.c?=functions-" + arg + "\"> " + 
	    cannonical_name + " </A>";
    } else {
	return "<LI> " + cannonical_name;
    }
}

string last_part(string str) {
    int i;
    
    if (str[<2..] == ".c") str = str[0..<3];
    
    i = strsrch(str, "/", -1);
    return str[i+1..];
}

string tree_form(string part) {
    string line;
    
    object ob = load_object(part);
    if (part[0] != '/')
	part = "/" + part;

    if (file_size("/help/autodoc/functions/" + last_part(part)) == -2) {
	line = "<A HREF=\"autodoc.c?=functions-" + last_part(part) + "\"> " + part + " </A>";
    } else {
	line = part;
    }
    
    return "<LI> " + line + "<UL>\n" + implode(map(inherit_list(ob), (: tree_form :)), "\n") + "</UL>\n";
}    
    
varargs string handle_tree(string part) {
    return "<title>Inheritance Tree of /std/" + part + "</title>\n"
	+ "<h1>Inheritance Tree of /std/" + part + "</h1>\n<HR> <UL>\n"
	    + tree_form("/std/" + part) + "</UL>\n";
}

string object_summary_line(string arg) {
    string cannonical_name;
    
    arg = arg[0..<3];
    cannonical_name = map(arg, (: upper[$1] :));

    return "<LI> <A HREF=\"autodoc.c?=tree-" + arg + "\"> " + 
	cannonical_name + " </A>";
}

string handle_overview(string part) {
    array tmp;
    
    string ret = @END
<title>Mudlib Overview</title>
<h1>Mudlib Overview</h1>
<hr>

<h3>Base Objects</h3>
<UL>
END;
    tmp = get_dir("/std/");
    tmp = filter(tmp, (: $1[<2..] == ".c" :));
    tmp = map(tmp, (: object_summary_line :));
    ret += implode(tmp, "\n") + "</UL>\n";

    ret += "<h3>Modules</h3>\n<UL>\n";
    
    tmp = get_dir("/std/modules/");
    tmp = filter(tmp, (: $1[<2..] == ".c" :));
    tmp = map(tmp, (: module_summary_line :));
    ret += implode(tmp, "\n") + "</UL>\n";
    
    return ret;
}

string main(string arg) {
    string part;

    if (arg)
	sscanf(arg, "%s-%s", arg, part);

    switch (arg) {
    case 0:
	return @END
<title>Automatic Documentation Interface</title>
<H1>Automatic Documentation Interface</H1>
<HR>
<UL>
<LI> <A HREF="autodoc.c?=command"> Commands </A>
<LI> <A HREF="autodoc.c?=functions"> Functions </A>
<LI> <A HREF="autodoc.c?=hook"> Hooks </A>
<LI> <A HREF="autodoc.c?=modules"> Modules </A>
</UL>

<A HREF="autodoc.c?=overview"> Mudlib Overview </A>
END;
    case "command":
	return handle_command(part);
    case "functions":
	return handle_function(part);
    case "hook":
	return handle_hook(part);
    case "modules":
	return handle_module(part);
    case "overview":
	return handle_overview(part);
    case "tree":
	return handle_tree(part);
    }
}
