#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
  node->children_count = 3;
  node->children[0] = r;
  node->children[1] = g;
  node->children[2] = b;
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
  if (self->kind == KIND_EXPR_NAME || self->kind == KIND_EXPR_COLOR) free(self->u.name);
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
      return NAN;
      break;

    
    case KIND_EXPR_BINOP:
      if (DEV) printf("evaluating binop : %c\n", self->u.op);
      ast_node_eval(self->children[0], ctx);
      ast_node_eval(self->children[1], ctx);
      switch (self ->u.op) {
        case '+':
          return ast_node_eval(self->children[0], ctx) + ast_node_eval(self->children[1], ctx);
          break;
        case '-':
          return ast_node_eval(self->children[0], ctx) - ast_node_eval(self->children[1], ctx);
          break;
        case '*':
          return ast_node_eval(self->children[0], ctx) * ast_node_eval(self->children[1], ctx);
          break;
        case '/':
          return ast_node_eval(self->children[0], ctx) / ast_node_eval(self->children[1], ctx);
          break;
        case '^':
          return pow(ast_node_eval(self->children[0], ctx), ast_node_eval(self->children[1], ctx));
          break;
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
          ctx->x -= res_bw * cos(ctx->angle * PI / 180);
          ctx->y -= res_bw * sin(ctx->angle * PI / 180);
          if (!ctx->up) {
            printf("LineTo %f %f\n", ctx->x, ctx->y);
          } else {
             printf("MoveTo %f %f\n", ctx->x, ctx->y);
          }
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
      ast_node_eval(self->children[0], ctx);
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
      ast_node_eval(self->children[0], ctx);
      ast_node_eval(self->children[1], ctx);
      break;
    case KIND_EXPR_FUNC:
      switch (self->u.func) {
        case FUNC_SIN:
          if (DEV) printf("evaluating sin\n");
          ast_node_eval(self->children[0], ctx);
          break;
        case FUNC_COS:
          if (DEV) printf("evaluating cos\n");
          ast_node_eval(self->children[0], ctx);
          break;
        case FUNC_TAN:
          if (DEV) printf("evaluating tan\n");
          ast_node_eval(self->children[0], ctx);
          break;
        case FUNC_SQRT:
          if (DEV) printf("evaluating sqrt\n");
          ast_node_eval(self->children[0], ctx);
          break;
        case FUNC_RANDOM:
          if (DEV) printf("evaluating random\n");
          ast_node_eval(self->children[0], ctx);
          break;
        default:
          printf("unknown function\n");
          break;
      }
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
        case CMD_COLOR:
          printf("color ");
          ast_node_print(self->children[0]);
          break;
        default:
          printf("unknown command\n");
          break;
      }
      break;
    case KIND_CMD_CALL:
      printf("call ");
      ast_node_print(self->children[0]);
      printf("\n");
      break;
    
    case KIND_CMD_REPEAT:
      printf("repeat ");
      ast_node_print(self->children[0]);
      ast_node_print(self->children[1]);
      break;
    
    case KIND_CMD_BLOCK:
      printf("{ ");
      ast_node_print(self->children[0]);
      printf(" } ");
      break;
    
    case KIND_CMD_SET:
      printf("set ");
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


