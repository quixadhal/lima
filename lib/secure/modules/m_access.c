/* Do not remove the headers from this file! see /USAGE for more info. */


#pragma save_types

#include <security.h>
#include <driver/origin.h>

private static mixed privilege;
private static mixed unguarded_privilege;
private static mixed *security_handle;

nomask static void set_privilege(mixed priv)
{
  if (origin() != ORIGIN_LOCAL)
    return;
  if (file_name(this_object())==SECURE_D)
  {
    privilege = priv;
    return;
  }
  if (!SECURE_D->valid_privilege(priv))
    return;
  privilege =
  SECURE_D->reduced_privilege(priv,SECURE_D->query_protection(this_object(),1));
}

nomask mixed query_privilege()
{
  return privilege;
}

nomask mixed query_unguarded_privilege()
{
  return unguarded_privilege;
}

nomask int verify_privilege_granted(mixed *handle)
{
  return security_handle == handle;
}

nomask varargs static mixed unguarded(mixed priv,function code)
{
  if (origin() != ORIGIN_LOCAL)
    return 0;
  unguarded_privilege = priv;
  return SECURE_D->call_unguarded(code,security_handle=allocate(1));
}

/* Do not remove the headers from this file! see /USAGE for more info. */
