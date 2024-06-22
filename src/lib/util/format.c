#include <kfun.h>
#include <type.h>

static string spaces(int width) {
    string str;

    str = "                                                               ";
    while (strlen(str) < width) {
        str += str;
    }

    return str[.. width - 1];
}

static string sprintf(string template, mixed value) {
#ifdef KF_SNPRINTF
    return snprintf(template, value);
#else
    error("snprintf() not implemented");
#endif
}

static string format(string template, mixed *values) {
    int i, sz, idx;
    string *output;

    i = 0;
    sz = strlen(template);
    output = ({});
    idx = 0;
    while (i < sz) {
        if (template[i++] == '%') {
            try {
                switch (template[i++]) {
                    case '%':
                        output += ({ "%" });
                        break;
                    case 's':
                        output += ({ values[idx++] });
                        break;
                    case 'f':
                    case 'd':
                        output += ({ (string) values[idx++] });
                        break;
                }
            } catch(...) {
                error("format: invalid template");
            }
        } else {
            int x;

            x = i - 1;
            output += ({ template[x..x] });
        }
    }
    return implode(output, "");
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
