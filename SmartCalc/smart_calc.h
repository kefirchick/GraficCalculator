#ifndef SMART_CALC_H
#define SMART_CALC_H

typedef struct {
    int n;
    char tokens[256][256];
} tokens;

int eq_button_handle(const char *input, char *result);
int plot_button_handle(const char *input, double *y_arr, int begin, int end);
int get_point(const char *input_string, double *result);
void prepare_input(const char *input, char *new_str);
int to_tokens(const char *input_string,  tokens *t);
int parse_number(const char *input_string, tokens *t, int i);
int parse_func(const char *input_string, tokens *t, int *i);
void tokens_push(tokens *t, char *str);
void tokens_pop(tokens *t, char *str);
void tokens_clean(tokens *t);
int kurwa(tokens *t, tokens *polish);
int calculate(const char *input_string, char *result);
void calc_func(char *func, tokens *t);
void calc_op(char *op, tokens *s);
void replace_x(const char *input, double x_value, char *new_str);

#endif // SMART_CALC_H
