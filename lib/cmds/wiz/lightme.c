/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE:  lightme <number>
//
//This command will allow any wizard to turn himself into a light source,
//thus illuminating any dark room he is in.
//You can also darken a lit room into a dark room by using a negative number,
//and turning into a light sink.
//
//The number 1 should suffice to illuminate dark rooms, and -1 to darken
//lit ones (unless another wizard is present, trying the same thing).
//To clear the effect, use "lightme 0".

inherit CMD;

private void main( mixed *arg)
{
  this_body()->set_light(arg[0]);
    outf("Light set to %i.\n", arg[0]);
}
