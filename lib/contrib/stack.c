/* Do not remove the headers from this file! see /USAGE for more info. */

class stack {
    int which;
    array elems;
};

class stack stack_new(int size) {
    class stack s = new(class stack);
    s->which = 0;
    s->elems = allocate(size);
    return s;
}

void stack_push(class stack s, mixed item) {
    if (s->which == sizeof(s->elems))
	error("Stack overflow.\n");
    s->elems[s->which++] = item;
}

mixed stack_pop(class stack s) {
    if (s->which == 0)
	error("Stack underflow.\n");
    return s->elems[--(s->which)];
}
