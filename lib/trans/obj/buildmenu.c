/* Do not remove the headers from this file! see /USAGE for more info. */

// Onyx @ Red Dragon Nov. 1995.
// Roommaker for Lima muds.
#include <menu.h>
#include <mudlib.h>
#include <playerflags.h>
#include <commands.h>
#include <security.h>   /* ### for now */
#include <edit.h>

inherit MENUS;
inherit M_ACCESS;       /* ### for now */


private string lng = "";
private string fpath;
private string fname = "RoomMaker.c";
private string  shrt = "RoomMaker Room";
private int lite = 1;
private int wthr =1;
private mapping xits = ([]);
private mapping tems = ([]);
private mapping bjects = ([]);
private string *clude = ({});

#define RM_VER "v0.9"

MENU toplevel;

// submenus of the toplevel (build) menu
MENU roommenu;

// sub menus of the roommenu
MENU itemmenu;
MENU objectmenu;
MENU exitmenu;
MENU filemenu;
MENU descmenu;


MENU_ITEM N_QUIT;
MENU_ITEM goto_main_menu_item;
MENU_ITEM goto_room_menu_item;
MENU_ITEM main_seperator;

// Right now, I'm too lazy to code in a help command, and this menu
// should be pretty self-explanatory anyway.

void get_include(string str){
    if ((str[0] != '<') && (str[0] != '"')) str = "<"+str+">";
    if (member_array(str,clude) != -1) return;
    write (str + "\n"); 

    clude +=({str});
}

void prompt_get_include(){
    get_input_then_call((:get_include:),"What file would you like to include? ");
}

void set_fname(string str){
    fname = str;
    write("File name changed to "+fname+"\n\n");
}

void prompt_set_fname(){
    get_input_then_call((: set_fname :),"Change file name to what?  ");
}

void set_fpath(string str){
    fpath = str;
    if (fpath[strlen(fpath)-1] != '/')
	fpath+="/";
    write("File path changed to "+fpath+"\n\n");
}

void prompt_set_fpath(){
    get_input_then_call((: set_fpath :),"Change path name to what?  ");
}


void change_light(string str){
    sscanf(str,"%d",lite);
    write("New light level is "+lite+".\n\n");
}

void prompt_change_light(){
    get_input_then_call((: change_light :),"Change light level to what?  ");
}
void change_weather(string str){
    sscanf(str,"%d",wthr);
    write("New weather is "+wthr+".\n\n");
}

void prompt_change_weather(){
    write("Change weather to what?\n");
    get_input_then_call((: change_weather :),"(0 for indoors, positive number for outdoors)  ");
}

void change_short(string str){
    shrt = str;
    write("Short desc changed to "+shrt+"\n\n");
}

void prompt_change_short(){
    get_input_then_call((: change_short :),"Change short desc to what?  ");
}

void make_long(string *long){
    write("Long description has been set .\n");
    lng = implode(long, "\n");
}

void change_long(){
    write("** Enter long description now **\n");
    new(EDIT_OB, EDIT_TEXT, 0, (: make_long :));
}

void idesc(string key, string *item){
    write("Item description has been set .\n");
    tems[key] = implode(item, "\n");
    write("Item "+key+" set to "+tems[key]+"\n\n");
}

void change_idesc(string key){
    write("** Enter item description now **\n");
    new(EDIT_OB, EDIT_TEXT, tems[key], (: idesc, key :));
}

int is_in_ikey(string check, string key){
    mixed key2;
    string *expl;   

    expl = explode(key, ", ");
    foreach(key2 in expl)
    if (key2 == check) return -1;
    return 0;
}

void new_ikey(string str){
    tems+=([str : ""]);
    change_idesc(str); 
} 

void prompt_new_ikey(){
    get_input_then_call((:new_ikey:),"What would you like the new key to be? ");
}

void list_keys(){
    mixed key,  value;

    write ("Items:\n");
    foreach(key, value in tems){
	write("Items: "+key+"\n");
	write(tems[key]+"\n");
    }
}

void new_idesc(string str){
    change_idesc(str);
    write("Item desc changed to "+tems[str]+"\n\n");
}

void prompt_new_idesc(){
    get_input_then_call((: new_idesc :),"Change which keys desc?  ");
}

void desc_summary(){
    string name;

    write("File Name : "+fpath+fname+"\n");
    write("Light     : "+lite+"\n");
    write("Weather   : "+wthr+"\n");
    write("Short Desc: "+shrt+"\n");
    write("Long Desc :\n"+lng+"\n\n");
    write("Include Files :\n");
    foreach(name in clude)
    write("    "+name+"\n");
}

void del_exit(string str){
    map_delete(xits,str);
    write("Exit "+str+" deleted.\n");
}

void prompt_del_exit(){
    get_input_then_call((:del_exit:),"What is the direction of the exit you would like to delete? ");
}

void xdesc(string key, string *item){
    write("Exit has been set .\n");
    xits[key] = implode(item, "\n");
    write("Item "+key+" set to "+xits[key]+"\n\n");
}

void change_exit(string key){
    write("** Edit path to room now **\n");
    new(EDIT_OB, EDIT_TEXT, xits[key], (: xdesc, key :));
} 

void prompt_change_exit(){
    get_input_then_call((:change_exit:), "Which exit should be changed? ");
}

void new_exit(string str){ 
    xits += ([str:""]);
    change_exit(str); 
    write ("Added exit "+xits[str]+"\n");
}

void prompt_new_exit(){
    get_input_then_call((:new_exit:), "Which direction should the new exit be? ");
}


void del_obj(string str){
    map_delete(bjects,str);
    write("Object "+str+" deleted.\n");
}

void prompt_del_obj(){
    get_input_then_call((:del_obj:),"What is the filename of the object you would like to delete? ");
}

void odesc(string key, string num){
    int i;
    sscanf(num, "%d", i); 
    bjects[key] = i;
}

void change_obj(string key){
    get_input_then_call((:odesc, key:), "How many times should "+key+" appear?");
}

void prompt_change_obj(){
    get_input_then_call((:change_obj:), "Which object should be changed? (please enter the object's file name) ");
}

void new_obj(string str){ 
    bjects += ([str:1]);
    change_obj(str); 
}

void prompt_new_obj(){
    get_input_then_call((:new_obj:), "What is the path to the new object's file? ");
}
void view_objects(){
    mixed key,value;
    write("Objects:\n");
    foreach(key,value in bjects)
    write(key + " : " + value + "\n");
}


void view_exits(){
    mixed key,value;
    write("Exits:\n");
    foreach(key,value in xits)
    write(key + " : " + value + "\n");
}

string write_mapping(mapping mmap){
    string retval = "";  
    mixed *keylist;
    mixed key;

    if (!sizeof(mmap)) return "";
    if (sizeof(mmap)==1)  return (keys(mmap)[0] + " : " + mmap[keys(mmap)[0]] + "\n"); 
    keylist = keys(mmap)[0..(sizeof(mmap)-2)];
    foreach(key in keylist)
    retval +=key+" : "+ mmap[key] + ",\n";
    retval += keys(mmap)[sizeof(mmap)-1] + " : " + mmap[keys(mmap)[sizeof(mmap)-1]] + "\n";
    return retval;
}

void save_this_room(){
    string name;
    string afname;

    string header = "";
    afname = fpath+fname;

    header += "/* " +fname+"\n";
    header += "** Coded by "+this_body()->query_name()+"@"+mud_name()+"\n";
    header += "** Code generated with RoomMaker "+RM_VER+" coded by Onyx@RedDragon\n";
    header += "** Do not remove this header.\n";
    header += "*/\n\n";
    foreach (name in clude)
    header +="#include "+name+"\n";
    header +="\ninherit ROOM;\n\n";
    header +="void create(){\n";
    header +="    room::create();\n";
    header +="    set_light("+lite+");\n";
    header += "    set_brief(\"" + shrt + "\");\n";
    header +="    set_weather("+wthr+");\n";
    header +="    set_long(\n";
    header +="\"" + lng + "\"";
    header +="\n);\n";
    header +="    set_items( ([\n";
    header += write_mapping(tems);
    header +="    ]) );\n"; 
    header +="    set_exits( ([\n";
    header += write_mapping(xits);
    header +="    ]) );\n";
    header +="    set_objects( ([\n";
    header += write_mapping(bjects);
    header +="    ]) );\n";
    header +="}";

    if (write_file(afname,header,1)){
	printf("Error in writing to file %s (in header)\n", afname);
	write("File not saved.\n");
	return;
    }
    write("File saved to "+afname+"\n");
}

void view_room(){
    desc_summary();
    list_keys();
    view_objects(); 
    view_exits();
}

void create() {
    set_privilege(1);
    toplevel = new_menu(mud_name()+" Build Menu");
    roommenu = new_menu("RoomMaker "+RM_VER);
    itemmenu = new_menu("Item Menu");
    exitmenu = new_menu("Exit Menu");
    objectmenu = new_menu("Object Menu"); 
    filemenu = new_menu("File Menu");
    descmenu = new_menu("Description Menu");  

    N_QUIT = new_menu_item("Quit",(:quit_menu_application:),"q");
    goto_main_menu_item = new_menu_item("Return to main menu",toplevel,"m");
    goto_room_menu_item = new_menu_item("Return to room menu",roommenu, "m");
    main_seperator =  new_seperator("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

    // Add items to the toplevel (build) menu
    add_menu_item(toplevel, main_seperator);
    add_menu_item(toplevel, new_menu_item("Build a Room", roommenu, "r"));
    add_menu_item(toplevel, N_QUIT);

    // Add menu items to the roommenu item
    add_menu_item(roommenu, main_seperator);
    add_menu_item(roommenu, new_menu_item("Edit descriptions",
	descmenu, "d"));
    add_menu_item(roommenu, new_menu_item("Edit items", 
	itemmenu, "i"));
    add_menu_item(roommenu, new_menu_item("Edit objects",
	objectmenu, "o"));
    add_menu_item(roommenu, new_menu_item("Edit exits",
	exitmenu, "x"));
    add_menu_item(roommenu, new_menu_item("Edit file info",
	filemenu, "f"));
    add_menu_item(roommenu, new_menu_item("View room",
	(:view_room:), "v"));
    add_menu_item(roommenu, N_QUIT);
    add_menu_item(roommenu,goto_main_menu_item);



    // Add menu items to the itemmenu item

    add_menu_item(itemmenu,main_seperator);
    add_menu_item(itemmenu, new_menu_item("Add a new item",
	(:prompt_new_ikey:), "a"));
    add_menu_item(itemmenu, new_menu_item("View current items",
	(:list_keys:), "v"));
    add_menu_item(itemmenu, new_menu_item("Edit current descs",
	(:prompt_new_idesc:), "e"));
    add_menu_item(itemmenu,N_QUIT);
    add_menu_item(itemmenu,goto_room_menu_item);

    // Add menu items to object menu
    add_menu_item(objectmenu,main_seperator);
    add_menu_item(objectmenu, new_menu_item("Add new object",
	(:prompt_new_obj:), "a"));
    add_menu_item(objectmenu, new_menu_item("Edit current object",
	(:prompt_change_obj:), "e"));
    add_menu_item(objectmenu, new_menu_item("Remove object",
	(:prompt_del_obj:), "r"));

    add_menu_item(objectmenu, new_menu_item("View objects",
	(:view_objects:), "v"));
    add_menu_item(objectmenu,N_QUIT);
    add_menu_item(objectmenu,goto_room_menu_item);

    // Add menu items to exitmenu;
    add_menu_item(exitmenu,main_seperator);
    add_menu_item(exitmenu, new_menu_item("Add new exit",
	(:prompt_new_exit:), "a"));
    add_menu_item(exitmenu, new_menu_item("Edit current exits",
	(:prompt_change_exit:), "e"));
    add_menu_item(exitmenu, new_menu_item("Remove exit",
	(:prompt_del_exit:), "r"));

    add_menu_item(exitmenu, new_menu_item("List exits",
	(:view_exits:), "v"));
    add_menu_item(exitmenu,N_QUIT);
    add_menu_item(exitmenu,goto_room_menu_item);


    // Add items to filemenu
    add_menu_item(filemenu,main_seperator); 
    add_menu_item(filemenu, new_menu_item("Change file name",
	(:prompt_set_fname:), "n"));
    add_menu_item(filemenu, new_menu_item("Change file path",
	(:prompt_set_fpath:), "p"));
    add_menu_item(filemenu, new_menu_item("Write room to file",
	(:save_this_room:), "f"));
    add_menu_item(filemenu,N_QUIT);
    add_menu_item(filemenu,goto_room_menu_item);

    /// Add items to descmenu

    add_menu_item(descmenu,main_seperator); 
    add_menu_item(descmenu, new_menu_item("Change light level",
	(:prompt_change_light:), "h"));
    add_menu_item(descmenu, new_menu_item("Change weather",
	(:prompt_change_weather:), "w"));
    add_menu_item(descmenu, new_menu_item("Change short description",
	(:prompt_change_short:), "s"));
    add_menu_item(descmenu, new_menu_item("Change long description",
	(:change_long:), "l"));
    add_menu_item(descmenu, new_menu_item("Add an include file",
	(:prompt_get_include:), "a"));
    add_menu_item(descmenu, new_menu_item("View description summary",
	(:desc_summary:),"v"));

    add_menu_item(descmenu,N_QUIT);
    add_menu_item(descmenu,goto_room_menu_item);
    fpath = wiz_dir(this_body()) + "/";
}

void start_menu() {
    init_menu_application(toplevel);
}

