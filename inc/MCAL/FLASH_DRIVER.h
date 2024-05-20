
#ifndef FLASH_DRIVER_
#define FLASH_DRIVER_

/**
 * file: FLASH_Driver.h
 *
 * brief: Header file for the Flash Driver.
 *
 * This file contains the declaration of functions and definitions of constants and types
 * for the Flash Driver. It provides an interface for Flash memory operations such as unlocking,
 * locking, starting, selecting operations, configuring program size, and selecting sectors.
 *
 * author: Moamen Hamed
 * date: 2024-05-21
 */

/* Include standard types */
#include "STD_Types.h"

/* Define Flash Operation Codes */
#define FLSH_OP_PG 0x00000001  /**< Flash Operation: Program */
#define FLSH_OP_SER 0x00000002 /**< Flash Operation: Sector Erase */
#define FLSH_OP_MER 0x00000004 /**< Flash Operation: Mass Erase */

/* Define Flash Program Size Codes */
#define FLSH_PRO_8 0x00000000  /**< Flash Program Size: 8 bits */
#define FLSH_PRO_16 0x00000100 /**< Flash Program Size: 16 bits */
#define FLSH_PRO_32 0x00000200 /**< Flash Program Size: 32 bits */
#define FLSH_PRO_64 0x00000300 /**< Flash Program Size: 64 bits */

/* Define Flash Sector Numbers */
#define FLSH_SEC_0 0x00000000 /**< Flash Sector Number: 0 */
#define FLSH_SEC_1 0x00000008 /**< Flash Sector Number: 1 */
#define FLSH_SEC_2 0x00000010 /**< Flash Sector Number: 2 */
#define FLSH_SEC_3 0x00000018 /**< Flash Sector Number: 3 */
#define FLSH_SEC_4 0x00000020 /**< Flash Sector Number: 4 */
#define FLSH_SEC_5 0x00000028 /**< Flash Sector Number: 5 */

/***********************************************************
/* Function name: FLASH_CTRL_FCR_Unlock
/* Description: Unlock the Flash Control Register (FCR).
/*              This function unlocks the Flash Control
/*              Register to allow programming or erasing operations.
/* Parameter X : None
/* Return value: Error_Status - Returns Status_OK if the operation
/*               is successful, otherwise returns Status_FLSH_UNLOCK_FAIL.
***********************************************************/
Error_Status FLASH_CTRL_FCR_Unlock();

/***********************************************************
/* Function name: FLASH_CTRL_FCR_Lock
/* Description: Lock the Flash Control Register (FCR).
/*              This function locks the Flash Control Register
/*              to prevent any accidental programming or erasing operations.
/* Parameter X : None
/* Return value: Error_Status - Returns Status_OK if the operation
/*               is successful, otherwise returns Status_FLSH_LOCK_FAIL.
***********************************************************/
Error_Status FLASH_CTRL_FCR_Lock();

/***********************************************************
/* Function name: FLASH_CTRL_FCR_Start
/* Description: Start the Flash operation.
/*              This function starts the Flash operation that
/*              has been configured.
/* Parameter X : None
/* Return value: Error_Status - Returns Status_OK if the operation
/*               is successful, otherwise returns Status_FLSH_BSY.
***********************************************************/
Error_Status FLASH_CTRL_FCR_Start();

/***********************************************************
/* Function name: FLASH_CFG_FCR_Operation_Select
/* Description: Select Flash operation.
/*              This function selects the Flash operation to be
/*              performed, such as program, sector erase, or mass erase.
/* Parameter 1 : u32_t FLSH_Op - The operation to be selected
/*               (use FLSH_OP_* macros).
/* Return value: Error_Status - Returns Status_OK if the operation
/*               is successfully selected, otherwise returns Status_FLSH_CR_LOCK or Status_Invalid_Input.
***********************************************************/
Error_Status FLASH_CFG_FCR_Operation_Select(u32_t FLSH_Op);

/***********************************************************
/* Function name: FLASH_CFG_FCR_Program_Size
/* Description: Configure Flash program size.
/*              This function configures the size of the data to
/*              be programmed into the Flash memory.
/* Parameter 1 : u32_t FLSH_Pro - The program size to be selected
/*               (use FLSH_PRO_* macros).
/* Return value: Error_Status - Returns Status_OK if the program size
/*               is successfully configured, otherwise returns an error status.
***********************************************************/
Error_Status FLASH_CFG_FCR_Program_Size(u32_t FLSH_Pro);

/***********************************************************
/* Function name: FLASH_SEL_FCR_Sector_Number
/* Description: Select Flash sector number.
/*              This function selects the Flash sector number
/*              for the operation.
/* Parameter 1 : u32_t FLSH_Sec - The sector number to be selected
/*               (use FLSH_SEC_* macros).
/* Return value: Error_Status - Returns Status_OK if the sector number
/*               is successfully selected, otherwise returns an error status.
***********************************************************/
Error_Status FLASH_SEL_FCR_Sector_Number(u32_t FLSH_Sec);

#endif // FLASH_DRIVER_
