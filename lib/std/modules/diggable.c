/* Do not remove the headers from this file! see /USAGE for more info. */

// Define a function called dig(obj)  for when obj tries to dig you.
// This module requires that M_INPUT also be inherited.
void modal_push(function,mixed);
void modal_pop();
void modal_pass(string);

int is_diggable()
{
  return 1;
}

int direct_dig_in_obj()
{
  return 1;
}

int direct_dig_out_obj()
{
  return 1;
}

int direct_dig_up_obj()
{
  return 1;
}

int direct_dig_obj()
{
  return 1;
}

int direct_dig_obj_with_obj()
{
  return 1;
}

int can_dig_in_obj()
{
  return 1;
}

int can_dig_out_obj()
{
  return 1;
}

int can_dig_up_obj()
{
  return 1;
}

int direct_dig_in_obj_with_obj()
{
  return 1;
}

int direct_dig_out_obj_with_obj()
{
  return 1;
}

int direct_dig_up_obj_with_obj()
{
  return 1;
}

int indirect_dig_with_obj_in_obj()
{
  return 1;
}

private void answer_question(string input)
{
  object o;
  
  if(!stringp(input))
    {
      modal_pop();
      return;
    }
  if(o = present(input, environment(this_body())) || 
     o = present(input, this_body()))
    o->do_digging(this_object());
  else
    modal_pass(input);
  modal_pop();
}

void complete_dig()
{
  object array obs;

  obs = filter(all_inventory(this_body())+all_inventory(environment(this_body())),
	       (: $1->can_dig() :));
  
  switch(sizeof(obs))
    {
    case 0:
      write("You have nothing with which to dig.\n");	
      return;
    case 1:
      	  printf("[with %s]\n",obs[0]->the_short());
	  obs[0]->do_digging(this_object());
	  return;
    default:
	  modal_push((:answer_question:),
				  "What do you want to dig with? ");
	  return;
    }
}

