/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// Yet another goddamn ls command - this one will properly parse
// colours and give me a nice Christmas tree effect 
// I hate having to choose between decent formatting and no colours
// or lame formatting with colours.  This would be so much easier
// if sprintf understood invisible (ansi) characters.. sigh
// Note the emphasis here is on speed, not on memory usage - hell
// there's always swap space  :)
// - Fadeout

#include <mudlib.h>
#include <colour_conf.h>

inherit M_GLOB;
inherit M_ANSI;
inherit CMD;


#define COLUMNS_IN_SCREEN		70


private string colourize(string file, int fsize) {
	// stick in ansi codes
	if (file[<2..] == ".h")
		return ansi(LS_HEADER_COLOUR + file + NORM);
	if (file[<2..] == ".c")
		return ansi(LS_CODE_COLOUR + file + NORM);
	if (file[<2..] == ".o")
		return ansi(LS_SAVE_COLOUR + file + NORM);
	if (fsize == -2)
		return ansi(LS_DIR_COLOUR + file + NORM);
	
	return file;
}


private string list_files(string* files, mapping flags) {
	// this func actually does the work
	string* fnames;
	int* fsizes;
	int* flens;
	int i, max_len, j, cols, rows, k, tmp;
	string txt, line;

	txt = "";

	// since we know the size beforehand - might as well
	// save us some malloc()'ing time
	fnames = allocate(sizeof(files));
	fsizes = allocate(sizeof(files));
	flens = allocate(sizeof(files));

	for (i=0; i < sizeof(files); i++) {	
		fsizes[i] = file_size(files[i]);
		fnames[i] = depath(files[i]);
		flens[i] = strlen(fnames[i]);
		if (flags["c"]) 
			fnames[i] = colourize(fnames[i], fsizes[i]);
		if (flens[i] > max_len)
			max_len = flens[i];
	}
	if (flags["F"]) {
		// add '/' and '*' formatting
		for (i=0; i < sizeof(fnames); i++) {
			if (fsizes[i] == -2) {
				fnames[i] += "/";
				flens[i]++;
			} else {
				if (find_object(files[i])) {
					fnames[i] += "*";
					flens[i]++;
				}
			}
		}
	}	// end -F formatting

	// -l formatting is quick and easy, get it over with
	if (flags["l"]) {
		for (i=0; i < sizeof(fnames); i++) {
			tmp = fsizes[i];
			if (tmp == -2) {
				txt += sprintf("%-10s %|25s %s\n", "-", "DIRECTORY", fnames[i]);
			} else {
				line = ctime(stat(files[i])[1])[4..];
				txt += sprintf("%-10d %|25s %s\n", tmp, line, fnames[i]);
			}
		}
		return txt;
	} 

	if (flags["s"]) {
		// just modify the file names to include their size in K
		for (i=0; i < sizeof(fnames); i++) {
			tmp = (fsizes[i]+1024)/1024;	
			fnames[i] = sprintf("%3d %s", tmp, fnames[i]);
			flens[i] += strlen(sprintf("%3d", tmp)) + 1;
			// gotta adjust the max length as well due to all the length 
			// adjusting going on
			if (flens[i] > max_len)
				max_len = flens[i];
		}
	}
	
	// begin 'plain' ls		
	
		max_len++;
		cols = COLUMNS_IN_SCREEN/max_len;
		rows = sizeof(fnames)/cols;		// extra row will be handled later

		for (i=0; i < rows; i++) {
			line = "";
			for (j=0; j < cols; j++) {
				tmp = i*cols + j;
				line += fnames[tmp];
				if (j != (cols-1)) {
					for (k=flens[tmp]; k <= max_len; k++)
						line += " ";	// add padding
				}
			}
			txt += line + "\n";
		}
		// now do extra row
		if (sizeof(fnames)%cols) {
			line = "";
			for (i=(rows*cols); i < sizeof(fnames); i++) {
				line += fnames[i];
				if (i != (sizeof(fnames)-1)) {
					for (k=flens[i]; k <= max_len; k++)
						line += " ";	// add padding
				}
			}
			txt += line + "\n";
		}
		
	// end 'plain' ls (no -s or -l options)
	
	return txt;
}

	
private void main(mixed argv, mapping flags) {
	string txt;
	int i;
	string* roots;

	roots = ({ evaluate_path("./") });

	if(!sizeof(argv) || !argv[0]) {
		argv = ({"",});
		argv[0] = glob(evaluate_path("./*"));
	} else {
		argv[0] = sort_array(argv[0], 1);
		for (i=0; i<sizeof(argv[0]); i++) {
			if (is_directory(argv[0][i])) {
				argv += ({ glob(evaluate_path(argv[0][i]) + "/*") });
				roots += ({ argv[0][i] });
				argv[0] -= ({ argv[0][i] });
				i--;
			} 
		}
	}
	txt = "";                       
	
	for (i=0; i < sizeof(argv); i++) { 
		if (!sizeof(argv[i]) && i==0 && sizeof(argv)>1) {
			continue;
		}
		// start with the directory name
		if (i_use_ansi() && flags["c"])
			txt += ansi(LS_PATH_COLOUR + roots[i] + NORM );
		else
			txt += roots[i];
		txt += ":\n";
		
		if (sizeof(argv[i]))
			txt += list_files(sort_array(argv[i], 1), flags) + "\n";
		else
			txt += "Empty directory.\n\n";
	}

        out(txt);
}


