/* Do not remove the headers from this file! see /USAGE for more info. */

// Command for generating templates for verbs by Rust.  8-95
//  This file is part of the Lima mudlib distribution.  
//  Don't remove this header.  See /USAGE for more info.

#include <mudlib.h>

inherit CMD;

private nomask void set_plan(mixed nothing, string * plan)
{
    write("Your plan is set, man.\n");
    this_body()->set_plan(implode(plan,"\n"));
}

private void main()
{
#ifdef EVERYONE_HAS_A_PLAN

    write("Enter your plan, Stan.\n");
    new(EDIT_OB)->edit_text("", (: set_plan :));

#else
    write("Sorry, plans are not available.\n");
#endif
}
