void create() {
#ifndef __NO_ADD_ACTION__
    enable_commands();
#endif
}

void catch_tell(string str) {
}

void send(string str, mixed x) {
    if (x)
	say(str, x);
    else
	say(str);
}

void move(object ob) {
    move_object(ob);
}

