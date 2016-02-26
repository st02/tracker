#ifndef ACQ_H
#define ACQ_H
#include <stdbool.h>
#include <stdint.h>
void my_ready_cb(bool success, uint8_t att_ecode, void *user_data);
void notify_cb(uint16_t value_handle, const uint8_t *value, uint16_t length, void *user_data);
void my_read_multiple_cb(const void *value);
static float dt = 1000;
static float in_curr[3]={0.f,0.f,0.f};
static float in_prev[3]={0.F,0.F,0.F};
static float out_curr[3]={0.F,0.F,0.F};
static float out_prev[3]={0.F,0.F,0.F};
static int time;



#endif
