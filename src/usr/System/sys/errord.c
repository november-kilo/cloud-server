# include <kernel/kernel.h>
# include <trace.h>
# include <type.h>
# include "tls.h"

# define SYSTEMAUTO	"/usr/System/lib/auto"

inherit auto AUTO;
inherit SYSTEMAUTO;
inherit format "/lib/util/format";

object driver;

/*
 * NAME:	create()
 * DESCRIPTION:	initialize this object
 */
static void create(void) {
    driver = find_object(DRIVER);
    driver->set_error_manager(this_object());
}

/*
 * NAME: record_error_metrics
 * DESCRIPTINON: send error message to monitoring service
 */
private void record_error_metrics(string source, string message) {
	/* TODO: implement */
}

/*
 * NAME:	record_runtime
 * DESCRIPTION: record a runtime error message
 */
private void record_runtime(string message) {
	record_error_metrics("runtime", message);
}

/*
 * NAME:	record_compile
 * DESCRIPTION: record a compile error message
 */
private void record_compile(string message) {
	record_error_metrics("compile", message);
}

/*
 * NAME:	record_atomic
 * DESCRIPTION: record an atomic error message
 */
private void record_atomic(string message) {
	record_error_metrics("atomic", message);
}

/*
 * NAME:	should_include_trace_entry()
 * DESCRIPTION:	determine if a trace entry should be included in output
 */
private int should_include_trace_entry(string progname, string func_name) {
	return progname != AUTO && (progname != SYSTEMAUTO || func_name[..2] != "_F_");
}

/*
 * NAME:	format_trace_line()
 * DESCRIPTION:	format a single trace line with line number and optional marker
 */
private string format_trace_line(int line, int is_caught, string func_name) {
	string line_prefix, caught_marker;
	
	line_prefix = (line != 0) ? format::pad_left(line, 5) : "     ";
	caught_marker = is_caught ? " * " : "   ";
	
	return line_prefix + caught_marker + func_name;
}

/*
 * NAME:	build_trace_table()
 * DESCRIPTION:	build trace data as a 2D array for table formatting
 */
private mixed **build_trace_table(mixed **trace, int start_index, int caught_index) {
	int i, j, sz, line;
	string progname, objname, last_obj, func_name, formatted_line;
	mixed *ftrace, **table_data;
	
	sz = sizeof(trace) - 1;
	table_data = allocate(sz * 2);
	j = 0;

	for (i = start_index; i < sz; i++) {
		ftrace = trace[i];
		objname = ftrace[TRACE_OBJNAME];
		progname = ftrace[TRACE_PROGNAME];
		func_name = ftrace[TRACE_FUNCTION];
		
		if (should_include_trace_entry(progname, func_name)) {
			if (objname != last_obj) {
				table_data[j++] = ({ ({ objname, "colspan:3" }), "" });
				last_obj = objname;
			}

			line = ftrace[TRACE_LINE];
			formatted_line = format_trace_line(line, i + 1 == caught_index, func_name);
			table_data[j++] = ({ formatted_line, progname });
		}
	}

	return table_data[..j - 1];
}

/*
 * NAME:	format_error_message()
 * DESCRIPTION:	format complete error message with trace
 */
private string format_error_message(string error, mixed **trace, int start_index, int caught_index) {
	mixed **table_data;
	string trace_output;
	
	table_data = build_trace_table(trace, start_index, caught_index);
	
	if (sizeof(table_data) == 0) {
		return error + "\n";
	}
	
	trace_output = format::table(table_data, 4, 0);
	return error + "\n" + trace_output + "\n";
}

/*
 * NAME:	send_error_messages()
 * DESCRIPTION:	send error message to driver and optionally to user
 */
private void send_error_messages(string message, int send_to_user) {
	object user;
	
	driver->message(message);
	
	if (send_to_user && (user = this_user())) {
		user->message(message);
	}
}

/*
 * NAME:	runtime_error()
 * DESCRIPTION:	report a runtime error
 */
void runtime_error(string error, int caught, mixed **trace) {
	if (previous_object() == driver) {
		string message;

		auto ::tls_set(TLS_ARGUMENTS, nil);

		if (caught != 0) {
			error += " [caught]";
		}
		
		message = format_error_message(error, trace, 0, caught);
		send_error_messages(message, caught == 0);
		record_runtime(message);
	}
}

/*
 * NAME:	atomic_error()
 * DESCRIPTION:	report a runtime error in atomic code
 */
void atomic_error(string error, int atom, mixed **trace) {
	if (previous_object() == driver) {
		string message;
		
		message = format_error_message(error + " [atomic]", trace, atom, 0);
		send_error_messages(message, 0);
		record_atomic(message);
	}
}

/*
 * NAME:	compile_error()
 * DESCRIPTION:	report a compile-time error
 */
void compile_error(string file, int line, string error) {
	if (previous_object() == driver) {
		string message;

		message = file + ", " + line + ": " + error;
		
		if (auto::tls_get(TLS_COMPILE_ERRORS)) {
			send_atomic_message(message);
		}
		
		message += "\n";
		send_error_messages(message, 1);
		record_compile(message);
	}
}
