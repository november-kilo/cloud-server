/*
 * NAME:	handle_zero_case()
 * DESCRIPTION:	return "0" if value is zero, nil otherwise
 */
private string handle_zero_case(int value)
{
	if (value == 0) {
		return "0";
	}
	return nil;
}

/*
 * NAME:	add_sign()
 * DESCRIPTION:	add negative sign if needed
 */
private string add_sign(string number, int negative)
{
	return negative ? "-" + number : number;
}

/*
 * NAME:	extract_sign()
 * DESCRIPTION:	extract sign and return absolute value
 * RETURNS:	({ absolute_value, is_negative })
 */
private int *extract_sign(int value)
{
	int negative;

	negative = (value < 0);
	
	if (negative) {
		value = -value;
	}

	return ({ value, negative });
}

/*
 * NAME:	convert_to_base_shift()
 * DESCRIPTION:	convert integer to string in power-of-2 base using bit shifts
 * PARAMETERS:	value - the integer to convert
 *		shift_amount - bits per digit (4 for hex, 3 for octal, 1 for binary)
 *		mask - bitmask for extracting digit (15 for hex, 7 for octal, 1 for binary)
 *		digits - digit character set
 */
private string convert_to_base_shift(int value, int shift_amount, int mask, string digits)
{
	string result;
	
	result = "";
	while (value > 0) {
		result = digits[value & mask .. value & mask] + result;
		value >>= shift_amount;
	}
	
	return result;
}

private string convert_to_hex(int value) {
    return convert_to_base_shift(value, 4, 15, "0123456789abcdef");
}

private string convert_to_octal(int value) {
    return convert_to_base_shift(value, 3, 7, "01234567");
}

private string convert_to_binary(int value) {
    return convert_to_base_shift(value, 1, 1, "01");
}

/*
 * NAME:	to_hex()
 * DESCRIPTION:	convert integer to hexadecimal string
 */
string to_hex(int value)
{
	string hex, zero_case;
	int negative;
	
	zero_case = handle_zero_case(value);
	if (zero_case) {
		return zero_case;
	}
	
	({ value, negative }) = extract_sign(value);
	hex = convert_to_hex(value);
	
	return add_sign(hex, negative);
}

/*
 * NAME:	to_octal()
 * DESCRIPTION:	convert integer to octal string
 */
string to_octal(int value)
{
	string oct, zero_case;
	int negative;
	
	zero_case = handle_zero_case(value);
	if (zero_case) {
		return zero_case;
	}
	
	({ value, negative }) = extract_sign(value);
	oct = convert_to_octal(value);
	
	return add_sign(oct, negative);
}

/*
 * NAME:	to_binary()
 * DESCRIPTION:	convert integer to binary string
 */
string to_binary(int value)
{
	string bin, zero_case;
	int negative;
	
	zero_case = handle_zero_case(value);
	if (zero_case) {
		return zero_case;
	}
	
	({ value, negative }) = extract_sign(value);
	bin = convert_to_binary(value);
	
	return add_sign(bin, negative);
}
