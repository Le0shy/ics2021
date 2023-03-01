#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
enum {
  TK_NOTYPE = 256, 
  /* TODO: Add more token types */
  TK_AND,     //and "&&"
  TK_EQ,      //equal, "=="
  TK_NEQ,     //not equal "!="
  TK_PLU,     //plus "+"
  TK_MIN,     //minus "-"
  TK_MUL,     //multiply "*"
  TK_DIV,     //divide "/"
  TK_DEREF,   //dereference "*"
  TK_LPR,     //left parenthesis "("
  TK_RPR,     //right parenthesis ")"
  TK_DEC_DIG, //decimal digits "[0-9]+"
  TK_HEX_DIG, //hexadecimal digits "0x[0-9A-F]"
  TK_REG,     //register name "$.."
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"( +|\")", TK_NOTYPE},    // spaces 
  {"\\+", TK_PLU},       // plus
  {"==", TK_EQ},        // equal
  {"\\-", TK_MIN},       // minus
  {"\\*", TK_MUL},       // multiply (dereference)
  {"\\/", TK_DIV},       // divide
  {"\\(", TK_LPR},       // right parenthesis
  {"\\)", TK_RPR},       // left parenthesis
  {"(0$|[1-9][0-9]*)", TK_DEC_DIG}, // decimal digits
  {"\\$(0|ra|(s|g|t)p|t[0-3]|a[0-7]|s([0-9]|1[01]))", TK_REG},       //  register sign
  {"0x[0-9A-Fa-f]+", TK_HEX_DIG}    // hex digits
};

#define NR_REGEX ARRLEN(rules)
#define STACK_CAP 256
#define TOKENS_SIZE 4096
word_t vaddr_read(vaddr_t addr, int len);
static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;


static Token tokens[TOKENS_SIZE] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

/* clear tokens after using one time */
static void clear_tokens (){
  for (int i = 0; i < nr_token; i += 1){
    tokens[i].type = 0;
    memset(tokens[i].str, 0, 32);
  }
  nr_token = 0;
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          //default: TODO();
            assert(substr_len<=32);
          
          case TK_DEC_DIG:
          case TK_REG:
          case TK_HEX_DIG:
          // the usage of case: !
            tokens[nr_token].type = rules[i].token_type;
            memcpy(tokens[nr_token++].str, substr_start, substr_len);
            printf("%d\n", nr_token);
            break;
          // skip spaces
          case TK_NOTYPE:
            break;
          default: 
            tokens[nr_token++].type = rules[i].token_type;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  // printf("%d\n", nr_token);
  return true;
}

static word_t power_ten(int power){
  if (power == 0) return 1;
  return 10*power_ten(power - 1);
}
/* strtol() could be used here */
static word_t str2word_t(char * str){
  int len = strlen(str);
  word_t res = 0;
  for(int i = 0; i<len ; i += 1){
    assert(str[i]>= '0' && str[i] <= '9');
    res += (word_t)(str[i] - '0')*power_ten(len - i -1);
  }
  return res;
}

typedef struct {
    int array [STACK_CAP];
    int size;
}s_stack;

/* pop an element if stack is empty return NULL */
static int pop(s_stack* stack){
  stack->size -= 1;
  if (stack->size < 0) return 0;
  int val = stack->array[stack->size];
  return val;
}

/* push an element, assert if size > length of inner array */
static int push(s_stack* stack, int val){
  assert(stack->size < STACK_CAP);
  stack->array[stack->size] = val;
  stack->size += 1;
  return val;
}

/* If expression is surrounded by a pair of parenthesis, return true, 
otherwise return ture. If expression does not meet the standard of a legitimate expression
then assign *check_expr false.*/
static bool check_parenthesis(int start_pos, int end_pos, bool *check_expr){
  bool flag;
  // int substr_len = end_pos - start_pos + 1;
  s_stack stack= {
    { 0 },
    0
  };

  switch(tokens[start_pos].type){
    case TK_LPR: 
      push(&stack, TK_LPR);
      flag = true; 
      break;
    case TK_RPR:
      *check_expr = false;
      return false;
    default:
      flag = false;
  }
  for (int i = start_pos + 1; i <= end_pos; i += 1){
    if (tokens[i].type == TK_LPR){
      push(&stack, TK_LPR); 
    } 
    else if (tokens[i].type == TK_RPR){
        if(stack.size == 0){
          *check_expr = false;
          return false;
        }
        else if(stack.size == 1 && i <end_pos){
          flag = false;
        }
        pop(&stack);
      }
  }
  if (stack.size == 0) *check_expr = true;
  return flag;
}

/* Determine the main operator (has the lowest precedence level) */
static int main_operator_posi (int start_pos, int end_pos, int* op_type){
  int present_op_type = 0, present_op = 0;
  s_stack stack = {
    { 0 },
    0
  };

  while (start_pos <= end_pos){
    /* Parenthesis has the highest precedence level */
    if (tokens[start_pos].type == TK_LPR){
      push(&stack, TK_LPR);
      for(start_pos +=1 ; stack.size != 0; start_pos +=1){
        if (tokens[start_pos].type == TK_LPR){
          push(&stack, TK_LPR);
        } 
        else if (tokens[start_pos].type == TK_RPR){
            pop(&stack);
          }
      }
    }

    else if (tokens[start_pos].type < TK_LPR){
      /* First operator as initial main operator */
      if (present_op_type == 0){
        present_op = start_pos;
        present_op_type = tokens[start_pos].type;
      } 

      else {
        // if (present_op_type < TK_MUL && tokens[start_pos].type >= TK_MUL){
        //   } else {
        //     present_op = start_pos;
        //     present_op_type = tokens[start_pos].type;
        //   }
        switch(tokens[start_pos].type){
          case TK_AND:
          if (TK_AND <= present_op){
            present_op = start_pos;
            present_op_type = TK_AND;
          }
          break;
          case TK_EQ:
          case TK_NEQ:
          if (TK_EQ <= present_op){
            present_op = start_pos;
            present_op_type = tokens[start_pos].type;
          }
          break;
          case TK_PLU:
          case TK_MIN:
          if (TK_PLU <= present_op){
            present_op =start_pos;
            present_op_type = tokens[start_pos].type;
          } 
          break;
          case TK_MUL:
          case TK_DIV:
          if (TK_MUL <= present_op){
            present_op =start_pos;
            present_op_type = tokens[start_pos].type;
          } 
          break;
          default:
          assert(0);
        }
        }
      start_pos += 1;
    } 
    else {
      start_pos += 1;
    }
  }
  * op_type = present_op_type;
  return present_op;
}

static word_t evaluate(int start_pos, int end_pos, bool* check_expr){
  if (start_pos > end_pos) {
    *check_expr = false;
    return 0;
  }

  /* op: position of main operator, 
     op_type: type of operator */
  int op, op_type;
  char *endptr;
  bool success = 0;
  
  /* hex, decimal number and register token */
  if (start_pos == end_pos) {
    if (tokens[start_pos].type == TK_DEC_DIG)
    return str2word_t(tokens[start_pos].str);
    else if (tokens[start_pos].type == TK_HEX_DIG){
      errno = 0;
      unsigned long val = strtoul(tokens[start_pos].str, &endptr, 16);
      if ((errno == ERANGE && val == ULONG_MAX) || ( errno == 0 && val > (unsigned long)UINT_MAX)
      || (errno !=0 && val ==0)) {
        perror("strtoul");
        exit(EXIT_FAILURE);
      }
      return (word_t)(val);
    }

    else if (tokens[start_pos].type == TK_REG)
    return isa_reg_str2val (tokens[start_pos].str+1, &success);
    /* No other possible type*/
    else {
      *check_expr = false;
      return 0;
    }
  }
 
  else if (check_parenthesis(start_pos, end_pos, check_expr) == true){
    return evaluate(start_pos + 1, end_pos - 1, check_expr);
  }
  else if (*check_expr == false) return 0 ;

  else {
    op = main_operator_posi (start_pos, end_pos, &op_type);
    /* If the main operator is dereference*/
    if (op_type == TK_DEREF){
      word_t val = evaluate(op+1, end_pos, check_expr);
      if (val == 0 && *check_expr == false) return 0;
      return vaddr_read(val, sizeof(word_t));
    }
    word_t val1 = evaluate (start_pos, op-1, check_expr);
    if (val1 == 0 && *check_expr == false) return 0;
    word_t val2 = evaluate (op+1, end_pos, check_expr);
    if (val2 == 0 && *check_expr == false) return 0;

    switch(op_type){
      case TK_PLU: return val1 + val2;
      case TK_MIN: return val1 - val2;
      case TK_MUL: return val1 * val2;
      case TK_DIV: return val1 / val2;
      case TK_AND: return val1 && val2;
      case TK_EQ:  return val1 == val2;
      case TK_NEQ: return val1 != val2;
      //case TK_DEREF: return vaddr_read(val2, sizeof(word_t));
      default: assert(0);
    }
  }
}

word_t expr(char *e, bool *success) {
  clear_tokens();
  if (!make_token(e)) {
    *success = false;
    printf("false to lexical analysis:invalid character\n");
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
  for (int i = 0; i < nr_token ; i += 1){
    if (tokens[i].type == TK_MUL && (i == 0 || tokens[i-1].type < TK_RPR)){
      tokens[i].type = TK_DEREF;
    }
  }
  printf("success\n");
  
  word_t result;
  bool check_expr = success;
  result = evaluate(0, nr_token - 1, &check_expr);
  if (result == 0 && check_expr == false){
    *success = false;
    printf("invalid expression\n");
    return 0;
  }
  return result;
}
