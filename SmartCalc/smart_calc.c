#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "smart_calc.h"

char funcs[10][5] = {
    "mod",
    "cos",
    "sin",
    "tan",
    "acos",
    "asin",
    "atan",
    "sqrt",
    "ln",
    "log"
};

// обработчик кнопки =
int eq_button_handle(const char *input, char *result) {
    char prepared[256] = "";

    prepare_input(input, prepared);
    int ret = calculate(prepared, result);

    return ret;
}

// обработчик унопки plot
int plot_button_handle(const char *input, double *y_arr, int begin, int end) {
    int ret = 1;
    char prepared[256] = "";
    char x_replaced[256] = "";
    double step = fabs((double)end - (double)begin)/256.;
    double x_value = (double)begin;

    for (int i = 0; i < 256; i++) {
        replace_x(input, x_value, x_replaced);
        prepare_input(x_replaced, prepared);
        ret = ret && get_point(prepared, &y_arr[i]);
        x_value += step;
    }

    return ret;
}

// заменяет x на число
void replace_x(const char *input, double x_value, char *new_str) {
    int j = 0, k = 0;
    char x_str[256] = "";

    snprintf(x_str, sizeof(x_str), "%lf", x_value);

    for (int i = 0; i < (int)strlen(input); i++) {
        if (input[i] == 'x') {
            while  (x_str[k]) new_str[j++] = x_str[k++];
            k = 0;
            i++;
        }

        new_str[j++] = input[i];
    }
    new_str[j] = '\0';
}

// убирает пробелы, переводы строки, добавляет 0 перед унарным -
void prepare_input(const char *input, char *new_str) {
    int j = 0;

    for (int i = 0; i < (int)strlen(input); i++) {
        if (i == 0 || (i > 0 && !strchr("x).0123456789", input[i - 1]))) {
            if (input[i] == '+') continue;
            if (input[i] == '-') new_str[j++] = '0';
        }

        if (strchr(" \n", input[i])) continue;

        new_str[j++] = input[i];
    }
    new_str[j] = '\0';
}

// выдает результат выражения в строку result
int calculate(const char *input_string, char *result) {
    tokens t = {-1, {{'\0'}}};
    tokens s = {-1, {{'\0'}}};
    tokens p = {-1, {{'\0'}}};
    int ret = 1;

    if (to_tokens(input_string, &t) && kurwa(&t, &p)) {
        for (int i = 0; i <= p.n; i++) {
            if (p.tokens[i][0] == 'x') ret = 0;
            if (strchr(".0123456789", p.tokens[i][0])) {
                tokens_push(&s, p.tokens[i]);
            } else if (strchr("cstal", p.tokens[i][0])) {
                calc_func(p.tokens[i], &s);
            } else {
                calc_op(p.tokens[i], &s);
            }
        }
    } else {
        ret = 0;
    }

    if (s.n) ret = 0;
    snprintf(result, sizeof(s.tokens[0]), "%s", s.tokens[0]);

    return ret;
}

// рассчитывает значение точки графика
int get_point(const char *input_string, double *result) {
    tokens t = {-1, {{'\0'}}};
    tokens s = {-1, {{'\0'}}};
    tokens p = {-1, {{'\0'}}};
    int ret = 1;

    if (to_tokens(input_string, &t) && kurwa(&t, &p)) {
        for (int i = 0; i <= p.n; i++) {
            if (strchr(".0123456789", p.tokens[i][0])) {
                tokens_push(&s, p.tokens[i]);
            } else if (strchr("cstal", p.tokens[i][0])) {
                calc_func(p.tokens[i], &s);
            } else {
                calc_op(p.tokens[i], &s);
            }
        }
    } else {
        ret = 0;
    }

    if (s.n) ret = 0;
    sscanf(s.tokens[0], "%lf", result);

    return ret;
}

// делит строку на токены
int to_tokens(const char *input_string,  tokens *t) {
    int result = 1;
    int i = 0;
    char c = 0;

    while ((c = input_string[i]) != 0) {
        if (strchr("()+-*/^x", c)) {
            t->n++;
            t->tokens[t->n][0] = c;
        } else if (strchr(".0123456789", c)) {
            i = parse_number(input_string, t, i);
        } else if (c >= 'a' && c <= 'z') {
            if (!parse_func(input_string, t, &i)) result = 0;
        } else {
            result = 0;
        }
        i++;
        if (!result) break;
    }

    return result;
}

// пишет число из строки с позиции i в структуру tokens
int parse_number(const char *input_string, tokens *t, int i) {
    int j = 0;
    char numb[256] = "";
    char c = input_string[i];

    while (c && strchr(".0123456789", c)) {
        numb[j] = c;
        j++;
        c = input_string[i + j];
    }

    tokens_push(t, numb);

    return i + j - 1;
}

// пишет функцию из строки с позиции i в структуру tokens
int parse_func(const char *input_string, tokens *t, int *i) {
    char func[5] = "";
    int result = 1;

    for (int j = 0; j < 10; j++) {
        for (int k = 0; k < (int)strlen(funcs[j]); k++) {
            if (input_string[*i + k] != funcs[j][k]) break;
            if (k == (int)strlen(funcs[j]) - 1) snprintf(func, sizeof(func), "%s", funcs[j]);
        }
    }

    if (strcmp(func, "")) {
        tokens_push(t, func);
    } else {
        result = 0;
    }

    *i += strlen(func) - 1;

    return result;
}

// переводит в обратную польскую запись
int kurwa(tokens *t, tokens *p) {
    tokens s = {-1, {{'\0'}}};
    char token[256] = "";
    tokens_clean(p);
    int result = 1;

    for (int i = 0; i <= t->n; i++) {
        if (strchr("x.0123456789", t->tokens[i][0])) tokens_push(p, t->tokens[i]);
        if (strchr("(cstal", t->tokens[i][0])) tokens_push(&s, t->tokens[i]);
        if (strchr("+-*/^m", t->tokens[i][0])) {
            while (strchr("+-", t->tokens[i][0]) && s.n >= 0 && strchr("*/^m", s.tokens[s.n][0])) {
                tokens_pop(&s, token);
                tokens_push(p, token);
            }
            tokens_push(&s, t->tokens[i]);
        }
        if (')' == t->tokens[i][0]) {
            while (s.n >= 0 && s.tokens[s.n][0] != '(') {
                tokens_pop(&s, token);
                tokens_push(p, token);
            }
            if (s.n >= 0 && s.tokens[s.n][0] == '(') {
                tokens_pop(&s, token);
            } else {
                result = 0;
            }
            if (s.n >= 0 && strchr("cstal", s.tokens[s.n][0])) {
                tokens_pop(&s, token);
                tokens_push(p, token);
            }
        }
    }

    for (int i = s.n; i >= 0; i--) {
        if (strchr("()", s.tokens[s.n][0])) result = 0;
        tokens_pop(&s, token);
        tokens_push(p, token);
    }

    return result;
}

// вычисляет функцию
void calc_func(char *func, tokens *s) {
    double number;
    char numb_str[256] = "";
    char result[256] = "";

    tokens_pop(s, numb_str);
    sscanf(numb_str, "%lf", &number);

    if (!strcmp(func, "cos")) {
        snprintf(result, sizeof(result), "%7lf", cos(number));
    } else if (!strcmp(func, "sin")) {
        snprintf(result, sizeof(result), "%7lf", sin(number));
    } else if (!strcmp(func, "tan")) {
        snprintf(result, sizeof(result), "%7lf", tan(number));
    } else if (!strcmp(func, "acos")) {
        snprintf(result, sizeof(result), "%7lf", acos(number));
    } else if (!strcmp(func, "asin")) {
        snprintf(result, sizeof(result), "%7lf", asin(number));
    } else if (!strcmp(func, "atan")) {
        snprintf(result, sizeof(result), "%7lf", atan(number));
    } else if (!strcmp(func, "sqrt")) {
        snprintf(result, sizeof(result), "%7lf", sqrt(number));
    } else if (!strcmp(func, "ln")) {
        snprintf(result, sizeof(result), "%7lf", log(number));
    } else if (!strcmp(func, "log")) {
        snprintf(result, sizeof(result), "%7lf", log10(number));
    }

    tokens_push(s, result);
}

// вычисляет выражение с оператором
void calc_op(char *op, tokens *s) {
    double numb1, numb2;
    char numb_str[256] = "";
    char result[256] = "";

    tokens_pop(s, numb_str);
    sscanf(numb_str, "%lf", &numb2);
    tokens_pop(s, numb_str);
    sscanf(numb_str, "%lf", &numb1);

    if (!strcmp(op, "+")) {
        snprintf(result, sizeof(result), "%.7lf", numb1 + numb2);
    } else if (!strcmp(op, "-")) {
        snprintf(result, sizeof(result), "%.7lf", numb1 - numb2);
    } else if (!strcmp(op, "*")) {
        snprintf(result, sizeof(result), "%.7lf", numb1 * numb2);
    } else if (!strcmp(op, "/")) {
        snprintf(result, sizeof(result), "%.7lf", numb1 / numb2);
    } else if (!strcmp(op, "^")) {
        snprintf(result, sizeof(result), "%.7lf", pow(numb1, numb2));
    } else if (!strcmp(op, "mod")) {
        snprintf(result, sizeof(result), "%.7lf", fmod(numb1, numb2));
    }

    tokens_push(s, result);
}

// добавляет токен
void tokens_push(tokens *t, char *str) {
    if (t->n < 255) {
        t->n++;
        snprintf(t->tokens[t->n], sizeof(t->tokens[t->n]), "%s", str);
    }
}

// извлекает токен
void tokens_pop(tokens *t, char *str) {
    if (t->n > -1) {
        snprintf(str, sizeof(t->tokens[t->n]), "%s", t->tokens[t->n]);
        snprintf(t->tokens[t->n], sizeof(t->tokens[t->n]), "%s", "");
        t->n--;
    }
}

// очищает структуру
void tokens_clean(tokens *t) {
    t->n = -1;
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            t->tokens[i][j] = 0;
}
