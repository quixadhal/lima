/* Do not remove the headers from this file! see /USAGE for more info. */

// task_d.c - Daemon for keeping track of (development) tasks.
//   Marroc@Lima Bean: Mar 1, 2002

inherit M_DAEMON_DATA;

#define TASK_ARRAY  ({ 0, 0, 0, 0, 0, ([ ]), ({ }) })

#define TASK_TITLE  0
#define TASK_DESC   1
#define TASK_OWNER  2
#define TASK_CTIME  3
#define TASK_STATUS 4
#define TASK_ATTR   5
#define TASK_SUB    6


nomask private array find_task(string);

private array tasks = ({});


//:FUNCTION query_title
// Return the title for a given task.
nomask string query_title(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_TITLE];
}


//:FUNCTION query_description
// Return the description for a given task.
nomask string query_description(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_DESC];
}


//:FUNCTION query_owner
// Return the owner of a given task.
nomask string query_owner(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_OWNER];
}


//:FUNCTION query_creation_time
// Return the creation time of a given task.
nomask string query_creation_time(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_CTIME];
}


//:FUNCTION query_status
// Return the status of a given task.
nomask string query_status(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_STATUS];
}


//:FUNCTION query_attribute
// Return a particular attribute of the specified task.
nomask mixed query_attribute(string task_id, string attr)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_ATTR][attr];
}


//:FUNCTION query_attributes
// Return all attributes of the specified task.
nomask mixed query_attributes(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return copy(task[TASK_ATTR]);
}


//:FUNCTION query_sub_tasks
// Return the sub-tasks for a given task.
nomask array query_sub_tasks(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return copy(task[TASK_SUB]);
}


//:FUNCTION set_title
// Set the title of a given task.
nomask void set_title(string task_id, string title)
{
    array task = find_task(task_id);
    
    if (!title)
      error("null title");
    
    if (!task)
      error("unable to find task " + task_id);
    
    task[TASK_TITLE] = title;
    save_me();
}


//:FUNCTION set_description
// Set the description of a given task.
nomask void set_description(string task_id, string desc)
{
    array task = find_task(task_id);
    
    if (!task)
      error("unable to find task " + task_id);
    
    task[TASK_DESC] = desc;
    save_me();
}


//:FUNCTION set_owner
// Set the owner of a given task.
nomask void set_owner(string task_id, string owner)
{
    array task = find_task(task_id);
    
    if (!owner)
      error("null owner");
    
    if (!task)
      error("unable to find task " + task_id);
    
    task[TASK_OWNER] = owner;
    save_me();
}


//:FUNCTION set_status
// Set the status of a given task.
nomask void set_status(string task_id, string status)
{
    array task = find_task(task_id);
    
    if (!status)
      error("null status");
    
    if (!task)
      error("unable to find task " + task_id);
    
    task[TASK_STATUS] = status;
    save_me();
}


//:FUNCTION set_attribute
// Set an attribute of a given task.
nomask void set_attribute(string task_id, string attr, mixed val)
{
    array task = find_task(task_id);
    
    if (!attr)
      error("null attribute");

    if (!val)
      error("null value; use remove_attribute()");
    
    if (!task)
      error("unable to find task " + task_id);
    
    task[TASK_ATTR][attr] = val;
    save_me();
}


//:FUNCTION remove_attribute
// Remove an attribute of a given task.
nomask void remove_attribute(string task_id, string attr)
{
    array task = find_task(task_id);
    
    if (!attr)
      error("null attribute");
    
    if (!task)
      error("unable to find task " + task_id);
    
    map_delete(task[TASK_ATTR], attr);
    save_me();
}


//:FUNCTION clear_attributes
// Clear all attributes of a given task.
nomask void clear_attributes(string task_id)
{
    array task = find_task(task_id);
    
    if (!task)
      error("unable to find task " + task_id);

    task[TASK_ATTR] = ([ ]);
    save_me();
}


// --------------------------------------------------------------


//:FUNCTION check_completed
// Recursively check an array of tasks and
// return 1 if they are all completed, 0 otherwise.
nomask private int check_completed(array task_list)
{
    int completed = 0;
    
    if (!task_list || sizeof(task_list) == 0)
      return 1;
    
    foreach (array task in task_list)
      if (task[TASK_STATUS] == "completed")
	if (check_completed(task[TASK_SUB]))
	  completed++;
    
    if (completed == sizeof(task_list))
      return 1;
    
    return 0;
}

//:FUNCTION complete_task
// Change status of a task to "completed".
// Only possible if all sub-tasks are also completed.
nomask mixed complete_task(string task_id)
{
    // Error on this, so that we get certainty in our return codes.
    if (!find_task(task_id))
      error("no such task");
    
    if (!check_completed(TASK_D->query_sub_tasks(task_id)))
      return "Sub-tasks not completed.";
    
    set_status(task_id, "completed");

    save_me();
    return 1;
}


// --------------------------------------------------------------


//:FUNCTION find_task
// Given a task id, traverse the task array
// and return the specified task, or 0 if error.
nomask private array find_task(string task_id)
{
    array arr = tasks;
    array t_id, task;
    
    if (!task_id)
      return 0;
    
    t_id = map(explode(task_id, "."), (: to_int($1) :));
    
    if (sizeof(t_id) == 0 || member_array(0, t_id) != -1)
      return 0;
    
    foreach (int n in t_id) {
	if (sizeof(arr) < n)
	  return 0;
	
	task = arr[n-1];
	arr = arr[n-1][TASK_SUB];
    }

    return task;
}


//:FUNCTION resolve_parent_id
// Given a task id, extract the parent id.
// Returns "0" for a top-level task.
nomask string resolve_parent_id(string task_id)
{
    array parent_id = explode(task_id, ".")[<2..<2];

    if (sizeof(parent_id) == 0)
      return "0";
    
    return parent_id[0];
}


//:FUNCTION query_task
// Return a copy of the specified task.
nomask array query_task(string task_id)
{
    return copy(find_task(task_id));
}


//:FUNCTION query_tasks
// Return a copy of the tasks array.
varargs nomask array query_tasks(string task_id)
{
    array task;
    
    if (!task_id) {
	return copy(tasks);
    }
	
    task = find_task(task_id);
    
    if (!task)
      return 0;
    
    return task[TASK_SUB];
}


//:FUNCTION add_task
// Add a new task below the specified parent task.
// Returns the task id of the new task.
string add_task(string parent_id, string title, string description, string who)
{
    array task_list, new_task;
    string new_id;
    
    if (!title || sizeof(title) == 0)
      return 0;
    
    new_task = TASK_ARRAY;
    
    new_task[TASK_TITLE]  = title;
    new_task[TASK_DESC]   = description;
    new_task[TASK_OWNER]  = who;
    new_task[TASK_CTIME]  = time();
    new_task[TASK_STATUS] = "open";
    
    if (parent_id == "0" || parent_id == 0) {
	tasks += ({ new_task });
	new_id = "" + (sizeof(tasks));
    }
    else {
	array task = find_task(parent_id);
    
	if (!task)
	  return 0;
	
	task[TASK_SUB] += ({ new_task });
	new_id = parent_id + "." + (sizeof(task[TASK_SUB]));
    }
    
    save_me();
    return new_id;
}


//:FUNCTION remove_task
// Remove the specified task.
array remove_task(string task_id)
{
    array task = copy(find_task(task_id));
    string parent_id = resolve_parent_id(task_id);

    // This catches problems with task_id being invalid.
    if (!task)
      return 0;

    if (parent_id == "0") {
	int n = to_int(task_id) - 1;
	
	tasks = tasks[0..n-1] + tasks[n+1..<1];
    }
    else {
	array parent = find_task(parent_id);
	array p_sub = parent[TASK_SUB];
	int n = to_int(explode(task_id, ".")[<1]) - 1;
	
	parent[TASK_SUB] = p_sub[0..n-1] + p_sub[n+1..<1];
    }

    save_me();
    return task;
}


