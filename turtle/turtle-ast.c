#include "turtle-ast.h"
#include "hashmap_procvar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.141592653589793
#define SQRT2 1.41421356237309504880
#define SQRT3 1.7320508075688772935
#define DEV 0 // 1 for dev mode, 0 for prod mode


/*
 * ast constructors
 */

/*
 * expr constructors
 */


// An expression can be a value in our grammar so we need to create a node for it
// This is a constructor for a value expression
struct ast_node *make_expr_value(double value) {
  if (DEV) printf("make_expr_value : %f\n", value);
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  node->children_count = 0;
  return node;  
}

// This is a constructor for a variable expression
struct ast_node *make_expr_name(char* name) {
  if (DEV) printf("make_expr_variable : %s\n", name);
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_NAME;
  node->u.name = name;
  node->children_count = 0;
  return node;
}

struct ast_node *make_expr_binop(char op,struct ast_node* left, struct ast_node* right) {
  if (DEV) printf("make_expr_binop : %c\n", op);
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = op;
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
  return node;
}

struct ast_node *make_expr_color_rbg(double r, double g, double b) {
  if (DEV) printf("make_expr_color : %f, %f, %f\n", r, g, b);
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_COLOR;
  node->u.name = strdup("rgb"); // we use the name field to store the color
  node->children[0] = make_expr_value(r);
  node->children[1] = make_expr_value(g);
  node->children[2] = make_expr_value(b);
  node->children_count = 3;
  return node;
}

struct ast_node *make_expr_color(char* color) {
  if (DEV) printf("make_expr_color : %s\n", color);
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_COLOR;
  node->u.name = color; // we use the name field to store the color
  node->children_count = 1;
  double r = get_color_r(color);
  double g = get_color_g(color);
  double b = get_color_b(color);
  node->children[0] = make_expr_color_rbg(r, g, b);
  return node;
}


struct ast_node *make_expr_neg(struct ast_node* expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_UNOP;
  node->u.op = '-';
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_expr_comma(struct ast_node* left, struct ast_node* right) {
  if (DEV) printf("make_expr_comma\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = ',';
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
  return node;
}

/*Function expr constructor*/

// This is a constructor cos function node
struct ast_node *make_expr_func_cos (struct ast_node* expr) {
  if (DEV) printf("make_cmd_func_cos\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_COS;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}


// This is a constructor sin function node
struct ast_node *make_expr_func_sin (struct ast_node* expr) {
  if (DEV) printf("make_cmd_func_sin\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_SIN;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor tan function node
struct ast_node *make_expr_func_tan (struct ast_node* expr) {
  if (DEV) printf("make_cmd_func_tan\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_TAN;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor sqrt function node
struct ast_node *make_expr_func_sqrt (struct ast_node* expr) {
  if (DEV) printf("make_cmd_func_sqrt\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_SQRT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor random function node
struct ast_node *make_expr_func_random (struct ast_node* expr,struct ast_node* expr2) {
  if (DEV) printf("make_cmd_func_random\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_RANDOM;
  node->children_count = 2;
  node->children[0] = expr;
  node->children[1] = expr2;
  return node;
}

// This is a constructor for a block expression node
struct ast_node *make_expr_block(struct ast_node* expr) {
  if (DEV) printf("make_expr_block\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_EXPR_BLOCK;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

/*
 * movements & rotations cmd constructors
 */

// This is a constructor for a forward command node with an expression as a child
struct ast_node *make_cmd_forward(struct ast_node* expr) {
  if (DEV) printf("make_cmd_forward\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_FORWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor for a backward command node with an expression as a child
struct ast_node *make_cmd_backward(struct ast_node* expr) {
  if (DEV) printf("make_cmd_backward\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_BACKWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor for a left command node with an expression as a child
struct ast_node *make_cmd_left(struct ast_node* expr) {
  if (DEV) printf("make_cmd_left\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_LEFT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor for a right command node with an expression as a child
struct ast_node *make_cmd_right(struct ast_node* expr) {
  if (DEV) printf("make_cmd_right\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_RIGHT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_up() {
  if (DEV) printf("make_cmd_up\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_UP;
  node->children_count = 0;
  return node;
}

struct ast_node *make_cmd_down() {
  if (DEV) printf("make_cmd_down\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_DOWN;
  node->children_count = 0;
  return node;
}

/*
 * Various cmd constructors
 */

// This is a constructor a color command node with an expression as a child
struct ast_node *make_cmd_color(struct ast_node* expr) {
  if (DEV) printf("make_cmd_color\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_COLOR;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_color_rgb(struct ast_node* r,struct ast_node* g, struct ast_node* b){
  if (DEV) printf("make_cmd_color_rbg\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));

  node->kind= KIND_CMD_SIMPLE;
  node->u.cmd = CMD_COLOR;
  node->children_count = 3;/*Function expr constructor*/

  node->children[0] = r;
  node->children[1] = g;
  node->children[2] = b;
  return node;
}


// This is a constructor a print command node with an message to print on stderr
struct ast_node *make_cmd_print(struct ast_node *msg) {
  if (DEV) printf("make_cmd_print\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_PRINT;
  
  node->children_count = 1;
  node->children[0] = msg;
  return node;
}


// This is a constructor a home command node
struct ast_node *make_cmd_home() {
  if (DEV) printf("make_cmd_home\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_HOME;
  node->children_count = 0;
  return node;
}

// This is a constructor a heading command node
struct ast_node *make_cmd_heading(struct ast_node* expr) {
  if (DEV) printf("make_cmd_heading\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_HEADING;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor a position command node
struct ast_node *make_cmd_position(struct ast_node* expr_x, struct ast_node* expr_y) {
  if (DEV) printf("make_cmd_position\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_POSITION;
  node->children_count = 2;
  node->children[0] = expr_x;
  node->children[1] = expr_y;
  return node;
}

/*Complex CMD Constructor*/

struct ast_node *make_cmd_repeat(struct ast_node* expr, struct ast_node* block) {
  if (DEV) printf("make_cmd_repeat\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_REPEAT;
  node->children_count = 2;
  node->children[0] = expr;
  node->children[1] = block;
  return node;
}

struct ast_node *make_cmd_block(struct ast_node* block) {
  if (DEV) printf("make_cmd_block\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_BLOCK;
  node->children_count = 1;
  node->children[0] = block;
  return node;
}

struct ast_node *make_cmd_set(char* name, struct ast_node* expr) {
  if (DEV) printf("make_cmd_set\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_SET;
  node->children_count = 2;
  node->children[0] = make_expr_name(name);
  node->children[1] = expr;
  return node;
}

struct ast_node *make_cmd_call(char* name) {
  if (DEV) printf("make_cmd_call\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_CALL;
  node->children_count = 1;
  node->children[0] = make_expr_name(name);
  return node;
}

struct ast_node *make_cmd_proc(char* name, struct ast_node* block) {
  if (DEV) printf("make_cmd_proc\n");
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  
  node->kind = KIND_CMD_PROC;
  node->children_count = 2;
  node->children[0] = make_expr_name(name);
  node->children[1] = block;
  return node;
}
/*
 * ast destructor
 */

void ast_destroy(struct ast *self) {
  if (self->unit) {
    ast_node_destroy(self->unit);
    free(self->unit);
  }
}

void ast_node_destroy(struct ast_node *self) {
  
  for (int i = 0; i < self->children_count; i++) {
    ast_node_destroy(self->children[i]);
    free(self->children[i]);
  }
  if (self->kind == KIND_EXPR_NAME || self->kind == KIND_EXPR_COLOR ) free(self->u.name);
  if (self->next) {
    ast_node_destroy(self->next);
    free(self->next);
  }
 
}

/*
 * context
 */

// This function initializes the context at the beginning of the program
// The context is a structure that contains the current position of the turtle and its orientation
// The context is used to evaluate the expressions and commands

void context_create(struct context *self) {
  self->x = 0;
  self->y = 0;
  self->angle = 0;
  self->up = 0;

  self->variables = hashmap_procvar_create(10);
  self->procedures = hashmap_procvar_create(10);
  self->consts = hashmap_procvar_create(10);

    //Constants PI, SQRT2, SQRT3
  hashmap_procvar_set_var(self->consts, "PI", PI);
  hashmap_procvar_set_var(self->consts, "SQRT2",SQRT2);
  hashmap_procvar_set_var(self->consts, "SQRT3", SQRT3);
}

// This function destroys the context at the end of the program
void context_destroy(struct context *self) {
  hashmap_procvar_destroy(self->variables);
  hashmap_procvar_destroy(self->procedures);
  hashmap_procvar_destroy(self->consts);
}

/*
 * eval
 */

double ast_node_eval(const struct ast_node *self, struct context *ctx) {
  switch (self->kind) {
    // If the node is a value, we return the value
    case KIND_EXPR_VALUE:
      if (DEV) printf("evaluating value : %f\n", self->u.value);
      return self->u.value;
      break;

    // If the node is a variable, we return the value of the variable
    case KIND_EXPR_NAME:
      if (DEV) printf("evaluating variable : %s\n", self->u.name);
      double data = hashmap_procvar_get_var(ctx->variables, self->u.name);
      if (!isnan(data)) {
        return data;
      }
      data = hashmap_procvar_get_var(ctx->consts, self->u.name);
      if (!isnan(data)) {
        return data;
      }
      ctx->exit_code = 5;
      fprintf(stderr, "Variable %s not found\n", self->u.name);
      exit(ctx->exit_code);
      return NAN;
      
      break;

    
    case KIND_EXPR_BINOP:
      if (DEV) printf("evaluating binop : %c\n", self->u.op);
      double x = ast_node_eval(self->children[0], ctx);
      double y = ast_node_eval(self->children[1], ctx);
      switch (self ->u.op) {
        case '+':
          return x + y;
          break;
        case '-':
          return x - y;
          break;
        case '*':
          return x * y;
          break;
        case '/':
          if (y == 0) {
            ctx->exit_code = 1;
            fprintf(stderr, "Division by zero\n");
            exit(ctx->exit_code);
          }
          return x / y;
          break;
        case '^':{
          double res = pow(x, y);
          if (isnan(res)) {
            ctx->exit_code = 2;
            fprintf(stderr, "Power arguments outside of definition range\n");
            exit(ctx->exit_code);
          }
          return res;
          break;
        }
        default:
          printf("unknown operator\n");
          return NAN;
          break;

      }
      break;
    
    case KIND_EXPR_UNOP:
      if (DEV) printf("evaluating unop : %c\n", self->u.op);
      switch (self ->u.op) {
        case '-':
          return -ast_node_eval(self->children[0], ctx);
          break;
        default:
          printf("unknown operator\n");
          return NAN;
          break;
      }
    break;

    case KIND_EXPR_BLOCK:
      if (DEV) printf("evaluating block\n");
      return ast_node_eval(self->children[0], ctx);
    break;

    case KIND_EXPR_COLOR:
      if (DEV) printf("evaluating color : %s\n", self->u.name);
      if (strcmp(self->u.name, "rgb") == 0) {
        double r = ast_node_eval(self->children[0], ctx);
        double g = ast_node_eval(self->children[1], ctx);
        double b = ast_node_eval(self->children[2], ctx);
        printf("Color %f %f %f\n", r, g, b);
      }
      else {
        ast_node_eval(self->children[0], ctx);
      }
      return NAN;
      break;
    case KIND_EXPR_FUNC:{
      double x = ast_node_eval(self->children[0], ctx);
      switch (self->u.func) {
        case FUNC_SIN:
          if (DEV) printf("evaluating sin\n");
          return sin(x);
          break;
        case FUNC_COS:
          if (DEV) printf("evaluating cos\n");
          return cos(x);
          break;
        case FUNC_TAN:
          if (DEV) printf("evaluating tan\n");
          return tan(x);
          break;
        case FUNC_SQRT:
          if (DEV) printf("evaluating sqrt\n");
          if (x < 0) {
            ctx->exit_code = 3;
            fprintf(stderr, "Negative square root\n");
            exit(ctx->exit_code);
          }
          return sqrt(x);
          break;
        case FUNC_RANDOM:
          if (DEV) printf("evaluating random\n");
          double y = ast_node_eval(self->children[1], ctx);
          if (y < x) {
            ctx->exit_code = 4;
            fprintf(stderr, "Invalid range for random\n");
            exit(ctx->exit_code);
          }
          return drand(x, y);
          break;
        default:
          printf("unknown function\n");
          return NAN;
          break;}
      break;
    }
    case KIND_CMD_SIMPLE:
      switch (self->u.cmd) {
        case CMD_FORWARD:
          if (DEV) printf("evaluating forward\n");
          double res_fw = ast_node_eval(self->children[0], ctx);
          ctx->x += res_fw * cos((ctx->angle-90) * PI / 180);
          ctx->y += res_fw * sin((ctx->angle-90) * PI / 180);
          if (!ctx->up) {
            printf("LineTo %f %f\n", ctx->x, ctx->y);
          } else {
            printf("MoveTo %f %f\n", ctx->x, ctx->y);
          }
          break;
        case CMD_BACKWARD:
          if (DEV) printf("evaluating backward\n");
          double res_bw = ast_node_eval(self->children[0], ctx);
          ctx->x -= res_bw * cos((ctx->angle-90) * PI / 180);
          ctx->y -= res_bw * sin((ctx->angle-90) * PI / 180);
          if (!ctx->up) {
            printf("LineTo %f %f\n", ctx->x, ctx->y);
          } else {
             printf("MoveTo %f %f\n", ctx->x, ctx->y);
          }
          break;
        case CMD_PRINT:
          if (DEV) printf("evaluating print\n");
            double res = ast_node_eval(self->children[0], ctx);
            fprintf(stderr, "%f\n", res);
          break;
        case CMD_POSITION:
          if (DEV) printf("evaluating position\n");
          ctx->x = ast_node_eval(self->children[0], ctx);
          ctx->y = ast_node_eval(self->children[1], ctx);
          if (!ctx->up) {
            printf("LineTo %f %f\n", ctx->x, ctx->y);
          } else {
            printf("MoveTo %f %f\n", ctx->x, ctx->y);
          }
          break;
        case CMD_HEADING:
          if (DEV) printf("evaluating heading\n");
          ctx->angle = floor(ast_node_eval(self->children[0], ctx));
          break;
        case CMD_HOME:
          if (DEV) printf("evaluating home\n");
          ctx->x = 0;
          ctx->y = 0;
          ctx->angle = 0;
          ctx->up = true;
          printf("Color 0.0 0.0 0.0\n");
          printf("MoveTo 0.0 0.0\n");
          break;
        case CMD_UP:
          if (DEV) printf("evaluating up\n");
          ctx->up = true;
          break;
        case CMD_DOWN:
          if (DEV) printf("evaluating down\n");
          ctx->up = false;
          break;
        case CMD_LEFT:
          if (DEV) printf("evaluating left\n");
          ctx->angle -= ast_node_eval(self->children[0], ctx);
          break;
        case CMD_RIGHT:
          if (DEV) printf("evaluating right\n");
          ctx->angle += ast_node_eval(self->children[0], ctx);
          break;
        case CMD_COLOR:
          if (DEV) printf("evaluating color\n");
          if (self->children_count==1){ //KW_COLOR
            ast_node_eval(self->children[0], ctx);
          }
          if (self->children_count==3){ //COLOR RGB DEFINITION
            double r = ast_node_eval(self->children[0], ctx);

            double g = ast_node_eval(self->children[1], ctx);

            double b = ast_node_eval(self->children[2], ctx);

             printf("Color %f %f %f\n", r, g, b);
          }
          
          break;
        default:
          printf("unknown command\n");
          break;
      }
      break;
    case KIND_CMD_CALL:
      if (DEV) printf("evaluating call\n");
      char *name = self->children[0]->u.name;
      struct ast_node *proc = hashmap_procvar_get_proc(ctx->procedures, name);
      
      if (proc) {
        // We evaluate the procedure
        ast_node_eval(proc, ctx);
      } else {
        ctx->exit_code = 6;
        fprintf(stderr, "Procedure %s not found\n", name);
        exit(ctx->exit_code);
      }
      break;
    case KIND_CMD_PROC:
      if (DEV) printf("evaluating proc\n");
      if (hashmap_procvar_get_proc(ctx->procedures, self->children[0]->u.name)) {
        ctx->exit_code =  7;
        fprintf(stderr, "Procedure %s already exists\n", self->children[0]->u.name);
        exit(ctx->exit_code);
      }
      hashmap_procvar_set_proc(ctx->procedures, self->children[0]->u.name, self->children[1]);
      break;
    case KIND_CMD_REPEAT:
      if (DEV) printf("evaluating repeat\n");
      double count = ast_node_eval(self->children[0], ctx);
      for (int i = 0; i < count; i++) {
        ast_node_eval(self->children[1], ctx); 
      }
      
      break;
    case KIND_CMD_BLOCK:
      if (DEV) printf("evaluating block\n");
      ast_node_eval(self->children[0], ctx);
      break;
    case KIND_CMD_SET:
      if (DEV) printf("evaluating set\n");
      double res = ast_node_eval(self->children[1], ctx);
     
      hashmap_procvar_set_var(ctx->variables, self->children[0]->u.name, res);
      break;
    default:
      printf("unknown node kind\n");
      break;
  }
  if (self->next) {
    ast_node_eval(self->next, ctx);
  }
  return NAN;
}



void ast_eval(const struct ast *self, struct context *ctx) {
  if (self->unit) {
    ast_node_eval(self->unit, ctx);
  }
}

/*
 * print
 */



void ast_node_print(const struct ast_node *self){
  switch (self->kind) {
    case KIND_EXPR_VALUE:
      printf("%f", self->u.value);
      break;
    case KIND_EXPR_NAME:
      printf("%s", self->u.name);
      break;
    case KIND_EXPR_COLOR:
      if (strcmp(self->u.name,"rgb")==0) {
        ast_node_print(self->children[0]);
        printf(", ");
        ast_node_print(self->children[1]);
        printf(", ");
        ast_node_print(self->children[2]);
      } else {
        printf("%s", self->u.name);
      }
      break;
    case KIND_EXPR_BINOP:
      ast_node_print(self->children[0]);
      printf(" %c ", self->u.op);
      ast_node_print(self->children[1]);
      break;
    case KIND_EXPR_UNOP:
      printf("%c", self->u.op);
      ast_node_print(self->children[0]);
      break;
    case KIND_CMD_SIMPLE:
      switch (self->u.cmd) {
        case CMD_FORWARD:
          printf("forward ");
          ast_node_print(self->children[0]);
          break;
        case CMD_BACKWARD:
          printf("backward ");
          ast_node_print(self->children[0]);
          break;
        case CMD_LEFT:
          printf("left ");
          ast_node_print(self->children[0]);
          break;
        case CMD_RIGHT:
          printf("right ");
          ast_node_print(self->children[0]);
          break;
        case CMD_UP:
          printf("up ");
          break;
        case CMD_DOWN:
          printf("down ");
          break;
        case CMD_COLOR:
          printf("color ");
          ast_node_print(self->children[0]);
          break;
        case CMD_PRINT:
          printf("print %s", self->children[0]->u.name);
          break;
        case CMD_HOME:
          printf("home");
          break;
        case CMD_HEADING:
          printf("heading ");
          ast_node_print(self->children[0]);
          break;
        case CMD_POSITION:
          printf("position ");
          ast_node_print(self->children[0]);
          printf(" ");
          ast_node_print(self->children[1]);
          break;

        default:
          printf("unknown command\n");
          break;
      }
      break;
    case KIND_CMD_CALL:
      printf("call ");
      ast_node_print(self->children[0]);
      break;
    
    case KIND_CMD_REPEAT:
      printf("repeat ");
      ast_node_print(self->children[0]);
      ast_node_print(self->children[1]);
      break;
    
    case KIND_CMD_BLOCK:
      printf(" { \n");
      ast_node_print(self->children[0]);
      printf(" \n} ");
      break;
    
    case KIND_CMD_SET:
      printf("set ");
      ast_node_print(self->children[0]);
      printf(" ");
      ast_node_print(self->children[1]);
      break;

    case KIND_CMD_PROC:
      printf("proc ");
      ast_node_print(self->children[0]);
      printf(" ");
      ast_node_print(self->children[1]);
      break;
    
    case KIND_EXPR_FUNC:
      switch (self->u.func) {
        case FUNC_SIN:
          printf("sin ");
          ast_node_print(self->children[0]);
          break;
        case FUNC_COS:
          printf("cos ");
          ast_node_print(self->children[0]);
          break;
        case FUNC_TAN:
          printf("tan ");
          ast_node_print(self->children[0]);
          break;
        case FUNC_SQRT:
          printf("sqrt ");
          ast_node_print(self->children[0]);
          break;
        case FUNC_RANDOM:
          printf("random ");
          ast_node_print(self->children[0]);
          break;
        default:
          printf("unknown function\n");
          break;
      
      }
    case KIND_EXPR_BLOCK:
      printf("( ");
      ast_node_print(self->children[0]);
      printf(" )");
      break;
    
    default:
      printf("unknown node kind\n");
      break;
  
  }
  if (self->next) {
    printf("\n");

    ast_node_print(self->next);
  }
}

void ast_print(const struct ast *self) {
  if (self->unit) {
    ast_node_print(self->unit);
    printf("\n");
  }
}


/*
 * color KW evaluation
 */
/*
red 1.0 0.0 0.0
green 0.0 1.0 0.0
blue 0.0 0.0 1.0
cyan 0.0 1.0 1.0
magenta 1.0 0.0 1.0
yellow 1.0 1.0 0.0
black 0.0 0.0 0.0
gray 0.5 0.5 0.5
white 1.0 1.0 1.0
*/
double get_color_r(char *color){
  if (strcmp(color, "red") == 0) return 1;
  if (strcmp(color, "green") == 0) return 0;
  if (strcmp(color, "blue") == 0) return 0;
  if (strcmp(color, "cyan") == 0) return 1;
  if (strcmp(color, "magenta") == 0) return 1;
  if (strcmp(color, "yellow") == 0) return 1;
  if (strcmp(color, "black") == 0) return 0;
  if (strcmp(color, "gray") == 0) return 0.5;
  if (strcmp(color, "white") == 0) return 1;
  return 0;
 }

double get_color_g(char *color){
  if (strcmp(color, "red") == 0) return 0;
  if (strcmp(color, "green") == 0) return 1;
  if (strcmp(color, "blue") == 0) return 0;
  if (strcmp(color, "cyan") == 0) return 1;
  if (strcmp(color, "magenta") == 0) return 0;
  if (strcmp(color, "yellow") == 0) return 1;
  if (strcmp(color, "black") == 0) return 0;
  if (strcmp(color, "gray") == 0) return 0.5;
  if (strcmp(color, "white") == 0) return 1;
  return 0;
 }

double get_color_b(char *color){
  if (strcmp(color, "red") == 0) return 0;
  if (strcmp(color, "green") == 0) return 0;
  if (strcmp(color, "blue") == 0) return 1;
  if (strcmp(color, "cyan") == 0) return 1;
  if (strcmp(color, "magenta") == 0) return 1;
  if (strcmp(color, "yellow") == 0) return 0;
  if (strcmp(color, "black") == 0) return 0;
  if (strcmp(color, "gray") == 0) return 0.5;
  if (strcmp(color, "white") == 0) return 1;
  return 0;
 }

// Function to generate random numbers between low and high - from https://stackoverflow.com/questions/55766058/how-can-i-generate-random-doubles-in-c
double drand ( double low, double high ){
    srand((unsigned int)clock());
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}