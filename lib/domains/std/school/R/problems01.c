#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Problems - parse errors");
    set_long(@LORIEL

These arise when there is a syntax error in the code,
and that prevents the code loading.

Usually caused by omitting or mistyping punctuation, eg " or ;.

They are reported, with a line number.
eg 'line 4 : parse error before )

Also they are logged, in the error-log of the area where you are working.

Sometimes the error is actually created on an earlier line,
but not discovered until this line.

LORIEL
    );
    set_exits( ([
	"corridor" : R "corridor09",
	"forward" : R "problems02",
     ]) );
    set_objects( ([
      ]) );
}
