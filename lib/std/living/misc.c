/* Do not remove the headers from this file! see /USAGE for more info. */

protected nosave int driving_vehicle = 0;
 
protected nosave mixed vehicle;

void set_driving_vehicle(object v)
{
    if(!v)
    {
        driving_vehicle = 0;
        vehicle = 0;
    }
    else
    {
        driving_vehicle = 1;
        vehicle = v;
    }
}

mixed query_vehicle()
{
   return vehicle;
}

mixed query_driving_vehicle()
{
    return driving_vehicle;
}

