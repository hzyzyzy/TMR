/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : ��ʱ������
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CH59x_common.h"
#include "WS2812.h"
__attribute__((aligned(4))) uint32_t CapBuf[100];
__attribute__((aligned(4))) uint32_t PwmBuf[100];

volatile uint8_t capFlag = 0;

/*********************************************************************
 * @fn      DebugInit
 *
 * @brief   ���Գ�ʼ��
 *
 * @return  none
 */
void DebugInit(void)
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

/*********************************************************************
 * @fn      main
 *
 * @brief   ������
 *
 * @return  none
 */
int main()
{
    uint8_t i;

    SetSysClock(CLK_SOURCE_PLL_60MHz);

    /* ���ô��ڵ��� */
    DebugInit();
    PRINT("Start @ChipID=%02X\n", R8_CHIP_ID);



    WS2812_PWM_Init();




    while(1)
    {


        WS2812_Style_Normal();

          WS2812_Send();

    }
}

