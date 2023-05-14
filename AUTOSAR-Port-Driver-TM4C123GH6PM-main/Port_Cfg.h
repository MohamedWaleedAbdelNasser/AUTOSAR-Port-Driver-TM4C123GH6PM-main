/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Ammar Moataz
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION (1U)
#define PORT_CFG_SW_MINOR_VERSION (0U)
#define PORT_CFG_SW_PATCH_VERSION (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT (STD_ON)

/* Pre-compile option for Set Pin Direction API */
#define PORT_SET_PIN_DIRECTION_API (STD_ON)

/* Pre-compile option for Set Pin Mode API */
#define PORT_SET_PIN_MODE_API (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API (STD_OFF)

/* Number of pins in the MCU */
#define PORT_NUMBER_OF_PINS (43U)

/* MCU Port Definitions */
#define PORT_PortA (0U)
#define PORT_PortB (1U)
#define PORT_PortC (2U)
#define PORT_PortD (3U)
#define PORT_PortE (4U)
#define PORT_PortF (5U)

/* MCU Pin Number Definitions */
#define PORT_Pin0 (0U)
#define PORT_Pin1 (1U)
#define PORT_Pin2 (2U)
#define PORT_Pin3 (3U)
#define PORT_Pin4 (4U)
#define PORT_Pin5 (5U)
#define PORT_Pin6 (6U)
#define PORT_Pin7 (7U)

/* Definitions for each alternate function mode*/
/* GPIO Mode value = 0 */
#define GPIO_MODE 0
/* ADC (Analog) Mode value = 1 */
#define ADC_MODE  1U
/* Value = 1 */
#define PA0_U0Rx  1U
#define PA1_U0Tx  1U
#define PB0_U1Rx  1U
#define PB1_U1Tx  1U
#define PC4_U4Rx  1U
#define PC5_U4Tx  1U
#define PC6_U3Rx  1U
#define PC7_U4Tx  1U
#define PD0_SSI3Clk  1U
#define PD1_SSI3Fss  1U
#define PD2_SSI3Rx  1U
#define PD3_SSI3Tx  1U
#define PD4_U6Rx  1U
#define PD5_U6Tx  1U
#define PD6_U2Rx  1U
#define PD7_U2Tx  1U
#define PE0_U7Rx  1U
#define PE1_U7Tx  1U
#define PE4_U5Rx  1U
#define PE5_U5Tx  1U
#define PF0_U1RTS  1U
#define PF1_U1CTS 1U
/* Value = 2 */
#define PA2_SSI0Clk  2U
#define PA3_SSI0Fss  2U
#define PA4_SSI0Rx  2U
#define PA5_SSI0Tx  2U
#define PB4_SSI2Clk  2U
#define PB5_SSI2Fss  2U
#define PB6_SSI2Rx 2U
#define PB7_SSI2Tx  2U
#define PC4_U1Rx  2U
#define PC5_U1Tx  2U
#define PD0_SSI1Clk  2U
#define PD1_SSI1Fss  2U
#define PD2_SSI1Rx  2U
#define PD3_SSI1Tx  2U
#define PF0_SSI1Rx  2U
#define PF1_SSI1Tx  2U
#define PF2_SSI1Clk  2U
#define PF3_SSI1Fss  2U
/* Value = 3 */
#define PA6_I2C1SCL  3U
#define PA7_I2C1SDA  3U
#define PB2_I2C0SCL  3U
#define PB3_I2C0SDA  3U
#define PD0_I2C3SCL  3U
#define PD1_I2C3SDA  3U
#define PE4_I2C2SCL  3U
#define PE5_I2C2SDA  3U
#define PF0_CAN0Rx  3U
#define PF3_CAN0Tx  3U
/* Value = 4 */
#define PB4_M0PWM2  4U
#define PB5_M0PWM3  4U
#define PB6_M0PWM0  4U
#define PB7_M0PWM1  4U
#define PC4_M0PWM6  4U
#define PC5_M0PWM7  4U
#define PD0_M0PWM6  4U
#define PD1_M0PWM7  4U
#define PD2_M0FAULT0  4U
#define PD6_M0FAULT0  4U
#define PE4_M0PWM4  4U
#define PE5_M0PWM5  4U
#define PF2_M0FAULT0  4U
/* Value = 5 */
#define PA6_M1PWM2  5U
#define PA7_M1PWM3  5U
#define PD0_M1PWM0  5U
#define PD1_M1PWM1  5U
#define PE4_M1PWM2  5U
#define PE5_M1PWM3  5U
#define PF0_M1PWM4  5U
#define PF1_M1PWM5  5U
#define PF2_M1PWM6  5U
#define PF3_M1PWM7  5U
#define PF4_M1FAULT0  5U
/* Value = 6 */
#define PC4_IDX1  6U
#define PC5_PhA1  6U
#define PC6_PhB1  6U
#define PD3_IDX0  6U
#define PD6_PhA0  6U
#define PD7_PhB0  6U
#define PF0_PhA0  6U
#define PF1_PhB0  6U
#define PF4_IDX0  6U
/* Value = 7 */
#define PB0_T2CCP0  7U
#define PB1_T2CCP1  7U
#define PB2_T3CCP0  7U
#define PB3_T3CCP1  7U
#define PB4_T1CCP0  7U
#define PB5_T1CCP1  7U
#define PB6_T0CCP0  7U
#define PB7_T0CCP1 7U
#define PC4_WT0CCP0  7U
#define PC5_WT0CCP1  7U
#define PC6_WT1CCP0  7U
#define PC7_WT1CCP1  7U
#define PD0_WT2CCP0  7U
#define PD1_WT2CCP1  7U
#define PD2_WT3CCP0  7U
#define PD3_WT3CCP1  7U
#define PD4_WT4CCP0  7U
#define PD5_WT4CCP1  7U
#define PD6_WT5CCP0  7U
#define PD7_WT5CCP1  7U
#define PF0_T0CCP0  7U
#define PF1_T0CCP1  7U
#define PF2_T1CCP0  7U
#define PF3_T1CCP1  7U
#define PF4_T2CCP0  7U
/* Value = 8 */
#define PA0_CAN1Rx  8U
#define PA1_CAN1Tx  8U
#define PB4_CAN0Rx  8U
#define PB5_CAN0Tx  8U
#define PC4_U1RTS  8U
#define PC5_U1CTS  8U
#define PC6_USB0EPEN  8U
#define PC7_USB0PFLT  8U
#define PD2_USB0EPEN  8U
#define PD3_USB0PFLT  8U
#define PD7_NMI  8U
#define PE4_CAN0Rx  8U
#define PE5_CAN0Tx  8U
#define PF0_NMI  8U
#define PF4_USB0EPEN  8U
/* Value = 9 */
#define PF0_C0o  9U
#define PF1_C1o  9U
/* No modes with values 10-13 */
/* Value = 14 */
#define PF1_TRD1  14U
#define PF2_TRD0  14U
#define PF3_TRCLK  14U

#endif /* PORT_CFG_H */
