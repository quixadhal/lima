/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __MOVE
#define __MOVE

//
// defines for move()
// Are the strings actually used????
#define MOVE_OK			1
#define MOVE_NOT_RELEASED	"It doesn't seem moveable."
#define MOVE_NOT_RECIEVED	"You can't seem to move it there."
#define MOVE_PREVENTED		"You can't seem to move it."
#define MOVE_NO_ROOM		"There isn't enough room."
#define MOVE_NOT_ALLOWED	"That doesn't seem possible."
#define MOVE_NO_ERROR		-1 // Rust wanted to clone another object
                                   // and move THAT instead.
// It's hard to say what the right error is here, since this should never
// happen.
#define MOVE_NO_DEST		"I can't figure out where you're moving that too."

#endif
