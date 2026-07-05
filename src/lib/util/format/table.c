#include <type.h>

inherit "/lib/util/format/spec";

int get_cell_alignment(mixed cell) {
	int type;
	
	type = typeof(cell);
	
	return (type == T_INT || type == T_FLOAT) ? 1 : 0;
}

string pad_cell(string cell, int width, int align_right) {
	return align_right ? pad_left(cell, width) : pad_right(cell, width);
}

mixed *get_colspan(mixed cell) {
	int colspan;
	
	if (typeof(cell) == T_ARRAY && sizeof(cell) == 2) {
		if (typeof(cell[1]) == T_STRING && sscanf(cell[1], "colspan:%d", colspan) == 1) {
			return ({ colspan, cell[0] });
		}
	}
	
	return ({ 1, cell });
}

int *calculate_column_widths(mixed **data, int rows, int cols, int column_padding) {
	int i, j, k, col_index, len, colspan, total_width, extra, per_col, remainder;
	int *col_widths;
	string cell_str;
	mixed *row, *colspan_info, cell_value;
	
	col_widths = allocate_int(cols);
	
	for (i = 0; i < rows; i++) {
		row = data[i];
		j = 0;
		col_index = 0;
		
		while (j < sizeof(row) && col_index < cols) {
			colspan_info = get_colspan(row[j]);
			colspan = colspan_info[0];
			cell_value = colspan_info[1];
			cell_str = cell_to_string(cell_value);
			len = strlen(cell_str);
			
			if (colspan == 1) {
				if (len > col_widths[col_index]) {
					col_widths[col_index] = len;
				}
			} else {
				total_width = 0;
				for (k = 0; k < colspan && col_index + k < cols; k++) {
					total_width += col_widths[col_index + k];
				}
				total_width += (colspan - 1) * column_padding;
				
				if (len > total_width) {
					extra = len - total_width;
					per_col = extra / colspan;
					remainder = extra % colspan;
					
					for (k = 0; k < colspan && col_index + k < cols; k++) {
						col_widths[col_index + k] += per_col + (k < remainder ? 1 : 0);
					}
				}
			}
			
			col_index += colspan;
			j++;
		}
	}
	
	return col_widths;
}

int *calculate_column_alignments(mixed **data, int rows, int cols, int has_header) {
	int i, j, col_index, start_row, colspan;
	int *col_alignments;
	mixed *row, *colspan_info, cell_value;
	
	col_alignments = allocate_int(cols);
	start_row = has_header ? 1 : 0;
	
	if (start_row < rows) {
		row = data[start_row];
		j = 0;
		col_index = 0;
		
		while (j < sizeof(row) && col_index < cols) {
			colspan_info = get_colspan(row[j]);
			colspan = colspan_info[0];
			cell_value = colspan_info[1];
			
			col_alignments[col_index] = get_cell_alignment(cell_value);
			
			col_index += colspan;
			j++;
		}
	}
	
	return col_alignments;
}

int calculate_total_columns(mixed **data) {
	int i, j, max_cols, row_cols, colspan;
	mixed *row, *colspan_info;
	
	max_cols = 0;
	
	for (i = 0; i < sizeof(data); i++) {
		row = data[i];
		row_cols = 0;
		j = 0;
		
		while (j < sizeof(row)) {
			colspan_info = get_colspan(row[j]);
			colspan = colspan_info[0];
			row_cols += colspan;
			j++;
		}
		
		if (row_cols > max_cols) {
			max_cols = row_cols;
		}
	}
	
	return max_cols;
}

string table_row(mixed *row, int *col_widths, int *col_alignments, 
                         int cols, int column_padding, int force_left_align) {
	int j, k, col_index, colspan, total_width;
	string cell;
	mixed *colspan_info, cell_value;
	string *cells;
	
	cells = ({});
	j = 0;
	col_index = 0;
	
	while (j < sizeof(row) && col_index < cols) {
		colspan_info = get_colspan(row[j]);
		colspan = colspan_info[0];
		cell_value = colspan_info[1];
		cell = cell_to_string(cell_value);
		
		total_width = 0;
		for (k = 0; k < colspan && col_index + k < cols; k++) {
			total_width += col_widths[col_index + k];
		}
		if (colspan > 1) {
			total_width += (colspan - 1) * column_padding;
		}
		
		if (force_left_align) {
			cell = pad_right(cell, total_width);
		} else {
			cell = pad_cell(cell, total_width, col_alignments[col_index]);
		}
		
		cells += ({ cell });
		
		col_index += colspan;
		j++;
	}
	
	return implode(cells, spaces(column_padding));
}

string create_header_separator(int *col_widths, int cols, int column_padding) {
	int j;
	string line;
	
	line = "";
	for (j = 0; j < cols; j++) {
		line += pad_right("", col_widths[j], "-");
		if (j < cols - 1) {
			line += spaces(column_padding);
		}
	}
	
	return line;
}

/*
 * NAME:	table()
 * DESCRIPTION:	format a 2D array into aligned columns
 * PARAMETERS:	data - 2D array of data to format
 *              column_padding - spaces between columns
 *              has_header - if true, first row is treated as header (left-aligned with separator)
 *              
 * COLSPAN:     To span multiple columns, use: ({ value, "colspan:N" })
 *              Example: ({ "Name", ({ "Full Address", "colspan:2" }), "Phone" })
 */
string table(mixed **data, int column_padding, int has_header) {
	int i, rows, cols;
	int *col_widths, *col_alignments;
	string *output_lines;
	mixed *row;
	
	if (!data || sizeof(data) == 0) {
		return "";
	}
	
	rows = sizeof(data);
	cols = calculate_total_columns(data);
	col_widths = calculate_column_widths(data, rows, cols, column_padding);
	col_alignments = calculate_column_alignments(data, rows, cols, has_header);
	output_lines = allocate(has_header ? rows + 1 : rows);
	
	for (i = 0; i < rows; i++) {
		row = data[i];
		
		if (i == 0 && has_header) {
			output_lines[i] = table_row(row, col_widths, col_alignments, 
			                            cols, column_padding, 1);
			output_lines[i + 1] = create_header_separator(col_widths, cols, column_padding);
		} else {
			output_lines[has_header ? i + 1 : i] = 
				table_row(row, col_widths, col_alignments, 
				          cols, column_padding, 0);
		}
	}
	
	return implode(output_lines, "\n");
}

static string to_string(mixed value, varargs mapping seen) {
    string str;
    int i, sz;
    mixed *indices, *values;

    if (seen == nil) {
        seen = ([]);
    }

    switch (typeof(value)) {
        case T_FLOAT:
            str = (string) value;
            if (sscanf(str, "%*s.") == 0 && sscanf(str, "%*se") == 0) {
                if (value >= 0.0) {
                    value += 0.05;
                    str = (string) floor(value);
                } else {
                    value -= 0.05;
                    str = (string) ceil(value);
                }
                str += "." + floor(fmod(fabs(value) * 10.0, 10.0));
            }
            return str;

        case T_INT:
            return (string) value;

        case T_STRING:
            str = value;
            if (sscanf(str, "%*s\\") != 0) {
                str = implode(explode("\\" + str + "\\", "\\"), "\\\\");
            }
            if (sscanf(str, "%*s\"") != 0) {
                str = implode(explode("\"" + str + "\"", "\""), "\\\"");
            }
            if (sscanf(str, "%*s\n") != 0) {
                str = implode(explode("\n" + str + "\n", "\n"), "\\n");
            }
            if (sscanf(str, "%*s\t") != 0) {
                str = implode(explode("\t" + str + "\t", "\t"), "\\t");
            }
            return "\"" + str + "\"";

        case T_OBJECT:
            return "<" + object_name(value) + ">";

        case T_ARRAY:
            if (seen[value]) {
                return "#" + (seen[value] - 1);
            }

            seen[value] = map_sizeof(seen) + 1;
            sz = sizeof(value);
            if (sz == 0) {
                return "({ })";
            }

            str = "({ ";
            for (i = 0, --sz; i < sz; i++) {
                str += to_string(value[i], seen) + ", ";
            }
            return str + to_string(value[i], seen) + " })";

        case T_MAPPING:
            if (seen[value]) {
                return "@" + (seen[value] - 1);
            }

            seen[value] = map_sizeof(seen) + 1;
            sz = map_sizeof(value);
            if (sz == 0) {
                return "([ ])";
            }

            str = "([ ";
            indices = map_indices(value);
            values = map_values(value);
            for (i = 0, --sz; i < sz; i++) {
                str += to_string(indices[i], seen) + ":" +
                       to_string(values[i], seen) + ", ";
            }
            return str + to_string(indices[i], seen) + ":" +
                   to_string(values[i], seen) + " ])";

        case T_NIL:
            return "nil";
    }
}
