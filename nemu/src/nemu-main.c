#include <common.h>
#define TEST_CMD_P_PATH "./tools/gen-expr/input2"
#define CONFIG_DEBUG 1
void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
void test_cmd_p();


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  engine_start();

  IFDEF(CONFIG_DEBUG, test_cmd_p());

  return is_exit_status_bad();
}

word_t expr(char *e, bool *success);


void test_cmd_p(){
  int test_num = 0;
  int counter = 0;
  int failed_num [100];
  int failed_counter = 0;

  char buffer[65535];
  char *expression;
  FILE *fp = fopen(TEST_CMD_P_PATH, "r");
  assert(fp != NULL);

  char* input = fgets(buffer, ARRLEN(buffer), fp);
  while (input != NULL){
    input[strlen(input) - 1] = '\0'; // 删去末尾'\n'
    uint32_t ans = 0;
    bool success = false;
    char* ans_text = strtok(input, " ");
    sscanf(ans_text, "%u", &ans);
    expression = input + strlen(ans_text) + 1;
    IFDEF(CONFIG_DEBUG, Log("Testing %u %s ...\n", ans, expression));
    uint32_t result = expr(expression, &success);
    printf("%d\n", ++test_num);
    // assert(result == ans);
    if(result == ans){
      printf("passed %d\n",++counter);
    } else {
      failed_num[failed_counter++] = test_num;
    }
    input = fgets(buffer, ARRLEN(buffer), fp);
    // counter += 1;
  } 
  Log("pass%d tests", counter);
  printf("failed tests:\n");
  for(int i =0; i< failed_counter; i += 1){
    printf("%d\n", failed_num[i]);
  }
}
