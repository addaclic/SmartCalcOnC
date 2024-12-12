#include "SmartCalcOnC.h"

int parse_num(char **str, stack *to_list) {
  int error_code = 0;
  int tochka = 0;
  token temp = {0};
  temp.val_ptr = *str;
  while (isdigit(**str) || **str == '.') {
    if (**str == '.') tochka++;
    ++(*str);
  }
  if (tochka > 1) error_code = 2;
  temp.priority = NUM;
  push_token(to_list, temp);
  return error_code;
}

int make_tokens(char *str, stack *infix_list) {
  int error_code = 0;
  if (strlen(str) > 255)
    error_code = INV_STR;
  else {
    while (*str) {
      if (isdigit(*str) || *str == '.')
        error_code = parse_num(&str, infix_list);
      else if (*str == '+' || *str == '-' || *str == '*' || *str == '/' ||
               *str == '^' || *str == 'x') {
        parse_ops(&str, infix_list);
      } else if (*str == '(' || *str == ')')
        parse_brckts(&str, infix_list);
      else if (*str == 'c' || *str == 's' || *str == 'l' || *str == 'a' ||
               *str == 't' || *str == 'm')
        error_code = parse_func(&str, infix_list);
      else if (*str == ' ') {
        str++;
        continue;
      } else {
        error_code = INV_STR;
        break;
      }
      if (error_code) break;
    }
  }
  return error_code;
}

int sequence_validation(stack *infix_list) {
  int error_code = 0;
  char validation_matrix[][ALL] = {
      {0, 1, 1, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 1, 1, 1, 1, 0, 0},
      {1, 0, 0, 1, 1, 1, 1, 0, 0}, {1, 0, 0, 1, 1, 1, 1, 1, 0},
      {1, 1, 1, 1, 0, 0, 0, 0, 1}, {0, 1, 1, 0, 0, 0, 0, 0, 1},
      {0, 1, 1, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 1, 1, 1, 1, 0, 0},
      {0, 1, 1, 0, 0, 0, 0, 0, 1}};  // validation matrix
  int i = 0;
  int count_brckts = 0;
  while (infix_list->leksems[i].priority) {
    if (infix_list->leksems[i].priority == OPN_BRCKTS) {
      ++count_brckts;
    }
    if (infix_list->leksems[i].priority == CLS_BRCKTS) {
      --count_brckts;
    }
    if (count_brckts < 0) {
      error_code = INV_SQNC;
      break;
    }
    if ((!i && validation_matrix[infix_list->leksems[i].priority][0] == 0) ||
        (i != 0 && validation_matrix[infix_list->leksems[i].priority]
                                    [infix_list->leksems[i - 1].priority] ==
                       0) ||
        (i == infix_list->head &&
         validation_matrix[0][infix_list->leksems[i].priority] == 0)) {
      error_code = INV_SQNC;
      break;
    }
    ++i;
  }
  if (count_brckts) error_code = INV_SQNC;
  return error_code;
}

void parse_ops(char **str, stack *to_list) {
  token temp = {0};
  temp.val_ptr = *str;
  if (**str == '+' || **str == '-') {
    temp.priority = T_ADD;
  } else if (**str == '*' || **str == '/')
    temp.priority = T_MULL;
  else if (**str == '^')
    temp.priority = T_POW;
  else if (**str == 'x')
    temp.priority = DBLX;
  ++(*str);
  push_token(to_list, temp);
}

void parse_brckts(char **str, stack *to_list) {
  token temp = {0};
  temp.val_ptr = *str;
  if (**str == '(') {
    temp.priority = OPN_BRCKTS;
  } else {
    temp.priority = CLS_BRCKTS;
  }
  ++(*str);
  push_token(to_list, temp);
}

void push_token(stack *to_list, token t) {
  to_list->head++;
  to_list->leksems[to_list->head] = t;
}

token pop_token(stack *from_list) {
  token temp = from_list->leksems[from_list->head];
  from_list->head--;
  return temp;
}

token peek_token(stack *from_list) {
  token temp = from_list->leksems[from_list->head];
  return temp;
}

double pop_double(const double *stack_result, int *j) {
  (*j)--;
  double tmp = stack_result[*j];
  return tmp;
}

void make_div(double *stack_result, int *j) {
  double B = pop_double(stack_result, j);
  double A = pop_double(stack_result, j);
  stack_result[*j] = A / B;
  ++(*j);
}

void make_mull(double *stack_result, int *j) {
  double B = pop_double(stack_result, j);
  double A = pop_double(stack_result, j);
  stack_result[*j] = A * B;
  ++(*j);
}

void make_pow(double *stack_result, int *j) {
  double B = pop_double(stack_result, j);
  double A = pop_double(stack_result, j);
  stack_result[*j] = pow(A, B);
  ++(*j);
}

void make_add(double *stack_result, int *j) {
  double B = pop_double(stack_result, j);
  double A = pop_double(stack_result, j);
  stack_result[*j] = A + B;
  ++(*j);
}

void make_sub(double *stack_result, int *j) {
  double B = pop_double(stack_result, j);
  double A = pop_double(stack_result, j);
  stack_result[*j] = A - B;
  ++(*j);
}

void make_cos(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = cos(A);
  ++(*j);
}

void make_sin(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = sin(A);
  ++(*j);
}

void make_tan(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = tan(A);
  ++(*j);
}

void make_ln(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = log(A);
  ++(*j);
}

void make_sqrt(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = sqrt(A);
  ++(*j);
}

void make_log(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = log10(A);
  ++(*j);
}

void make_mod(double *stack_result, int *j) {
  double B = pop_double(stack_result, j);
  double A = pop_double(stack_result, j);
  stack_result[*j] = fmod(A, B);
  ++(*j);
}

void make_atan(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = atan(A);
  ++(*j);
}

void make_acos(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = acos(A);
  ++(*j);
}

void make_asin(double *stack_result, int *j) {
  double A = pop_double(stack_result, j);
  stack_result[*j] = asin(A);
  ++(*j);
}

void get_result(stack *postfix_list, double *result) {
  int i = 0;  // postfix index
  int j = 0;  // double index
  double stack_result[128] = {0};
  while (postfix_list->leksems[i].priority) {
    if (postfix_list->leksems[i].priority == NUM) {
      double tmp = strtod(postfix_list->leksems[i].val_ptr, NULL);
      stack_result[j] = tmp;
      j++;
    } else if (postfix_list->leksems[i].priority == DBLX) {
      stack_result[j] = *result;
      j++;
    } else if (postfix_list->leksems[i].priority > OPN_BRCKTS &&
               postfix_list->leksems[i].priority < CLS_BRCKTS) {
      if (postfix_list->leksems[i].val_ptr[0] == '+') {
        make_add(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == '-') {
        make_sub(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == '*') {
        make_mull(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == '/') {
        make_div(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == '^') {
        make_pow(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'c') {
        make_cos(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 's') {
        make_sin(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 't') {
        make_tan(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'g') {
        make_log(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'n') {
        make_ln(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'm') {
        make_mod(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'a') {
        make_atan(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'i') {
        make_asin(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'o') {
        make_acos(stack_result, &j);
      } else if (postfix_list->leksems[i].val_ptr[0] == 'q') {
        make_sqrt(stack_result, &j);
      }
    }
    i++;
  }
  *result = stack_result[0];
}

void sort_list(stack *infix_list, stack *postfix_list) {
  stack ops_list = {0};
  ops_list.head = -1;
  int i = 0;
  while (infix_list->leksems[i].priority) {
    if (infix_list->leksems[i].priority == NUM ||
        infix_list->leksems[i].priority == DBLX)  // double sorting
      push_token(postfix_list, infix_list->leksems[i]);
    else if (infix_list->leksems[i].priority == OPN_BRCKTS)
      push_token(&ops_list, infix_list->leksems[i]);  // bracket sorting
    else if (infix_list->leksems[i].priority > OPN_BRCKTS &&
             infix_list->leksems[i].priority < CLS_BRCKTS) {
      if ((infix_list->leksems[i].priority == T_ADD && i == 0) ||
          (infix_list->leksems[i].priority == T_ADD && i != 0 &&
           infix_list->leksems[i - 1].priority == OPN_BRCKTS)) {
        token null_token = {"0", NUM};
        push_token(postfix_list, null_token);
      }
      while (!(ops_list.leksems[ops_list.head].priority == T_POW &&
               infix_list->leksems[i].priority == T_POW) &&
             ops_list.head != -1 &&
             ((infix_list->leksems[i].priority <=
               ops_list.leksems[ops_list.head].priority))) {
        push_token(postfix_list, pop_token(&ops_list));  // write ops in postfix
      }
      push_token(&ops_list, infix_list->leksems[i]);  //  ops sorting
    } else if (infix_list->leksems[i].priority == CLS_BRCKTS) {
      while (peek_token(&ops_list).priority != OPN_BRCKTS) {
        push_token(postfix_list, pop_token(&ops_list));
      }
      pop_token(&ops_list);
    }
    i++;
  }
  while (ops_list.head != -1) push_token(postfix_list, pop_token(&ops_list));
}

int valid_token(char *str, int len) {
  int offset_value = -1;
  char func_name[10][4] = {"cos", "sin",  "tan",  "log",  "ln",
                           "mod", "atan", "asin", "acos", "sqrt"};
  const int offset[] = {0, 0, 0, 2, 1, 0, 0, 2, 2, 1};

  for (int i = 0; i < 10; ++i) {
    if (strncmp(str, func_name[i], len) == 0) {
      offset_value = offset[i];
      break;
    }
  }
  return offset_value;
}

int parse_func(char **str, stack *to_list) {
  int error_code = 0;
  token temp = {0};
  temp.val_ptr = *str;
  int tokenlen = 0;
  while (isalpha(**str)) {
    tokenlen++;
    (*str)++;
  }
  int offset_value = valid_token(temp.val_ptr, tokenlen);
  if (offset_value == -1) {
    error_code = INV_FUNC;
  } else {
    temp.val_ptr += offset_value;
  }
  temp.priority = FUNC_T;
  if (temp.val_ptr[0] == 'm') temp.priority = T_MULL;
  push_token(to_list, temp);
  return error_code;
}

int calc_process(stack *postfix_list, graph_data_t *in_out_param) {
  int error_code = 0;
  if (in_out_param->x_max > in_out_param->x_min) {
    int i = 0;
    double step = (in_out_param->x_max - in_out_param->x_min) / STEP;
    while (i < STEP) {
      double result = in_out_param->x_min + step * i;
      in_out_param->x_vals[i] = result;
      get_result(postfix_list, &result);
      in_out_param->y_vals[i] = result;
      ++i;
    }
  } else {
    error_code = INV_PARAMS;
  }
  return error_code;
}

int smartcalc(char *src, double *result) {  // need mode & void result
  int error_code = 0;
  stack infix_list = {0};
  infix_list.head = -1;
  stack postfix_list = {0};
  postfix_list.head = -1;
  error_code = make_tokens(src, &infix_list);
  if (!error_code) {
    error_code = sequence_validation(&infix_list);
    if (!error_code) {
      sort_list(&infix_list, &postfix_list);
      get_result(&postfix_list, result);
    }
  }
  return error_code;
}

int calcgrafics(char *src, graph_data_t *result) {
  int error_code = 0;
  stack infix_list = {0};
  infix_list.head = -1;
  stack postfix_list = {0};
  postfix_list.head = -1;
  error_code = make_tokens(src, &infix_list);
  if (!error_code) {
    error_code = sequence_validation(&infix_list);
    if (!error_code) {
      sort_list(&infix_list, &postfix_list);
      error_code = calc_process(&postfix_list, result);
    }
  }
  return error_code;
}
