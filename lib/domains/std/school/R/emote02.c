#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 2");
  set_long(@MILAMBER
Now for the third part.  This is where it gets fun.  The message for
the rule.  Always enter the message in first person.  Like the
message for smile for the basic rule, "LIV", is "$N $vsmile at $t."
Now let me explain some of the tags.  The dollar sign ($) is what
signals a tag.  $n is the name of the typer.  If you capitalize it,
"$N" then it will cap the name.  $v is to signal that the following
word is a verb.  This is so an "s" will be added or "is" will be changed
to "are" for other people in the room to view.  ALWAYS put this in front
of every verb.  $t is the target.  The target is either the LIV or the
OBJ in the rule.  If the target is a proper noun, such as a name, it
will be automatically capitalized; therefore, there is no need to use
a $T because a $t will suffice.  The only time you really need to use a
$<cap letter> is at the start of a sentence.

Now for multiple targets.  Let us go back to our smile example.  One of
the smile rules is "LIV LIV" so if I was to type: "smile may lancelot",
it is this rule that would be used.  The message for this rule is:
"$N $vsmile at $t1 and $t2." See how you can call target 1 and target 2?
MILAMBER
  );
  set_exits( ([
    "north" : R + "emote03",
    "south" : R + "emote01",
   ]) );
  set_objects( ([
   ]) );
}
