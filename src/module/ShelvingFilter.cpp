#include "../../includes/module/ShelvingFilter.hpp"

void Filter::ShelvingFilter::setType(int type, int fc, double G)
{
    if (type < LF_BOOST || type > HF_CUT) {
        hu_alert("It still run but the type might be wrong?");
    }

    double G_d = G;
    double V0_d = pow(N10_F, G_d / N20_F);
    double K_d = tan(M_PI * fc / static_cast<double>(m_fs));
    double K_square_d = K_d * K_d;
    double denominator;
    double sqrt_2_d = sqrt(N2_F);
    double sqrt_V0_d = sqrt(V0_d);
    double sqrt_2_K_d = sqrt_2_d * K_d;
    double sqrt_V0_K_d = sqrt_V0_d * K_d;
    double b0_val = N0_F, b1_val = N0_F, b2_val = N0_F, a1_val = N0_F, a2_val = N0_F;

    /*
        All the formulas were used from DAFX-Digital Audio Effects book - Chapter 2
    */

    switch (type) {
        case LF_BOOST:
            denominator = N1_F + sqrt_2_K_d + K_square_d;
            b0_val = (N1_F + sqrt_V0_K_d + V0_d * K_square_d) / denominator;
            b1_val = (N2_F * (V0_d * K_square_d - N1_F)) / denominator;
            b2_val = (N1_F - sqrt_V0_K_d + V0_d * K_square_d) / denominator;
            a1_val = (N2_F * (K_square_d - N1_F)) / denominator;
            a2_val = (N1_F - sqrt_2_K_d + K_square_d) / denominator;
            break;

        case LF_CUT:
            denominator = V0_d + sqrt_2_K_d + K_square_d;
            b0_val = (V0_d * (N1_F + sqrt_2_K_d + K_square_d)) / denominator;
            b1_val = (N2_F * V0_d * (K_square_d - N1_F)) / denominator;
            b2_val = (V0_d * (N1_F - sqrt_2_K_d + K_square_d)) / denominator;
            a1_val = (N2_F * (K_square_d - V0_d)) / denominator;
            a2_val = (V0_d - sqrt_2_K_d + K_square_d) / denominator;
            break;

        case HF_BOOST:
            denominator = N1_F + sqrt_2_K_d + K_square_d;
            b0_val = (V0_d + sqrt_V0_K_d + K_square_d) / denominator;
            b1_val = (N2_F * (K_square_d - V0_d)) / denominator;
            b2_val = (V0_d - sqrt_V0_K_d + K_square_d) / denominator;
            a1_val = (N2_F * (K_square_d - N1_F)) / denominator;
            a2_val = (N1_F - sqrt_2_K_d + K_square_d) / denominator;
            break;

        case HF_CUT:
            denominator = V0_d + sqrt_V0_K_d + V0_d * K_square_d;
            b0_val = (N1_F + sqrt_2_K_d + K_square_d) / denominator;
            b1_val = (N2_F * (K_square_d - N1_F)) / denominator;
            b2_val = (N1_F - sqrt_2_K_d + K_square_d) / denominator;
            a1_val = (N2_F * (V0_d * K_square_d - N1_F)) / denominator;
            a2_val = (V0_d - sqrt_V0_K_d + V0_d * K_square_d) / denominator;
            break;

        default:
            b0_val = b1_val = b2_val = a1_val = a2_val = N0_F;
            break;
    }

    std::vector<double> coeff = {b0_val, b1_val, b2_val, a1_val, a2_val};
    this->normalize(coeff);

    hu_debug(m_a1);
    hu_debug(m_a2);
    hu_debug(m_b0);
    hu_debug(m_b1);
    hu_debug(m_b2);
    hu_debug(m_gain);
}

void Filter::ShelvingFilter::setSampleFreq(int fs) {
    m_fs = fs;
}