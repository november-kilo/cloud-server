#ifndef METRICS_ERROR_H
#define METRICS_ERROR_H

#include <stdbool.h>
#include "lpc_ext.h"

#define CREATE_ERROR_METRICS_TABLE_SQL \
"DROP TABLE IF EXISTS error_metrics; " \
"CREATE TABLE IF NOT EXISTS error_metrics (" \
"id INTEGER PRIMARY KEY AUTOINCREMENT," \
"timestamp INTEGER NOT NULL," \
"error_source TEXT NOT NULL," \
"error_message TEXT NOT NULL" \
");"

#define INSERT_ERROR_METRICS_SQL \
"INSERT INTO error_metrics (" \
"timestamp, error_source, error_message" \
") VALUES (?, ?, ?);"

typedef struct {
    LPC_int timestamp;
    LPC_string error_source;
    LPC_string error_message;
} ErrorMetrics;

void collect_error_metrics(LPC_frame frame, LPC_mapping metrics, ErrorMetrics *m);
bool save_error_metrics(LPC_frame frame, const ErrorMetrics *m);

#endif
