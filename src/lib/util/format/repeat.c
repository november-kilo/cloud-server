private string repeat(string str, int count) {
	string result;
    int i;

	result = "";
    for (i = 0; i < count; i++) {
        result += str;
    }

    return result;
}

private void validate(int count) {
    if (count < 1) {
        error("str_repeat: count must be greater than zero");
    }
}

private mixed *str_and_length(mixed value) {
    string str;
    int length;

    str = (string)value;
    length = strlen(str);

    return ({ str, length });
}

private int get_padding_needed(int width, int length) {
    return width - length;
}

private string get_pad_char(varargs string pad_char) {
    if (!pad_char || strlen(pad_char) == 0) {
		pad_char = " ";
	}
	
	if (strlen(pad_char) > 1) {
		pad_char = pad_char[0..0];
	}

    return pad_char;
}

string str_repeat(string str, int count) {
    validate(count);
    return repeat(str, count);
}

string spaces(int count) {
    if (count < 1) {
        count = 1;
    }

    return str_repeat(" ", count);
}

string pad_right(mixed value, int width, varargs string pad_char) {
	string str;
	int len, padding_needed;
	
    ({ str, len }) = str_and_length(value);
	
	if (len >= width) {
		return str;
	}
	
	padding_needed = get_padding_needed(width, len);
	pad_char = get_pad_char(pad_char);
	
	return str + str_repeat(pad_char, padding_needed);
}

string pad_left(mixed value, int width, varargs string pad_char) {
	string str;
	int len, padding_needed;
	
	str = (string)value;
	len = strlen(str);
	
	if (len >= width) {
		return str;
	}
	
	padding_needed = width - len;
	pad_char = get_pad_char(pad_char);
	
	return str_repeat(pad_char, padding_needed) + str;
}
