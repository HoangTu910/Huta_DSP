#include "../../includes/dsp_math/QNumber.h"

std::vector<int> tan_lookup_table(TAN_TABLE_SIZE);

int DSP_MATH::q17_14_add(int a, int b)
{
    return a + b;
}

int DSP_MATH::q17_14_subtract(int a, int b)
{
    return a - b;
}

int DSP_MATH::q17_14_multiply(int a, int b)
{
    long long result = static_cast<long long>(a) * static_cast<long long>(b);
    return static_cast<int>((result + (Q_SCALE >> 1)) >> Q_BITS);
}

int DSP_MATH::q17_14_divide(int a, int b)
{
    if (b == 0) {
        return INT_MAX; 
    }
    long long result = static_cast<long long>(a) * Q_SCALE;
    return static_cast<int>(result / b);
}

int DSP_MATH::float_to_q17_14(double x)
{
    return static_cast<int>(round(x * Q_SCALE));
}

double DSP_MATH::q17_14_to_float(int q)
{
    return static_cast<double>(q) / Q_SCALE;
}

int DSP_MATH::int_to_q17_14(int x) {
    return static_cast<int>(round(static_cast<double>(x) * Q_SCALE));
}

int DSP_MATH::q17_14_to_int(int q) {
    return static_cast<int>(round(static_cast<double>(q) / Q_SCALE));
}

void initialize_tan_table() {
    for (int i = 0; i < TAN_TABLE_SIZE; ++i) {
        double angle_rad = (M_PI / 4.0) * static_cast<double>(i) / (TAN_TABLE_SIZE - 1);
        tan_lookup_table[i] = static_cast<int32_t>(round(tan(angle_rad) * Q_SCALE));
    }
}

int32_t q17_14_tan_from_table(double angle_rad) {
    debug(angle_rad);
    initialize_tan_table();
    if (angle_rad < 0) angle_rad = 0;
    if (angle_rad > M_PI / 4.0) angle_rad = M_PI / 4.0;

    double normalized_angle = angle_rad / (M_PI / 4.0);
    double index_float = normalized_angle * (TAN_TABLE_SIZE - 1);

    int index_low = static_cast<int>(index_float);
    int index_high = index_low + 1;
    if (index_high >= TAN_TABLE_SIZE) index_high = TAN_TABLE_SIZE - 1;

    double fraction = index_float - index_low;

    double value_low = static_cast<double>(tan_lookup_table[index_low]);
    double value_high = static_cast<double>(tan_lookup_table[index_high]);
    debug(tan_lookup_table[index_low]);

    return static_cast<int32_t>(round(value_low + fraction * (value_high - value_low)));
}


int DSP_MATH::q17_14_tan(int q_input) {
    double angle = static_cast<double>(q_input) / Q_SCALE;
    debug(angle);
    angle = fmod(angle, 2 * M_PI);
    if (angle > M_PI) {
        angle -= 2 * M_PI;
    }

    if (fabs(angle - M_PI / 2.0) < 1e-6) {
        return INT_MAX;
    }
    if (fabs(angle + M_PI / 2.0) < 1e-6) {
        return INT_MIN;
    }

    bool negative = false;
    if (angle < 0) {
        angle = -angle;
        negative = true;
    }

    int result;
    if (angle <= M_PI / 4.0) {
        result = q17_14_tan_from_table(angle);
        debug(result);
    } else {
        double reduced_angle = M_PI / 2.0 - angle;
        int tan_reduced = q17_14_tan_from_table(reduced_angle);
        if (tan_reduced == 0) {
            return negative ? INT_MIN : INT_MAX;
        }
        result = q17_14_divide(float_to_q17_14(1.0), tan_reduced);
    }
    
    return negative ? -result : result;
}



