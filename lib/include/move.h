/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __MOVE
#define __MOVE

//
// defines for move()
// Are the strings actually used????
// Yes, they are.  Many verbs use the return code of move() directly as
// an error message.
#define MOVE_OK			1
#define MOVE_NOT_RELEASED	"It doesn't seem moveable.\n"
#define MOVE_NOT_RECEIVED	"You can't seem to move it there.\n"
#define MOVE_PREVENTED		"You can't seem to move it.\n"
#define MOVE_NO_ROOM		"There isn't enough room.\n"
#define MOVE_NOT_ALLOWED	"That doesn't seem possible.\n"
#define MOVE_NO_ERROR		-1 // Rust wanted to clone another object
                                   // and move THAT instead.
// It's hard to say what the right error is here, since this should never
// happen.
#define MOVE_NO_DEST		"I can't figure out where you're moving that to."

#endif
