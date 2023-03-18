#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define BUFF_SIZE 65536
enum  { 
  BLANK, OPERATOR, LEFT_PARENTHESIS, RIGHT_PARENTHESIS
}; 
// this should be enough
static char buf[BUFF_SIZE] = {};
static char input_buf[BUFF_SIZE] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static char* gen_blank(){
  return (rand()%2)?" ":"";
}

static char* gen_rand_op(){
  switch(rand()%4){
    case 0:return "+"; break;
    case 1:return "-"; break;
    case 2:return "*"; break;
    case 3:return "/"; break;
    default: assert(0);
  }
}

static char* gen_num (char *num_str){
  snprintf(num_str, 6, "%uu", rand() % 2048);
  return num_str;
}

static char* generator(int options){
  switch(options){
    case BLANK: return gen_blank();
    case OPERATOR: return gen_rand_op();
    case LEFT_PARENTHESIS: return "(";
    case RIGHT_PARENTHESIS: return ")"; 
    default: assert(0);
  }
}

/* take out 'u' behind every digits to put in test file */
static void input_form (){
  int input_size = 0;
  for(int i = 0; i < strlen(buf); i+= 1){
    if (buf[i] != 'u'){
      input_buf[input_size++] = buf[i];
    }
  }
  input_buf[input_size] = '\0';
}

/* harder than expected... */

static char *gen_rand_expr(char *upper_buf) {
  
  if (upper_buf == buf){
    buf[0] = '\0';
  }

  switch(rand()%3) {
    case 0: 
      return gen_num( upper_buf );
      break;
    case 1:{
      char *temp1;
      temp1 = (char *)malloc(BUFF_SIZE);
      temp1[0] = '\0';
      snprintf(upper_buf, BUFF_SIZE - strlen(upper_buf), "%s%s%s%s%s", 
        generator(LEFT_PARENTHESIS), generator(BLANK), gen_rand_expr(temp1), 
        generator(BLANK), generator(RIGHT_PARENTHESIS));
      free(temp1);
      break;
    }
    default:{
      char *temp1 = (char* )malloc(BUFF_SIZE);
      temp1[0] = '\0';
      char *temp2 = (char* )malloc(BUFF_SIZE);
      temp2[0] = '\0';
      snprintf(upper_buf, BUFF_SIZE - strlen(upper_buf), "%s%s%s%s%s", 
        gen_rand_expr(temp1), generator(BLANK), generator(OPERATOR),
        generator(BLANK), gen_rand_expr(temp2));
      free(temp1);
      free(temp2);
      break;
    }
  }
  return upper_buf;
  //buf[0] = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr(buf);

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr -Wall -Werror");
    if (ret != 0) {
      i -= 1;
      continue;
    }
    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    input_form(); 
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, input_buf);
  }
  return 0;
}
