#include <kfun.h>
#include <Array.h>
#include <Iterator.h>
#include <Math.h>
#include <Number.h>
#include <String.h>
#include <Statistics.h>
#include <Try.h>
#include <status.h>
#include "/usr/LPC/api/include/compiler.h"
#include "/usr/LPC/api/include/Code.h"
#include <nk_lib.h>

inherit terminal "/lib/util/terminal";
inherit format "/lib/util/format";
inherit base64 "/lib/util/base64";

#define JSONENCODE	"/sys/jsonencode"

private object me;

private void load_functions(void);

static void create(void) {
    me = "/usr/System/sys/userd"->find_user("nik");
    load_functions();
}

private void load_functions(void) {
    int i, sz;
    string *functions;

    functions = ({
        AGE_RANGE_KEY_EXTRACTOR,
        GREATER_THAN_PREDICATE,
        NAME_LOCATION_KEY_EXTRACTOR
    });
    try {
        for (i = 0, sz = sizeof(functions); i < sz; i++) {
            me->println("reloading: " + functions[i] + ".c");
            compile_object(functions[i]);
        }
    } catch (...) {}
}

private int valid(string str) {
    return str == "/usr/System/obj/user";
}

void clear_screen(void) {
    if (valid(previous_program())) {
        me->message(terminal::clear());
    }
}

string get_prompt(void) {
    if (valid(previous_program())) {
        string name;
        string cwd;
        string end;
        string prompt;
        mapping opts;

        name = terminal::cyan(me->query_name(), 1);
        cwd = terminal::green(me->query_directory());
        end = terminal::arrowRight();

        opts = ([
            "name": ([ "value": name ]),
            "cwd":  ([ "value": cwd ]),
            "end":  ([ "value": end ])
        ]);
        prompt = format::str_format("{:name}:{:cwd} {:end} ", opts);

        if (prompt != nil) {
            return prompt;
        }
    }

    return "> ";
}

void cmd_cls(string arg) {
    if (valid(previous_program())) {
        me->message(terminal::clear());
    }
}

private string base64_encode(string str) {
    return base64::encode(str);
}

private string json_encode(mapping map) {
    return JSONENCODE->encode(map);
}

private string encode_discord_message(string str) {
    mapping map;
    map = ([]);
    map["name"] = me->query_name();
    map["command"] = str;
    return json_encode(map);
}

private int starts_with(string a, string b) {
    if (strlen(a) < strlen(b)) {
        return FALSE;
    }

    return a[0..strlen(b) - 1] == b;
}

private int is_command_known_to_discord(string str) {
    if (!str || str == "") {
        return FALSE;
    }

    return str == "mcstatus";
}

void cmd_disco(string str) {
    if (valid(previous_program())) {
        object discord_bot;

        if (!is_command_known_to_discord(str)) {
            me->println("Unknown Discord bot command.");
            return;
        }

        discord_bot = "/usr/System/discord/sys/discord_botd"->get_discord_bot();
        if (discord_bot == nil) {
            me->println("The Discord bot is not connected.");
            return;
        }

        if (discord_bot->send(encode_discord_message(str))) {
            me->println("Message sent to Discord bot.");
        }
    }
}

void cmd_pi(string str) {
    if (valid(previous_program())) {
        string template, pi, prefix;
        int i;

        if (!str) {
            me->println("how many digits?");
            return;
        }

        sscanf(str, "%d", i);
        template = "%." + i + "LF";
        prefix = format::sprintf("%26s", "pi =");
        pi = format::sprintf(template, 4.0 * atan(1.0));
        me->println(format::str_format("{:prefix} {:pi}", ([
            "prefix": ([ "value": prefix ]),
            "pi": ([ "value": pi ])
        ])));
    }
}

void cmd_colors(string str) {
    if (valid(previous_program())) {
        Iterator iterator;
        int *heatMap;

        heatMap = allocate_int(7);
        heatMap[0] = 46;
        heatMap[1] = 226;
        heatMap[2] = 220;
        heatMap[3] = 214;
        heatMap[4] = 208;
        heatMap[5] = 202;
        heatMap[6] = 196;
        iterator = new IntIterator(0, sizeof(heatMap) - 1);

        while (!iterator->end()) {
            me->message(terminal::color256(heatMap[iterator->next()], "x"));
        }

        me->println("\nDone.");
    }
}

void cmd_arrays(string str) {
    if (valid(previous_program())) {
        Array a, b, c;
        a = new Array(({ 3, 1, 2 }));
        me->println(a->stringify());
        c = a->toSorted();
        me->println(a->stringify());
        me->println(c->stringify());

        b = new Array(({ 1 }));
        me->println("Subset: " + (b->is_subset(a)));

        a = new Array(({}));
        me->println("find first (empty): " + format::to_string(a->find_first(42)));

        a = new Array(({ 1, 2, 3 }));
        me->println("find first (found): " + format::to_string(a->find_first(2)));
        me->println("find first (not found): " + format::to_string(a->find_first(4)));
    }
}

void cmd_grouping(string str) {
    if (valid(previous_program())) {
        Array people;
        mapping grouping;
        Function key_extractor;
        
        people = new Array(({
            ([ "name": "Alice", "age": 25, "city": "New York", "state": "NY" ]),
            ([ "name": "Bob", "age": 35, "city": "Los Angeles", "state": "CA" ]),
            ([ "name": "Charlie", "age": 45, "city": "New York", "state": "NY" ])
        }));

        grouping = people->group_by("age");
        me->println("case1:\n" + format::to_string(grouping));

        grouping = people->group_by("age", TRUE);
        me->println("case2:\n" + format::to_string(grouping));

        grouping = people->group_by(new KeyExtractor("age"));
        me->println("case3:\n" + format::to_string(grouping));

        grouping = people->group_by(new NameLocationKeyExtractor());
        me->println("case4:\n" + format::to_string(grouping));

        grouping = people->group_by(new AgeRangeKeyExtractor());
        me->println("case5:\n" + format::to_string(grouping));
    }
}

void cmd_reload(string str) {
    if (valid(previous_program())) {
        load_functions();
    }
}

void cmd_joining(string str) {
    if (valid(previous_program())) {
        Array array;
        mapping type_map;

        array = new Array(({ 69, 3.14, "forty two", 42 }));
        me->println(array->joining(", ", TRUE));

        type_map = array->partition_by_type();
        me->println(format::to_string(type_map));

        type_map = array->partition(new GreaterThanPredicate(25));
        me->println(format::to_string(type_map));
    }
}

void cmd_cmp(string str) {
    if (valid(previous_program())) {
        Number a, b, c;
        int x;
        float f;

        a = new Number(0.000000000000000001);
        b = new Number(2.01);
        c = b / a;
        me->println("c = " + c->to_float());
    }
}

void cmd_flt(string str) {
    if (valid(previous_program())) {
        int i;
        float* tests;
        Rational r;

        tests = ({
            0.0, 1.0, -1.0, 3.0, 3.1415926535, -2.5, 1e-10, 4.2
        });
        
        for (i = 0; i < 8; i++) {
            r = new Rational(new Number(tests[i]));
            me->println(r->to_string());
        }
    }
}

void cmd_complex(string str) {
    if (valid(previous_program())) {
        Complex za, zb, z;

        za = new Complex(1, 2);
        zb = new Complex(3, 4);
        
        z = za + zb;
        me->println(z->to_string());

        z = za - zb;
        me->println(z->to_string());

        z = za * zb;
        me->println(z->to_string());

        z = za / zb;
        me->println(z->to_string());
    }
}

void cmd_rando(string str) {
    if (valid(previous_program())) {
		int r;
		float f;
		r = random(0);
		f = (float)r / 1000.0;
		me->println(format::to_string(f));
	}
}

void cmd_uuid(string str) {
    if (valid(previous_program())) {
		me->println(uuid());
	}
}

private float *read_data(string filename) {
    string content;
    string *lines;
    float *data;
    int i, sz, j;

    me->println("Reading data from " + filename);
    content = read_file(filename);
    lines = explode(content, "\n");
    sz = sizeof(lines);
    data = allocate_float(sz);
    for (i = 0, j = 0; i < sz; i++, j++) {
        float f;
        f = (float) lines[i];
        data[i] = f;

        if (j < 10) {
            me->println("data " + i + ": " + data[i]);
        }
    }
    return data;
}

void cmd_distgen(string str) {
    if (valid(previous_program())) {
        float *data;
        DistributionGenerator distgen;
        int i, sample_size;

        if (!str || str == "") {
            me->println("Choose a sampler: uniform, right-skew");
            return;
        }

        sample_size = 10;
        distgen = new DistributionGenerator(sample_size);

        switch (str) {
            case "uniform": data = distgen->generate_distribution("uniform", -5.0, 5.0); break;
            case "right-skew": data = distgen->generate_distribution("gamma", 3.0); break;
            default: data = distgen->generate_distribution("normal", 40.0, 3.0); break;
        }

        for (i = 0; i < sample_size; i++) {
            me->println("" + i + ": " + data[i]);
        }
    }
}

void cmd_histogram(string str) {
    if (valid(previous_program())) {
        float *data;
        string *stats;
        string histogram, analysis, distribution, outliers;
        int n;

        if (str) {
            string filename;
            
            filename = "/usr/nik/data/" + str + ".txt";
            data = read_data(filename);
        } else {
            int i;
            n = 32767;

            me->println("Generating random data");
            data = allocate_float(n);
            for (i = 0; i < n; i++) {
                int mod;
                mod = (int)pow(2.0, 16.0);
                data[i] = (float) random(mod) * 0.01;
            }
        }
		
        stats = statistics(data);
        histogram = stats[0];
        analysis = stats[1];
        distribution = stats[2];
        outliers = stats[3];

        me->println(terminal::bold("\nHISTOGRAM"));
		me->println(histogram);

        me->println(terminal::bold("\nANALYSIS"));
        me->println(analysis);

        me->println(terminal::bold("\nDISTRIBUTION"));
        me->println(distribution);

        me->println(terminal::bold("\nOUTLIERS"));
        me->println(outliers);
	}
}

void cmd_tabular(string str) {
    if (valid(previous_program())) {
        mixed **data;
        string result;

        data = ({
            ({ "A", "B", "C" }),
            ({ "asfsdasf", "lassdfdf", "alssdffsdds" }),
            ({ "alsdlkfas lsdf lsd f", "colspan:3", nil }),
            ({ "aissdfjffsasd", "alsdfsdsdf", "aldfsdfsd" })
        });
        result = format::table(data, 4, TRUE);
        me->println(result);
    }
}

void cmd_table(string str) {
    if (valid(previous_program())) {
        mixed **data;
        string output;
        
        data = ({
            ({ "Name", "Age", "Balance" }),
            ({ "Alice", 25, 1234.56 }),
            ({ "Bob", 30, 987.65 })
        });

        output = table(data, 4, 1);
        me->println(output);

        data = ({
            ({ "Name", ({ "Address Info", "colspan:2" }), "Phone" }),
            ({ "Alice", "123 Main St", "Apt 4", "555-1234" })
        });
        output = table(data, 4, 1);
        me->println(output);
    }
}

void cmd_format(string str) {
    if (valid(previous_program())) {
        string output;

        output = str_format("Hello {:name}!", ([ "name": "World" ]));
        me->println(output);


        output = str_format("Price: {:price}", ([ 
            "price": ([ 
                "value": 123.456, 
                "type": "f", 
                "precision": 2,
                "width": 8
            ])
        ]));
        me->println(output);
        output = str_format("{:label} {:num}", ([
            "label": ([
                "value": "Count",
                "width": 10,
                "align": "left"
            ]),
            "num": ([
                "value": 42,
                "width": 5,
                "pad": "0"
            ])
        ]));
        me->println(output);
        output = str_format("This {} is not a placeholder", ([]));
        me->println(output);
        output = str_format("Name: {:name}, Age: {:age}, Balance: {:balance}", ([
            "name": "Alice",
            "age": 25,
            "balance": ([
                "value": 1234.56,
                "type": "f",
                "precision": 2
            ])
        ]));
        me->println(output);

        output = format::str_format("{:small} {:normal} {:large}", ([
            "small": ([ "value": -0.00001, "type": "g", "precision": 2 ]),
            "normal": ([ "value": -123.456, "type": "g", "precision": 2 ]),
            "large": ([ "value": -1234567.0, "type": "g", "precision": 2 ])
        ]));
        me->println(output);
    }
}

void cmd_try(string str) {
    if (valid(previous_program())) {
        string try_object_path;
        object try_object;
        string answer;

        try_object_path = "/usr/nik/obj/try_object";
        compile_object(try_object_path);
        try_object = clone_object(try_object_path, 42);

        answer = TRY_SUCCESS(try_object->initial_value())
            ->map(try_object, "add", 1)
            ->map(try_object, "divide", 0)
            ->map(try_object, "subtract", 1)
            ->fold();

        me->println(answer);
        me->println("mutated? " + try_object->was_mutated());
    }
}
