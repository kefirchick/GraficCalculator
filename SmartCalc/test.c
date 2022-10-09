#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smart_calc.h"

tokens t = {-1, {{'\0'}}};
tokens p = {-1, {{'\0'}}};
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
char str9[256] = "tan(1)+atan(0)+acos(0)+asin(0)+ln(1)+log(1)";
char str10[256] = "sqrt(5mod3^2/1)";


START_TEST(parse_number_test) {
    tokens_clean(&t);
    ret = parse_number(str1, &t, start_pos1);
    ck_assert_int_eq(ret, 6);
    ck_assert_str_eq(t.tokens[t.n], "234");
}
END_TEST

START_TEST(parse_func_test) {
    tokens_clean(&t);
    ret = parse_func(str2, &t, &start_pos1);
    ck_assert_int_eq(ret, 1);
    ck_assert_str_eq(t.tokens[t.n], "acos");
}
END_TEST

START_TEST(to_tokens_test) {
    tokens_clean(&t);
    ret = to_tokens(str3, &t);
    ck_assert_int_eq(ret, 1);
    ck_assert_str_eq(t.tokens[t.n], ")");
}
END_TEST

START_TEST(kurwa_test) {
    tokens_clean(&t);
    to_tokens(str3, &t);
    ret = kurwa(&t, &p);
    ck_assert_int_eq(ret, 1);
    ck_assert_str_eq(p.tokens[p.n], "atan");
}
END_TEST

START_TEST(calculate_test) {
    ret = calculate(str6, result);
    ck_assert_int_eq(ret, 1);
    ck_assert_str_eq(result, "7.0000000");
}
END_TEST

START_TEST(prepare_input_test) {
    prepare_input(str7, result);
    ck_assert_str_eq(result, "0-sqrt(x)-1/2*sin(x^2-2)");
}
END_TEST

START_TEST(plot_button_handle_test) {
    ret = plot_button_handle(str8, array, -10, 10);
    ck_assert_int_eq(ret, 1);
    snprintf(result, sizeof(result), "%.7lf", array[0]);
    ck_assert_str_eq(result, "0.5440210");
}
END_TEST

START_TEST(eq_button_handle_test) {
    ret = eq_button_handle(str9, result);
    ck_assert_str_eq(result, "3.1282040");
    ret = eq_button_handle(str10, result);
    ck_assert_str_eq(result, "2.236068");
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);

    tcase_add_test(tc1_1, parse_number_test);
    tcase_add_test(tc1_1, parse_func_test);
    tcase_add_test(tc1_1, to_tokens_test);
    tcase_add_test(tc1_1, kurwa_test);
    tcase_add_test(tc1_1, calculate_test);
    tcase_add_test(tc1_1, prepare_input_test);
    tcase_add_test(tc1_1, plot_button_handle_test);
    tcase_add_test(tc1_1, eq_button_handle_test);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
