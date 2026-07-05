#ifndef METRICS_STATUS_H
#define METRICS_STATUS_H

#include <stdbool.h>
#include "lpc_ext.h"

#define CREATE_STATUS_METRICS_TABLE_SQL \
"DROP TABLE IF EXISTS status_metrics; " \
"CREATE TABLE IF NOT EXISTS status_metrics (" \
"id INTEGER PRIMARY KEY AUTOINCREMENT," \
"timestamp INTEGER NOT NULL," \
"start_time INTEGER NOT NULL," \
"boot_time INTEGER NOT NULL," \
"uptime INTEGER NOT NULL," \
"swap_used INTEGER NOT NULL," \
"swap_total INTEGER NOT NULL," \
"static_mem_used INTEGER NOT NULL," \
"static_mem_total INTEGER NOT NULL," \
"dynamic_mem_used INTEGER NOT NULL," \
"dynamic_mem_total INTEGER NOT NULL," \
"objects INTEGER NOT NULL," \
"objects_total INTEGER NOT NULL," \
"users INTEGER NOT NULL," \
"users_total INTEGER NOT NULL," \
"callouts_short INTEGER NOT NULL," \
"callouts_long INTEGER NOT NULL" \
");"

#define INSERT_STATUS_METRICS_SQL \
"INSERT INTO status_metrics (" \
"timestamp, start_time, boot_time, uptime, swap_used, swap_total, static_mem_used, static_mem_total, " \
"dynamic_mem_used, dynamic_mem_total, objects, objects_total, " \
"users, users_total, callouts_short, callouts_long" \
") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"

typedef struct {
    LPC_int timestamp;
    LPC_int start_time;
    LPC_int boot_time;
    LPC_int uptime;
    LPC_int swap_used;
    LPC_int swap_total;
    LPC_int static_mem_used;
    LPC_int static_mem_total;
    LPC_int dynamic_mem_used;
    LPC_int dynamic_mem_total;
    LPC_int objects;
    LPC_int objects_total;
    LPC_int users;
    LPC_int users_total;
    LPC_int callouts_short;
    LPC_int callouts_long;
} StatusMetrics;

void collect_status_metrics(LPC_frame frame, LPC_mapping metrics, StatusMetrics *m);
bool save_status_metrics(LPC_frame frame, const StatusMetrics *m);

#endif
