#include <pebble_worker.h>
#define WORKER_CGM_TIME 42
#define PERSIST_CGM_TIME 52

static const uint8_t MINUTEAGO = 60;
static const uint16_t HOURAGO = 60*(60);
static const uint32_t DAYAGO = 24*(60*60);
static const uint32_t WEEKAGO = 7*(24*60*60);

uint32_t current_cgm_timeago = 0;

static void send_to_foreground() {
  // Send the data to the foreground app
  persist_write_int(WORKER_CGM_TIME, current_cgm_timeago);
}

static void cgm_time_handler(struct tm *tick_timer, TimeUnits units_changed) {
      
  uint32_t time_now = time(NULL);
  uint32_t current_cgm_time = persist_read_int(PERSIST_CGM_TIME);  
        
  current_cgm_timeago = abs(time_now - current_cgm_time);
        	  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "LOAD CGMTIME, CURRENT CGM TIME: %lu", current_cgm_time);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "LOAD CGMTIME, CURRENT CGM TIME AGO: %lu", current_cgm_timeago);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "LOAD CGMTIME, TIME NOW: %lu", time_now);
  
  send_to_foreground();
}

static void init() {
  // Initialize your worker here
  APP_LOG(APP_LOG_LEVEL_DEBUG, "INITIALIZING WORKER");
  tick_timer_service_subscribe(MINUTE_UNIT, cgm_time_handler);
}

static void deinit() {
  // Deinitialize your worker here
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  worker_event_loop();
  deinit();
}