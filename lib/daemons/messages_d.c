/* Do not remove the headers from this file! see /USAGE for more info. */

// This really be dynamic, with managing commands, but for now ...

//###TODO: make an interface to this crap.  Put it in a file.
//###combat-torch also needs serious help.

mapping messages = 
([
  "combat-unarmed" :
  ([
    "miss" :
    ({ "$N $vswing a fist at $t, but $vmiss wildly.",
       "$N $vjab a fist at $t, but just $vmiss." }),
    "none" : "$N $vpunch $t, but $n1 $v1shrug it off.",
    "disarm" : "$N $vgrab $p1 arm, and $vslam it down on $p knee, causing $n1 to drop $p1 $o1.",
    "knockdown" : "$N $vstep behind $t, and trips $t, sending $t flying.",
    "knockout" : "$N $vfeint past $p1 guard and $vknock $t senseless.",
    "fatal" : "$N $vdrive $t back with a series of punches and kicks, and $vend with a stunning kick to the head.  $N1 $v1slump to the ground, motionless.",
    "dam1" : "$N $vland a weak punch to $p1 $o2.",
    "dam2" : "$N $vcatch $t square in the $o2 with a quick jab.",
    "dam3" : "$N $vconnect with a kick to $p1 $o2.",
    "dam4" : "$N $vtrip $t, and $vland a vicious blow in $p1 $o2.",
    "dam5" : "$N1 $v1are sent flying backwards by $p well-executed kick to $p1 $o2.",
    "dam6" : "$N1 $v1are caught off guard, and $n $vtake advantage and $vpummel $t soundly.",
    "dam7" : "$N relentlessly $vpound $t with repeated blows to the $o2.",
    "dam8" : "$N $vland an incredible combination of punches and kicks, beating $t badly.  $N1 $v1are not in very good shape.",
    "dam9" :"$N $vunleash a series of stunning blows to $p1 $o2!  $N1 $v1are hurt badly!",
    "dam10" : "$N $vbeat $t within an inch of $p1 life."
    ]),
  "combat-sword" :
  ([
    "miss" :
    ({ "$N $vslash at $t but $vmiss by a mile.",
       "$N just $vmiss $t with a quick thrust." }),
    "none" : "$N just barely $vnick $t with a swipe of $p $o.",
    "dam1" : "$N $vprick $t with a quick stab of $p $o.",
    "dam2" : "Blood flows from $p1 $o2 as $n $vsneak past $p1 guard.",
    "dam3" : "$N1 $v1are not quite fast enough; $p $o slices $t across the $o2.",
    "dam4" : "$N $vcatch $t in the $o2 with a quick thrust.",
    "dam5" : "$N $vcut $t with a slice of $p $o.",
    "dam6" : "$N $vwound $t with a quick slash.",
    "dam7" : "$N $vthrust quickly and $vwithdraw, leaving $t wounded.",
    "dam8" : "$N $vconnect!  $N1 $v1are hurt badly.",
    "dam9" : "$N $vbring $p $o down hard on $p1 $o2, injuring $t badly.",
    "dam10" : "$N $vdrive $p $o deep into $p1 $o2.  It looks serious, if not fatal.",
    "fatal" :
    ({ "$N neatly $vinsert $p $o between $p1 ribs.  $N1 $v1slump to the ground, dead.",
       "$N $vremove $p1 head from $p1 shoulders.  $P1 lifeless body slumps to the ground." }),
    "suicide" : "$N $vtake $p $o, and $vdrive it deep into $p chest.",
    "disarm" : 
    ({ "$N $vsend $p1 $o1 flying with a quick move $ns learned in Swordfighting 201.",
       "$N $vslash $p1 hand with $p $o, causing $t to drop $p1 $o1." }),

    ]),
  "combat-torch" : 
  ([ "miss" :
    ({ "$N $vswipe $p $o at $t but $vmiss by a mile.",
       "$N just $vmiss $t with a quick thrust." }),
    "none" : "$N $vhit $t with $p $o, but $t $vdo not seem hurt by it.",
    "disarm" : "$N $vgrab $p1 arm, and $vslam it down on $p knee, causing $n1 to drop $p1 $o1.",
    "knockdown" : "$N $vstep behind $t, and $vtrip $t, sending $t flying.",
    "fatal" : "$N $vdrive $t back with a series of punches of kicks, and $vend with a stunning kick to the head.  $N1 $v1slump to the ground, motionless.",
    "knockout" : "$N $vfeint past $p1 guard and $vknock $t senseless.",
    "dispatch" : "$N $vhave no sense of honor; $n $vlop $p1 head clean off.",
    "wakeup" : "$N slowly $vregain $p feet.",
    "dam1" : "$N $vland a weak punch to $p1 $o2.",
    "dam2" : "$N $vcatch $t square in the $o2 with $p $o.",
    "dam3" : "$N $vconnect with a kick to $p1 $o2.",
    "dam4" : "$N $vtrip $t, and lands a vicious blow to $p1 $o2.",
    "dam5" : "$N1 $v1are sent flying backwards by $p well-executed kick to $p1 $o2.",
    "dam6" : "$N1 $v1are caught off guard, and $n $vtake advantage and pummel $t soundly.",
    "dam7" : "$N relentlessly $vpound $t with repeated blows to the $o2.",
    "dam8" : "$N $vland an incredible combination of punches and kicks, beating $t badly.  $N1 $v1are not in very good shape.",
    "dam9" :"$N $vunleashes a series of stunning blows to $p1 $o2!  $N1 $v1are hurt badly!",
    "dam10" : "$N $vbeat $t within an inch of $p1 life."
    ]),

  "living-default" :
  ([
    "leave": "$N $vleave $o.",
    "mleave": "$N $vdisappear in a puff of smoke.",
    "enter": "$N $venter.",
    "menter": "$N $vappear in a puff of smoke.",
    "invis": "$N $vfade from view.",
    "vis": "$N $vfade into view.",
    "home": "$N $vgo home.",
    "clone": "$N $vclone the $o.",
    "destruct": "$N $vdest the $o.",
    "fol_leave": "$N $vfollow $t $o.",
    "fol_enter": "$N $venter following $t.",
    "clean": "$N $vtake a broom and $vclean $p surroundings.",
    ]),
  // These messages stolen from Distortion :-)
  "player-death" : ({
      "The world mourns the loss of $N.",
      "You are crushed to hear that $N $vhave died.",
  }),
 // These messages mostly stolen from Zork =)
  "nonsense" : ({
      "A valiant attempt.",
      "You can't be serious.",
      "I don't understand you at all.",
      "An interesting idea.",
      "What a concept!",
      "Get serious.",
      "Getting desperate?",
  }),
  "discarded" : ({
      "Someone has left %s lying on the ground.",
      "It seems that someone has left %s lying here.",
      "%s lies here, discarded.",
      "%s lies at your feet.",
      "Some luckless fool has left %s here.",
      "%s lies here, abandoned.",
  }),
  "discarded-plural" : ({
      "Someone has left %s lying on the ground.",
      "It seems that someone has left %s lying here.",
      "%s lie here, discarded.",
      "%s lie at your feet.",
      "Some luckless fool has left %s here.",
      "%s lie here, abandoned.",
  }),
  ]);

mixed get_messages(string type) {
    return messages[type];
}
