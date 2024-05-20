#include "APP/TestBench.h"

void TestCB(void);
void CommService_Handler(void);
void InitService_Handler(void);
void ResponseHandler(u8_t source);
void test();

u8_t TestBench_ReceiveBuffer[3];
u8_t TestBench_ResponseBuffer[3];
void *Ports[3] = {(void *)0x40020000, (void *)0x40020400, (void *)0x40020800};
static u8_t Request_State = STATE_NO_REQ;

USART_Req_t USARAT_ReceiveReq = {.length = 3, .buffer = TestBench_ReceiveBuffer, .USART_Peri = USART_Peri_1, .CB = TestCB};
USART_Req_t USARAT_RsponseReq = {.length = 3, .buffer = TestBench_ResponseBuffer, .USART_Peri = USART_Peri_1, .CB = NULL};

void TestRun()
{
    switch (Request_State)
    {
    case STATE_NO_REQ:
        USART_RXBufferAsyncZC(USARAT_ReceiveReq);
        Request_State = STATE_PENDING_REQ;
        break;
    case STATE_HANDLE_REQ:
        switch (TestBench_ReceiveBuffer[SERVICE_INDEX])
        {
        case SERVICE_ID_INIT_IO:
            InitService_Handler();
            break;

        case SERVICE_ID_COMM_IO:
            CommService_Handler();
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void TestCB(void)
{
    Request_State = STATE_HANDLE_REQ;
}

void InitService_Handler(void)
{
    static u8_t Service_state = SERVICE_STATE_HANDLER;
    u8_t Service_Pin = TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PIN_MASK;
    u8_t Service_Port = (TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PORT_MASK) >> PORT_OFFSET;
    u8_t Service_Value = (TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PARAM_MASK) >> VAL_OFFSET;
    GPIO_Pin_t GPIO_Pin = {.Port = Ports[Service_Port], .Pin = Service_Pin, .Mode = Service_Value, .Speed = GPIO_SPEED_MED};

    switch (Service_state)
    {
    case SERVICE_STATE_HANDLER:
        switch (TestBench_ReceiveBuffer[METHOD_INDEX])
        {
        case METHOD_ID_INIT_IO:
            GPIO_Init(&GPIO_Pin, Service_Value);
            break;

        default:
            break;
        }
        Service_state = SERVICE_STATE_WAIT;
        break;

    case SERVICE_STATE_WAIT:
        Service_state = SERVICE_STATE_RESPONSE;
        break;

    case SERVICE_STATE_RESPONSE:
        ResponseHandler(RESPONSE_SOURCE_INIT);
        Request_State = STATE_NO_REQ;
        Service_state = SERVICE_STATE_HANDLER;
        break;

    default:
        break;
    }
}

void CommService_Handler(void)
{
    static u8_t Service_state = SERVICE_STATE_HANDLER;
    u8_t Service_Pin = TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PIN_MASK;
    u8_t Service_Port = (TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PORT_MASK) >> PORT_OFFSET;
    u8_t Service_Value = (TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PARAM_MASK) >> VAL_OFFSET;

    switch (Service_state)
    {
    case SERVICE_STATE_HANDLER:
        switch (TestBench_ReceiveBuffer[METHOD_INDEX])
        {
        case METHOD_ID_SET_IO:
            GPIO_Set_PinValue(Ports[Service_Port], Service_Pin, Service_Value);
            break;
        case METHOD_ID_GET_IO:
            break;

        default:
            break;
        }
        Service_state = SERVICE_STATE_WAIT;
        break;

    case SERVICE_STATE_WAIT:
        Service_state = SERVICE_STATE_RESPONSE;
        break;

    case SERVICE_STATE_RESPONSE:
        ResponseHandler(RESPONSE_SOURCE_NO_INIT);
        Request_State = STATE_NO_REQ;
        Service_state = SERVICE_STATE_HANDLER;
        break;

    default:
        break;
    }
}

void ResponseHandler(u8_t source)
{
    u8_t CurrentValue = 0;
    u8_t Service_Pin = TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PIN_MASK;
    u8_t Service_Port = (TestBench_ReceiveBuffer[VALUE_INDEX] & GET_PORT_MASK) >> PORT_OFFSET;

    switch (source)
    {
    case RESPONSE_SOURCE_INIT:
        TestBench_ResponseBuffer[VALUE_INDEX] = INIT_ACK;
        break;

    case RESPONSE_SOURCE_NO_INIT:
        GPIO_Get_PinValue(Ports[Service_Port], Service_Pin, &CurrentValue);
        TestBench_ResponseBuffer[VALUE_INDEX] = TestBench_ReceiveBuffer[VALUE_INDEX] & (~GET_PARAM_MASK);
        TestBench_ResponseBuffer[VALUE_INDEX] |= (CurrentValue << VAL_OFFSET);
        break;

    default:
        break;
    }
    TestBench_ResponseBuffer[SERVICE_INDEX] = TestBench_ReceiveBuffer[SERVICE_INDEX];
    TestBench_ResponseBuffer[METHOD_INDEX] = TestBench_ReceiveBuffer[METHOD_INDEX];

    USART_TXBufferAsyncZC(USARAT_RsponseReq);
}

