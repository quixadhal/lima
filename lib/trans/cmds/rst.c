// Restore command "rst" - Halo@Timescapes

//:COMMAND
// This command takes one argument, either a path and a file or just a
// file. It will then prompt you, asking which file you'd like to restore
// from.
//
// Obviously, restorations will fail if no "bak" directory is
// present off the given path, or if the file has not been
// previously backed up.
//
// Menu item ? will show your selection, again. Menu item
// q will cancel the restoration process.

//:FLAME
// 1998-Oct-15 : Halo@Timescapes        : Created.
// 1999-Mar-17 : Rodney@Timescapes      : Updated to recognize new ".c.bakDATE"
//                                      : formatted filenames
//                                      : You can now provide just a file name
//                                      : (which is assumed to be in your pwd),
//                                      : a relative path with a file (there
//                                      : may be some weird-looking displays with
//                                      : this if you use ".."'s, but it works),
//                                      : and absolute paths.
// 1999-Mar-19 : Rodney@Timescapes      : If no arg is supplied, will now use cwf.
//                                      : Also sets your cwf to the file it rst's.

inherit CMD;
inherit M_INPUT;

#undef DEBUG_ME
// If you change this, must also change in bk.c as well.
#define BACKUP_EXT "bak"

void prompt_user(string *asContents, string sPath, string sOrigFile,
                 string sSelection);

void main(string sFile) {
  string *asBakDirContents, *asPathList;
  string  sModFName, sPath, sFilename, sMsg;
  int     iStartAtRoot = 0;

  if (!sFile)
    sFile = sprintf("%s", get_user_variable("cwf"));

  if (sFile[0] == '/')
    iStartAtRoot = 1;
  
  asPathList = explode(sFile, "/");
  
  if (sizeof(asPathList) > 1) {
    sPath     = implode(asPathList[0..<2], "/");
    sFilename = asPathList[sizeof(asPathList) - 1];
    if (iStartAtRoot)
      sPath = "/" + sPath;
  } else {
    sPath     = ".";
    sFilename = sFile;
  }

  // Make sure the file isn't a directory.
  if (is_directory(evaluate_path((sPath ? sPath : "") + "/" + sFilename))) {
    out("Invalid input. You must specify a file.\n");
    return;
  }
  
  if (!is_directory(evaluate_path((sPath ? sPath : ".") + "/bak"))) {
    sMsg = sprintf("There is no %s/bak directory.  Therefore, there can \
be no backups to restore.  Exiting...\n", (sPath ? sPath : "."));
    outf("%-=75s", sMsg);
    return;
  }

/* Commented out by Rodney - 1999-Mar-17
 * There shouldn't need to be a file of sFile's name in the directory in
 * order to restore a file from the bak/ directory.
  if (!is_file(evaluate_path(sFile))) {
    out("No such file to restore.\n");
    return;
  }
 */

  out("Checking for backups...\n");
  sModFName = sFilename + "." + BACKUP_EXT;

  //### "rst /secure/daemons/user_d.c" -> doesn't work.
  //### This command currently requires you only to supply a filename,
  //###   and will not work if given a fully-qualified path.
  asBakDirContents = get_dir(evaluate_path((sPath ? sPath : ".") + "/bak/" + 
    sModFName + "*"));

#ifdef DEBUG_ME 
  out("sFile: " + sFilename + "\n");
  out("Mod'd: " + sModFName + "\n");
  out("Directory contents:\n");
  out(english_list(asBakDirContents) + "\nDone.\n");
#endif

  if (!sizeof(asBakDirContents)) {
    sMsg = sprintf("Sorry!  The file %s has not been previously backed up.\n",
                   (sPath ? sPath : ".") + "/" + sFilename);
    outf("%-=75s", sMsg);
    return;
  } else {
    out(sizeof(asBakDirContents)+" backups found.\n\n");
    prompt_user(asBakDirContents, sPath, sFilename, "menu");
  }
}

void prompt_user(string *asContents, string sPath, string sOrigFile,
                 string sSelection) {
  int i;
  string sMenu = "", sFileContents;
  
  if (sSelection == "menu") {
    for (i = 0; i < sizeof(asContents); i++) {
      sMenu += "  " + (i + 1) + ".) " + asContents[i] + "\n";
    }

    out(sMenu);
    modal_push((: prompt_user, asContents, sPath, sOrigFile :), 
      "\nWhich would you like to restore (#?q)? ");

  } else {
    // Validate input.
    if (!sSelection ||
         sSelection == "q" || sSelection == "x") {
      write("Restore canceled. Exiting...\n");
      modal_pop();
      return;
    }

    if (sSelection == "?") {
      for (i = 0; i < sizeof(asContents); i++) {
        sMenu += "  " + (i + 1) + ".) " + asContents[i] + "\n";
      }

      write("\n" + sMenu);
      modal_simple((: prompt_user, asContents, sPath, sOrigFile :), 
        "\nWhich would you like to restore (#?q)? ");

      return;
    }

    if (!to_int(sSelection) ||
      (to_int(sSelection) > sizeof(asContents) ||
         to_int(sSelection) < 0)) {
      write("Invalid selection.\n");
      modal_simple((: prompt_user, asContents, sPath, sOrigFile :), 
        "\nWhich would you like to restore (#?q)? ");

      return;
    }

    write("Restoring \"" + asContents[to_int(sSelection) - 1] + "\" to \"" +
          sPath + "/" + sOrigFile + "\"...\n");

    if (!sFileContents = read_file(
        evaluate_path(sPath + "/bak/" + asContents[to_int(sSelection) - 1]))) {
      write("Error: Reading backup file. Exiting...\n");
      modal_pop();
      return;
    }
    
    if (!write_file(evaluate_path(sPath + "/" + sOrigFile), sFileContents, 1)) {
      write("Error: Restoring file. Exiting...\n");
      modal_pop();
      return;
    }
    
    this_user()->query_shell_ob()->set_cwf(sPath + "/" + sOrigFile);
    write("Done. File restored.\n");
    modal_pop();

    return;
  }
}
