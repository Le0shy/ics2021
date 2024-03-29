#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/vaddr.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int print_mem (int words, vaddr_t address);

static int cmd_p(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  /* TODO: Add more commands */
  { "si", "Program pauses after executing N instructions, default [N] is 1", cmd_si},
  { "info", "Print status of registers and information about watchpoint", cmd_info},
  { "x", "scan mem at posi of EXPR, output subsequent N four-bytes in form of hex\
    command format:x N EXPR", cmd_x},
  { "p", "evaluates the EXPR and prints out value, command format: p EXPR", cmd_p}

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args){
  char *arg = strtok(NULL," ");
  uint64_t steps;
  
  if(arg == NULL){
    cpu_exec(1);
  }
  else{
    sscanf(arg, "%lu", &steps);
    cpu_exec(steps);
  }
  return 0;
}

static int cmd_info(char *args){
  char *arg = strtok(NULL, " ");
  if ( arg==NULL || strcmp(arg, "r" )==0){
  isa_reg_display(); 
  } else {
    printf("invalid argument \n");
  }
  return 0;
}

static int cmd_x(char *args){
  if (args == NULL){
    printf("%s - %s\n", cmd_table[5].name, cmd_table[5].description);
    return 0;
  }
  int mem_words;
  vaddr_t address;
  printf("%s\n", args);
  char *arg = strtok(args, " ");
  sscanf(arg, "%d", &mem_words);
  arg = strtok(NULL, " ");
  if (arg == NULL){
    printf("specify the expr/addr need to be scanned\n");
    return 0;
  }
  sscanf(arg, FMT_PADDR, &address);
  
  print_mem(mem_words, address);

  return 0;
}

int print_mem(int words, vaddr_t address){
  vaddr_t word_len = sizeof(word_t);
  vaddr_t offset = 0;
  for(int i = 0; i<words ; i +=1 ){
    offset = word_len*i;
    printf(FMT_PADDR": "FMT_WORD"\n", address+offset, vaddr_read(address+offset,word_len));
  }
  return 0;
}

int cmd_p(char *args){
  //printf("%s\n", args);
  char *arg = strtok(NULL, "");
  if (arg == NULL){
    printf("%s - %s\n", cmd_table[6].name, cmd_table[6].description);
    return 0;
  }
  printf("%s\n", arg);
  bool success = true;
  word_t result = expr (arg, &success);
  printf("%u\n", result);
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return ; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
