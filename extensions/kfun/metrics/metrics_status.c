#include "metrics_common.h"

void collect_status_metrics(LPC_frame frame, LPC_mapping metrics, StatusMetrics *m) {
    LPC_dataspace dataspace = lpc_frame_dataspace(frame);

    m->timestamp = get_metric_int(dataspace, metrics, "timestamp");
    m->start_time = get_metric_int(dataspace, metrics, "start_time");
    m->boot_time = get_metric_int(dataspace, metrics, "boot_time");
    m->uptime = get_metric_int(dataspace, metrics, "uptime");
    m->swap_used = get_metric_int(dataspace, metrics, "swap_used");
    m->swap_total = get_metric_int(dataspace, metrics, "swap_total");
    m->static_mem_used = get_metric_int(dataspace, metrics, "static_mem_used");
    m->static_mem_total = get_metric_int(dataspace, metrics, "static_mem_total");
    m->dynamic_mem_used = get_metric_int(dataspace, metrics, "dynamic_mem_used");
    m->dynamic_mem_total = get_metric_int(dataspace, metrics, "dynamic_mem_total");
    m->objects = get_metric_int(dataspace, metrics, "objects");
    m->objects_total = get_metric_int(dataspace, metrics, "objects_total");
    m->users = get_metric_int(dataspace, metrics, "users");
    m->users_total = get_metric_int(dataspace, metrics, "users_total");
    m->callouts_short = get_metric_int(dataspace, metrics, "callouts_short");
    m->callouts_long = get_metric_int(dataspace, metrics, "callouts_long");
}

bool save_status_metrics(LPC_frame frame, const StatusMetrics *m) {
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, INSERT_STATUS_METRICS_SQL, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        handle_sqlite_error(frame, "prepare statement for status metrics", rc);
        return false;
    }

    sqlite3_bind_int64(stmt, 1, m->timestamp);
    sqlite3_bind_int64(stmt, 2, m->start_time);
    sqlite3_bind_int64(stmt, 3, m->boot_time);
    sqlite3_bind_int64(stmt, 4, m->uptime);
    sqlite3_bind_int64(stmt, 5, m->swap_used);
    sqlite3_bind_int64(stmt, 6, m->swap_total);
    sqlite3_bind_int64(stmt, 7, m->static_mem_used);
    sqlite3_bind_int64(stmt, 8, m->static_mem_total);
    sqlite3_bind_int64(stmt, 9, m->dynamic_mem_used);
    sqlite3_bind_int64(stmt, 10, m->dynamic_mem_total);
    sqlite3_bind_int64(stmt, 11, m->objects);
    sqlite3_bind_int64(stmt, 12, m->objects_total);
    sqlite3_bind_int64(stmt, 13, m->users);
    sqlite3_bind_int64(stmt, 14, m->users_total);
    sqlite3_bind_int64(stmt, 15, m->callouts_short);
    sqlite3_bind_int64(stmt, 16, m->callouts_long);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        handle_sqlite_error(frame, "execute statement for status metrics", rc);
        return false;
    }

    return true;
}

