/* Do not remove the headers from this file! see /USAGE for more info. */

 /*
**  Innovative little box macro by Rust @ Lima Bean.
**  December 17, 1995 - Yaynu @ Red Dragon added framing, corners and sides
**
**  Usage for quote_box (box is similar): quote_box(arg1,arg2,arg3,arg4,arg5,arg6)
**  ------------------------------------------------------------------------------
**  arg1 is the text to display inside the box
**  arg2 is the width
**  arg3 is author
**  arg4 is frame string        | optional defaults to "-"
**  arg5 is side string         | optional defaults to "|"
**  arg6 is the corner string   | optional defaults to frame string
*/

varargs string quote_box(string quote, int width, string author,
                         string frame, string sframe, string corner)
{
  string array lines;
  string array author_lines;
  string s;
  if(!frame)  frame  = "-";
  if(!sframe) sframe = "|";
  if(!width)
    width = 70;
  if(!corner){
    s = frame + frame;
    s += power_str(frame,width);
 }else{
    s =  corner;
    s += power_str(frame,width);
    s += corner;
 }
  quote = wrap(quote,width);
  author = "- " + author;
  author_lines = explode(wrap(author, width-2),"\n");
  author_lines = map(author_lines, (: $(sframe)+rjust($1,$(width)-2)+"  "+$(sframe):));
  lines = explode(quote,"\n");
  lines += ({""});
  lines = map(lines, (: $(sframe) + center($1,$(width)) + $(sframe) :));
  lines += author_lines + ({s});
  lines = ({s}) + lines;
  lines = map(lines, (: center($1,78) :));
  return "\n" + implode(lines,"\n")+"\n";
}


varargs string box(string text, int width,
                   string frame, string sframe, string corner)
{
  string array lines;
  string s;
  if(!frame)  frame  = "-";
  if(!sframe) sframe = "|";
  if(!width)
    width = 70;
  if(!corner){
    s = frame + frame;
    s += power_str(frame,width);
 }else{
    s =  corner;
    s += power_str(frame,width);
    s += corner;
 }
  text = wrap(text, width-2);
  lines = explode(text,"\n");
  lines = map(lines, (: $(sframe) + center($1,$(width)) + $(sframe) :));
  lines = ({s}) + lines + ({s});
  lines = map(lines, (: center($1,78) :));
  return "\n" + implode(lines,"\n")+"\n";
}

