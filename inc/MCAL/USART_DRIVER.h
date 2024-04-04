#ifndef USART_DRIVER_
#define USART_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_Types.h"
#include "USART_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define USART_Peri_1 0
#define USART_Peri_2 1
#define USART_Peri_6 2

#define USART_WORD_LENGTH_8 0x00000000
#define USART_WORD_LENGTH_9 0x00010000
#define USART_PARITY_DISABLE 0x00000000
#define USART_PARITY_ENABLE 0x00000400
#define USART_PARITY_EVEN 0x00000000
#define USART_PARITY_ODD 0x00000200
#define USART_STOP_BITS_1 0x0000000
#define USART_STOP_BITS_1_5 0x00001000
#define USART_STOP_BITS_2 0x00002000
#define USART_OVERSAMPLING_16 0x00000000
#define USART_OVERSAMPLING_8 0x00008000

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*CallBack_t)(void);

typedef struct
{
    u32_t address;
    u32_t BaudRate;
    u32_t WordLength;
    u32_t ParityControl;
    u32_t ParitySelect;
    u32_t StopBits;
    u32_t OverSampling;
} USART_cfg_t;

typedef struct
{
    u8_t USART_Peri;
    u8_t *buffer;
    u16_t length;
    CallBack_t CB;
} USART_Req_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to initialize the USART peripherals
 * parameters:
 * USART_CfgArr -> the configurtion array of the desired USART
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status USART_Init(USART_cfg_t USART_CfgArr);

/*
 * use this function to send a byte synchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be sent
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_TimeOut
 */
Error_Status USART_SendByte(USART_Req_t USART_Req);

/*
 * use this function to recieve a byte synchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be recieved
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_TimeOut
 */
Error_Status USART_GetByte(USART_Req_t USART_Req);

/*
 * use this function to send a buffer of bytes asynchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be sent
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_TXBufferAsyncZC(USART_Req_t USART_Req);

/*
 * use this function to recieve a buffer of bytes asynchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be recieved
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_RXBufferAsyncZC(USART_Req_t USART_Req);

/*
 * use this function to generate a break condition on the USART
 * parameters:
 * USART_Peri -> the address of the USART peripheral to be used
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_GenerateBreak(USART_Req_t USART_Req);

#endif // USART_DRIVER_