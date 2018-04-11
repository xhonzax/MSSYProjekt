/*
 * Brana.c
 *
 * Created: 12.3.2018 15:52:23
 * Author : Benedikt
 */ 

#include <avr/io.h>
/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "sysTimer.h"
#include "halBoard.h"
#include "halUart.h"

/*- our-----Includes -------------------------------------------------------*/
#include "lib/var.h"
#include "lib/UART.h"
#include "lib/dhcp.h"
#include "lib/communication.h"
#include "lib/endpoint_service.h"


/*- Definitions ------------------------------------------------------------*/
#ifdef NWK_ENABLE_SECURITY
#define APP_BUFFER_SIZE     (NWK_MAX_PAYLOAD_SIZE - NWK_SECURITY_MIC_SIZE)
#else
#define APP_BUFFER_SIZE     NWK_MAX_PAYLOAD_SIZE
#endif

/*- Types ------------------------------------------------------------------*/
typedef enum AppState_t
{
	APP_STATE_INITIAL,
	APP_STATE_IDLE,
} AppState_t;

AppState_t appState = APP_STATE_INITIAL;

static SYS_Timer_t appTimer;

//obsluha prichozich ramcu
static bool funkceObsluhy (NWK_DataInd_t *ind)
{
		int cislo = ind->srcAddr;
		char buffer[3] = "000";
		sprintf (buffer,"%d",cislo);
	
		UART_SendString("from :0x");
		UART_SendString(buffer);
		
		dhcp_sendAddress(ind);
		
		UART_SendString(HELLO_ACK_t_size);
		
		UART_SendString("| data :");
		for (int p = 0;p< ind->size;p++){
			
			UART_SendChar(ind->data[p]);
		}
		
		UART_SendString("\r\n");
		return true;
}





static void appTimerHandler(SYS_Timer_t *timer)
{

  // com_debug_send_hello(0,1);
   //com_send(0,1,(uint8_t) 2);
	SYS_TimerStop(&appTimer);
    SYS_TimerStart(&appTimer);

	(void)timer;
}

void appInit(){
	 NWK_SetAddr(APP_ADDR);
	 NWK_SetPanId(APP_PANID);
	 PHY_SetChannel(APP_CHANNEL);
	 PHY_SetRxState(true);
	
	 NWK_OpenEndpoint(APP_ENDPOINT, funkceObsluhy);
	
	  HAL_BoardInit();

	  appTimer.interval = APP_FLUSH_TIMER_INTERVAL;   //interval jak casto ma timer posilat zpravy
	  appTimer.mode = SYS_TIMER_INTERVAL_MODE;
	  appTimer.handler = appTimerHandler;
}

static void APP_TaskHandler(void)
{
	switch (appState)
	{
		case APP_STATE_INITIAL:
		{
			appInit();
			SYS_TimerStart(&appTimer);
			appState = APP_STATE_IDLE;
		} break;
		case APP_STATE_IDLE:
		break;
		default:
		break;
	}
}

int main(void)
{	
	SYS_Init();
	UART_init(9600);
	//QUEUE_init(*FRONTA);

	//QUEUE_init(*FRONTA);


	appInit();
	while (1)
	{ 
		//QUEUE_send_last();
		SYS_TaskHandler();
		APP_TaskHandler();
	}
}