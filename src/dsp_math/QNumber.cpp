#include "../../includes/dsp_math/QNumber.hpp"

std::vector<int> tan_lookup_table(TAN_TABLE_SIZE);
std::vector<int> sin_lookup_table(SIN_TABLE_SIZE);

int DSP_MATH::q16_15_add(int a, int b)
{
    long long result = static_cast<long long>(a) + static_cast<long long>(b);
    if (result > SATURATION_LIMIT_UPPER) return static_cast<int>(SATURATION_LIMIT_UPPER);
    if (result < SATURATION_LIMIT_LOWER) return static_cast<int>(SATURATION_LIMIT_LOWER);
    return static_cast<int>(result);
}

int DSP_MATH::q16_15_subtract(int a, int b)
{
    long long result = static_cast<long long>(a) - static_cast<long long>(b);
    if (result > SATURATION_LIMIT_UPPER) return static_cast<int>(SATURATION_LIMIT_UPPER);
    if (result < SATURATION_LIMIT_LOWER) return static_cast<int>(SATURATION_LIMIT_LOWER);
    return static_cast<int>(result);
}

int DSP_MATH::q16_15_multiply(int a, int b)
{
    long long result = static_cast<long long>(a) * static_cast<long long>(b);
    // Q format multiply: (a * b) >> Q_BITS with rounding
    result = (result + (static_cast<long long>(Q_SCALE) >> 1)) >> Q_BITS;
    if (result > SATURATION_LIMIT_UPPER) return static_cast<int>(SATURATION_LIMIT_UPPER);
    if (result < SATURATION_LIMIT_LOWER) return static_cast<int>(SATURATION_LIMIT_LOWER);
    return static_cast<int>(result);
}

int DSP_MATH::q16_15_divide(int a, int b)
{
    if (b == 0) {
        return (a >= 0) ? SATURATION_LIMIT_UPPER : SATURATION_LIMIT_LOWER;
    }
    long long numerator = static_cast<long long>(a) * static_cast<long long>(Q_SCALE);

    long long absb = llabs(static_cast<long long>(b));
    long long half = absb / 2;
    if ((numerator >= 0 && b > 0) || (numerator <= 0 && b < 0)) numerator += half;
    else numerator -= half;

    long long result = numerator / b;
    if (result > SATURATION_LIMIT_UPPER) return static_cast<int>(SATURATION_LIMIT_UPPER);
    if (result < SATURATION_LIMIT_LOWER) return static_cast<int>(SATURATION_LIMIT_LOWER);
    return static_cast<int>(result);
}

int DSP_MATH::float_to_q16_15(double x)
{
    if (!std::isfinite(x)) return (x > 0.0) ? static_cast<int>(SATURATION_LIMIT_UPPER) : static_cast<int>(SATURATION_LIMIT_LOWER);

    const double max_representable = static_cast<double>(SATURATION_LIMIT_UPPER) / static_cast<double>(Q_SCALE);
    const double min_representable = static_cast<double>(SATURATION_LIMIT_LOWER) / static_cast<double>(Q_SCALE);

    if (x > max_representable) x = max_representable;
    if (x < min_representable) x = min_representable;

    long long result = std::llround(x * static_cast<double>(Q_SCALE));
    if (result > SATURATION_LIMIT_UPPER) result = SATURATION_LIMIT_UPPER;
    if (result < SATURATION_LIMIT_LOWER) result = SATURATION_LIMIT_LOWER;
    return static_cast<int>(result);
}

double DSP_MATH::q16_15_to_float(int q)
{
    return static_cast<double>(q) / Q_SCALE;
}

int DSP_MATH::int_to_q16_15(int x) {
    long long result = static_cast<long long>(x) * static_cast<long long>(Q_SCALE);
    if (result > SATURATION_LIMIT_UPPER) return static_cast<int>(SATURATION_LIMIT_UPPER);
    if (result < SATURATION_LIMIT_LOWER) return static_cast<int>(SATURATION_LIMIT_LOWER);
    return static_cast<int>(result);
}

int DSP_MATH::q16_15_to_int(int q) {
    long long r;
    if (q >= 0) r = (static_cast<long long>(q) + (static_cast<long long>(Q_SCALE) >> 1)) >> Q_BITS;
    else r = (static_cast<long long>(q) - (static_cast<long long>(Q_SCALE) >> 1)) >> Q_BITS;
    if (r > INT_MAX) return INT_MAX;
    if (r < INT_MIN) return INT_MIN;
    return static_cast<int>(r);
}

void initialize_tan_table() {
    for (int i = 0; i < TAN_TABLE_SIZE; ++i) {
        double angle_rad = (M_PI / 4.0) * static_cast<double>(i) / (TAN_TABLE_SIZE - 1);
        tan_lookup_table[i] = static_cast<int32_t>(round(tan(angle_rad) * Q_SCALE));
    }
}

void initialize_sin_table() {
    for (int i = 0; i < SIN_TABLE_SIZE; ++i) {
        double angle_rad = (M_PI / 2.0) * static_cast<double>(i) / SIN_TABLE_SIZE;
        sin_lookup_table[i] = static_cast<int>(round(sin(angle_rad) * Q_SCALE));
    }
}

int32_t q16_15_tan_from_table(double angle_rad) {
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

    return static_cast<int32_t>(round(value_low + fraction * (value_high - value_low)));
}


int DSP_MATH::q16_15_tan(int q_input) {
    double angle = static_cast<double>(q_input) / Q_SCALE;
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
        result = q16_15_tan_from_table(angle);
    } else {
        double reduced_angle = M_PI / 2.0 - angle;
        int tan_reduced = q16_15_tan_from_table(reduced_angle);
        if (tan_reduced == 0) {
            return negative ? INT_MIN : INT_MAX;
        }
        result = q16_15_divide(float_to_q16_15(1.0), tan_reduced);
    }
    
    return negative ? -result : result;
}

int q16_15_sin_from_table(double angle_rad) {
    initialize_sin_table();
    if (angle_rad < 0 || angle_rad > M_PI / 2.0) {
        return 0; 
    }

    double normalized_angle = angle_rad / (M_PI / 2.0);
    double index_float = normalized_angle * (SIN_TABLE_SIZE - 1);
    
    int index_low = static_cast<int>(index_float);
    int index_high = index_low + 1;
    
    if (index_high >= SIN_TABLE_SIZE) {
        return sin_lookup_table[SIN_TABLE_SIZE - 1];
    }
    
    double fraction = index_float - index_low;
    
    double value_low = static_cast<double>(sin_lookup_table[index_low]);
    double value_high = static_cast<double>(sin_lookup_table[index_high]);
    
    return static_cast<int>(round(value_low + fraction * (value_high - value_low)));
}

int DSP_MATH::q16_15_sin(int q_input) {
    double angle = static_cast<double>(q_input) / Q_SCALE;

    angle = fmod(angle, 2 * M_PI);
    if (angle < 0) {
        angle += 2 * M_PI;
    }

    bool negative = false;
    double reduced_angle;
    
    if (angle <= M_PI / 2.0) { 
        reduced_angle = angle;
        negative = false;
    } else if (angle <= M_PI) { 
        reduced_angle = M_PI - angle;
        negative = false;
    } else if (angle <= 3.0 * M_PI / 2.0) { 
        reduced_angle = angle - M_PI;
        negative = true;
    } else { 
        reduced_angle = 2 * M_PI - angle;
        negative = true;
    }
    
    int result = q16_15_sin_from_table(reduced_angle);
    
    return negative ? -result : result;
}

int DSP_MATH::q16_15_square(int q_input)
{
    return q16_15_multiply(q_input, q_input);
}

int DSP_MATH::q16_15_sqrt(int q_input)
{
    if (q_input < 0) {
        return INT_MIN; 
    }
    double float_value = q16_15_to_float(q_input);
    double sqrt_value = sqrt(float_value);
    return float_to_q16_15(sqrt_value);
}

int DSP_MATH::q16_15_pow(int q_base, int q_exp) {
    double base_float = q16_15_to_float(q_base);
    double exp_float = q16_15_to_float(q_exp);

    double result_float = pow(base_float, exp_float);

    int result_q = float_to_q16_15(result_float);

    return result_q;
}
