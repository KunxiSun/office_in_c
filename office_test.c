#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "office.h"

typedef struct command command_t;

struct command {
	char* str;
	int (*exe)();
};


int test_office_place_1() {
  int pass = 0;
  const int p_match = 2;
  struct office* off = malloc(sizeof(struct office));
  off->department_head = NULL;

  struct employee emp = {
    .name = "Employee1",
    .supervisor = NULL,
    .subordinates = NULL,
    .n_subordinates = 0
  };

  office_employee_place(off, NULL, &emp);
  if(off->department_head != NULL) {
    pass++;
    if(strcmp("Employee1", off->department_head->name) == 0) {
    	pass++;
    } else {
      fputs(off->department_head->name, stdout);
    	fputs("Name does not match expected",
       stdout);
    }
  } else {
  	fputs("Department Head is NULL",
       stdout);
  }

  free(off->department_head->name);
  free(off->department_head);
  free(off);
  return pass == p_match;
}

int test_office_place_2() {
  int pass = 0;
  const int p_match = 3;
  struct office* off = malloc(sizeof(struct office));
  off->department_head = NULL;

  struct employee emp1 = {
    .name = "Employee1",
    .supervisor = NULL,
    .subordinates = NULL,
    .n_subordinates = 0
  };

  office_employee_place(off, NULL, &emp1);
  if(off->department_head != NULL) {
    pass++;
  } else {
    fputs("Department Head is NULL",
       stdout);
  }

  struct employee emp2 = {
    .name = "Employee2",
    .supervisor = NULL,
    .subordinates = NULL,
    .n_subordinates = 0
  };

  office_employee_place(off, NULL, &emp2);
  if(off->department_head->subordinates != NULL) {
    pass++;
    if(strcmp(off->department_head->subordinates[0].name,
      "Employee2") == 0) {

      pass++;
    } else {
      fputs("Object did not match expected", stdout);
    }

  } else {
    fputs("Check if your subordinates list is initialised",
       stdout);
  }

  free(off->department_head->subordinates->name);
  free(off->department_head->subordinates);
  free(off->department_head->name);
  free(off->department_head);
  free(off);
  return pass == p_match;
}


command_t tests[] = {
  { "test_office_place_1", test_office_place_1 },
  { "test_office_place_2", test_office_place_2 }
};


int main(int argc, char** argv) {
  int test_n = sizeof(tests) / sizeof(command_t);
  if(argc >= 2) {
		for(int i = 0; i < test_n; i++) {
			if(strcmp(argv[1], tests[i].str) == 0) {
				if(tests[i].exe()) {
				  fprintf(stdout, "%s Passed\n", tests[i].str);
				} else {
				  fprintf(stdout, "%s Failed\n", tests[i].str);
				}
			}
		}
	}
}