/* Do not remove the headers from this file! see /USAGE for more info. */

// This really be dynamic, with managing commands, but for now ...

//###TODO: make an interface to this crap.  Put it in a file.
//###combat-torch also needs serious help.

mapping messages = 
([
#if COMBAT_STYLE == COMBAT_SIMPLE
"combat-unarmed" :
  ([ "miss" :
    ({ "$N $vswing a fist at $t, but $vmiss wildly.\n",
       "$N $vjab a fist at $t, but just $vmiss.\n" }),
     "light" :
    ({ "$N $vcatch $t square in the face with a quick jab.\n",
	"$N $vconnect with a kick to $p1 midsection.\n" }),
     "serious" :
    ({ "$N $vunleashes a series of stunning blows to $p1 head!  $N1 $v1are hurt badly!\n",
       "$N1 $v1are caught off guard, and $n $vtake advantage and pummel $t soundly.\n" }),
     "fatal" : "$N $vdrive $t back with a series punches of kicks, and $vend with a stunning kick to the head.  $N1 $v1slump to the ground, motionless.\n",
    
    "disarm" : "$N $vgrab $p1 arm, and $vslam it down on $p knee, causing $n1 to drop $p1 $o1.\n",
    "knockdown" : "$N $vstep behind $t, and trips $t, sending $t flying.\n",
    "knockout" : "$N $vdeliver a quick jab to the head; $N1 $v1are knocked cold.\n",
    "dispatch" : "$N $vwalk up to $p1 motionless body, and calmly $vsnap $p1 neck.\n",
    "wakeup": "$N slowly $vregain $p feet.\n"
    ]),

  "combat-torch" : 
  ([ "miss" :
    ({ "$N $vswipe $p $o at $t but $vmiss by a mile.\n",
       "$N just $vmiss $t with a quick thrust.\n" }),
    "light" :
    ({ "$N $vhit $t in the midsection with $p $o.\n" }),
    "serious" :
    ({ "$N $vconnect with quick blows from $p $o!  $N1 $v1are hurt badly.\n" }),
     "fatal" : "$N $vdrive $t back with a series punches of kicks, and $vend with a stunning kick to the head.  $N1 $v1slump to the ground, motionless.\n",
    "disarm" : "$N $vgrab $p1 arm, and $vslam it down on $p knee, causing $n1 to drop $p1 $o1.\n",
    "knockdown" : "$N $vstep behind $t, and trips $t, sending $t flying.\n",
    "knockout" : "$N $vdeliver a quick jab to the head; $N1 $v1are knocked cold.\n",
    "dispatch" : "$N $vwalk up to $p1 motionless body, and calmly $vsnap $p1 neck.\n",
    "wakeup" : "$N slowly $vregain $p feet.\n",
    ]),
#else
  "combat-unarmed" :
  ([
    "miss" :
    ({ "$N $vswing a fist at $t, but $vmiss wildly.\n",
       "$N $vjab a fist at $t, but just $vmiss.\n" }),
    "none" : "$N $vpunch $t, but $n1 $v1shrug it off.\n",
    "disarm" : "$N $vgrab $p1 arm, and $vslam it down on $p knee, causing $n1 to drop $p1 $o1.\n",
    "knockdown" : "$N $vstep behind $t, and trips $t, sending $t flying.\n",
    "knockout" : "$N $vfeint past $p1 guard and $vknock $t senseless.\n",
    "fatal" : "$N $vdrive $t back with a series punches of kicks, and $vend with a stunning kick to the head.  $N1 $v1slump to the ground, motionless.\n",
    "dam1" : "$N $vland a weak punch to $p1 midsection.\n",
    "dam2" : "$N $vcatch $t square in the face with a quick jab.\n",
    "dam3" : "$N $vconnect with a kick to $p1 midsection.\n",
    "dam4" : "$N $vtrip $t, and lands a vicious blow just below $p1 ribs.\n",
    "dam5" : "$N1 $v1are sent flying backwards by $p well-executed kick to $p1 chest.\n",
    "dam6" : "$N1 $v1are caught off guard, and $n $vtake advantage and pummel $t soundly.\n",
    "dam7" : "$N relentlessly pounds $t with repeated blows to the head.\n",
    "dam8" : "$N $vland an incredible combination of punches and kicks, beating $t badly.  $N1 $v1are not in very good shape.\n",
    "dam9" :"$N $vunleashes a series of stunning blows to $p1 head!  $N1 $v1are hurt badly!\n",
    "dam10" : "$N $vbeat $t within an inch of $p1 life.\n"
    ]),
  "combat-sword" :
  ([
    "miss" :
    ({ "$N $vslash at $t but $vmiss by a mile.\n",
       "$N just $vmiss $t with a quick thrust.\n" }),
    "none" : "$N just barely nick $t with a swipe of $p $o.\n",
    "dam1" : "$N $vprick $t with a quick stab of $p $o.\n",
    "dam2" : "Blood flows from $p1 hand as $n $vsneak past $p1 guard.\n",
    "dam3" : "$N1 $vare not quit fast enough; $n $o slices $t across the arm.\n",
    "dam4" : "$N $vcatch $t in the arm with a quick thrust.\n",
    "dam5" : "$N $vcut $t with slice of $p $o.\n",
    "dam6" : "$N $vwound $t with a quick slash.\n",
    "dam7" : "$N $vthrust quickly and $vwithdraw, leaving $t wounded.\n",
    "dam8" : "$N $vconnect!  $N1 $v1are hurt badly.\n",
    "dam9" : "$N $vbring $p $o down hard on $p1 shoulder, injuring $t badly.\n",
    "dam10" : "$N $vdrive $p $o into $p1 side.  It looks serious, if not fatal.\n",
    "fatal" :
    ({ "$N neatly $vinsert $p $o between $p1 ribs.  $N1 $v1slump to the ground, dead.\n",
       "$N $vremove $p1 head from $p1 shoulders.  $P1 lifeless body slumps to the ground.\n" }),
    "suicide" : "$N $vtake $p $o, and $vdrive it deep into $p chest.\n",
    "disarm" : 
    ({ "$N $vsend $p1 $o1 flying with a quick move $ns learned in Swordfighting 201.\n",
       "$N $vslash $p1 hand with $p $o, causing $t to drop $p1 $o1.\n" }),

    ]),
  "combat-torch" : 
  ([ "miss" :
    ({ "$N $vswipe $p $o at $t but $vmiss by a mile.\n",
       "$N just $vmiss $t with a quick thrust.\n" }),
    "none" : "$N $vhit $t with $p $o, but $t $vdo not seem hurt by it.\n",
    "disarm" : "$N $vgrab $p1 arm, and $vslam it down on $p knee, causing $n1 to drop $p1 $o1.\n",
    "knockdown" : "$N $vstep behind $t, and trips $t, sending $t flying.\n",
    "fatal" : "$N $vdrive $t back with a series punches of kicks, and $vend with a stunning kick to the head.  $N1 $v1slump to the ground, motionless.\n",
    "knockout" : "$N $vfeint past $p1 guard and $vknock $t senseless.\n",
    "dispatch" : "$N $vhave no sense of honor; $n $vlop $p1 head clean off.\n",
    "wakeup" : "$N slowly $vregain $p feet.\n",
    "dam1" : "$N $vland a weak punch to $p1 midsection.\n",
    "dam2" : "$N $vcatch $t square in the face with $p $o.\n",
    "dam3" : "$N $vconnect with a kick to $p1 midsection.\n",
    "dam4" : "$N $vtrip $t, and lands a vicious blow just below $p1 ribs.\n",
    "dam5" : "$N1 $v1are sent flying backwards by $p well-executed kick to $p1 chest.\n",
    "dam6" : "$N1 $v1are caught off guard, and $n $vtake advantage and pummel $t soundly.\n",
    "dam7" : "$N relentlessly pounds $t with repeated blows to the head.\n",
    "dam8" : "$N $vland an incredible combination of punches and kicks, beating $t badly.  $N1 $v1are not in very good shape.\n",
    "dam9" :"$N $vunleashes a series of stunning blows to $p1 head!  $N1 $v1are hurt badly!\n",
    "dam10" : "$N $vbeat $t within an inch of $p1 life.\n"
    ]),
#endif

  "living-default" :
  ([
    "leave": "$N $vleave $o.\n",
    "mleave": "$N $vdisappear in a puff of smoke.\n",
    "enter": "$N $venter.\n",
    "menter": "$N $vappear in a puff of smoke.\n",
    "invis": "$N $vfade from view.\n",
    "vis": "$N $vfade into view.\n",
    "home": "$N $vgo home.\n",
    "clone": "$N $vclone the $o.\n",
    "destruct": "$N $vdest the $o.\n",
    ]),
  // These messages stolen from Distortion :-)
  "player-death" : ({
      "The world mourns the loss of $N.",
      "You are crushed to hear that $N $vhave died.",
  }),
 // These messages mostly stolen from Zork =)
  "nonsense" : ({
      "A valiant attempt.\n",
      "You can't be serious.\n",
      "I don't understand you at all.\n",
      "An interesting idea.\n",
      "What a concept!\n",
      "Get serious.\n",
      "Getting desperate?\n",
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
