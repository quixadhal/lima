/* Do not remove the headers from this file! see /USAGE for more info. */

//:FUNCTION add_item
// Simple Items 
//
// Simple items should be things that were mentioned in your long description 
// that  are just there for decoration.  You can look at them, give them 
// adjectives, and make it give error messages back other than the default, 
// "That doesn't make any sense!" type message.  
//
// The following example creates an old, faded sign that you can't read, that 
// also answers to 'board' (just for the example.
//
//    add_item ("sign", "board",
//              ([ "adjs" : ({ "old", "faded" }),
//                 "read" : "The sign is too faded to read.",
//                 "look" : "The sign is very faded.",
//                 "get"  : "It would probably crumble to dust!",
//              ]) ); 
//
// However, if you actually want to be able to read the sign, or get the sign,
// you need to make an object for it.
//
// DEPRECATED:
//
// The old "add_item ("dust", "There is a lot of it!");" style is still
// valid, but remains in only for compatability reasons.  
void add_item(mixed array stuff ...) {
  mapping compat;
  if ( mapp(stuff[<1]) ) {
    new(SIMPLE_OB, stuff[<1], stuff[0..<2]);
    return;
  }
 
  if ( stuff[<1][<1] != '\n' ) {
    stuff[<1] += "\n";
  }
  
  /*
   * Kept for compatability purposes with the old fake_item_mgr, but 
   * should not be used anymore
   */  
  new(SIMPLE_OB,(["look":stuff[<1] ]),stuff[0..<2]);
}
