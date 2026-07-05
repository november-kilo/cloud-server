#include <type.h>

inherit "/lib/util/format/float";
inherit "/lib/util/format/integer";
inherit "/lib/util/format/repeat";

private mapping process_spec(mapping spec) {
    string pad_char;
	int width, precision, left_align;

    width = spec["width"] ? spec["width"] : 0;
	precision = spec["precision"] ? spec["precision"] : -1;
	left_align = spec["align"] && spec["align"] == "left" ? 1 : 0;
	pad_char = spec["pad"] ? spec["pad"] : " ";

    return ([
        "width": width,
        "precision": precision,
        "left_align": left_align,
        "pad_char": pad_char
    ]);
}

string cell_to_string(mixed cell) {
	return cell == nil ? "nil" : (string)cell;
}

string apply_format_spec(mixed value, mapping spec) {
	string str, pad_char;
	int width, precision, left_align;
    mapping processed_spec;

    processed_spec = process_spec(spec);
	width = processed_spec["width"];
	precision = processed_spec["precision"];
	left_align = processed_spec["left_align"];
	pad_char = processed_spec["pad_char"];
	
	if (spec["type"]) {
		switch (spec["type"]) {
			case "i":
				str = (string)((int)value);
				break;
				
			case "f":
				if (typeof(value) == T_FLOAT || typeof(value) == T_INT) {
					str = format_float((float)value, precision >= 0 ? precision : 6);
				} else {
					str = (string)value;
				}
				break;
				
			case "e":
				if (typeof(value) == T_FLOAT || typeof(value) == T_INT) {
					str = format_scientific((float)value, precision >= 0 ? precision : 6, 0);
				} else {
					str = (string)value;
				}
				break;
				
			case "E":
				if (typeof(value) == T_FLOAT || typeof(value) == T_INT) {
					str = format_scientific((float)value, precision >= 0 ? precision : 6, 1);
				} else {
					str = (string)value;
				}
				break;
				
			case "g":
			case "G":
				if (typeof(value) == T_FLOAT || typeof(value) == T_INT) {
					float fval;
					int use_scientific;
					
					fval = (float)value;
					
					if (fval == 0.0) {
						use_scientific = 0;
					} else {
						float abs_val;
						abs_val = fabs(fval);
						use_scientific = (abs_val < 0.0001 || abs_val >= 1000000.0);
					}
					
					if (use_scientific) {
						int mantissa_precision;
						mantissa_precision = (precision >= 0 ? precision : 6) - 1;
						if (mantissa_precision < 0) {
							mantissa_precision = 0;
						}
						str = format_scientific(fval, mantissa_precision, 
						                       spec["type"] == "G" ? 1 : 0);
					} else {
						str = format_float(fval, precision >= 0 ? precision : 6);
					}
				} else {
					str = (string)value;
				}
				break;
				
			case "x":
				str = "0x" + to_hex((int)value);
				break;
				
			case "X":
				str = "0X" + to_hex((int)value);
				break;
				
			case "o":
				str = "0" + to_octal((int)value);
				break;
				
			case "s":
			default:
				str = cell_to_string(value);
				break;
		}
	} else {
		str = cell_to_string(value);
	}
	
	if (width > 0 && strlen(str) < width) {
		str = left_align ? pad_right(str, width, pad_char) : pad_left(str, width, pad_char);
	}
	
	return str;
}
