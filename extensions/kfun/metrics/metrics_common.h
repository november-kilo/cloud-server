#ifndef METRICS_KFUN_H
#define METRICS_KFUN_H

#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include "lpc_ext.h"
#include "metrics_error.h"
#include "metrics_status.h"

#define DB_PATH "../metrics.db"

extern sqlite3 *db;

void handle_sqlite_error(LPC_frame frame, const char* operation, int rc);
bool init_database(void);
bool cleanup_old_metrics(LPC_frame frame, int days);

LPC_int get_metric_int(LPC_dataspace dataspace, LPC_mapping metrics, const char *metric_name);
LPC_string get_metric_string(LPC_dataspace dataspace, LPC_mapping metrics, const char *metric_name);

#endif
