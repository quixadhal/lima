/* Do not remove the headers from this file! see /USAGE for more info. */


private static object my_sparse_obj;

void add_item(mixed array stuff ...)
{

  if(mapp(stuff[<1]))
    {
      new("/std/less_sparse_obj", stuff[<1], stuff[0..<2]);
      return;
    }
  if(stuff[<1][<1] != '\n')
    {
      stuff[<1] += "\n";
    }

  if(my_sparse_obj)
    {
      my_sparse_obj->add_simple_fake_item(stuff[<1], stuff[0..<2]);
    }
  else
    {
      my_sparse_obj = new(SPARSE_OBJ, stuff[<1], stuff[0..<2]);
    }
}





