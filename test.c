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

int same_int(int a, int b) {
	return a == b;
}

void _assert_register(struct MM57109_register* reg, float value, int line) {
	float rvalue = mm57109_get_register(reg);
    ck_assert_msg(same_float(rvalue, value), "line %d: expected %f but was %f", line, value, rvalue);
}
#define assert_register(reg,value) _assert_register(reg,value,__LINE__)

void _assert_state(struct MM57109* mm, enum MM57109_state state, int line) {
    ck_assert_msg(same_int(mm->state, state), "line %d: expected state %d but was %d", line, state, mm->state);
}
#define assert_state(mm,state) _assert_state(mm,state,__LINE__)

void dump(struct MM57109* mm) {
	printf("dump %p: x=%f, y=%f, z=%f, t=%f, m=%f\n", mm, mm->x.value, mm->y.value, mm->z.value, mm->t.value, mm->m.value);
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

START_TEST(test_leading_zero)
{
	struct MM57109 mm;
	mm57109_init(&mm);

	assert_state(&mm, normal);
    mm57109_op(&mm, OP_0);
    assert_state(&mm, integer_entry);
    mm57109_op(&mm, OP_5);
    assert_state(&mm, integer_entry);
    assert_register(&mm.x, 5);
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

START_TEST(test_pop)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_4);
    assert_register(&mm.x, 4);
    assert_register(&mm.y, 3);
    assert_register(&mm.z, 2);
    assert_register(&mm.t, 1);

    mm57109_op(&mm, OP_POP);
    assert_register(&mm.x, 3);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 1);
    assert_register(&mm.t, 0);
}
END_TEST

START_TEST(test_roll)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_4);
    assert_register(&mm.x, 4);
    assert_register(&mm.y, 3);
    assert_register(&mm.z, 2);
    assert_register(&mm.t, 1);

    mm57109_op(&mm, OP_ROLL);
    assert_register(&mm.x, 3);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 1);
    assert_register(&mm.t, 4);
}
END_TEST

START_TEST(test_xey)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    assert_register(&mm.x, 1);
    assert_register(&mm.y, 0);

    mm57109_op(&mm, OP_XEY);
    assert_register(&mm.x, 0);
    assert_register(&mm.y, 1);

    mm57109_op(&mm, OP_XEY);
    assert_register(&mm.x, 1);
    assert_register(&mm.y, 0);
}
END_TEST

START_TEST(test_xem)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    assert_register(&mm.x, 1);
    assert_register(&mm.m, 0);

    mm57109_op(&mm, OP_XEM);
    assert_register(&mm.x, 0);
    assert_register(&mm.m, 1);

    mm57109_op(&mm, OP_XEM);
    assert_register(&mm.x, 1);
    assert_register(&mm.m, 0);
}
END_TEST

START_TEST(test_ms)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    assert_register(&mm.x, 1);
    assert_register(&mm.m, 0);

    mm57109_op(&mm, OP_MS);
    assert_register(&mm.x, 1);
    assert_register(&mm.m, 1);
}
END_TEST

START_TEST(test_mr)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_4);
    mm57109_set_register(&mm.m, 5);
    assert_register(&mm.x, 4);
    assert_register(&mm.y, 3);
    assert_register(&mm.z, 2);
    assert_register(&mm.t, 1);
    assert_register(&mm.m, 5);

    mm57109_op(&mm, OP_MR);
    assert_register(&mm.x, 5);
    assert_register(&mm.y, 4);
    assert_register(&mm.z, 3);
    assert_register(&mm.t, 2);
    assert_register(&mm.m, 5);
}
END_TEST

START_TEST(test_plus)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);

    assert_register(&mm.x, 3);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 1);

    mm57109_op(&mm, OP_PLUS);

    assert_register(&mm.x, 5);
    assert_register(&mm.y, 1);
    assert_register(&mm.z, 0);

    mm57109_op(&mm, OP_PLUS);

    assert_register(&mm.x, 6);
    assert_register(&mm.y, 0);
    assert_register(&mm.z, 0);
}
END_TEST

START_TEST(test_minus)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);

    assert_register(&mm.x, 3);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 1);

    mm57109_op(&mm, OP_MINUS);

    assert_register(&mm.x, 1);
    assert_register(&mm.y, 1);
    assert_register(&mm.z, 0);

    mm57109_op(&mm, OP_MINUS);

    assert_register(&mm.x, 0);
    assert_register(&mm.y, 0);
    assert_register(&mm.z, 0);
}
END_TEST

START_TEST(test_mul)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);

    assert_register(&mm.x, 3);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 1);

    mm57109_op(&mm, OP_MUL);

    assert_register(&mm.x, 6);
    assert_register(&mm.y, 1);
    assert_register(&mm.z, 0);

    mm57109_op(&mm, OP_MUL);

    assert_register(&mm.x, 6);
    assert_register(&mm.y, 0);
    assert_register(&mm.z, 0);
}
END_TEST

START_TEST(test_div)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);

    assert_register(&mm.x, 3);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 1);

    mm57109_op(&mm, OP_DIV);

    assert_register(&mm.x, 1.5);
    assert_register(&mm.y, 1);
    assert_register(&mm.z, 0);

    mm57109_op(&mm, OP_DIV);

    assert_register(&mm.x, 1.5);
    assert_register(&mm.y, 0);
    assert_register(&mm.z, 0);
}
END_TEST

START_TEST(test_inv)
{
	struct MM57109 mm;
	mm57109_init(&mm);

	assert_state(&mm, normal);
    mm57109_op(&mm, OP_INV);
	assert_state(&mm, invert);
    mm57109_op(&mm, OP_INV);
	assert_state(&mm, normal);
}
END_TEST

START_TEST(test_inv_plus)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_MS);
    assert_register(&mm.m, 1);
    mm57109_op(&mm, OP_INV);
    mm57109_op(&mm, OP_PLUS);
    assert_register(&mm.m, 2);
}
END_TEST

START_TEST(test_inv_minus)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_7);
    mm57109_op(&mm, OP_MS);
    assert_register(&mm.m, 7);
    mm57109_op(&mm, OP_2);
    assert_register(&mm.x, 2);
    mm57109_op(&mm, OP_INV);
    mm57109_op(&mm, OP_MINUS);
    assert_register(&mm.m, 5);
}
END_TEST

START_TEST(test_inv_mul)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_7);
    mm57109_op(&mm, OP_MS);
    assert_register(&mm.m, 7);
    mm57109_op(&mm, OP_2);
    assert_register(&mm.x, 2);
    mm57109_op(&mm, OP_INV);
    mm57109_op(&mm, OP_MUL);
    assert_register(&mm.m, 14);
}
END_TEST

START_TEST(test_inv_div)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_7);
    mm57109_op(&mm, OP_MS);
    assert_register(&mm.m, 7);
    mm57109_op(&mm, OP_2);
    assert_register(&mm.x, 2);
    mm57109_op(&mm, OP_INV);
    mm57109_op(&mm, OP_DIV);
    assert_register(&mm.m, 3.5);
}
END_TEST

START_TEST(test_1_x)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_1_X);
    assert_register(&mm.x, 0.5);
}
END_TEST

START_TEST(test_sq)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_SQ);
    assert_register(&mm.x, 4);
}
END_TEST

START_TEST(test_sqrt)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_9);
    mm57109_op(&mm, OP_SQRT);
    assert_register(&mm.x, 3);
}
END_TEST

START_TEST(test_tenx)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_10X);
    assert_register(&mm.x, 1000);
}
END_TEST

START_TEST(test_ex)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_EX);
    assert_register(&mm.x, 20.085537);
}
END_TEST

START_TEST(test_log)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_1);
    mm57109_op(&mm, OP_0);
    mm57109_op(&mm, OP_0);
    mm57109_op(&mm, OP_0);
    mm57109_op(&mm, OP_LOG);
    assert_register(&mm.x, 3);
}
END_TEST

START_TEST(test_ln)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_0);
    mm57109_op(&mm, OP_DP);
    mm57109_op(&mm, OP_0);
    mm57109_op(&mm, OP_8);
    mm57109_op(&mm, OP_5);
    mm57109_op(&mm, OP_5);
    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_7);
    mm57109_op(&mm, OP_LN);
    assert_register(&mm.x, 3);
}
END_TEST

START_TEST(test_yx)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_YX);
    assert_register(&mm.x, 9);
}
END_TEST

START_TEST(test_mclr)
{
	struct MM57109 mm;
	mm57109_init(&mm);

    mm57109_op(&mm, OP_5);
    mm57109_op(&mm, OP_XEM);
    mm57109_op(&mm, OP_4);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_3);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_2);
    mm57109_op(&mm, OP_EN);
    mm57109_op(&mm, OP_1);

    assert_register(&mm.x, 1);
    assert_register(&mm.y, 2);
    assert_register(&mm.z, 3);
    assert_register(&mm.t, 4);
    assert_register(&mm.m, 5);

    mm57109_op(&mm, OP_MCLR);
    assert_register(&mm.x, 0);
    assert_register(&mm.y, 0);
    assert_register(&mm.z, 0);
    assert_register(&mm.t, 0);
    assert_register(&mm.m, 0);
}
END_TEST

void build_suite(TCase* tc) {
    tcase_add_test(tc, test_single_digit);
    tcase_add_test(tc, test_multiple_digit);
    tcase_add_test(tc, test_leading_zero);
    tcase_add_test(tc, test_digits_and_enter);
    tcase_add_test(tc, test_digits_and_decimals);
    tcase_add_test(tc, test_pop);
    tcase_add_test(tc, test_roll);
    tcase_add_test(tc, test_xey);
    tcase_add_test(tc, test_xem);
    tcase_add_test(tc, test_ms);
    tcase_add_test(tc, test_mr);
    tcase_add_test(tc, test_plus);
    tcase_add_test(tc, test_minus);
    tcase_add_test(tc, test_mul);
    tcase_add_test(tc, test_div);
    tcase_add_test(tc, test_inv);
    tcase_add_test(tc, test_inv_plus);
    tcase_add_test(tc, test_inv_minus);
    tcase_add_test(tc, test_inv_mul);
    tcase_add_test(tc, test_inv_div);
    tcase_add_test(tc, test_1_x);
    tcase_add_test(tc, test_sq);
    tcase_add_test(tc, test_sqrt);
    tcase_add_test(tc, test_tenx);
    tcase_add_test(tc, test_ex);
    tcase_add_test(tc, test_log);
    tcase_add_test(tc, test_ln);
    tcase_add_test(tc, test_yx);
    tcase_add_test(tc, test_mclr);
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
