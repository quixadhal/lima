/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**	Posix tar object in LPC. Zellski, zell@alyx.com, 1996.
**
**	Written for Igor Mud, a DGD-based 2.4.5. Should be fairly
**	portable -- does need mutable, non-zero-sensitive strings.
**
**	Anybody is free to do anything with this code as far as I
**	am concerned. Of course, I also consider myself free to call
**	people bloody bastards when they are.
**
**      MudOS port by Beek
*/

# define FILES_PER_SECOND	10

# define LF_DIR			5
# define LF_NORMAL		0


/* change these any way your lil' hearts desire */

# define UID			1701
# define GID			2261
# define UNAME			to_buffer("zell")
# define GNAME			to_buffer("alyx")

buffer to_buffer(mixed arg) {
    buffer ret;
    
    if (intp(arg))
	arg = arg + "";

    if (stringp(arg)) {
	ret = allocate_buffer(strlen(arg));
	write_buffer(ret, 0, arg);
    } 

    return ret;
}

void header_insert(buffer what, int offset, int size);

#define null_record	allocate_buffer(0x200)

buffer	header_record;


void tar(string dir, string archive) {
   rm(archive);
   call_out("add_next_files", 0, ({ dir }), archive);
}


buffer octal(int num, int width) {
   buffer	result;
   int		i;

   result = null_record;
   i = width;

   result[-- i] = ' ';

   do {
      result[-- i] = '0' + (num & 0x07);
      num >>= 3;
   } while (num != 0);

   while (i > 0) result[-- i] = ' ';

   return result[0 .. width-1];
}


void header_insert(buffer what, int offset, int size) {
   int	i, mid, end;

   if (sizeof(what) > size) {
      what = what[0 .. size-1];
   }
   mid = offset + sizeof(what);
   end = offset + size;
   header_record[offset..mid-1] = what;
   if (end - mid)
       header_record[mid..end-1] = allocate_buffer(end - mid);
}

void write_header(string archive, string name, int size, int type) {
   buffer	str;
   int		sum;
   int		i;

   header_insert(to_buffer(name),	0x00, 0x64);
   header_insert(octal(size, 12),	0x7C, 0x0C);
   header_insert(octal(time(), 12),	0x88, 0x0C);
   header_insert(to_buffer(type),	0x9C, 0x01);
   header_insert(to_buffer("        "),	0x94, 0x08);

   for (i = 0; i < 0x200; i ++) {
      sum += header_record[i];
   }
   str = octal(sum, 7)[0..<2] + allocate_buffer(2);
   str[7] = ' '; /* tar SUCKS */

   header_insert(str, 			0x94, 0x08);

   write_buffer(archive, file_size(archive), header_record);
}

void add_next_files(string *files, string archive) {
   int	count;

   for (count = 0; count < FILES_PER_SECOND; count ++) {
      if (file_size(files[0]) == -2) {
	 /* a directory: descend and push contents onto stackish thing */
	 int		i;
	 string	       *contents;

	 contents = get_dir(files[0] + "/");

	 for (i = 0; i < sizeof(contents); i ++) {
	    contents[i] = files[0] + "/" + contents[i];
	 }
	 write_header(archive, files[0] + "/", 0, LF_DIR);

	 files = contents + files[1 ..];
      } else {
	 if (files[0] != archive) {
	    int	i, size;

	    /* don't include archive in archive :) */

	    size = file_size(files[0]);

	    write(files[0] + "\t (" + size + ") : ");

	    if (size > 0) {

	       write_header(archive, files[0], size, LF_NORMAL);

	       /* write all full records first */
	       for (i = 0; i < (size & 0xFFFE00); i += 0x200) {
		  string	data;

		  data = read_buffer(files[0], i, 0x200);
		  if (!data) {
		     error("failed to read 512 bytes from " + files[0]);
		  }
		  write_buffer(archive, file_size(archive), data);
	       }
	       /* pad last record, if anything left to write */
	       if (size % 0x200) {
		  buffer	data;

		  data = read_buffer(files[0], i, size % 0x200) + null_record;
		  if (!data) {
		     error("failed to read " + (size % 0x200) +
			   "  bytes from " + files[0]);
		  }
		  write_buffer(archive, file_size(archive), data[0 .. 0x1FF]); /* write 512 bytes */
	       }
	       write("done.\n");
	    } else {
	       write("skipped.\n");
	    }
	 }
	 files = files[1 ..];
	 if (!sizeof(files)) {
	    write("DONE.\n");
	    return;
	 }
      }
   }
   call_out("add_next_files", 1, files, archive);
}


void create() {
      header_record = null_record;

      header_insert(to_buffer("ustar  "), 0x101, 0x08);	/* POSIX-tar magic */
      header_insert(to_buffer(" 40755 "), 0x64, 0x08);
      header_insert(octal(UID, 7),0x6C, 0x08);
      header_insert(octal(GID, 7),0x74, 0x08);
      header_insert(UNAME, 0x109, 0x20);
      header_insert(GNAME, 0x129, 0x20);
}


