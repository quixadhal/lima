/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit LIVING;
inherit M_ACTIONS;
inherit M_TRIGGERS;

int starts_with(string str, string pref) {
    return str[0..strlen(pref)-1] == pref;
}

int ends_with(string str, string suff) {
    return str[<strlen(suff)..] == suff;
}

void setup() {
    int *handle;
    
    set_name("Logical Genius");
    set_gender(1);
    set_proper_name("Logical Genius");
    set_in_room_desc("A Logical Genius eyes you suspiciously.");
    add_id("dude");
    add_id("genius");
    set_long("Logical Genius peers at you.\nLogical Genius says: Tell me something about your name.");

    add_pattern("%s says: %s.", function(string name, mixed sub) {
        if (functionp(sub)) {
	    if (evaluate(sub, lower_case(name)))
		return "say That's true.";
	    else
		return "say You're lying.";
	}
	if (sub && intp(sub))
	    return "say I can't figure out what you mean.";
	if (random(5))
	    return 0;
	return "say That's boring.  I want to know things about your name.";
    }, 0, "say_rule");
    
    add_sub_pattern("say_rule", "%s name %s", (: $2 :), 0, "predicate");
     
    add_sub_pattern("predicate", "%s or %s", function(function f1, function f2) {
	return (: evaluate($(f1), $1) || evaluate($(f2), $1) :);
    }, "and_pred", "predicate");
    add_sub_pattern("predicate", "%s", (: $1 :), "and_pred");

    add_sub_pattern("and_pred", "%s and %s", function(function f1, function f2) {
	return (: evaluate($(f1), $1) && evaluate($(f2), $1) :);
    }, "simple_pred", "and_pred");
    add_sub_pattern("and_pred", "%s", (: $1 :), "simple_pred");
    
    add_sub_pattern("simple_pred", "is not %s", function(string name) {
	return (: $1 != $(name) :);
    } );
    add_sub_pattern("simple_pred", "is %s", function(string name) {
	return (: $1 == $(name) :);
    } );
    add_sub_pattern("simple_pred", "starts with %s", function(string prefix) {
	return (: starts_with($1, $(prefix)) :);
    } );
    add_sub_pattern("simple_pred", "ends with %s", function(string suffix) {
	return (: ends_with($1, $(suffix)) :);
    } );
    add_sub_pattern("simple_pred", 0, 1);
}
