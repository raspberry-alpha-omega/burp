#include <check.h>

#include <stdio.h>
#include "burp.h"


int same_float(float a, float b) {
	char float_a[32];
	char float_b[32];
	if (a == b) return 1;
	sprintf(float_a, "%f", a);
	sprintf(float_b, "%f", b);
	return strcmp(float_a, float_b) == 0;
}

void assert_register(struct MM57109_register* reg, float value) {
    ck_assert(same_float(mm57109_get_register(reg), value));
}

void assert_state(struct MM57109* mm, enum MM57109_state state) {
    ck_assert_int_eq(mm->state, state);
}

START_TEST(test_single_digit)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    assert_state(&mm, normal);
    mm57109_op(&mm, OP_1);
    assert_state(&mm, integer_entry);
    assert_register(&mm.x, 1.0);
    assert_register(&mm.y, 0.0);
}
END_TEST

START_TEST(test_multiple_digit)
{
	struct MM57109 mm;
	mm57109_init(&mm);

	assert_state(&mm, normal);
    mm57109_op(&mm, OP_1);
    assert_state(&mm, integer_entry);
    mm57109_op(&mm, OP_2);
    assert_state(&mm, integer_entry);
    assert_register(&mm.x, 12);
    assert_register(&mm.y, 0);
}
END_TEST

START_TEST(test_digits_and_enter)
{
	struct MM57109 mm;
	mm57109_init(&mm);

	assert_state(&mm, normal);
    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    assert_state(&mm, normal);
    assert_register(&mm.x, 0);
    assert_register(&mm.y, 12);
}
END_TEST

START_TEST(test_digits_and_decimals)
{
	struct MM57109 mm;
	mm57109_init(&mm);

	assert_state(&mm, normal);
    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_DP);
    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_4);
    assert_state(&mm, decimal_entry);
    assert_register(&mm.x, 12.34);
}
END_TEST

void build_suite(TCase* tc) {
    tcase_add_test(tc, test_single_digit);
    tcase_add_test(tc, test_multiple_digit);
    tcase_add_test(tc, test_digits_and_enter);
    tcase_add_test(tc, test_digits_and_decimals);
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
