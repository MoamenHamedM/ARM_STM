/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/LCD_DRIVER.h"
#include "MCAL/GPIO_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LCD_USER_STATE_BUSY 0
#define LCD_USER_STATE_READY 1
#define LCD_REQ_TYPE_WRITE 0
#define LCD_REQ_TYPE_CLEAR 1
#define LCD_REQ_TYPE_SET_P 2
#define LCD_MAX_BUFFER_SIZE 5
#define STATIC_STATE_BUSY 0
#define STATIC_STATE_READY 1
#define COMMAND_NIBBLE_HIGH 1
#define COMMAND_NIBBLE_LOW 0
#define WRITE_COMMAND_STATE 0
#define WRITE_DATA_STATE 1

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    const char *U_string;
    u8_t U_length;
    u8_t U_State;
    u8_t U_Type;
} User_Req_t;

typedef struct
{
    u8_t Curr_Pos;
    CallBack_t CallBack;
} Write_Req_t;

typedef struct
{
    CallBack_t CallBack;
} Clear_Req_t;

typedef struct
{
    u8_t X_Position;
    u8_t Y_Position;
    CallBack_t CallBack;
} SetP_Req_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern LCD_cfg_t LCDs_PinCfg;
static u8_t LCD_State = LCD_STATE_OFF;
static User_Req_t User_Resquest[LCD_MAX_BUFFER_SIZE];
static u8_t User_CurrentRequest = 0;
static Write_Req_t Write_Request;
static Clear_Req_t Clear_Request;
static SetP_Req_t SetP_Request;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void LCD_WriteToPins(u8_t Info, u8_t State);
static void Init_State_Func();
static void OperationState_WriteFunc();
static void OperationState_ClearFunc();
static void OperationState_SetPFunc();

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status LCD_InitAsync()
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t LcdPins;
    u8_t LCD_Index, LCD_Pin_Index;

    for (LCD_Pin_Index = 0; LCD_Pin_Index < NUMBER_OF_DATA_LINES; LCD_Pin_Index++)
    {
        LcdPins.Pin = LCDs_PinCfg.LCD_data_pins[LCD_Pin_Index].Pin;
        LcdPins.Port = LCDs_PinCfg.LCD_data_pins[LCD_Pin_Index].Port;
        LcdPins.Mode = GPIO_MODE_OP_PP;
        LcdPins.Speed = GPIO_SPEED_MED;
        LOC_Status = GPIO_Init(&LcdPins);
    }
    LcdPins.Pin = LCDs_PinCfg.E_pin.Pin;
    LcdPins.Port = LCDs_PinCfg.E_pin.Port;
    LcdPins.Mode = GPIO_MODE_OP_PP;
    LcdPins.Speed = GPIO_SPEED_MED;
    LOC_Status = GPIO_Init(&LcdPins);

    LcdPins.Pin = LCDs_PinCfg.R_S_pin.Pin;
    LcdPins.Port = LCDs_PinCfg.R_S_pin.Port;
    LcdPins.Mode = GPIO_MODE_OP_PP;
    LcdPins.Speed = GPIO_SPEED_MED;
    LOC_Status = GPIO_Init(&LcdPins);

    LcdPins.Pin = LCDs_PinCfg.R_W_pin.Pin;
    LcdPins.Port = LCDs_PinCfg.R_W_pin.Port;
    LcdPins.Mode = GPIO_MODE_OP_PP;
    LcdPins.Speed = GPIO_SPEED_MED;
    LOC_Status = GPIO_Init(&LcdPins);

    LCD_State = LCD_STATE_INIT;
    return LOC_Status;
}

Error_Status LCD_WriteStringAsync(u8_t *string, u8_t length, CallBack_t CB)
{
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            User_Resquest[Index].U_string = string;
            User_Resquest[Index].U_length = length;
            Write_Request.CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_WRITE;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = Status_LCD_Full_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

Error_Status LCD_SetCursorPositionAsync(u8_t X_pos, u8_t Y_pos, CallBack_t CB)
{
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            SetP_Request.X_Position = X_pos;
            SetP_Request.Y_Position = Y_pos;
            SetP_Request.CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_SET_P;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = Status_LCD_Full_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

Error_Status LCD_ClearScreenAsync(CallBack_t CB)
{
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            Clear_Request.CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_CLEAR;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = Status_LCD_Full_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

static void Init_State_Func()
{
    static u8_t Init_Time_Stamp = 0;

#if NUMBER_OF_DATA_LINES == DATA_8_PINS
    u8_t LOC_FunctionSetCommand = 0x30;  /*variable to represent the function set command*/
    u8_t LOC_DisplayOnOffCommand = 0x08; /*variable to represent the display on/off command*/
    u8_t LOC_DisplayClearCommand = 0x01; /*variable to represent the display clear command*/
    u8_t LOC_EntryModeCommand = 0x04;    /*variable to represent the entry mode set command*/

    /*set the font and number of lines configuration to the command*/
    LOC_FunctionSetCommand |= (FONT_SIZE << 2);
    LOC_FunctionSetCommand |= (NUMBER_OF_LINES << 3);

    /*set the blink, cursor and display state configuration to the command*/
    LOC_DisplayOnOffCommand |= (BLINK_STATE);
    LOC_DisplayOnOffCommand |= (CUSOR_STATE << 1);
    LOC_DisplayOnOffCommand |= (DISPLAY_STATE << 2);

    /*set the font and number of lines configuration to the command*/
    LOC_EntryModeCommand |= (SHIFT_STATE);
    LOC_EntryModeCommand |= (INCREMENT_STATE << 1);

    if (Init_Time_Stamp < 2)
    {
        /*call the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Time_Stamp++;
    }

    else if (Init_Time_Stamp < 4)
    {
        /*call the display on/off command*/
        LCD_WriteToPins(LOC_DisplayOnOffCommand, WRITE_COMMAND_STATE);
        Init_Time_Stamp++;
    }

    else if (Init_Time_Stamp < 6)
    {
        /*call the display clear command*/
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Init_Time_Stamp++;
    }

    else if (Init_Time_Stamp < 8)
    {
        /*call the entry mode set command*/
        LCD_WriteToPins(LOC_EntryModeCommand, WRITE_COMMAND_STATE);
        Init_Time_Stamp++;
    }

    else
    {
        LCD_State = LCD_STATE_OPER;
        Init_Time_Stamp = 0;
    }
#endif

#if NUMBER_OF_DATA_LINES == DATA_4_PINS

    u8_t LOC_FunctionSetCommand = 0x02;  /*variable to represent the function set command*/
    u8_t LOC_DisplayOnOffCommand = 0x00; /*variable to represent the display on/off command*/
    u8_t LOC_DisplayClearCommand = 0x01; /*variable to represent the display clear command*/
    u8_t LOC_EntryModeCommand = 0x00;    /*variable to represent the entry mode set command*/

    /********************************** call the function set command **********************************/
    if (Init_Time_Stamp < 4)
    {
        /*first call of the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Time_Stamp++;
    }

    if (Init_Time_Stamp < 8)
    {
        /*set the font and number of lines configuration to the command*/
        LOC_FunctionSetCommand = 0x20;
        LOC_FunctionSetCommand |= (FONT_SIZE << 2);
        LOC_FunctionSetCommand |= (NUMBER_OF_LINES << 3);

        /*second call of the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
    }
    /********************************** call the display on/off command **********************************/
    if (Init_Time_Stamp < 12)
    {
        /*set the blink, cursor and display state configuration to the command*/
        LOC_DisplayOnOffCommand |= (BLINK_STATE);
        LOC_DisplayOnOffCommand |= (CUSOR_STATE << 1);
        LOC_DisplayOnOffCommand |= (DISPLAY_STATE << 2);

        /*setting the DB7 bit to high*/
        LOC_DisplayOnOffCommand |= (1 << 3);

        /*call of the display on/off command*/
        LCD_WriteToPins(LOC_DisplayOnOffCommand, WRITE_COMMAND_STATE);
    }
    /********************************** call the display clear command **********************************/
    if (Init_Time_Stamp < 16)
    {
        /*call of the display clear command*/
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
    }
    /********************************** call the entry mode set command **********************************/
    if (Init_Time_Stamp < 20)
    {
        /*set the font and number of lines configuration to the command*/
        LOC_EntryModeCommand |= (SHIFT_STATE);
        LOC_EntryModeCommand |= (INCREMENT_STATE << 1);

        /*call of the entry mode set command*/
        LCD_WriteToPins(LOC_EntryModeCommand, WRITE_COMMAND_STATE);
    }
    else
    {
        LCD_State = LCD_STATE_OPER;
        Init_Time_Stamp = 0;
    }
#endif
}

static void LCD_WriteToPins(u8_t Info, u8_t State)
{
#if NUMBER_OF_DATA_LINES == DATA_8_PINS
    static u8_t Command_State = STATIC_STATE_READY;
    u8_t Index;

    switch (Command_State)
    {
    case STATIC_STATE_READY:
        GPIO_Set_PinValue(LCDs_PinCfg.R_W_pin.Port, LCDs_PinCfg.R_W_pin.Pin, GPIO_STATE_RESET);
        GPIO_Set_PinValue(LCDs_PinCfg.R_S_pin.Port, LCDs_PinCfg.R_S_pin.Pin, State);
        for (Index = 0; Index < 8; Index++)
        {
            GPIO_Set_PinValue(LCDs_PinCfg.LCD_data_pins[Index].Port,
                              LCDs_PinCfg.LCD_data_pins[Index].Pin,
                              ((Info >> Index) & 1));
        }
        GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_SET);
        Command_State = STATIC_STATE_BUSY;
        break;

    case STATIC_STATE_BUSY:
        GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_RESET);
        Command_State = STATIC_STATE_READY;
        break;

    default:
        break;
    }
#endif

#if NUMBER_OF_DATA_LINES == DATA_4_PINS
    static u8_t Command_State = STATIC_STATE_READY;
    static u8_t Command_Nibble = COMMAND_NIBBLE_HIGH;
    u8_t Index;

    switch (Command_Nibble)
    {
    case COMMAND_NIBBLE_HIGH:
        switch (Command_State)
        {
        case STATIC_STATE_READY:
            GPIO_Set_PinValue(LCDs_PinCfg.R_W_pin.Port, LCDs_PinCfg.R_W_pin.Pin, GPIO_STATE_RESET);
            GPIO_Set_PinValue(LCDs_PinCfg.R_S_pin.Port, LCDs_PinCfg.R_S_pin.Pin, State);
            for (Index = 0; Index < 4; Index++)
            {
                GPIO_Set_PinValue(LCDs_PinCfg.LCD_data_pins[Index].Port,
                                  LCDs_PinCfg.LCD_data_pins[Index].Pin,
                                  ((Info >> (Index + 4)) & 1));
            }
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_SET);
            Command_State = STATIC_STATE_BUSY;
            break;

        case STATIC_STATE_BUSY:
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_RESET);
            Command_State = STATIC_STATE_READY;
            Command_Nibble = COMMAND_NIBBLE_LOW;
            break;

        default:
            break;
        }
        break;

    case COMMAND_NIBBLE_LOW:
        switch (Command_State)
        {
        case STATIC_STATE_READY:
            GPIO_Set_PinValue(LCDs_PinCfg.R_W_pin.Port, LCDs_PinCfg.R_W_pin.Pin, GPIO_STATE_RESET);
            GPIO_Set_PinValue(LCDs_PinCfg.R_S_pin.Port, LCDs_PinCfg.R_S_pin.Pin, State);
            for (Index = 0; Index < 4; Index++)
            {
                GPIO_Set_PinValue(LCDs_PinCfg.LCD_data_pins[Index].Port,
                                  LCDs_PinCfg.LCD_data_pins[Index].Pin,
                                  ((Info >> Index) & 1));
            }
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_SET);
            Command_State = STATIC_STATE_BUSY;
            break;

        case STATIC_STATE_BUSY:
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_RESET);
            Command_State = STATIC_STATE_READY;
            Command_Nibble = COMMAND_NIBBLE_LOW;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

#endif
}

static void OperationState_WriteFunc()
{
    static u8_t write_state = STATIC_STATE_READY;

    if (Write_Request.Curr_Pos < User_Resquest[User_CurrentRequest].U_length)
    {
        switch (write_state)
        {
        case STATIC_STATE_READY:
            LCD_WriteToPins(User_Resquest[User_CurrentRequest].U_string + Write_Request.Curr_Pos, WRITE_DATA_STATE);
            write_state = STATIC_STATE_BUSY;
            break;
        case STATIC_STATE_BUSY:
            LCD_WriteToPins(User_Resquest[User_CurrentRequest].U_string + Write_Request.Curr_Pos, WRITE_DATA_STATE);
            write_state = STATIC_STATE_READY;
            Write_Request.Curr_Pos++;
            break;

        default:
            break;
        }
    }
    else
    {
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;
        Write_Request.Curr_Pos = 0;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        if (Write_Request.CallBack)
        {
            Write_Request.CallBack();
        }
    }
}

static void OperationState_ClearFunc()
{
    static u8_t Clear_state = STATIC_STATE_READY;
    u8_t LOC_DisplayClearCommand = 0x01; /*variable to represent the display clear command*/

    switch (Clear_state)
    {
    case STATIC_STATE_READY:
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Clear_state = STATIC_STATE_BUSY;
        break;
    case STATIC_STATE_BUSY:
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Clear_state = STATIC_STATE_READY;
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        if (Clear_Request.CallBack)
        {
            Clear_Request.CallBack();
        }
        break;

    default:
        break;
    }
}

static void OperationState_SetPFunc()
{
    static u8_t SetPState = STATIC_STATE_READY;
    u8_t LOC_Location = 128; /*variable to represent the value to execute GotoDDRAM_XY command*/

    /*check if the location is in the first line*/
    if (SetP_Request.X_Position == 0)
    {
        LOC_Location += SetP_Request.Y_Position;
    }
    else
    {
        LOC_Location += (SetP_Request.Y_Position + 0x40);
    }

    switch (SetPState)
    {
    case STATIC_STATE_READY:
        LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
        SetPState = STATIC_STATE_BUSY;
        break;
    case STATIC_STATE_BUSY:
        LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
        SetP_Request.X_Position = 0;
        SetP_Request.Y_Position = 0;
        SetPState = STATIC_STATE_READY;
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        if (SetP_Request.CallBack)
        {
            SetP_Request.CallBack();
        }
        break;

    default:
        break;
    }
}

void LCD_Runnable(void)
{
    if (LCD_State == LCD_STATE_INIT)
    {
        Init_State_Func();
    }
    else if (LCD_State == LCD_STATE_OPER)
    {
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_BUSY)
        {
            switch (User_Resquest[User_CurrentRequest].U_Type)
            {
            case LCD_REQ_TYPE_WRITE:
                OperationState_WriteFunc();
                break;
            case LCD_REQ_TYPE_CLEAR:
                OperationState_ClearFunc();
                break;
            case LCD_REQ_TYPE_SET_P:
                OperationState_SetPFunc();
                break;

            default:
                break;
            }
        }
    }
}