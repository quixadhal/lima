/* Do not remove the headers from this file! see /USAGE for more info. */

//Belboz

private int	mass;

static
void
set_mass( int m )
{
  object	env;

  mass = m;
  if( env = environment( this_object() ) )
	env->update_capacity();
}

void set_weight( int m ) // for back compat
{
  set_mass( m );
}

int query_mass()
{
  return mass;
}
