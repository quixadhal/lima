/* Do not remove the headers from this file! see /USAGE for more info. */

string short();
string query_subjective();

private mixed for_sale;
private mixed will_buy;

//:FUNCTION set_for_sale
//Set the array of object names which this living object is willing to sell.
//set_for_sale(1) means everything is for sale.  set_for_sale(0) means nothing
//is.  A function pointer is legal here.
void set_for_sale(mixed f) {
    for_sale = f;
}

//:FUNCTION set_will_buy
//Set the array of object names which this living object is willing to buy.
//set_will_buy(1) means it will buy anything.  set_will_buy(0) means it wont
//by anything.  A function pointer is legal here.
void set_will_buy(mixed f) {
    will_buy = f;
}

mixed indirect_buy_obj_from_liv(object ob, object liv) {
    mixed tmp = evaluate(for_sale);
    
    if (intp(tmp)) return tmp;
    if (!pointerp(tmp)) return 0;

    foreach (string name in tmp) {
	if (ob->id(name)) return 1;
    }
    return 0;
}

mixed indirect_sell_obj_to_liv(object ob, object liv) {
    mixed tmp = evaluate(will_buy);
    
    if (intp(tmp)) return tmp;
    if (!pointerp(tmp)) return 0;

    foreach (string name in tmp) {
	if (ob->id(name)) return 1;
    }
    return 0;
}

void buy_object(object ob) {
    if (ob->move(this_object()) != MOVE_OK)
	write("You can't seem to give " + ob->the_short() + " to " + short() + ".\n");
    else {
//:FIXME we need to pay them ...
        this_body()->targetted_action("$N $vsell a $o to $t.\n", this_object(), ob);

    }
}

void sell_object(object ob) {
    if (ob->move(this_body()) != MOVE_OK)
	write(capitalize(query_subjective()) + " can't seem to give " + ob->the_short() + " to you.\n");
    else {
//:FIXME we need to charge them ...
        this_body()->targetted_action("$N $vbuy a $o from $t.\n", this_object(), ob);
    }
}
