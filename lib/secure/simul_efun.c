/* Do not remove the headers from this file! see /USAGE for more info. */

#ifdef nomask
#undef nomask
#endif

// private so they don't become simuls
private inherit "/secure/check_config";

// these first since others might use them.
inherit "/secure/simul_efun/overrides";

inherit "/secure/simul_efun/security";
inherit "/secure/simul_efun/objcalls";
inherit "/secure/simul_efun/userfuncs";
inherit "/secure/simul_efun/path";
inherit "/secure/simul_efun/string";
inherit "/secure/simul_efun/misc";
inherit "/secure/simul_efun/more";
