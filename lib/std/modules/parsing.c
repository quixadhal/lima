/* Do not remove the headers from this file! see /USAGE for more info. */

varargs
mixed
condense_phrase( mixed phrase ) // OBJ PREP OBJ
{
  if( !pointerp( phrase ) || sizeof( phrase ) != 3 ||
    !stringp( phrase[1] ) || !objectp( phrase[2] ) ||
	(!objectp( phrase[0] ) && !pointerp( phrase[0] ) ) )
    return 0;

    if( objectp( phrase[0] ) )
	phrase[0] = ({ phrase[0] });


    if( phrase[2]->query_prep() == phrase[1] )
      return deep_inventory( phrase[2] ) & phrase[0];


    return notify_fail( "That doesn't seem possible.\n" );
}

private static string *nonsense_msgs = ({
    "A valiant attempt.\n",
    "You can't be serious.\n",
    "An interesting idea.\n",
    "What a concept!\n",
  });

string nonsense() {
  notify_fail(nonsense_msgs[random(sizeof(nonsense_msgs))]);
}
mixed	useless_msgs;

string
useless(string str)
{
    if( !pointerp( useless_msgs ) )
        useless_msgs = ({
            " doesn't seem to work.\n",
            " isn't notably helpful.\n",
           " has no effect.\n",
            " is a waste of time.\n",
            " might cause people to laugh at you.\n",
            " is a sign of senility.\n"
        });
    return str + useless_msgs[random(sizeof(useless_msgs))];
}
