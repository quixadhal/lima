/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: dataedit file
//
//'dataedit' allows you to edit .o files created by save_object(), without
//having to directly edit the compressed form, and possibly make mistakes
//causing the file to restore improperly.

//When you start up dataedit, the file is read, and the filename and variables
//in the file appear at the top of the menu.  No changes are actually made on
//disk until you exit with the 'q' command, or save the new version to a
//different file with the 's' command.  Using the 'Q' command leaves the file
//in its original form.
//
//Here is a summary of the options available:
//
//d) Delete Variable:
//
//Remove the specified variable and value.
//
//p) Print Expression:
//
//Print the value of a given expression.  The variables may be refered to
//using '$varname'.  $$, or $ followed by an non-alphanumeric character
//can be used to mean a literal $.
//
//q) Quit and Save:
//
//Save the internal values to the file being edited, and quit.
//
//r) Rename Variable:
//
//Create a new variable with the same value as an old variable, and delete
//the old one.
//
//m) Merge Datafile:
//
//Merge values from another data file into the current datafile.  Arrays and
//Mappings are merged by simply adding them; other values will produce mismatches
//if the variable exists in both objects.  The original version is then kept.
//
//Q) Quit w/o Saving:
//
//Quit, leaving the file on disk unmodified.
//
//=) Set Variable:
//
//Set a variable (or define one, if it doesn't exist already) to a given value.
//The value is any expression, as explained under Print Expression.
//
//s) Save as ...
//
//Save the modified file under a new filename.  That filename becomes the
//current filename.

inherit CMD;

private void main(mixed *arg)
{
    new(DATAEDIT)->start_menu(arg[0]);
}
