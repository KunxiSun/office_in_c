#include "office.h"

#include <stdio.h>
#include <string.h>

#define BUF_LEN (100)
#define SUB_LEN (10)
#define NAM_LEN (40)

struct employee* find_last_supervisor(struct employee* buf[], int len) {
  if (buf == NULL || len < 0) {
    return NULL;
  }

  struct employee* list[BUF_LEN];

  int i, j;
  int ctr = 0;
  for (i = 0; i < len; i++) {
    if (buf[i]->n_subordinates == 0) {
      return buf[i];
    } else {
      for (j = 0; j < buf[i]->n_subordinates; j++) {
        list[ctr] = &(buf[i]->subordinates[j]);
        ctr++;
      }
    }
  }
  return find_last_supervisor(list, ctr);
}

/**
 * Places an employee within the office, if the supervisor field is NULL
 *  it is assumed the employee will be placed under the next employee that is
 * not superivising any other employee (top-down, left-to-right).
 *
 * If the supervisor is not NULL, it will be added to the supervisor's
 * subordinates list of employees (make sure the supervisor exists in
 * hierarchy). if the office or employee are null, the function not do anything.
 */
void office_employee_place(struct office* off, struct employee* supervisor,
                           struct employee* emp) {
  if (off == NULL || emp == NULL) {
    return;
  }
  // insert first emp to office
  if (off->department_head == NULL) {
    struct employee* head = (struct employee*)malloc(sizeof(struct employee));
    head->name = (char*)malloc(sizeof(char) * NAM_LEN);
    strcpy(head->name, emp->name);
    head->subordinates = NULL;
    head->supervisor = NULL;
    head->n_subordinates = 0;
    off->department_head = head;
  } else {
    // insert more to office
    if (supervisor == NULL) {
      struct employee* buf1[BUF_LEN];
      buf1[0] = off->department_head;
      supervisor = find_last_supervisor(buf1, 1);
    }
    if (supervisor->n_subordinates == 0) {
      supervisor->subordinates =
          (struct employee*)malloc(sizeof(struct employee) * SUB_LEN);
    }
    struct employee sub;
    sub.name = (char*)malloc(sizeof(char) * NAM_LEN);
    strcpy(sub.name, emp->name);
    sub.subordinates = NULL;
    sub.supervisor = supervisor;
    sub.n_subordinates = 0;

    supervisor->subordinates[supervisor->n_subordinates] = sub;
    supervisor->n_subordinates += 1;
  }
}

/**
 * Fires an employee, removing from the office
 * If employee is null, nothing should occur
 * If the employee does not supervise anyone, they will just be removed
 * If the employee is supervising other employees, the first member of that
 *  team will replace him.
 */
void office_fire_employee(struct employee* employee) {
  if (employee == NULL) {
    return;
  }

  int i;
  
  // just remove
  if (employee->n_subordinates == 0) {

    int ctr=-1;
    for(i=0;i<employee->supervisor->n_subordinates;i++){
      if(employee==&(employee->supervisor->subordinates[i])){
        ctr=i;
        break;
      }
  }
    
    for(i=ctr;i<employee->supervisor->n_subordinates-1;i++){
      strcpy(employee->supervisor->subordinates[i].name, employee->supervisor->subordinates[i+1].name);
      (employee->supervisor->subordinates[i].subordinates) = employee->supervisor->subordinates[i+1].subordinates;
    }

    free(employee->supervisor->subordinates[employee->supervisor->n_subordinates-1].name);
    employee->supervisor->subordinates[employee->supervisor->n_subordinates-1].subordinates=NULL;
    employee->supervisor->n_subordinates-=1;
    employee->supervisor=NULL;
  }else{
    //employ has subs
    strcpy(employee->name, employee->subordinates[0].name);
    for(i=0;i<employee->n_subordinates-1;i++){
      strcpy(employee->subordinates[i].name,employee->subordinates[i+1].name);
      employee->subordinates[i].subordinates = employee->subordinates[i+1].subordinates;
    }
    free(employee->subordinates[employee->n_subordinates-1].name);
    employee->subordinates[employee->n_subordinates-1].subordinates=NULL;
    employee->subordinates[employee->n_subordinates-1].supervisor=NULL;
    employee->n_subordinates-=1;
  }
}

struct employee* first_helper(struct employee* buf[], int len,
                              const char* name) {
  if (len < 1) {
    return NULL;
  }

  struct employee* list[BUF_LEN];

  int i, j;
  int ctr = 0;

  for (i = 0; i < len; i++) {
    if (strcmp(buf[i]->name, name) == 0) {
      return buf[i];
    }
    if (buf[i]->n_subordinates != 0) {
      for (j = 0; j < buf[i]->n_subordinates; j++) {
        list[ctr] = &(buf[i]->subordinates[j]);
        ctr++;
      }
    }
  }
  return first_helper(list, ctr, name);
}

/**
 * Retrieves the first encounter where the employee's name is matched to one in
 * the office If the employee does not exist, it must return NULL if office or
 * name are NULL, your function must do nothing
 */
struct employee* office_get_first_employee_with_name(struct office* office,
                                                     const char* name) {
  if (office == NULL || name == NULL) {
    return NULL;
  }

  struct employee* buf[BUF_LEN];
  buf[0] = office->department_head;

  return first_helper(buf, 1, name);
}

void last_helper(struct employee* buf[], struct employee* buf2[], int len,
                 int* number, const char* name) {
  if (len < 1) {
    return;
  }

  struct employee* list[BUF_LEN];

  int i, j;
  int ctr = 0;

  for (i = 0; i < len; i++) {
    if (buf[i]->subordinates !=NULL) {
      for (j = 0; j < buf[i]->n_subordinates; j++) {
        list[ctr] = &(buf[i]->subordinates[j]);
        buf2[(*number)] = &(buf[i]->subordinates[j]);
        (*number)++;
        ctr++;
      }
    }
  }
  last_helper(list, buf2, ctr, number, name);
}

/**
 * Retrieves the last encounter where the employee's name is matched to one in
 * the office If the employee does not exist, it must return NULL if office or
 * name are NULL, your function must do nothing
 */
struct employee* office_get_last_employee_with_name(struct office* office,
                                                    const char* name) {
  if (office->department_head==NULL || office == NULL || name == NULL) {
    return NULL;
  }
 
  struct employee* buf1[BUF_LEN];
  buf1[0] = office->department_head;
  struct employee* buf2[BUF_LEN];
  buf2[0] = office->department_head;
  int number = 1;
  last_helper(buf1, buf2, 1, &number, name);
  for (int i = number - 1; i >= 0; i--) {
    
      if (strcmp(buf2[i]->name, name) == 0) {
        return buf2[i];
      }
  }
  return NULL;
}

void level_helper(struct employee* emp, size_t level, struct employee** emplys,
                  size_t* n_employee) {
  if (emp == NULL) {
    return;
  }
  if (level == 0) {
    struct employee new;
    new.name = (char*)malloc(sizeof(char) * NAM_LEN);
    strcpy(new.name, emp->name);
    new.subordinates = NULL;
    new.supervisor = NULL;
    new.n_subordinates = 0;

    //*emplys = (struct employe*)realloc(*emplys,(*n_employee+1)*sizeof(struct
    // employee));
    (*emplys)[(*n_employee)] = new;
    (*n_employee) += 1;

    return;
  } else {
    for (size_t i = 0; i < emp->n_subordinates; i++) {
      level_helper(&(emp->subordinates[i]), level - 1, emplys, n_employee);
    }
  }
}

/**
 * This function will need to retrieve all employees at a level.
 * A level is defined as distance away from the boss. For example, all
 * subordinates of the boss are 1 level away, subordinates of the boss's
 * subordinates are 2 levels away.
 *
 * if office, n_employees or emplys are NULL, your function must do nothing
 * You will need to provide an allocation for emplys and specify the
 * correct number of employees found in your query.
 */
void office_get_employees_at_level(struct office* office, size_t level,
                                   struct employee** emplys,
                                   size_t* n_employees) {
  if (n_employees == NULL || emplys == NULL || office == NULL) {
    return;
  }

  *emplys = (struct employee*)malloc(BUF_LEN * sizeof(struct employee));

  level_helper(office->department_head, level, emplys, n_employees);
}

void name_helper(struct employee* buf[], int len, const char* name,
                 struct employee** emplys, size_t* n_employees) {
  if (len < 1) {
    return;
  }

  struct employee* list[BUF_LEN];

  int i, j;
  int ctr = 0;

  for (i = 0; i < len; i++) {
    if (strcmp((buf[i]->name), name) == 0) {
      struct employee new;
      new.name = (char*)malloc(sizeof(char) * NAM_LEN);
      strcpy(new.name, buf[i]->name);
      new.subordinates = NULL;
      new.supervisor = NULL;
      new.n_subordinates = 0;

      (*emplys)[(*n_employees)] = new;
      (*n_employees) += 1;
    }

    if (buf[i]->subordinates != NULL) {
      for (j = 0; j < (buf[i]->n_subordinates); j++) {
        list[ctr] = &(buf[i]->subordinates[j]);
        ctr++;
      }
    }
  }

  return name_helper(list, ctr, name, emplys, n_employees);
}

/**
 * Will retrieve a list of employees that match the name given
 * If office, name, emplys or n_employees is NULL, this function should do
 * nothing
 * if office, n_employees, name or emplys are NULL, your function must do
 * nothing.
 * You will need to provide an allocation to emplys and specify the
 * correct number of employees found in your query.
 */
void office_get_employees_by_name(struct office* office, const char* name,
                                  struct employee** emplys,
                                  size_t* n_employees) {
  if (n_employees == NULL || name == NULL || emplys == NULL || office == NULL) {
    return;
  }

  *emplys = (struct employee*)malloc(BUF_LEN * sizeof(struct employee));

  struct employee* buf[BUF_LEN];
  buf[0] = office->department_head;
  name_helper(buf, 1, name, emplys, n_employees);
}

void postorder_helper(struct employee* emp, struct employee** emplys,
                      size_t* n_employees) {
  if (emp == NULL) {
    return;
  } else {
    int i;
    for (i = 0; i < emp->n_subordinates; i++) {
      postorder_helper(&(emp->subordinates[i]), emplys, n_employees);
    }

    struct employee new;
    new.name = (char*)malloc(sizeof(char) * NAM_LEN);
    strcpy(new.name, emp->name);
    new.subordinates = NULL;
    new.supervisor = NULL;
    new.n_subordinates = 0;

    (*emplys)[(*n_employees)] = new;
    (*n_employees) += 1;
  }
}

/**
 * You will traverse the office and retrieve employees using a postorder
 * traversal If off, emplys or n_employees is NULL, this function should do
 * nothing
 *
 * You will need to provide an allocation to emplys and specify the
 * correct number of employees found in your query.
 */
void office_get_employees_postorder(struct office* off,
                                    struct employee** emplys,
                                    size_t* n_employees) {
  if (n_employees == NULL || emplys == NULL || off == NULL) {
    return;
  }

  *emplys = (struct employee*)malloc(BUF_LEN * sizeof(struct employee));

  postorder_helper(off->department_head, emplys, n_employees);
}

/**
 * The employee will be promoted to the same level as their supervisor and will
 *  join their supervisor's team.
 * If the employee has members on their team, the first employee from that team
 *   will be promoted to manage that team.
 * if emp is NULL, this function will do nothing
 * if the employee is at level 0 or level 1, they cannot be promoted
 */
void office_promote_employee(struct employee* emp) {}

/**
 * Demotes an employee, placing them under the supervision of another employee.
 * If supervisor or emp is null, nothing should occur
 * If the employee does not supervise anyone, they will not be demoted as they
 *  are already at the lowest position
 * If an employee is to be demoted but their new distance from the boss is less
 *  than the previous position, nothing will happen.
 * Otherwise, the employee should be assigned at the end the supervisor's team
 *  and the first employee from the previously managed team will be promoted.
 *
 * Edge case:
 * if the supervisor use to be an subordinate to the demoted employee
 *   (they will get promoted)
 * the demoted employee will be attached to subordinate's new subordinate's
 *   list not their previous list.
 */
void office_demote_employee(struct employee* supervisor, struct employee* emp) {

}

void office_disband_helper(struct employee* current) {
  if (current == NULL) {
    return;
  }

  for (int i = 0; i < current->n_subordinates; i++) {
    office_disband_helper(&current->subordinates[i]);
  }

  if (current->subordinates != NULL) {
    free(current->subordinates);
  }
  free(current->name);
}

/**
 * The office disbands
 * (You will need to free all memory associated with employees attached to
 *   the office and the office itself)
 */
void office_disband(struct office* office) {
  office_disband_helper(office->department_head);
  free(office->department_head);
  free(office);
}
