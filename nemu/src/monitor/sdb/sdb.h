#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

word_t expr(char *e, bool *success);

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  char expr[64];
  word_t result;
} WP;

WP* new_wp (char* expr, bool *success);

void free_wp (unsigned seq, bool *success);

bool check_watchpoints();

void display_watchpoints();

void info_watchpoints();
#endif
