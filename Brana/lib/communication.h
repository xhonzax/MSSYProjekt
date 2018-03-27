/*
 * communication.h
 *
 * Created: 21.3.2018 15:48:37
 *  Author: Student
 */ 


#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "var.h"

void com_send(uint16_t adresa, uint8_t endpoint, uint8_t *data);
void com_reply(NWK_DataInd_t *ind, uint8_t *data);


#endif /* COMMUNICATION_H_ */