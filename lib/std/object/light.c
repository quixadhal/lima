/* Do not remove the headers from this file! see /USAGE for more info. */

private int our_light;

//:FUNCTION set_light
//Set the amount of light that this object gives off
void set_light(int x)
{
    int old_light = our_light;
    object env = environment();

    our_light = x;

    if ( env )
	env->containee_light_changed(x - old_light);
}

//:FUNCTION adjust_light
//Add/subtract some light to/from the object
void adjust_light(int x)
{
    object env = environment();

    our_light += x;

    if ( env )
	env->containee_light_changed(x);
}

//:FUNCTION query_light
//Find out how much light the current object gives off
int query_light()
{
    return our_light;
}
