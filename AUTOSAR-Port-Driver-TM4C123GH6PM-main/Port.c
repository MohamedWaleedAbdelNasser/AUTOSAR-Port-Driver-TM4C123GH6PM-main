/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ammar Moataz
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION) || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION) || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigType *Port_ConfigPtr = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/************************************************************************************
 * Service Name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to configuration set.
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Initializes the Port Driver module.
 ************************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr)
{

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_INIT_FAILED);
    }
    else
#endif
    {
        /*
         * Set the module state to initialized and point to the PB configuration structure using a global pointer.
         * This global pointer is global to be used by other functions to read the PB configuration structures
         */
        Port_Status = PORT_INITIALIZED;
        Port_ConfigPtr = ConfigPtr; /* address of the first pinConfig structure --> pinConfig[0] */
    }
    volatile uint32 delay = 0;

    Port_PinType index;
    for (index = PA0; index < PORT_NUMBER_OF_PINS; index++)
    {
        /* point to the required Port Registers base address */
        volatile uint32 *Port_Ptr = NULL_PTR;
        switch (Port_ConfigPtr->pinConfig[index].portNumber)
        {
        case PORT_PortA:
            Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_PortB:
            Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_PortC:
            Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_PortD:
            Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_PortE:
            Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_PortF:
            Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
            break;
        }

        /* Open clock for the required port */
        SYSCTL_REGCGC2_REG |= (1 << Port_ConfigPtr->pinConfig[index].portNumber);
        delay = SYSCTL_REGCGC2_REG;

        /************************************************ Checking for special pins (PD7,PF0,PC0-PC3) ****************************************/
        /* If PC0-PC3, then do nothing (JTAG Pins) */
        if (((Port_ConfigPtr->pinConfig[index].portNumber == PORT_PortC) && (Port_ConfigPtr->pinConfig[index].pinNumber <= PORT_Pin3)))
        {
            continue;
        }
        /* If PD7 or PF0, then we need to unlock and commit */
        else if (((Port_ConfigPtr->pinConfig[index].portNumber == PORT_PortD) && (Port_ConfigPtr->pinConfig[index].pinNumber == PORT_Pin7)) || ((Port_ConfigPtr->pinConfig[index].portNumber == PORT_PortF) && (Port_ConfigPtr->pinConfig[index].pinNumber == PORT_Pin0)))
        {
            /* Unlock port */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;

            /* Set the required bit in commit register according to the pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
        }
        else
        {
            /* Do Nothing */
        }

        /************************************************ Configuring the pin direction ****************************************/
        /* If pin direction is output */
        if (Port_ConfigPtr->pinConfig[index].direction == PORT_PIN_OUT)
        {
            /* Set corresponding pin in the port direction register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);

            /* If initial value is high */
            if (Port_ConfigPtr->pinConfig[index].initialValue == PORT_PIN_LEVEL_HIGH)
            {
                /* Set corresponding pin in the port data register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
            }
            /* If initial value is low */
            else
            {
                /* Clear corresponding pin in the port data register */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
            }
        }
        /* If pin direction is input */
        else if (Port_ConfigPtr->pinConfig[index].direction == PORT_PIN_IN)
        {
            /* Clear corresponding pin in the port direction register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);

            /* If internal resistor is set to pull up */
            if (Port_ConfigPtr->pinConfig[index].internalResistor == PULL_UP)
            {
                /* Set corresponding bit in Pull up register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
            }
            /* If internal resistor is set to pull up */
            else if (Port_ConfigPtr->pinConfig[index].internalResistor == PULL_DOWN)
            {
                /* Set corresponding bit in Pull down register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
            }
            /* If internal resistor is disabled */
            else
            {
                /* Clear both corresponding bits in the pull up and pull down registers */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
            }
        }
        else
        {
            /* Do nothing */
        }

        /************************************************ Configuring the pin mode ****************************************/
        /* ADC Mode */
        if (Port_ConfigPtr->pinConfig[index].mode == ADC_MODE)
        {
            /* Clear corresponding bit in the digital enable register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);

            /* Set corresponding bit in the analog mode select register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
        }
        /* Digital Mode */
        else
        {
            /* Set corresponding bit in the digital enable register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);

            /* Clear corresponding bit in the analog mode select register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
        }

        /* If mode is DIO */
        if (Port_ConfigPtr->pinConfig[index].mode == GPIO_MODE)
        {
            /* Clear corresponding bit in the alternate function register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);

            /* Clear PMCx bits in port control register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(PMCx_BITS_MASK << (Port_ConfigPtr->pinConfig[index].pinNumber * 4));
        }
        /* If alternate (Not DIO) */
        else
        {
            /* Set corresponding bit in the alternate function register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);

            /* Set PMCx bits in port control register according to the required mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ((Port_ConfigPtr->pinConfig[index].mode & PMCx_BITS_MASK) << (Port_ConfigPtr->pinConfig[index].pinNumber * 4));
        }
    }
}

/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number, Direction - Port Pin Direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin direction.
 ************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    /************************************************ DET Errors Check ****************************************/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Port status uninitialized DET error */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
    }
    else
    {
        /* Do nothing */
    }
    /* Invalid port pin number DET error */
    if (Port_ConfigPtr->pinConfig[Pin].pinNumber < 0 || Port_ConfigPtr->pinConfig[Pin].pinNumber >= PORT_NUMBER_OF_PINS)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
    }
    else
    {
        /* Do nothing */
    }
    /* Direction of the pin is configured unchangeable DET error */
    if (Port_ConfigPtr->pinConfig[Pin].isDirectionChangeable == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
    }
    else
    {
        /* Do nothing */
    }
#endif

    /************************************************ JTAG Pins Check ****************************************/
    if ((Port_ConfigPtr->pinConfig[Pin].portNumber == PORT_PortC) && (Port_ConfigPtr->pinConfig[Pin].pinNumber <= PORT_Pin3))
    {
        return;
    }
    else
    {
        /* Do nothing */
    }
    /* Point to the required Port Register base address */
    volatile uint32 *Port_Ptr = NULL_PTR;
    switch (Port_ConfigPtr->pinConfig[Pin].portNumber)
    {
    case PORT_PortA:
        Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
        break;
    case PORT_PortB:
        Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
        break;
    case PORT_PortC:
        Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
        break;
    case PORT_PortD:
        Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
        break;
    case PORT_PortE:
        Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
        break;
    case PORT_PortF:
        Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
        break;
    }

    if (Direction == PORT_PIN_OUT)
    {
        /* Set corresponding pin in the port direction register */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);
    }
    else if (Direction == PORT_PIN_IN)
    {
        /* Clear corresponding pin in the port direction register */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);
    }
    else
    {
        /* Do nothing */
    }
}
#endif

/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Refreshes port direction.
 ************************************************************************************/
void Port_RefreshPortDirection(void)
{
    /************************************************ DET Error checks ****************************************/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Port status uninitialized DET error */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
    }
    else
    {
        /* Do Nothing */
    }
#endif
    Port_PinType index;
    for (index = PA0; index < PORT_NUMBER_OF_PINS; index++)
    {
        /* point to the required Port Registers base address */
        volatile uint32 *Port_Ptr = NULL_PTR;
        switch (Port_ConfigPtr->pinConfig[index].portNumber)
        {
        case PORT_PortA:
            Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
            break;
        case PORT_PortB:
            Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
            break;
        case PORT_PortC:
            Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
            break;
        case PORT_PortD:
            Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
            break;
        case PORT_PortE:
            Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
            break;
        case PORT_PortF:
            Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
            break;
        }

        /************************************************ Checking for special pins (PD7,PF0,PC0-PC3) ****************************************/
        /* If PC0-PC3, then do nothing (JTAG Pins) */
        if (((Port_ConfigPtr->pinConfig[index].portNumber == PORT_PortC) && (Port_ConfigPtr->pinConfig[index].pinNumber <= PORT_Pin3)))
        {
            continue;
        }

        /************************************************ Configuring the pin direction ****************************************/
        /* If pin direction is input */
        if (Port_ConfigPtr->pinConfig[index].direction == PORT_PIN_IN)
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
        }
        /* If pin direction is output */
        else if (Port_ConfigPtr->pinConfig[index].direction == PORT_PIN_OUT)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Port_ConfigPtr->pinConfig[index].pinNumber);
        }
        else
        {
            /* Do Nothing */
        }
    }
}

/************************************************************************************
 * Service Name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): versioninfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Returns the version information of this module.
 ************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    /************************************************ DET Error checks ****************************************/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if input pointer is not Null pointer */
    if (NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
}
#endif

/************************************************************************************
 * Service Name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number, Mode - New Port Pin mode to be set on port pin
 * Parameters (inout): None
 * Parameters (out): versioninfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Sets the port pin mode.
 ************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    /************************************************ DET Errors Check ****************************************/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Port status uninitialized DET error */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
    }
    else
    {
        /* Do nothing */
    }
    /* Invalid port pin number DET error */
    if (Port_ConfigPtr->pinConfig[Pin].pinNumber < 0 || Port_ConfigPtr->pinConfig[Pin].pinNumber >= PORT_NUMBER_OF_PINS)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
    }
    else
    {
        /* Do nothing */
    }
    /* Mode of the pin is configured unchangeable DET error */
    if (Port_ConfigPtr->pinConfig[Pin].isModeChangeable == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
    }
    else
    {
        /* Do nothing */
    }
#endif

    /************************************************ JTAG Pins Check ****************************************/
    if ((Port_ConfigPtr->pinConfig[Pin].portNumber == PORT_PortC) && (Port_ConfigPtr->pinConfig[Pin].pinNumber <= PORT_Pin3))
    {
        return;
    }
    else
    {
        /* Do nothing */
    }

    volatile uint32 *Port_Ptr = NULL_PTR;
    switch (Port_ConfigPtr->pinConfig[Pin].portNumber)
    {
    case PORT_PortA:
        Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
        break;
    case PORT_PortB:
        Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
        break;
    case PORT_PortC:
        Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
        break;
    case PORT_PortD:
        Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
        break;
    case PORT_PortE:
        Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
        break;
    case PORT_PortF:
        Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
        break;
    }

    /* ADC Mode */
    if (Mode == ADC_MODE)
    {
        /* Clear corresponding bit in the digital enable register */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);

        /* Set corresponding bit in the analog mode select register */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);
    }
    /* Digital Mode */
    else
    {
        /* Set corresponding bit in the digital enable register */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);

        /* Clear corresponding bit in the analog mode select register */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);
    }

    /* If mode is DIO */
    if (Mode == GPIO_MODE)
    {
        /* Clear corresponding bit in the alternate function register */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);

        /* Clear PMCx bits in port control register */
        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(PMCx_BITS_MASK << (Port_ConfigPtr->pinConfig[Pin].pinNumber * 4));
    }
    /* If alternate (Not DIO) */
    else
    {
        /* Set corresponding bit in the alternate function register */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Port_ConfigPtr->pinConfig[Pin].pinNumber);

        /* Clear all the PMCx bits then set them in port control register according to the required mode */
        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(PMCx_BITS_MASK << (Port_ConfigPtr->pinConfig[Pin].pinNumber * 4));
        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ((Mode & PMCx_BITS_MASK) << (Port_ConfigPtr->pinConfig[Pin].pinNumber * 4));
    }
}
#endif