/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __SKILLS_H__
#define __SKILLS_H__

#define MAX_SKILL_VALUE		10000
#define LEARNING_FACTOR		10	/* divide new by 1/N of current */
#define TRAINING_FACTOR		10	/* 1/N goes to training pts */
#define PROPAGATION_FACTOR	2	/* 1/N propagates upward */
#define AGGREGATION_FACTOR	3	/* 1/N^i of parent skills aggregate
					   into the specified skill */
#define SKILL_ON_FAILURE	1	/* learn by N on failure */
#define SKILL_MIN_ON_WIN	2	/* minimum to learn on a win */
#define SKILL_MAX_ON_WIN	20	/* maximum to learn on a win */


#endif /* __SKILLS_H__ */
