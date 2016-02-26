#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acq.h"
#include "btgatt-client.h"

/*
 * docs in http://processors.wiki.ti.com/index.php/SensorTag_User_Guide
 * for my gatt conf sensor:
 * http://processors.wiki.ti.com/images/archive/a/a8/20140521115217!BLE_SensorTag_GATT_Server.pdf
 */
struct client *cli ;

void my_ready_cb(bool success, uint8_t att_ecode, void *user_data)
{
    cli = user_data;

    //configure accelerometer
    // enable notification
    char cmd[512]="0x2e 01 00";
    cmd_write_value(cli,cmd);
    cmd_read_value (cli,"0x2e");

    int mult=10;
    dt=mult*10; // ms
    // define period
    sprintf(cmd,"0x34 %d",mult);
    cmd_write_value(cli,cmd);
    // cmd_read_value (cli,"0x34");

    // start sensor measure
    strcpy(cmd,"0x31 01"); 
    cmd_write_value(cli,cmd);
    // cmd_read_value (cli,"0x31");

    // configure gyro
    memset(cmd,0,512);
    // notification not needed 
    strcpy(cmd,"0x58 01 00");
    cmd_write_value(cli,cmd);
    // cmd_read_value (cli,"0x58");

    // measure all dir: 
    strcpy(cmd,"0x5b 07");
    cmd_write_value(cli,cmd);
    cmd_read_value (cli,"0x5b");

    strcpy(cmd,"0x2d 0x57");
    cmd_read_multiple(cli,cmd);

    //notify accelerometer
    memset(cmd,0,512);
    strcpy(cmd,"0x2d"); 
    cmd_register_notify(cli,cmd);

}

static void inline by_pass(const float alpha)
{
    for(int i=0;i<3;i++)	
        out_curr[i]=alpha*in_curr[i];
}

static void inline low_pass(const float alpha)
{
    for(int i=0;i<3;i++)	
        out_curr[i]=alpha*in_curr[i]+(1-alpha)*out_prev[i];
}

static void inline high_pass(const float alpha)
{
    for(int i=0;i<3;i++)	
        out_curr[i]=alpha*out_prev[i]+alpha*(in_curr[i]-in_prev[i]);
}
static void compute_distance(const float *vals,void * user_data)
{

    float alpha = 0.8;
    float linear_acc[3];
    int tc=10;
    for(int i=0;i<3;i++)	
        in_curr[i]=vals[i];
    low_pass(0.8);
    for(int i=0;i<3;i++)	
        out_prev[i]=out_curr[i];

    float gt=0.5*dt*dt;
    float dx=out_curr[0];
    float dy=out_curr[1];
    float dz=out_curr[2];
    fprintf(fp,"%d %f %f %f \n",time ,dx,dy,dz);
    printf("\tvals = %d %f %f %f \n",time,dx,dy,dz);
    // read gyro
    // struct client *cli = user_data;
    // cmd_read_value (cli,"0x57");
    time++;
}

void call_me(void *user_data)
{
    char cmd[512];
    strcpy(cmd,"0x2d 0x57");
    cmd_read_multiple(cli,cmd);
}

void notify_cb(uint16_t value_handle, const uint8_t *value, uint16_t length, void *user_data)
{
    int i;
//    if (length == 0) {
//        PRLOG("(0 bytes)\n");
//        return;
//    }
    call_me(user_data);
    return;
    if(value_handle==0x2d)
    {
//    float vals[3];
//    vals[0]=((int8_t)value[0] * 1.) / 64;
//    vals[1]=((int8_t)value[1] * 1.) / 64;
//    vals[2]=((int8_t)value[2] * 1.) / 64;
//    compute_distance(vals,user_data);
//    return;
    }
}
typedef struct sensor_data{
    int8_t acc_raw[3];
    int16_t gyro_raw[3];
}sensor_data;

void my_read_multiple_cb(const void*value)
{
    
    const sensor_data *data=value;
     
    float acc[3];
    acc[0]=(data->acc_raw[0] * 1.) / 64;
    acc[1]=(data->acc_raw[1] * 1.) / 64;
    acc[2]=(data->acc_raw[2] * 1.) / 64;
    float gyro[3];
    gyro[0]=(data->gyro_raw[3] * 1.0) / (65536/ 500); 
    gyro[1]=(data->gyro_raw[4] * 1.0) / (65536/ 500); 
    gyro[2]=(data->gyro_raw[5] * 1.0) / (65536/ 500); 
    printf("%f %f %f--- %f %f %f \n",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);
}


