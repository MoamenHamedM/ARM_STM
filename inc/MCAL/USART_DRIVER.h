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
#define USART_Peri_1 (void *)0x40011000
#define USART_Peri_2 (void *)0x40004401
#define USART_Peri_6 (void *)0x40011402

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
typedef void (*TXCB)(void);
typedef void (*RXCB)(void);

typedef struct
{
    void *address;
    f32_t BaudRate;
    u32_t WordLength;
    u32_t ParityControl;
    u32_t ParitySelect;
    u32_t StopBits;
    u32_t OverSampling;
} USART_cfg_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to initialize the USART peripherals
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status USART_Init();

/*
 * use this function to send a byte synchronously through USART
 * parameters:
 * USART_Peri -> the address of the USART peripheral to be used
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * byte -> the byte to be sent
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_TimeOut
 */
Error_Status USART_SendByte(void *USART_Peri, u8_t byte);

/*
 * use this function to recieve a byte synchronously through USART
 * parameters:
 * USART_Peri -> the address of the USART peripheral to be used
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * byte -> the address of the byte to be recieved
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_TimeOut
 */
Error_Status USART_GetByte(void *USART_Peri, u8_t *byte);

/*
 * use this function to send a buffer of bytes asynchronously through USART
 * parameters:
 * USART_Peri -> the address of the USART peripheral to be used
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * buffer -> the address of the to be sent
 * length -> the length of the buffer
 * CB -> address of the function to be called after finishing - use NULL if no function is needed -
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_TXBufferAsyncZC(void *USART_Peri, u8_t *buffer, u16_t length, TXCB CB);

/*
 * use this function to recieve a buffer of bytes asynchronously through USART
 * parameters:
 * USART_Peri -> the address of the USART peripheral to be used
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * buffer -> the address of the to be recieved
 * length -> the length of the buffer
 * CB -> address of the function to be called after finishing - use NULL if no function is needed -
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_RXBufferAsyncZC(void *USART_Peri, u8_t *buffer, u16_t length, RXCB CB);

#endif // USART_DRIVER_