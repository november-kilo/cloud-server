inherit "/lib/util/format/repeat";

/*
 * NAME:	calculate_power_of_10()
 * DESCRIPTION:	calculate 10^n as a multiplier
 */
private int calculate_power_of_10(int n)
{
	int multiplier, i;
	
	multiplier = 1;
	for (i = 0; i < n; i++) {
		multiplier *= 10;
	}
	
	return multiplier;
}

/*
 * NAME:	round_float_to_int()
 * DESCRIPTION:	round a float to nearest integer (0.5 rounds up)
 */
private int round_float_to_int(float value)
{
	if (value >= 0.5) {
		return (int)floor(value + 0.5);
	}
	return (int)floor(value);
}

/*
 * NAME:	format_fractional_part()
 * DESCRIPTION:	format the fractional part of a float with given precision
 * RETURNS:	({ fractional_string, overflow_flag })
 */
private mixed *format_fractional_part(float frac_float, int precision)
{
	int multiplier, frac_val, overflow;
	string frac_part;
	
	multiplier = calculate_power_of_10(precision);
	frac_val = (int)floor(frac_float * (float)multiplier + 0.5);
	
	overflow = (frac_val >= multiplier);
	if (overflow) {
		frac_val = 0;
	}
	
	frac_part = pad_left(frac_val, precision, "0");
	
	return ({ frac_part, overflow });
}

/*
 * NAME:	split_float()
 * DESCRIPTION:	split a float into integer and fractional parts
 * RETURNS:	({ integer_part, fractional_part })
 */
private mixed *split_float(float value)
{
	float *parts;
	
	parts = modf(value);
	return ({ (int)floor(parts[1]), parts[0] });
}

/*
 * NAME:	format_float()
 * DESCRIPTION:	format a float with specified precision
 */
string format_float(float value, int precision)
{
	string int_part, frac_part;
	int int_val, overflow;
	float frac_float;
	mixed *parts, *frac_result;
	
	if (precision < 0) {
		return (string)value;
	}
	
	if (value < 0.0) {
		return "-" + format_float(-value, precision);
	}
	
	parts = split_float(value);
	int_val = parts[0];
	frac_float = parts[1];
	
	if (precision == 0) {
		return (string)(int_val + round_float_to_int(frac_float));
	}
	
	frac_result = format_fractional_part(frac_float, precision);
	frac_part = frac_result[0];
	overflow = frac_result[1];
	
	if (overflow) {
		int_val++;
	}
	
	int_part = (string)int_val;
	
	return int_part + "." + frac_part;
}

/*
 * NAME:	calculate_exponent()
 * DESCRIPTION:	calculate the exponent to normalize a float to [1.0, 10.0)
 * RETURNS:	exponent value
 */
private int calculate_exponent(float value)
{
	int exponent;
	float abs_value;
	
	abs_value = value;
	exponent = 0;
	
	if (abs_value >= 10.0) {
		while (abs_value >= 10.0) {
			abs_value /= 10.0;
			exponent++;
		}
	} else if (abs_value < 1.0) {
		while (abs_value < 1.0) {
			abs_value *= 10.0;
			exponent--;
		}
	}
	
	return exponent;
}

/*
 * NAME:	normalize_to_mantissa()
 * DESCRIPTION:	normalize a value to mantissa in range [1.0, 10.0)
 */
private float normalize_to_mantissa(float value, int exponent)
{
	float power_of_10;
	
	power_of_10 = pow(10.0, (float)exponent);
	return value / power_of_10;
}

/*
 * NAME:	format_exponent_part()
 * DESCRIPTION:	format the exponent part of scientific notation
 */
private string format_exponent_part(int exponent, int uppercase)
{
	string exp_sign;
	int abs_exponent;
	
	exp_sign = exponent >= 0 ? "+" : "-";
	abs_exponent = exponent >= 0 ? exponent : -exponent;
	
	return (uppercase ? "E" : "e") + exp_sign + pad_left(abs_exponent, 2, "0");
}

/*
 * NAME:	extract_sign()
 * DESCRIPTION:	extract sign from value and return unsigned value
 * RETURNS:	({ sign_string, absolute_value })
 */
private mixed *extract_sign(float value)
{
    return value < 0.0
        ? ({ "-", -value })
        : ({ "", value });
}

/*
 * NAME:	format_scientific_zero()
 * DESCRIPTION:	format zero in scientific notation
 */
private string format_scientific_zero(int precision, int uppercase)
{
	return "0." + str_repeat("0", precision) + (uppercase ? "E" : "e") + "+00";
}

/*
 * NAME:	format_scientific()
 * DESCRIPTION:	format a float in scientific notation
 */
string format_scientific(float value, int precision, int uppercase)
{
	string sign, mantissa, exp_part;
	int exponent;
	float abs_value;
	mixed *sign_parts;
	
	if (precision < 0) {
		precision = 6;
	}
	
	if (value == 0.0) {
		return format_scientific_zero(precision, uppercase);
	}
	
	sign_parts = extract_sign(value);
	sign = sign_parts[0];
	abs_value = sign_parts[1];
	
	exponent = calculate_exponent(abs_value);
	abs_value = normalize_to_mantissa(abs_value, exponent);
	
	mantissa = format_float(abs_value, precision);
	exp_part = format_exponent_part(exponent, uppercase);
	
	return sign + mantissa + exp_part;
}
