/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** file_d.c
**
** This daemons is used to provide a call_other() interface to some of
** the basic file system operations.  The RPC mechanism supported by
** remote_d only provides call_other() support, so remote file operations
** must be handled by something like this daemon.
**
** Note: we do not need to use unguarded() calls within this module -- we
**       can simply rely on the stack-based security to ensure that whatever
**       invokes these functions has the right privilege.  The priv of
**       this object does need to be 1, however, so that it will not
**       interfere with the stack.  We'll use DAEMON to do this.
**
** 15-Apr-95.  Deathblade.  Created.
*/

#include <mudlib.h>

inherit DAEMON;

DOC_MODULE("Provides a call_other() interface to the filesystem efuns");

/*
** File System Manipulation functions
*/
nomask int perform_cp(string src, string dst)
{
    return cp(src, dst);
}
nomask int perform_rename(string oldname, string newname)
{
    return rename(oldname, newname);
}
nomask int perform_rm(string fname)
{
    return rm(fname);
}
nomask int perform_mkdir(string dirname)
{
    return mkdir(dirname);
}
nomask int perform_rmdir(string dirname)
{
    rmdir(dirname);	/* ### void return(?) doc or spec is wrong */
    return 1;
}

/*
** File System Query functions
*/
nomask mixed perform_stat(string str)
{
    return stat(str);
}
nomask mixed perform_get_dir(string dir, int style)
{
    return get_dir(dir, style);
}
nomask int perform_file_size(string fname)
{
    return file_size(fname);
}

/*
** Read/Write functions
*/
nomask int perform_write_file(string file, string str, int flag)
{
DBBUG("writing to: "+file);
    return write_file(file, str, flag);
}
nomask string perform_read_file(string file, int start, int number)
{
    return read_file(file, start, number);
}
