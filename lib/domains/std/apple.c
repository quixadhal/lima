/* Do not remove the headers from this file! see /USAGE for more info. */

inherit FOOD;


void setup()
{
    set_id("apple");
    set_adj("red");
    set_long("It's an ordinary red apple.");

    set_num_eats(5);
    set_eat_action("$N $vtake a bite from an apple.");
    set_last_eat_action("$N $veat the last of $p apple.");
}
