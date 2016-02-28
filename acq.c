#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acq.h"
#include "shared/btgatt-client.h"

/*
 * docs in http://processors.wiki.ti.com/index.php/SensorTag_User_Guide
 * for my sensor after a firmware update:
 * http://processors.wiki.ti.com/images/a/a8/BLE_SensorTag_GATT_Server.pdf
 */
struct client *cli ;

void my_ready_cb(bool success, uint8_t att_ecode, void *user_data)
{
	cli = user_data;
	//configure accelerometer
	// enable notification
	char cmd[512]="0x31 01 00";
	cmd_write_value(cli,cmd);

	int mult=10;
	dt=mult*10; // ms
	// define period
	sprintf(cmd,"0x37 %d",mult);
	cmd_write_value(cli,cmd);
	sprintf(cmd,"0x67 %d",mult);
	cmd_write_value(cli,cmd);

	// start sensor measure
	strcpy(cmd,"0x34 01"); 
	cmd_write_value(cli,cmd);

	// configure gyro
	memset(cmd,0,512);

	// measure all diriections: 
	strcpy(cmd,"0x64 07");
	cmd_write_value(cli,cmd);
	// cmd_read_value (cli,"0x5b");

	strcpy(cmd,"0x30 0x60");
	cmd_read_multiple(cli,cmd);

	// notify accelerometer
	memset(cmd,0,512);
	strcpy(cmd,"0x30"); 
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
	time++;
}

void call_me()
{
	char cmd[512];
	strcpy(cmd,"0x30 0x60");
	cmd_read_multiple(cli,cmd);
}

void notify_cb(uint16_t value_handle, const uint8_t *value, uint16_t length, void *user_data)
{
	call_me();
}

#pragma pack(1)
struct sensor_data{
	int8_t acc_raw[3];
	int16_t gyro_raw[3];
}sensor_data;

void my_read_multiple_cb(const void*value)
{

	const struct sensor_data *data=value;
	float acc[3];
	acc[0]=(data->acc_raw[0] * 1.) / 64;
	acc[1]=(data->acc_raw[1] * 1.) / 64;
	acc[2]=(data->acc_raw[2] * 1.) / 64;
	float gyro[3];
	gyro[0]=(data->gyro_raw[0] * 1.0) / (65536/ 500); 
	gyro[1]=(data->gyro_raw[1] * 1.0) / (65536/ 500); 
	gyro[2]=(data->gyro_raw[2] * 1.0) / (65536/ 500); 
	fprintf(fp,"%d %f %f %f %f %f %f \n",time++,acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);
	printf("%d %f %f %f %f %f %f \n",time++,acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2]);
}

int main(int argc,char *argv[])
{

	fp=fopen("out.dat","w");
	if (argc==1)
	{
		char *opt[]={"./tracker","-d","5C:31:3E:BF:FA:77"}; 
		main_gatt(3,opt);
	} else {
		main_gatt(argc,argv);
	}
}

