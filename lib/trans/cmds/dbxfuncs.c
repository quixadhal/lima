/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek

#include <mudlib.h>

// These are in a include somewhere ...
#define TYPE_MOD_ARRAY          0x0020  /* Pointer to a basic type */
#define TYPE_MOD_CLASS          0x0040  /* a class */
#define TYPE_MOD_HIDDEN         0x0080  /* used by private vars */
#define TYPE_MOD_STATIC         0x0100  /* Static function or variable */
#define TYPE_MOD_NO_MASK        0x0200  /* The nomask => not redefineable */
#define TYPE_MOD_PRIVATE        0x0800  /* Can't be inherited */
#define TYPE_MOD_PROTECTED      0x1000
#define TYPE_MOD_PUBLIC         0x2000  /* Force inherit through private */
#define TYPE_MOD_VARARGS        0x4000  /* Used for type checking */

#define TYPE_MASK       15

#define TYPE_ANY        1       /* Will match any type */
#define TYPE_NOVALUE    2
#define TYPE_VOID       3
#define TYPE_NUMBER     4
#define TYPE_STRING     5
#define TYPE_OBJECT     6
#define TYPE_MAPPING    7
#define TYPE_FUNCTION   8
#define TYPE_REAL       9
#define TYPE_BUFFER     10

inherit DAEMON;

int main(string str) {
    string obname, func;
    string *funcs;
    mixed *result;
    string where;
    object ob;
    string line;

    if (!str || sscanf(str, "%s %s", obname, func) != 2) {
        write("dbxfuncs <ob> <funcs>\n");
        return 1;
    }
    
    obname = evaluate_path(obname);

    if (!(ob = find_object(obname))) {
        write("Can't find object " + obname + "\n");
        return 1;
    }

    funcs = filter(functions(ob, 1), function(mixed *result, string func) {
        return (sizeof(regexp(({ result[0] }), func)));
    }, func);
    
    if (!sizeof(funcs)) {
        write("No matches\n");
        return 1;
    }

    write("Matches:\n");
    foreach (result in funcs) {
        int type = result[2];
        line = "";

        if (type & TYPE_MOD_VARARGS) line += ("varargs ");
        if (type & TYPE_MOD_PUBLIC) line += ("public ");
        if (type & TYPE_MOD_NO_MASK) line += ("nomask ");
        if (type & TYPE_MOD_STATIC) line += ("static ");
        if (type & TYPE_MOD_CLASS) 
            line += ("class x ");
        else {
            switch (type & TYPE_MASK) {
            case TYPE_ANY: line += ("mixed "); break;
            case TYPE_VOID: line += ("void "); break;
            case TYPE_NUMBER: line += ("int "); break;
            case TYPE_STRING: line += ("string "); break;
            case TYPE_OBJECT: line += ("object "); break;
            case TYPE_MAPPING: line += ("mapping "); break;
            case TYPE_FUNCTION: line += ("function "); break;
            case TYPE_REAL: line += ("float "); break;
            case TYPE_BUFFER: line += ("buffer "); break;
            }
        }
        if (type & TYPE_MOD_ARRAY) line += ("*");
        line += (result[0]+"(");
        while (result[1]--) {
            line += ("x");
            if (result[1])
                line += (", ");
        }
        line += (")");

        if (where = function_exists(result[0], ob))
            printf("%-30s(defined in %s)\n", line, where);
        else
            printf("%s\n", line);
    }
    return 1;
}
