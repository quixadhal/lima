/* Do not remove the headers from this file! see /USAGE for more info. */

//treefor
//By Beek - derived from codefor, which was derived from eval

#include <mudlib.h>
inherit CMD;

#define SYNTAX "USAGE:  treefor <lpc expression>\n"

private void ind(int indent) {
    printf("%*-' 's", indent, "");
}

private int dump(mixed *value, int indent, int flag) {
    mixed val;
    int subindent;

    if (!value)
	return flag;
    
    switch (value[0]) {
    case "two values":
	if (flag) {
	    printf("\n");
	    subindent = indent + 4;
	} else
	    subindent = indent;
	
	if (dump(value[1], subindent, 0))
	    printf("\n");

	if (dump(value[2], subindent, 0));
	    printf("\n");
	
	return 0;
    case "efun":
	if (!flag)
	    ind(indent);
	printf("(efun %O (", value[1]);
	flag = 1;

	foreach (val in value[2]) {
	    if (arrayp(val)) {
		if (flag)
		    printf(" ");
		flag = dump(val, indent + 2, flag);
	    } else {
		if (!flag)
		    ind(indent);
		printf(" %O", val);
		flag = 1;
	    }
	}

	if (!flag)
	    ind(indent);
	printf(" )");
	flag = 1;

	foreach (val in value[3..]) {
	    if (arrayp(val)) {
		if (flag)
		    printf(" ");
		flag = dump(val, indent + 2, flag);
	    } else {
		if (!flag)
		    ind(indent);
		printf(" %O", val);
		flag = 1;
	    }
	}
	if (!flag)
	    ind(indent);
	printf(")");
	return 1;
    default:
	if (!flag)
	    ind(indent);
	printf("(%O", value[0]);
	flag = 1;

	foreach (val in value[1..]) {
	    if (arrayp(val)) {
		if (flag)
		    printf(" ");
		flag = dump(val, indent + 2, flag);
	    } else {
		if (!flag)
		    ind(indent);
		printf(" %O", val);
		flag = 1;
	    }
	}
	if (!flag)
	    ind(indent);
	printf(")");
	return 1;
    }
}

void main(string str)
{
    string tmp_file;
    string error;
    string initial_write_to_file;
    mixed ret;
    object o;

    initial_write_to_file = 
        "#include <mudlib.h> \n"
        "#include <daemons.h>\n"
        "#include <config.h>\n"
        "mixed eval_function(){\n return __TREE__{";
    tmp_file = wiz_dir(this_user()) + "/EVAL.c";
    rm(tmp_file);
    if(o = find_object(tmp_file)) destruct(o);
    if(!write_file(tmp_file,initial_write_to_file)) {
        write("eval: Unable to write to "+tmp_file+". \n");  //shouldn't happen.
            return 0;
    }
    write_file(tmp_file,str+"; };\n");
    write_file(tmp_file,"}\n");
    
    if (ret = find_object(tmp_file)) destruct(ret);
    ret = tmp_file->eval_function();
    rm( tmp_file );

    destruct(find_object(tmp_file));
    dump(ret, 0, 0);
    printf("\n");
    return;
}
