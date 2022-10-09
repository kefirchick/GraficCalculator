#include <stdio.h>
#include <string.h>
#include "smart_calc.h"

int main() {
    tokens t = {-1, {'\0'}};
    tokens p = {-1, {'\0'}};
    int ret = 0;
    char result[256] = "";
    double array[256] = {0.};
    int start_pos1 = 4;
    char str1[256] = "f234234jhjkhjkhkj";
    char str2[256] = "atanacos34jhjkhjkhkj";
    char str3[256] = "atan(acos(-12.33322*3+4.2323424))";
    char str4[256] = "sqrt(x)-1/2*sin(x^2-2)";
    char str5[256] = "1^2/(5*3)+10";
    char str6[256] = "cos(3-3)+2*3";
    char str7[256] = " -sqrt(x)-1/ +2*sin(x^2-2)";
    char str8[256] = "sin(x)";
    char str9[256] = "1+0.0000005";
    
    ret = parse_number(str1, &t, start_pos1);
    printf("%s %c %d\n", t.tokens[t.n], str1[ret], t.n);

    ret = parse_func(str2, &t, &start_pos1);
    printf("%s %c %d\n", t.tokens[t.n], str2[ret], t.n);
    
    tokens_clean(&t);
    ret = to_tokens(str3, &t);
    for (int i = 0; i <= t.n; i++) printf("%s ", t.tokens[i]);
    printf("\n%d", ret);

    ret = kurwa(&t, &p);
    for (int i = 0; i <= t.n; i++) printf("%s ", p.tokens[i]);
    printf("\n%d\n", ret);

    tokens_clean(&t);
    tokens_clean(&p);
    ret = to_tokens(str4, &t);
    ret = ret && kurwa(&t, &p);
    for (int i = 0; i <= t.n; i++) printf("%s ", p.tokens[i]);
    printf("\n%d\n", ret);

    tokens_clean(&t);
    tokens_clean(&p);
    ret = to_tokens(str5, &t);
    ret = ret && kurwa(&t, &p);
    for (int i = 0; i <= t.n; i++) printf("%s ", p.tokens[i]);
    printf("\n%d\n", ret);

    tokens_clean(&p);
    tokens_push(&p, "1");
    for (int i = 0; i <= p.n; i++) printf("%s\n", p.tokens[i]);
    calc_func("sin", &p);
    for (int i = 0; i <= p.n; i++) printf("%s\n", p.tokens[i]);

    tokens_clean(&p);
    tokens_push(&p, "1");
    tokens_push(&p, "3");
    for (int i = 0; i <= p.n; i++) printf("%s\n", p.tokens[i]);
    calc_op("/", &p);
    for (int i = 0; i <= p.n; i++) printf("%s\n", p.tokens[i]);

    printf("%d: %s\n", calculate(str6, result), result);
    printf("%d: %s\n", calculate(str5, result), result);
    printf("%d: %s\n", calculate(str4, result), result);
    printf("%d: %s\n", calculate(str9, result), result);

    prepare_input(str7, result);
    printf("%s\n", result);

    prepare_input(str5, result);
    printf("%s\n", result);
    printf("%d: %s\n", calculate(result, result), result);

    plot_button_handle(str8, array, -10, 10);
    for (int i = 0; i < 256; i+= 10) printf("%lf\n", array[i]);

    tokens_clean(&t);
    to_tokens(str4, &t);
    tokens_push(&t, "sin");
    for (int i = 0; i <= t.n; i++) printf("%s ", t.tokens[i]);
    tokens_pop(&t, result);
    printf("\n%s ", result);
    tokens_pop(&t, result);
    printf("\n%s ", result);
    tokens_pop(&t, result);
    printf("\n%s \n", result);
    for (int i = 0; i <= t.n; i++) printf("%s ", t.tokens[i]);

    return 0;
}