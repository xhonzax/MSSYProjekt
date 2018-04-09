/*
 * communication.c
 *
 * Created: 21.3.2018 15:48:13
 *  Author: Student
 */ 

#include "communication.h"

static void appDataConf(NWK_DataReq_t *req)
{
	if (NWK_SUCCESS_STATUS == req->status)
	{
		//QUEUE_rm_last();
	}
	else
	{int a;}
	

}

void com_send(uint16_t adresa, uint8_t endpoint, uint8_t *data){
	volatile int delka;
	
	//for(delka = 0; data[delka] != '\0'; ++delka);
	delka=strlen(data);
	delka =8;
	
	appDataReq.dstAddr = adresa;
	appDataReq.dstEndpoint = endpoint;
	appDataReq.srcEndpoint = endpoint;
	appDataReq.data = data;
	appDataReq.size = delka;
	NWK_DataReq(&appDataReq);
}

void com_debug_send_hello(uint16_t adresa, uint8_t endpoint){
	volatile int delka;
	
	volatile uint8_t data[5];
	
	volatile uint8_t device_id = 128; //HELLO
	volatile uint8_t device_typ = B(00010010); //typ
	volatile uint8_t device_sleep = 0;
	volatile uint8_t device_read = 1;
	volatile uint8_t device_write = 0;
	
	data[0] = device_id;
	data[1] = device_typ;
	//data[2] = device_pocet;
	data[2] = device_sleep;
	data[3] = device_read;
	data[4] = device_write;
	
	
	
	delka=sizeof(data)*8;
	appDataReq.dstAddr = adresa;
	appDataReq.dstEndpoint = endpoint;
	appDataReq.srcEndpoint = endpoint;
	appDataReq.data = data;
	appDataReq.size = delka;
	NWK_DataReq(&appDataReq);
}

void com_reply(NWK_DataInd_t *ind, uint8_t *data){
	volatile int delka;
	
	//for(delka = 0; data[delka] != '\0'; ++delka);
	delka=strlen(data);
	
	appDataReq.dstAddr = ind->srcAddr;
	appDataReq.dstEndpoint = ind->srcEndpoint;
	appDataReq.srcEndpoint = ind->dstEndpoint;
	appDataReq.data = data;
	appDataReq.size = delka;
	appDataReq.confirm= appDataConf;
	//QUEUE_add(&appDataReq);
	NWK_DataReq(&appDataReq);
	
}