reset(arg)
{
    if (!arg)
	set_light(1);
    extra_reset();
}

extra_reset()
{
    object orc, weapon;
    int n,i,class,value,weight;
    string w_name,alt_name;

    i = 0;
    if (!present("orc")) {
	while(i<8) {
	    i += 1;
	    orc = clone_object("obj/monster");
	    call_other(orc, "set_name", "orc");
	    call_other(orc, "set_alias", "dirty crap");
	    call_other(orc, "set_race", "orc");
	    call_other(orc, "set_level", random(2) + 1);
	    call_other(orc, "set_hp", 30);
	    call_other(orc, "set_ep", 1014);
	    call_other(orc, "set_al", -60);
	    call_other(orc, "set_short", "An orc");
	    call_other(orc, "set_ac", 0);
	    call_other(orc, "set_aggressive", 1);
	    call_other(orc, "load_a_chat", 40,
		       call_other("room/orc_vall", "get_chats"));
	    n = random(3);
	    weapon = clone_object("obj/weapon");
	    if (n == 0) {
		w_name = "knife";
		class = 5;
		value = 8;
		weight = 1;
		alt_name = "knife";  /* JnA: 901127 axes != knives */
	    }
	    if (n == 1) {
		w_name = "curved knife";
		class = 7;
		value = 15;
		weight = 1;
		alt_name = "knife";
	    }
	    if (n == 2) {
		w_name = "hand axe";
		class = 9;
		value = 25;
		weight = 2;
		alt_name = "axe";
	    }
	    call_other(weapon, "set_name", w_name);
	    call_other(weapon, "set_class", class);
	    call_other(weapon, "set_value", value);
	    call_other(weapon, "set_weight", weight);
	    call_other(weapon, "set_alt_name", alt_name);
	    transfer(weapon, orc);
	    command("wield " + w_name, orc);
	    move_object(orc, this_object());
	}
    }
}
init()
{
    add_action("south", "south");
    add_action("north", "north");
}

north()
{
    if (present("orc")) {
	write("An orc bars your way.\n");
	return 1;
    }
    call_other(this_player(), "move_player", "north#room/orc_treasure");
    return 1;
}

south()
{
    call_other(this_player(),"move_player", "south#room/orc_vall");
    return 1;
}

long()
{
    write("This is the local strong point of the orcs.\n");
    write("There is an entrance to a small room to the north.\n");
}

short() {
    return "The orc fortress";
}
