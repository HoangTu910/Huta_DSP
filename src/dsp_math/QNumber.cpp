#include "../../includes/dsp_math/QNumber.hpp"

std::vector<int> tan_lookup_table(TAN_TABLE_SIZE);
std::vector<int> sin_lookup_table(SIN_TABLE_SIZE);

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

void initialize_sin_table() {
    for (int i = 0; i < SIN_TABLE_SIZE; ++i) {
        double angle_rad = (M_PI / 2.0) * static_cast<double>(i) / SIN_TABLE_SIZE;
        sin_lookup_table[i] = static_cast<int>(round(sin(angle_rad) * Q_SCALE));
    }
}

int32_t q17_14_tan_from_table(double angle_rad) {
    hu_debug(angle_rad);
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
    hu_debug(tan_lookup_table[index_low]);

    return static_cast<int32_t>(round(value_low + fraction * (value_high - value_low)));
}


int DSP_MATH::q17_14_tan(int q_input) {
    double angle = static_cast<double>(q_input) / Q_SCALE;
    hu_debug(angle);
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
        hu_debug(result);
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

int q17_14_sin_from_table(double angle_rad) {
    initialize_sin_table();
    hu_debug(angle_rad);
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

/**
 * @brief Tính giá trị sin của một góc ở định dạng Q17.14.
 * * Hàm này xử lý các góc trong phạm vi [-∞, +∞] bằng cách chuẩn hóa
 * và sử dụng tính chất đối xứng của hàm sin để tra cứu bảng.
 * * @param q_input Góc đầu vào ở định dạng Q17.14.
 * @return Giá trị sin tương ứng ở định dạng Q17.14.
 */
int DSP_MATH::q17_14_sin(int q_input) {
    // 1. Chuyển input Q17.14 sang số thực
    double angle = static_cast<double>(q_input) / Q_SCALE;

    // 2. Chuẩn hóa góc về phạm vi [0, 2π)
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0) {
        angle += 2 * M_PI;
    }

    bool negative = false;
    double reduced_angle;
    
    // 3. Phân tích góc vào các góc chuẩn của vòng tròn đơn vị
    if (angle <= M_PI / 2.0) { // Góc trong [0, π/2] (Góc phần tư I)
        reduced_angle = angle;
        negative = false;
    } else if (angle <= M_PI) { // Góc trong (π/2, π] (Góc phần tư II)
        reduced_angle = M_PI - angle;
        negative = false;
    } else if (angle <= 3.0 * M_PI / 2.0) { // Góc trong (π, 3π/2] (Góc phần tư III)
        reduced_angle = angle - M_PI;
        negative = true;
    } else { // Góc trong (3π/2, 2π) (Góc phần tư IV)
        reduced_angle = 2 * M_PI - angle;
        negative = true;
    }
    
    // 4. Tra cứu giá trị từ bảng
    int result = q17_14_sin_from_table(reduced_angle);
    
    // 5. Điều chỉnh dấu dựa trên góc phần tư ban đầu
    return negative ? -result : result;
}

int DSP_MATH::q17_14_square(int q_input)
{
    return q17_14_multiply(q_input, q_input);
}

int DSP_MATH::q17_14_sqrt(int q_input)
{
    if (q_input < 0) {
        return INT_MIN; 
    }
    double float_value = q17_14_to_float(q_input);
    double sqrt_value = sqrt(float_value);
    return float_to_q17_14(sqrt_value);
}

int DSP_MATH::q17_14_pow(int q_base, int q_exp) {
    double base_float = q17_14_to_float(q_base);
    double exp_float = q17_14_to_float(q_exp);

    double result_float = pow(base_float, exp_float);

    int result_q = float_to_q17_14(result_float);

    return result_q;
}
