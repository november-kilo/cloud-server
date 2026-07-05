#include "metrics_common.h"

void collect_error_metrics(LPC_frame frame, LPC_mapping metrics, ErrorMetrics *m) {
    LPC_dataspace dataspace = lpc_frame_dataspace(frame);

    m->timestamp = get_metric_int(dataspace, metrics, "timestamp");
    m->error_source = get_metric_string(dataspace, metrics, "error_source");
    m->error_message = get_metric_string(dataspace, metrics, "error_message");
}

bool save_error_metrics(LPC_frame frame, const ErrorMetrics *m) {
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, INSERT_ERROR_METRICS_SQL, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        handle_sqlite_error(frame, "prepare statement for error metrics", rc);
        return false;
    }

    sqlite3_bind_int64(stmt, 1, m->timestamp);
    sqlite3_bind_text(stmt, 2, lpc_string_text(m->error_source), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, lpc_string_text(m->error_message), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        handle_sqlite_error(frame, "execute statement for error metrics", rc);
        return false;
    }

    return true;
}


