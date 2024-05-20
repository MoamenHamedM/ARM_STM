
#include "MCAL/FLASH_DRIVER.h"

#define FLSH_BASE_ADDRESS 0x40023C00

#define FLSH_FCR_KEY_1 0x45670123
#define FLSH_FCR_KEY_2 0xCDEF89AB

#define FLSH_FCR_LOCK_FLAG 0x80000000
#define FLSH_FCR_STRT_FLAG 0x00010000
#define FLSH_FCR_BSY_FLAG 0x00010000

#define FLSH_PRO_SIZE_CLEAR_MASK 0x00000300
#define FLSH_SEC_NUM_CLEAR_MASK 0x00000078

typedef struct
{
    volatile u32_t ACR;
    volatile u32_t KEYR;
    volatile u32_t OPTKEYR;
    volatile u32_t SR;
    volatile u32_t CR;
    volatile u32_t OPTCR;
} FLSH_t;

FLSH_t *const FLSH = (FLSH_t *)FLSH_BASE_ADDRESS;

Error_Status FLASH_CTRL_FCR_Unlock()
{
    Error_Status LOC_Status = Status_NOK;

    FLSH->KEYR = FLSH_FCR_KEY_1;
    FLSH->KEYR = FLSH_FCR_KEY_2;

    if (FLSH->CR & FLSH_FCR_LOCK_FLAG)
    {
        LOC_Status = Status_FLSH_UNLOCK_FAIL;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

Error_Status FLASH_CTRL_FCR_Lock()
{
    Error_Status LOC_Status = Status_NOK;

    FLSH->CR |= FLSH_FCR_LOCK_FLAG;

    if (!(FLSH->CR & FLSH_FCR_LOCK_FLAG))
    {
        LOC_Status = Status_FLSH_LOCK_FAIL;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

Error_Status FLASH_CTRL_FCR_Start()
{
    Error_Status LOC_Status = Status_NOK;

    if (FLSH->SR & FLSH_FCR_BSY_FLAG)
    {
        LOC_Status = Status_FLSH_BSY;
    }
    else
    {
        LOC_Status = Status_OK;
        FLSH->CR |= FLSH_FCR_STRT_FLAG;

        while (FLSH->SR & FLSH_FCR_BSY_FLAG) //???????
            ;
    }

    return LOC_Status;
}

Error_Status FLASH_CFG_FCR_Operation_Select(u32_t FLSH_Op)
{
    Error_Status LOC_Status = Status_NOK;

    if (FLSH_Op > FLSH_OP_MER)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (FLSH->CR & FLSH_FCR_LOCK_FLAG)
    {
        LOC_Status = Status_FLSH_CR_LOCK;
    }
    else
    {
        LOC_Status = Status_OK;
        FLSH->CR |= FLSH_Op;
    }
    return LOC_Status;
}

Error_Status FLASH_CFG_FCR_Program_Size(u32_t FLSH_Pro)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_CRValue = FLSH->CR;

    if (FLSH_Pro > FLSH_PRO_64)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (FLSH->CR & FLSH_FCR_LOCK_FLAG)
    {
        LOC_Status = Status_FLSH_CR_LOCK;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_CRValue &= ~FLSH_PRO_SIZE_CLEAR_MASK;
        LOC_CRValue |= FLSH_Pro;
        FLSH->CR = LOC_CRValue;
    }
    return LOC_Status;
}

Error_Status FLASH_SEL_FCR_Sector_Number(u32_t FLSH_Sec)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_CRValue = FLSH->CR;

    if (FLSH_Sec > FLSH_SEC_5)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (FLSH->CR & FLSH_FCR_LOCK_FLAG)
    {
        LOC_Status = Status_FLSH_CR_LOCK;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_CRValue &= ~FLSH_SEC_NUM_CLEAR_MASK;
        LOC_CRValue |= FLSH_Sec;
        FLSH->CR = LOC_CRValue;
    }
    return LOC_Status;
}