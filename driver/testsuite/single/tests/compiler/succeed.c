#pragma no_strict_types
foo() { }
bar(x) { }
#pragma strict_types
create() { }

void foo(int);

; // only a warning
