/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "transient_effect";

int alcohol_strength = 1;

//:FUNCTION set_alcohol_strength
//Sets the strength of the alcohol, determines how much healing
//and loss of stamina it provides.
void set_alcohol_strength(int i) { alcohol_strength = i; }
int query_alcohol_strength() { return alcohol_strength; }
void add_alcohol_strength(int i) { 
  if ((alcohol_strength+i)<=0) alcohol_strength = 0;
  alcohol_strength+=i;
}

void setup() {
  set_effect_duration(10);
  set_effect_type("drunk");
  set_effect_name("alcohol");
}

void do_effect(object ob) {
  int action = random(20);

  ob->stun_us(random(alcohol_strength));
  ob->heal_us(random(alcohol_strength));

  if (!random(2)) {
     switch (action) {
            case 0: ob->force_game_command("trip", 1); break;
            case 1: ob->force_game_command("stumble", 1); break;
            case 2: ob->simple_action("$N $vdrool."); break;
            case 3: ob->force_game_command("appear dizzy", 1); break;
            case 4: ob->force_game_command("burp", 1); break;
            case 5: ob->force_game_command("hiccup", 1); break;
            case 6: ob->force_game_command("puke", 1); break;
            default: break;
     }
  }
}

void another_effect_exists(object existing_effect) {
  // Add alcohol strength to existing alcohol effect, then...
  existing_drunkeness->add_alcohol_strength(alcohol_strength);
  // Remove us, as the existing effect will now carry on with our
  // strength added to it (duration is added by the transient_effect.c
  // inheritable).
  this_object()->remove();
}
