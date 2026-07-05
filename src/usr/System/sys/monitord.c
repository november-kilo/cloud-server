#include <kernel/kernel.h>
#include <status.h>

#define STATUS_RATE 1
#define REPORT_STATUS_FN "report_status_metrics"

#define JSONENCODE "/sys/jsonencode"

object driver;

private void notify_driver(string str);
private object get_driver(void);

static void create(void) {
    get_driver();
    call_out(REPORT_STATUS_FN, STATUS_RATE);
}

private void notify_driver(string str) {
    driver->message(str + "\n");
}

private object get_driver(void) {
    driver = find_object(DRIVER);
    notify_driver("\t- Starting metrics monitor.");
}

private send_metrics(mapping metrics) {
    string json;

    json = JSONENCODE->encode(metrics);
    /* TODO: implement */
}

private mapping create_status_metrics(void) {
    return ([
        "created_at": time(),
        "start_time": status(ST_STARTTIME),
        "boot_time": status(ST_BOOTTIME),
        "uptime": status(ST_UPTIME),
        "swap_size": status(ST_SWAPSIZE),
        "swap_used": status(ST_SWAPUSED),
        "static_mem_total": status(ST_SMEMSIZE),
        "static_mem_used": status(ST_SMEMUSED),
        "dynamic_mem_total": status(ST_DMEMSIZE),
        "dynamic_mem_used": status(ST_DMEMUSED),
        "objects_total": status(ST_OTABSIZE),
        "objects": status(ST_NOBJECTS),
        "users_total": status(ST_UTABSIZE),
        "users": status(ST_NUSERS)
    ]);
}

private void try_send_status_metrics(void) {
    try {
        mapping status_metrics;
        status_metrics = create_status_metrics();
        send_metrics(status_metrics);
    } catch (err) {
        notify_driver("Error reporting status metrics.");
        error(err);
    }
}

private void continue_status_monitor(void) {
    call_out(REPORT_STATUS_FN, STATUS_RATE);
}

static void report_status_metrics(void) {
    try_send_status_metrics();
    continue_status_monitor();
}
