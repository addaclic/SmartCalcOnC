#ifndef BACKEND_SMARTCALCONC_H
#define BACKEND_SMARTCALCONC_H

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define OPSCHAR "+-*/^"
#define FUNCCHAR "cslat"
#define STEP 10000

enum errors { OK, INV_STR, INV_NUM, INV_FUNC, INV_SQNC, INV_PARAMS };

enum priority {
  NULL_T,
  NUM,
  DBLX,
  OPN_BRCKTS,
  T_ADD,
  T_MULL,
  T_POW,
  FUNC_T,
  CLS_BRCKTS,
  ALL
};

typedef struct {
  char *val_ptr;
  short priority;
} token;

typedef struct {
  token leksems[255];
  int head;
} stack;

typedef struct {
  double x_min;
  double x_max;
  double x_vals[STEP];
  double y_vals[STEP];
} graph_data_t;

int smartcalc(char *, double *);
int calcgrafics(char *, graph_data_t *);

int make_tokens(char *, stack *);

int parse_num(char **, stack *);
void parse_ops(char **, stack *);
int parse_func(char **, stack *);
void parse_brckts(char **, stack *);

int sequence_validation(stack *infix_list);

void sort_list(stack *from_list, stack *to_list);

int calc_process(stack *, graph_data_t *);

void get_result(stack *, double *);

double pop_double(const double *, int *);
void make_add(double *, int *);
void make_sub(double *, int *);
void make_pow(double *, int *);
void make_mull(double *, int *);
void make_div(double *, int *);
void make_cos(double *, int *);
void make_sin(double *, int *);
void make_tan(double *, int *);
void make_sqrt(double *, int *);
void make_ln(double *, int *);
void make_log(double *, int *);
void make_mod(double *, int *);
void make_atan(double *, int *);
void make_asin(double *, int *);
void make_acos(double *, int *);

void push_token(stack *, token);
token pop_token(stack *);
token peek_token(stack *);

#endif  // BACKEND_SMARTCALCONC_H