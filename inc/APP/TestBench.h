#include "MCAL/USART_DRIVER.h"
#include "MCAL/GPIO_DRIVER.h"

#define STATE_NO_REQ 0
#define STATE_PENDING_REQ 1
#define STATE_HANDLE_REQ 2

#define SERVICE_INDEX 0
#define METHOD_INDEX 1
#define VALUE_INDEX 2
#define GET_PIN_MASK 0x0F
#define GET_PORT_MASK 0x30
#define GET_PARAM_MASK 0x40
#define PORT_OFFSET 4
#define VAL_OFFSET 6
#define INIT_ACK 0x55

#define SERVICE_ID_INIT_IO 0x10
#define SERVICE_ID_COMM_IO 0x20
#define METHOD_ID_INIT_IO 0x01
#define METHOD_ID_SET_IO 0x02
#define METHOD_ID_GET_IO 0x03

#define RESPONSE_SOURCE_INIT 0
#define RESPONSE_SOURCE_NO_INIT 1
#define SERVICE_STATE_HANDLER 0
#define SERVICE_STATE_WAIT 1
#define SERVICE_STATE_RESPONSE 2