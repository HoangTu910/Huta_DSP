#include "../../includes/module/ShelvingFilter.hpp"

void Filter::ShelvingFilter::setType(int type, int fc, double G)
{
    if(type < LF_BOOST || type > HF_CUT) {
        hu_alert("It still run but the type might be wrong?");
    }
    
    // Convert G to V0
    int G_q = DSP_MATH::float_to_q17_14(G);
    int V0_q = DSP_MATH::q17_14_pow(DSP_MATH::int_to_q17_14(10), DSP_MATH::q17_14_divide(G_q, DSP_MATH::int_to_q17_14(20)));
    
    int K_q = DSP_MATH::q17_14_tan(DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::float_to_q17_14(M_PI), DSP_MATH::int_to_q17_14(fc)), m_fs));
    int K_square_q = DSP_MATH::q17_14_square(K_q);

    // Common terms for all filters
    int denominator;
    int sqrt_2_q = DSP_MATH::q17_14_sqrt(DSP_MATH::int_to_q17_14(2));
    int sqrt_V0_q = DSP_MATH::q17_14_sqrt(V0_q);

    int sqrt_2_K_q = DSP_MATH::q17_14_multiply(sqrt_2_q, K_q);
    int sqrt_V0_K_q = DSP_MATH::q17_14_multiply(sqrt_V0_q, K_q);

    /*
        All the formulas was used from DAFX-Digital Audio Effects book - Chapter 2
    */

    switch (type){
        case LF_BOOST:
            denominator = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q);
            m_b0 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::int_to_q17_14(1), sqrt_V0_K_q), DSP_MATH::q17_14_multiply(V0_q, K_square_q)), denominator);
            m_b1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(DSP_MATH::q17_14_multiply(V0_q, K_square_q), DSP_MATH::int_to_q17_14(1))), denominator);
            m_b2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::int_to_q17_14(1), sqrt_V0_K_q), DSP_MATH::q17_14_multiply(V0_q, K_square_q)), denominator);
            m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::int_to_q17_14(1))), denominator);
            m_a2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q), denominator);
            break;

        case LF_CUT:
            denominator = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(V0_q, sqrt_2_K_q), K_square_q);
            m_b0 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(V0_q, DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q)), denominator);
            m_b1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), V0_q), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::int_to_q17_14(1))), denominator);
            m_b2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(V0_q, DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q)), denominator);
            m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(K_square_q, V0_q)), denominator);
            m_a2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(V0_q, sqrt_2_K_q), K_square_q), denominator);
            break;

        case HF_BOOST:
            denominator = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q);
            m_b0 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(V0_q, sqrt_V0_K_q), K_square_q), denominator);
            m_b1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(K_square_q, V0_q)), denominator);
            m_b2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(V0_q, sqrt_V0_K_q), K_square_q), denominator);
            m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::int_to_q17_14(1))), denominator);
            m_a2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q), denominator);
            break;

        case HF_CUT:
            denominator = DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(V0_q, sqrt_V0_K_q), DSP_MATH::q17_14_multiply(V0_q, K_square_q));
            m_b0 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_add(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q), denominator);
            m_b1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(K_square_q, DSP_MATH::int_to_q17_14(1))), denominator);
            m_b2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(DSP_MATH::int_to_q17_14(1), sqrt_2_K_q), K_square_q), denominator);
            m_a1 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_multiply(DSP_MATH::int_to_q17_14(2), DSP_MATH::q17_14_subtract(DSP_MATH::q17_14_multiply(V0_q, K_square_q), DSP_MATH::int_to_q17_14(1))), denominator);
            m_a2 = DSP_MATH::q17_14_divide(DSP_MATH::q17_14_add(DSP_MATH::q17_14_subtract(V0_q, sqrt_V0_K_q), DSP_MATH::q17_14_multiply(V0_q, K_square_q)), denominator);
            break;
    }
}