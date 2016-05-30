#include <check.h>

#include <stdio.h>
#include "burp.h"


START_TEST(test_single_digit)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    ck_assert_int_eq(mm.state, normal);
    mm57109_op(&mm, OP_1);
    ck_assert_int_eq(mm.state, integer_entry);
    ck_assert_int_eq((int)mm57109_get_register(&mm.x), 1);
    ck_assert_int_eq((int)mm57109_get_register(&mm.y), 0);
}
END_TEST

START_TEST(test_multiple_digit)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    ck_assert_int_eq(mm.state, normal);
    mm57109_op(&mm, OP_1);
    ck_assert_int_eq(mm.state, integer_entry);
    mm57109_op(&mm, OP_2);
    ck_assert_int_eq(mm.state, integer_entry);
    ck_assert_int_eq((int)mm57109_get_register(&mm.x), 12);
    ck_assert_int_eq((int)mm57109_get_register(&mm.y), 0);
}
END_TEST

START_TEST(test_digits_and_enter)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    ck_assert_int_eq(mm.state, normal);
    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    ck_assert_int_eq(mm.state, integer_entry);
    ck_assert_int_eq((int)mm57109_get_register(&mm.x), 0);
    ck_assert_int_eq((int)mm57109_get_register(&mm.y), 12);
}
END_TEST

void build_suite(TCase* tc) {
    tcase_add_test(tc, test_single_digit);
    tcase_add_test(tc, test_multiple_digit);
    tcase_add_test(tc, test_digits_and_enter);
}

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;
    suite_add_tcase(s1, tc1_1);
    build_suite(tc1_1);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
