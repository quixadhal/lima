/* Do not remove the headers from this file! see /USAGE for more info. */

int query_ghost();

private nosave int stunned; // time to awake from stunned status
private int asleep;         // flag for "asleep" status
private nosave int chance;  // counter for attempted "wake up"
private nosave int attack_speed = 1;
private nosave object * readied = ({}); // array of ready ammunition

void add_readied(object ob)
{
  readied -= ({ob});
  readied += ({ob});
  readied -= ({0});
}

void remove_readied(object ob){ readied -= ({ ob }); }

object * query_readied(){ return copy(readied); }

void set_attack_speed(int speed)
{
  if(speed < 1)
    return;

  attack_speed = speed > MAX_ATTACK_SPEED ? MAX_ATTACK_SPEED : speed;
}

int query_attack_speed(){ return attack_speed; }

int query_stunned()
{
  if(stunned > time())
    return 1;
  return 0;
}

int query_asleep(){ return asleep;}

void stun(int period){ stunned = time() + 5 + period; }

void wake_up()
{
  if(asleep)
  {
    asleep = 0;
    stun(0);
  }
}

void knock_out()
{
  asleep = 1;
  chance = 0;
}

mixed check_condition(int urgent)
{
  if(query_ghost())
  {
    stunned = 0;
    asleep = 0;
    return 0;
  }

  if(urgent && stunned > time())
    return "You $vare still recovering from that last blow, ";

  if(asleep)
  {
    if(urgent)
    {
      if(random(5) <= chance++)
      {
        wake_up();
      } else {
        return "You try desperately to wake up, but fail.\n";
      }
    } else {
      return "But you are asleep!\n";
    }
  }

  return 0;
}
