#include "sdb.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

/* Number of watchpoints */
static int number = 0;
/* buffer for address of watchpoints to display*/
//static WP* wp_buffer[NR_WP] = {};

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *expr_, bool *success) {
  if (free_->next == NULL) {
    *success = false;
    Log ("Unable to add more watchpoints!");
    return NULL;
  }
  

  
  /* Copy string of expression */
  //memcpy(head->expr, expr, strlen(expr)+1);
  word_t res = expr(expr_, success);

  if(*success == true) {
    /* Allocate a watchpoint from "free_" to "head" */
    WP* old_head = head;
    WP* old_free_ = free_;
    free_ = free_->next;
    head = old_free_;
    head->next = old_head;

    strcpy(head->expr, expr_);
    Log("Watchpoint expression: %s", head->expr);
    head->result = res;
    number += 1;
    *success = true;
  }
  
  return head;
}

void free_wp(unsigned seq, bool *success) {
  if(number == 0) {
    Log("No more watchpoints!");
    *success = false;
    return;
  }

  
  WP* prev = head;
  if(number == 1){
    if ( head->NO != seq ){
      *success =  false;
      Log("Invalid number of watchpoints");
    }
    else if (head->NO == seq) {
      head->next = free_->next;
      free_ = head;
      head = NULL;
      number -= 1;
      *success = true;
    }
    return;
  }

  if (head ->NO == seq){
    WP* new_free = head;
    head = head->next;

    new_free->next = free_;
    free_ = new_free;
    number -= 1;
    *success = true;
    return;
  }


  for (int i = 0; i < number - 1; i += 1){
    if (prev->next->NO == seq) break;
    prev = prev->next;
  }
  
  WP* new_free = prev->next;
  prev->next = prev->next->next;

  new_free->next = free_;
  free_ = new_free;
  number -= 1; 
  *success =true;

  return;
}

bool check_watchpoints(){
  WP* iter = head;
  bool success;
  bool check = false;
  //int buffer_size = 0;
  /* Save value after execution */
  word_t res;

  for(; iter!= NULL; iter = iter->next){
    if ((res = expr(iter->expr, &success))!= iter->result){
      Log("%s :", iter->expr);
      printf("Old value = %u \n", iter->result);
      printf("New value = %u \n", res);
      //wp_buffer[buffer_size] = iter;
      //buffer_size += 1;
      iter->result = res;
      check = success;
    }
  }
  return check;
}

void display_watchpoints(){
  
  return;
}

void info_watchpoints(){
  if (number == 0) {
    Log("No enable watchpoints");
    return;
  }
  printf("Num  Value       What\n");
  WP* cur = head;
  for(int i = 0; i < number; i += 1){
    printf("%-5d%-12u%-s\n", cur->NO, cur->result, cur->expr);
    cur = cur->next;
  }
  return;
}