#include <type.h>

inherit "/lib/util/format/spec";

/*
 * NAME:	str_format()
 * DESCRIPTION:	format a string with named placeholders
 * USAGE:	str_format("Hello {:name}!", ([ "name": "World" ]))
 *		str_format("Value: {:price}", ([ "price": ([ "value": 123.45, "type": "f", "width": 8, "precision": 2 ]) ]))
 *		str_format("{:label} {:num}", ([ "label": ([ "value": "Count", "width": 10, "align": "left" ]), 
 *		                                  "num": ([ "value": 42, "width": 5, "pad": "0" ]) ]))
 *
 * Format spec keys (when value is a mapping):
 *   - value: the actual value (required)
 *   - width: minimum field width
 *   - precision: decimal places for floats
 *   - align: "left" for left alignment (default is right)
 *   - pad: padding character (default is space)
 *   - type: "s" (string), "i" (int), "f" (float), "e"/"E" (scientific), 
 *           "g"/"G" (auto), "x"/"X" (hex), "o" (octal)
 */
string str_format(string template, mapping values) {
	int i, sz, start, end;
	string *output, key, replacement;
	mixed value;
	
	i = 0;
	sz = strlen(template);
	output = ({});
	
	while (i < sz) {
		if (i < sz - 1 && template[i] == '{' && template[i + 1] == ':') {
			start = i + 2;
			end = start;
			while (end < sz && template[end] != '}') {
				end++;
			}
			
			if (end >= sz) {
				error("str_format: unclosed placeholder at position " + i);
			}
			
			key = template[start .. end - 1];
			
			if (!values[key]) {
				error("str_format: undefined placeholder '" + key + "'");
			}
			
			value = values[key];
			
			if (typeof(value) == T_MAPPING) {
				if (!value["value"]) {
					error("str_format: placeholder '" + key + "' mapping must have 'value' key");
				}
				replacement = apply_format_spec(value["value"], value);
			} else {
				replacement = cell_to_string(value);
			}
			
			output += ({ replacement });
			i = end + 1;
		} else if (template[i] == '{' && i + 1 < sz && template[i + 1] == '}') {
			output += ({ "{}" });
			i += 2;
		} else {
			output += ({ template[i .. i] });
			i++;
		}
	}
	
	return implode(output, "");
}
