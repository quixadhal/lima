/* Do not remove the headers from this file! see /USAGE for more info. */

private static string *useless_msgs = ({
    " doesn't seem to work.\n",
    " isn't notably helpful.\n",
    " has no effect.\n",
    " is a waste of time.\n",
    " might cause people to laugh at you.\n",
    " is a sign of senility.\n",
});

string useless(string str)
{
    return str + choice(useless_msgs);
}
