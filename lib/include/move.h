/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __MOVE
#define __MOVE

//
// defines for move()
//
#define MOVE_OK			1
#define MOVE_NOT_RELEASED	"It doesn't seem moveable."
#define MOVE_NOT_RECIEVED	"You can't seem to move it there."
#define MOVE_PREVENTED		"You can't seem to move it."
#define MOVE_NO_ROOM		"There isn't enough room."
#define MOVE_NOT_ALLOWED	"That doesn't seem possible."
// It's hard to say what the right error is here, since this should never
// happen.
#define MOVE_NO_DEST		"I can't figure out where you're moving that too."

#endif
