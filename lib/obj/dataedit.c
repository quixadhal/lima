/* Do not remove the headers from this file! see /USAGE for more info. */

#include <menu.h>
#include <security.h>

inherit MENUS;
inherit M_ACCESS;

/****** Begin private data and functions */
private:

MENU toplevel;
MENU_ITEM quit1, quit2, blank;

string fname;
mapping data;

string menu_title() {
    return "\nSave file editor: \"" + fname + "\"\nVariables: " + implode(keys(data), ", ") + "\n";
}

void update() {
    set_menu_title(toplevel, menu_title());
}

mapping load_data_file(string fn) {
    string text = read_file(fn);
    mapping result;
    string name, value;
    
    if (!text) return 0;
    result = ([]);
    foreach (string line in explode(text, "\n")) {
	if (line[0] == '#') continue;
	if (sscanf(line, "%s %s", name, value) != 2)
	    error("Syntax error in data file.\n");
	result[name] = restore_variable(value);
    }
    return result;
}

int save_data_file(string fname) {
    string result = "";
    
    foreach (string var, mixed value in data)
	result += var + " " + save_variable(value) + "\n";
    
    return write_file(fname, result, 1);
}

mixed expression(string str) {
    array parts = reg_assoc(str, ({ "\\$\\$", "\\$[a-zA-Z0-9_]+" }), ({ 1, 2 }));
    string expr = "mixed expression(mapping internal_data) {\nreturn ";
    mixed value;
    object ob;
    
    for (int i = 0; i < sizeof(parts[0]); i++) {
	switch (parts[1][i]) {
	case 0:
	    expr += parts[0][i];
	    break;
	case 1:
	    expr += "$";
	    break;
	case 2:
	    expr += "(internal_data[\"" + parts[0][i][1..] + "\"])";
	    break;
	}
    }
    expr += ";\n}\n";
    
    write_file("/tmp/dataedit.c", expr, 1);
    if (ob = find_object("/tmp/dataedit"))
	destruct(ob);
    ob = load_object("/tmp/dataedit");
    value = ob->expression(data);
    destruct(ob);
    rm("/tmp/dataedit.c");
    
    return value;
}

void quit_and_discard() {
    quit_menu_application();
}

void quit_and_save() {
    if (save_data_file(fname))
	quit_menu_application();
    else
	write("Failed.\n");
}

void remove_variable(string var) {
    if (undefinedp(data[var]))
	write("No such variable.\n");
    else {
	map_delete(data, var);
	write("Done.\n");
    }
    update();
}

void remove_var() {
    get_input_then_call( (: remove_variable :), "Variable: ");
}

void rename_variable(string oldv, string newv) {
    if (!undefinedp(data[newv]))
	write("A variable by that name already exists.\n");
    else {
	data[newv] = data[oldv];
	map_delete(data, oldv);
	write("Done.\n");
    }
    update();
}

void get_new_name(string old) {
    if (undefinedp(data[old]))
	write("No such variable.\n");
    else {
	get_input_then_call( (: rename_variable, old :), "New name: ");
    }
}

void rename_var() {
    get_input_then_call( (: get_new_name :), "Old name: ");
}

void set_variable(string vname, string valstr) {
    mixed value = expression(valstr);
    data[vname] = value;
    write("Done.\n");
}

void get_value(string vname) {
    get_input_then_call( (: set_variable, vname :), "Value: ");
}

void set_var() {
    get_input_then_call( (: get_value :), "Variable: ");
}

void print_variable(string expr) {
    printf("Value is: %O\n", expression(expr));
}

void print() {
    get_input_then_call( (: print_variable :), "Expression: ");
}

void merge_file(string fname) {
    mapping data2;
    
    if (!(data2 = load_data_file(fname))) {
	write("No such file.\n");
	return;
    }
    
    foreach (mixed key, mixed value in data2) {
	if (!data[key])
	    data[key] = value;
	else {
	    if (typeof(data[key]) != typeof(value))
		write("Type mismatch in merging variable '" + key + "': " +
		      typeof(data[key]) + " vs " + typeof(value) + "\n");
	    switch (typeof(value)) {
	    case "mapping":
	    case "array":
		data[key] += value;
		break;
	    default:
		if (value == data[key])
		    continue;
		write("While merging variable '" + key + "': Cannot merge objects of type " + typeof(value) + "\n");
	    }
	}
    }
    update();
}

void merge() {
    get_input_then_call( (: merge_file :), "File to merge with: ");
}

void save_to_file(string file) {
    if (save_data_file(file)) {
	fname = file;
	update();
    } else write("Failed.\n");
}

void save_to() {
    get_input_then_call( (: save_to_file :), "New filename: ");
}

/****** End of private functions */
public:

void start_menu(string file) {
    if (!data = load_data_file(fname = file)) {
	write(file + ": No such file.\n");
	destruct(this_object());
	return;
    }
    
    toplevel = new_menu(menu_title());
    blank = new_seperator("");
#define NME(X) add_menu_item(toplevel, new_menu_item X)
    
    add_menu_item(toplevel, blank);
    NME(("Delete Variable", (: remove_var :), "d"));
    NME(("Rename Variable", (: rename_var :), "r"));
    NME(("Set Variable", (: set_var :), "="));
    NME(("Print Expression", (: print :), "p"));
    NME(("Merge Datafile", (: merge :), "m"));
    NME(("Save as ...", (: save_to :), "s"));
    NME(("Quit and Save", (: quit_and_save :), "q"));
    NME(("Quit w/o Saving", (: quit_and_discard :), "Q"));
    add_menu_item(toplevel, blank);

    set_menu_prompt(toplevel, "[dr=pmsqQ] ");
    allow_empty_selection(toplevel);

    init_menu_application(toplevel);
}

void create() {
    set_privilege(1);
}

