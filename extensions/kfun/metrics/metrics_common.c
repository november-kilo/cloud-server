#include <string.h>
#include <stdio.h>
#include "metrics_common.h"

sqlite3 *db = NULL;

void handle_sqlite_error(LPC_frame frame, const char* operation, int rc) {
    if (rc != SQLITE_OK && rc != SQLITE_DONE) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "SQLite error during %s: %s",
                 operation, sqlite3_errmsg(db));
        lpc_runtime_error(frame, error_msg);
    }
}

bool init_database(void) {
    int rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    return true;
}

bool cleanup_old_metrics(LPC_frame frame, int days) {
    const char *tables[] = {"error_metrics", "status_metrics"}; // Add any other tables here
    const int table_count = sizeof(tables) / sizeof(tables[0]);

    time_t now = time(NULL);
    time_t cutoff = now - (days * 24 * 60 * 60);

    for (int i = 0; i < table_count; i++) {
        char sql[256];
        sqlite3_stmt *stmt;
        int rc;

        snprintf(sql, sizeof(sql),
                "DELETE FROM %s WHERE timestamp < ?;",
                tables[i]);

        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            handle_sqlite_error(frame, "prepare cleanup statement", rc);
            return false;
        }

        sqlite3_bind_int64(stmt, 1, cutoff);

        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            handle_sqlite_error(frame, "execute cleanup statement", rc);
            return false;
        }
    }

    return true;
}

LPC_int get_metric_int(LPC_dataspace dataspace, LPC_mapping metrics, const char *metric_name) {
    LPC_value value = lpc_value_temp(dataspace);
    LPC_string index = lpc_string_new(dataspace, metric_name, strlen(metric_name));
    lpc_string_putval(value, index);
    value = lpc_mapping_index(metrics, value);
    return lpc_int_getval(value);
}

LPC_string get_metric_string(LPC_dataspace dataspace, LPC_mapping metrics, const char *metric_name) {
    LPC_value value = lpc_value_temp(dataspace);
    LPC_string index = lpc_string_new(dataspace, metric_name, strlen(metric_name));
    lpc_string_putval(value, index);
    value = lpc_mapping_index(metrics, value);
    return lpc_string_getval(value);
}
