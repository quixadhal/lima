/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust.

private int	mass;

//:FUNCTION set_mass
//Change the mass of an object
protected
void
set_mass( int m )
{
  object	env;

  mass = m;
  if( env = environment( this_object() ) )
	env->update_capacity();
}

//:FUNCTION set_weight
//calls set_mass() for compat reasons
void set_weight( int m )
//### for back compat
{
  set_mass( m );
}

//:FUNCTION query_mass
//returns the mass of an object
int query_mass()
{
  return mass;
}
