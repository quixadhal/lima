/* Do not remove the headers from this file! see /USAGE for more info. */

// Simple cp, no -r yet.
#include <mudlib.h>
inherit CMD;

private void copy_one(string src, string dst, int force)
{
    string contents;

    if ( is_file(dst) && !force )
    {
	printf("%s already exists.  Copy failed.\n", dst);
    }
    else if ( !(contents = read_file(src)) )
    {
	printf("Could not read %s.  Copy failed.\n", src);
    }
    else if ( !write_file(dst, contents, 1) )
    {
	printf("%s could not be written to.  Copy failed.\n", dst);
    }
    else
    {
	printf("%s copied to %s.\n", src, dst);
    }
}

private void main(mixed argv, mapping flags)
{
    string src;
    string dst;
  
    if(sizeof(argv[0]) > 1 && !is_directory(argv[1]))
    {
	printf("cp: files dir not files file\n");
	return;
    }
    if(sizeof(argv[0]) > 1)
    {
	if(argv[1][<1] != '/')
	    argv[1] += "/";

	foreach(src in argv[0])
	    {
		dst = argv[1] + split_path(src)[1];
		copy_one(src, dst, flags["f"]);
	    }
    }
    else 
    {
	dst = argv[1];
	src = argv[0][0];

	if(is_directory(dst))
	{
	    if(dst[<1] != '/')
		dst += "/";
	    dst += depath(src);
	}

	copy_one(src, dst, flags["f"]);
    }
}
