/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

mapping skill_restrictions;
mapping trainer_msgs = ([
  "unable_msg" : "\"I cannot teach you that.\", the trainer tells $N.",
  "fail_msg" : "$N $vtrain for a while, but $vundergo no improvement.",
  "low_msg" : "$N $vtrain, but $vimprove little.",
  "mid_msg" : "$N $vtrain and $vimprove a little.",
  "high_msg" : "$N $vtrain hard, and $vimprove significantly",
  "very_high_msg" : "$N $vhas an excellent lesson, and $vimprove remarkably.",
  "reject_msg" : "\"I can teach you no more.\", the trainer tells $N.",
]);


void set_trainer_msgs( mapping msgs )
{
    string array msg_names = keys( trainer_msgs );
    foreach( string key, string msg in msgs )
    {
	if( !member_array( key, msg_names ) + 1 )
	    error( "Invalid msg name: " + key );
	trainer_msgs[ key ] = msg;
    }
}


void set_train_restrict( array restrictions )
{
    string array valid_skills = SKILL_D->query_skills();

    foreach( string skill, string restriction in restrictions )
    {
	if( member_array( skill, valid_skills ) == -1 )
	    error( "Invalid skill: " + skill );
	skill_restrictions += ([ skill : restriction ]);
    }
}


void do_training( object trainee, string skill )
{
    int trainee_skill = trainee->get_skill( skill );
    int trainer_skill = this_object()->get_skill( skill );
    string msg;

    if( !member_array( skill, keys( skill_restrictions )) + 1 || member_array( skill , keys( this_object()->get_skills()))) 
	msg = trainer_msgs[ "unable_msg" ];
    else if( trainee_skill < trainer_skill )
    {
	trainee->test_skill( skill, trainee_skill + 1 );
	switch( trainee->get_skill(skill) - trainee_skill )
	{
	case 0..1:
	    msg = trainer_msgs[ "fail_msg" ];
	    break;
	case 2..6:
	    msg = trainer_msgs[ "low_msg" ];
	    break;
	case 7..13:
	    msg = trainer_msgs[ "mid_msg" ];
	    break;
	case 14..18:
	    msg = trainer_msgs[ "high_msg" ];
	    break;
	default:
	    msg = trainer_msgs[ "very_high_msg" ];
	}
    }
    else msg = trainer_msgs["reject_msg"];

    this_object()->simple_action( msg );
}
