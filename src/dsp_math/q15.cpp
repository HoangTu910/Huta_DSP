#include "../../includes/dsp_math/q15.h"

std::vector<int> tan_lookup_table(1024);

int DSP_MATH::q15_add(int a, int b)
{
    return a + b;
}
int DSP_MATH::q15_subtract(int a, int b)
{
    return a - b;
}

int DSP_MATH::q15_multiply(int a, int b)
{
    long long result = static_cast<long long>(a) * static_cast<long long>(b);
    return static_cast<int>((result + (Q_SCALE >> 1)) >> Q_BITS);
}

int DSP_MATH::q15_divide(int a, int b)
{
    if(b == 0) {
        return INT_MAX;
    }
    return static_cast<int>((static_cast<long long>(a) * Q_SCALE + (b >> 1)) / b);
}

int DSP_MATH::float_to_q15(double x)
{
    return static_cast<int>(round(x * Q_SCALE));
}

double DSP_MATH::q15_to_float(int q)
{
    return static_cast<double>(q) / Q_SCALE;
}

int DSP_MATH::int_to_q15(int x) {
    return static_cast<int>(round(static_cast<double>(x) / (double)SHRT_MAX * Q_SCALE));
}

int DSP_MATH::q15_to_int(int q) {
    return static_cast<int>(round(static_cast<double>(q) / (double)Q_SCALE * (double)SHRT_MAX));
}

void initialize_tan_table() {
    for (int i = 0; i < TAN_TABLE_SIZE; ++i) {
        double angle_rad = (M_PI / 4.0) * static_cast<double>(i) / TAN_TABLE_SIZE;
        tan_lookup_table[i] = static_cast<int>(round(tan(angle_rad) * Q_SCALE));
    }
}

int q15_tan_from_table(double angle_rad) {
    if (angle_rad < 0 || angle_rad > M_PI / 4.0) {
        return 0; 
    }
    double normalized_angle = angle_rad / (M_PI / 4.0);
    double index_float = normalized_angle * (TAN_TABLE_SIZE - 1);
    
    int index_low = static_cast<int>(index_float);
    int index_high = index_low + 1;
    
    if (index_high >= TAN_TABLE_SIZE) {
        return tan_lookup_table[TAN_TABLE_SIZE - 1];
    }
    
    double fraction = index_float - index_low;
    double value_low = static_cast<double>(tan_lookup_table[index_low]);
    double value_high = static_cast<double>(tan_lookup_table[index_high]);
    
    return static_cast<int>(round(value_low + fraction * (value_high - value_low)));
}

int DSP_MATH::q15_tan(int q_input) {
    double float_input = static_cast<double>(q_input) / Q_SCALE;
    
    double angle = fmod(float_input, 2.0 * M_PI);
    if (angle < 0) {
        angle += 2.0 * M_PI;
    }
    
    bool negative = false;
    
    if (angle > M_PI / 2.0 && angle < M_PI) {
        angle = M_PI - angle;
        negative = true;
    } else if (angle > M_PI && angle < 3.0 * M_PI / 2.0) {
        angle = angle - M_PI;
    } else if (angle > 3.0 * M_PI / 2.0 && angle < 2.0 * M_PI) {
        angle = 2.0 * M_PI - angle;
        negative = true;
    }

    if (abs(angle - M_PI / 2.0) < 1e-6 || abs(angle - 3.0 * M_PI / 2.0) < 1e-6) {
        return INT_MAX;
    }

    int result = q15_tan_from_table(angle);
    return negative ? -result : result;
}

