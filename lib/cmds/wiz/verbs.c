/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE:  verbs  or   verbs <verb>
//
//This command will show you the rules that a certain verbs has,
//or a list of all the verbs and their rules
//
//verbs
//
//ffear: STR
//move: OBJ
//look: OBJ with OBJ, STR, at OBJ with OBJ, STR OBJ, for OBJ, for LIV, at OBJ, OBJ
//,
//open: OBJ with OBJ, up OBJ, OBJ
//pour: OBJ in OBJ
//get: OBJ with OBJ, OBJ out of OBJ, OBJ from OBJ, OBJ
//wind: OBJ with OBJ, OBJ
//put: OBJ in OBJ
//give: OBJ to LIV
//wait:
//whisper: to LIV STR, STR to LIV, LIV STR
//close: OBJ
//fill: OBJ with OBJ, OBJ
//exit: OBJ
//go: in OBJ, into OBJ, on OBJ, over OBJ, to OBJ, around OBJ, up OBJ, down OBJ, STR
//
//
//verbs go
//
//go: in OBJ, into OBJ, on OBJ, over OBJ, to OBJ, around OBJ, up OBJ, down OBJ, STR

inherit CMD;

private void main(string str)
{
  string array info = explode(parse_dump(), "\n");
  string line, verb, rule;
  int ignore = 0;
  int first;
  mapping emote;

  if (str == "")
    str = 0;

  foreach (line in info)
  {
    if (sscanf(line, "Verb %s:", verb))
    {
      if (str && verb != str)
      {
        ignore = 1;
        continue;
      }
      ignore = 0;
      first = 1;
      emote = SOUL_D->query_emote(verb);
    } else {
      if (ignore)
      continue;

      if (sscanf(line, "  (%*s) %s", rule))
      {
        if (emote && emote[rule])
          continue;

        if (first)
        {
          outf("\n%s: ", verb);
          first = 0;
        } else
          out(", ");
        out(rule);
      }
    }
  }
  out("\n");
}