//:COMMAND
// This command allows you to backup a file.
// Given the argument, file, it assumes your current
// directory. 
// 
// It then copies this file to ./bak/ directory while
// also date stamping it. Timestamps are appended, as
// well, if two files are backed up on the same day.
// 
// Also, if no ./bak/ directory exists, it automagically
// creates one for you.
//
// See also: rst

//:FLAME
// 1998-Oct-13 : Halo@Timescapes        : Created.
// 1999-Jan-28 : Rodney@Timescapes      : Updated date creation to be string, fixed
//                                      : calculation of the month.
// 1999-Mar-15 : Rodney@Timescapes      : Made this command set your cwf to the file
//                                      : you just bk'ed.  
//                                      : Also added a ".c" to filename given if it's
//                                      : not found at first.
// 1999-Mar-19 : Rodney@Timescapes      : If no arg supplied, use your cwf.

#include <localtime.h>
inherit CMD;

#undef DEBUG_ME
#define BACKUP_EXT    "bak"

void main(string sFile) {
  int    *aiTimeVals;
  string  sFileContents, sDest, sSecDest, sPwd;
  int     iYear, iMonth, iDay, iHour, iMins, iSecs, iFileNameLen;
  string  sYear, sMonth, sDay, sFullDate, sHour, sMins, sSecs, sFullTime;

  if (!sFile)
    sFile = sprintf("%s", get_user_variable("cwf"));

  sPwd = get_user_variable("pwd");
  
  // Make sure the file isn't a directory.
  if (is_directory(sPwd +"/"+ sFile)) {
    write("Invalid input. Argument must be a file.\n");
    return;
  }
  
  // 1999-Mar-15 : Rodney
  if (!is_file(evaluate_path(sFile)))
    sFile = sFile + ".c";

  if (!is_file(evaluate_path(sFile))) {
    out("No such file found.\n");
    return;
  }

  // Read in file and error if problems.
  if ( !(sFileContents = read_file(sPwd +"/"+ sFile)) ) {
    outf("Could not read %s.  Backup failed.\n", sFile);
  }

  // Check to see if there's a backup dir.
  if ( !is_directory(sPwd +"/"+ "bak")) {
    write("Backup directory not found. Creating one...\n");
 
    // Create the dir, and error if problems.
    if (!mkdir(sPwd +"/"+ "bak")) {
      write("Error: Creating backup directory. Exiting...\n");
      return;
    }
  }
  
#ifdef DEBUG_ME
  out("DEBUGGING:\n");
  out("sFile: "+sFile+"; Pwd:"+sPwd+"\n");
  out("Contents:\n"+sFileContents+"\nDone.\n");
  return;
#endif

  iFileNameLen = sizeof(sFile);

  // Figure out datestamping. Don't worry about time timestamp, yet.
  aiTimeVals = localtime(time());
  iYear      = aiTimeVals[LT_YEAR];
  iMonth     = aiTimeVals[LT_MON];
  iDay       = aiTimeVals[LT_MDAY];

  // Rodney - 1999-Jan-28 - Fixing month, turing date into a string.
  iMonth++;
  sYear     = sprintf("%04d", iYear);
  sMonth    = sprintf("%02d", iMonth);
  sDay      = sprintf("%02d", iDay);
  sFullDate = sYear + sMonth + sDay;

  sDest = sPwd +"/"+ "bak/" + sFile + "." + BACKUP_EXT + sFullDate;

  // Check to see if it exists, already.
  if (is_file(sDest)) {
    out("\""+ sDest +"\" already exists. Adding timestamping...\n");
    
    // Get time values for timestamping.
    iHour = aiTimeVals[LT_HOUR];
    iMins = aiTimeVals[LT_MIN];
    iSecs = aiTimeVals[LT_SEC];
    
    // Rodney - 1999-Jan-28 - Fixing month, turing date into a string.
    sHour     = sprintf("%02d", iHour);
    sMins     = sprintf("%02d", iMins);
    sSecs     = sprintf("%02d", iSecs);
    sFullTime = sHour + sMins + sSecs;

    // Save original destination before changing.
    // This in case the next check fails.
    sSecDest = sDest;
    sDest += "-" + sFullTime;

    // Just to be anal, let's do one more check.
    if (is_file(sDest)) {
      out("\""+ sDest +"\" exists, as well. Incrementing seconds by one...\n");

      iSecs++;

      // Rodney - 1999-Jan-28 - Recreate the new timestamp.
      sSecs     = sprintf("%02d", iSecs);
      sFullTime = sHour + sMins + sSecs;

      sSecDest += "-" + sFullTime;
      if (!write_file(sSecDest, sFileContents, 1)) {
        out("Error: Writing backup file. Exiting...\n");
        return;
      } else {
        out("\""+ sPwd +"/"+ sFile +"\" backed up to \""+ sSecDest +"\"\n");
        this_user()->query_shell_ob()->set_cwf(sPwd + "/" + sFile);
        return;
      }
    } else {
      if (!write_file(sDest, sFileContents, 1)) {
        out("Error: Writing backup file. Exiting...\n");
        return;
      } else {
        out("\""+ sPwd +"/"+ sFile +"\" backed up to \""+ sDest +"\"\n");
        this_user()->query_shell_ob()->set_cwf(sPwd + "/" + sFile);
        return;
      }
    }
  } else {
    // Throw an error message if problems.
    if (!write_file(sDest, sFileContents, 1)) {
      out("Error: Writing backup file. Exiting...\n");
      return;
    } else {
      out("\""+ sPwd +"/"+ sFile +"\" backed up to \""+ sDest +"\"\n");
      this_user()->query_shell_ob()->set_cwf(sPwd + "/" + sFile);
      return;
    }
  }
}
