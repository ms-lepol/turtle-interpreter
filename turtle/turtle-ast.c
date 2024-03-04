#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793


/*
 * ast constructors
 */

/*
 * expr constructors
 */


// An expression can be a value in our grammar so we need to create a node for it
// This is a constructor for a value expression
struct ast_node *make_expr_value(double value) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  return node;
}

struct ast_node *make_expr_sum(struct ast_node* left, struct ast_node* right) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = '+';
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
  return node;
}

struct ast_node *make_expr_sub(struct ast_node* left, struct ast_node* right) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = '-';
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
  return node;
}

struct ast_node *make_expr_mul(struct ast_node* left, struct ast_node* right) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = '*';
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
  return node;
}

struct ast_node *make_expr_div(struct ast_node* left, struct ast_node* right) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = '/';
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
  return node;
}

struct ast_node *make_expr_pow(struct ast_node* left, struct ast_node* right) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = '^';
  node->children_count = 2;
  node->children[0] = left;
  node->children[1] = right;
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
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = CMD_FORWARD;
  node->u.cmd = CMD_FORWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor for a backward command node with an expression as a child
struct ast_node *make_cmd_backward(struct ast_node* expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = CMD_BACKWARD;
  node->u.cmd = CMD_BACKWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor for a left command node with an expression as a child
struct ast_node *make_cmd_left(struct ast_node* expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = CMD_LEFT;
  node->u.cmd = CMD_LEFT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

// This is a constructor for a right command node with an expression as a child
struct ast_node *make_cmd_right(struct ast_node* expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = CMD_RIGHT;
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
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = CMD_COLOR;
  node->u.cmd = CMD_COLOR;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}


/*
 * ast destructor
 */

void ast_destroy(struct ast *self) {
  if (self->unit) {
    ast_node_destroy(self->unit);
  }
}

void ast_node_destroy(struct ast_node *self) {
  int i = 0;
  while(self->children[i]) {
    ast_node_destroy(self->children[i]);
    ++i;
  }
  free(self);
}

/*
 * context
 */

void context_create(struct context *self) {

}

/*
 * eval
 */

void ast_eval(const struct ast *self, struct context *ctx) {

}

/*
 * print
 */

void ast_print(const struct ast *self) {

}
