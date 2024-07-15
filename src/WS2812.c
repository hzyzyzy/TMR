/*
 * WS2812.c
 *
 *  Created on: 2024��7��12��
 *      Author: hp
 */

#include "WS2812.h"
//#include "CH59x_common.h"
//#include "CH592SFR.h"

__attribute__((aligned(4))) UINT32 LED_DMA_Buffer[LED_NUMBER*24 + RESET_FRAME_SIZE] = { TIMING_RESET };  // LED��PWM���巭ת����ֵ������
UINT8 LED_BYTE_Buffer[LED_NUMBER][3] = { 0 };
WS2812_Style_Func led_style_func = WS2812_Style_Off;  // Ĭ�ϱ��⺯��
uint8_t g_LED_brightness = LED_DEFAULT_BRIGHTNESS;
static uint8_t style_dir = 0;
static uint32_t style_cnt = 0;
BOOL WS2812_status = WS2812_STATUS_UNCHANGEED;
uint8_t normal_style_color[3] = { LED_DEFAULT_BRIGHTNESS, LED_DEFAULT_BRIGHTNESS, LED_DEFAULT_BRIGHTNESS };

/*******************************************************************************
 * Function Name  : WS2812_PWM_Init
 * Description    : ��ʼ��PWM
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_PWM_Init( void )
{
  // ��ʼ�� TMR1 PA10 �������PWM
  WS2812_GPIO_(SetBits)( WS2812_Pin );
  WS2812_GPIO_(ModeCfg)( WS2812_Pin, GPIO_ModeOut_PP_5mA );
  WS2812_GPIO_(ResetBits)( WS2812_Pin );



  WS2812_Style_Off( );
//  led_style_func = WS2812_Style_Breath;
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Off
 * Description    : �ر�WS2812
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Off( void )
{
  uint16_t i;
  for (i = 0; i < LED_NUMBER*24; i++) LED_DMA_Buffer[i] = TIMING_ZERO;
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Normal
 * Description    : �̶�WS2812����
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Normal( void )
{
  uint16_t i, j, memaddr = 0;
  for (i = 0; i < LED_NUMBER; i++)
  {
    LED_BYTE_Buffer[i][GREEN_INDEX] = normal_style_color[GREEN_INDEX];
    LED_BYTE_Buffer[i][RED_INDEX] = normal_style_color[RED_INDEX];
    LED_BYTE_Buffer[i][BLUE_INDEX] = normal_style_color[BLUE_INDEX];
    /* transfer data */
    for (j = 0; j < 8; j++) // GREEN data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // RED data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // BLUE data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Breath
 * Description    : PWM����WS2812�����Ʊ仯����
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Breath( void )
{
  uint16_t i, j, memaddr = 0;
  for (i = 0; i < LED_NUMBER; i++)
  {
    /* transfer data */
    for (j = 0; j < 8; j++) // GREEN data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // RED data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // BLUE data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    /* change LED state */
    if (style_dir == 0) {  // �𽥱��� - GRB��������
      ++LED_BYTE_Buffer[i][GREEN_INDEX];
      ++LED_BYTE_Buffer[i][RED_INDEX];
      ++LED_BYTE_Buffer[i][BLUE_INDEX];
    }
    else {  // �𽥱䰵 - GRB�����ݼ�
      --LED_BYTE_Buffer[i][GREEN_INDEX];
      --LED_BYTE_Buffer[i][RED_INDEX];
      --LED_BYTE_Buffer[i][BLUE_INDEX];
    }
  }
  if (LED_BYTE_Buffer[0][GREEN_INDEX] == g_LED_brightness || LED_BYTE_Buffer[0][GREEN_INDEX] == 0 ) {
    style_dir = !style_dir;
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Purple_Breath
 * Description    : PWM����WS2812��ɫ�����Ʊ仯����
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Purple_Breath( void )
{
  uint16_t i, j, memaddr = 0;
  for (i = 0; i < LED_NUMBER; i++)
  {
    /* transfer data */
    for (j = 0; j < 8; j++) // GREEN data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // RED data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // BLUE data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    /* change LED state */
    if (style_dir == 0) {  // �𽥱��� - GRB��������
      ++LED_BYTE_Buffer[i][RED_INDEX];
      ++LED_BYTE_Buffer[i][BLUE_INDEX];
    }
    else {  // �𽥱䰵 - GRB�����ݼ�
      --LED_BYTE_Buffer[i][RED_INDEX];
      --LED_BYTE_Buffer[i][BLUE_INDEX];
    }
  }
  if (LED_BYTE_Buffer[0][RED_INDEX] == g_LED_brightness || LED_BYTE_Buffer[0][RED_INDEX] == 0 ) {
    style_dir = !style_dir;
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Cyan_Breath
 * Description    : PWM����WS2812��ɫ�����Ʊ仯����
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Cyan_Breath( void )
{
  uint16_t i, j, memaddr = 0;
  for (i = 0; i < LED_NUMBER; i++)
  {
    /* transfer data */
    for (j = 0; j < 8; j++) // GREEN data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // RED data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // BLUE data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    /* change LED state */
    if (style_dir == 0) {  // �𽥱��� - GRB��������
      ++LED_BYTE_Buffer[i][GREEN_INDEX];
      ++LED_BYTE_Buffer[i][BLUE_INDEX];
    }
    else {  // �𽥱䰵 - GRB�����ݼ�
      --LED_BYTE_Buffer[i][GREEN_INDEX];
      --LED_BYTE_Buffer[i][BLUE_INDEX];
    }
  }
  if (LED_BYTE_Buffer[0][GREEN_INDEX] == g_LED_brightness || LED_BYTE_Buffer[0][GREEN_INDEX] == 0 ) {
    style_dir = !style_dir;
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Waterful
 * Description    : PWM����WS2812��ˮ�Ʊ仯����
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Waterful( void )
{
  uint16_t j;
  uint32_t slow_cnt;
  if (style_cnt % Waterful_Repeat_Times != 0) {  // ��������*Waterful_Repeat_Times = ��ˮ������
    ++style_cnt;
    if (style_cnt >= LED_NUMBER * 3 * Waterful_Repeat_Times ) { // GRB�����л� + 120ms�ƶ�һ����
      style_cnt = 0;
    }
    return;
  } else {
    slow_cnt = style_cnt / Waterful_Repeat_Times;
  }

  // �ر���һ����
  uint32_t last_cnt = slow_cnt == 0 ? LED_NUMBER-1 : slow_cnt-1;
  for (j = 0; j < 24; j++) {
    LED_DMA_Buffer[(last_cnt % LED_NUMBER) * 24 + j] = TIMING_ZERO;
  }
  // ������һ����
  for (j = 0; j < 24; j++) {
    if ( j >= slow_cnt / LED_NUMBER * 8 + 4 && j < slow_cnt / LED_NUMBER * 8 + 8 ) {
      LED_DMA_Buffer[(slow_cnt % LED_NUMBER) * 24 + j] = TIMING_ONE;
    } else {
      LED_DMA_Buffer[(slow_cnt % LED_NUMBER) * 24 + j] = TIMING_ZERO;
    }
  }
  ++style_cnt;
  if (style_cnt >= LED_NUMBER * 3 * Waterful_Repeat_Times ) { // GRB�����л� + 120ms�ƶ�һ����
    style_cnt = 0;
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Rainbow
 * Description    : PWM����WS2812�ʺ�Ʊ仯����
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Rainbow( void )
{
  signed int i;
  uint16_t j, memaddr = 0;

  i = style_cnt / 7;
  if (i >= LED_NUMBER)
      i = 0;

  // RED
  LED_BYTE_Buffer[i][GREEN_INDEX] = 0;
  LED_BYTE_Buffer[i][RED_INDEX] = 128 - ABS(i-0-(signed int)style_cnt*7/42)*17;
  LED_BYTE_Buffer[i][BLUE_INDEX] = 0;
  i = i + 1;
  if (i >= LED_NUMBER)
      i = 0;
  // CYAN
  LED_BYTE_Buffer[i][GREEN_INDEX] = 128 - ABS(i-8-(signed int)style_cnt*11/42)*11;
  LED_BYTE_Buffer[i][RED_INDEX] = 0;
  LED_BYTE_Buffer[i][BLUE_INDEX] = 128 - ABS(i-8-(signed int)style_cnt*11/42)*11;
  i = i + 1;
  if (i >= LED_NUMBER)
      i = 0;
  // YELLOW
  LED_BYTE_Buffer[i][GREEN_INDEX] = 128 - ABS(i-20-(signed int)style_cnt*12/42)*10;
  LED_BYTE_Buffer[i][RED_INDEX] = 128 - ABS(i-20-(signed int)style_cnt*12/42)*10;
  LED_BYTE_Buffer[i][BLUE_INDEX] = 0;
  i = i + 1;
  if (i >= LED_NUMBER)
      i = 0;
  // PURPLE
  LED_BYTE_Buffer[i][GREEN_INDEX] = 0;
  LED_BYTE_Buffer[i][RED_INDEX] = 128 - ABS(i-33-(signed int)style_cnt*13/42)*9;
  LED_BYTE_Buffer[i][BLUE_INDEX] = 128 - ABS(i-33-(signed int)style_cnt*13/42)*9;
  i = i + 1;
  if (i >= LED_NUMBER)
      i = 0;
  // BLUE
  LED_BYTE_Buffer[i][GREEN_INDEX] = 0;
  LED_BYTE_Buffer[i][RED_INDEX] = 0;
  LED_BYTE_Buffer[i][BLUE_INDEX] = 128 - ABS(i-47-(signed int)style_cnt*13/42)*9;
  i = i + 1;
  if (i >= LED_NUMBER)
      i = 0;
  // GREEN
  LED_BYTE_Buffer[i][GREEN_INDEX] = 128 - ABS(i-61-(signed int)style_cnt*13/42)*9;
  LED_BYTE_Buffer[i][RED_INDEX] = 0;
  LED_BYTE_Buffer[i][BLUE_INDEX] = 0;

  for (i = 0; i < LED_NUMBER; i++)
  {
    /* transfer data */
    for (j = 0; j < 8; j++) // GREEN data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // RED data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // BLUE data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
  }
  if (style_dir == 0) { // ��������
    ++style_cnt;
  } else {  // ��������
    --style_cnt;
  }
  if (style_cnt == 42 || style_cnt == 0) style_dir = !style_dir;
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Custom
 * Description    : PWM����WS2812�Զ���仯����(�Ƚ�RGBͼ��д��LED_BYTE_Buffer)
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Custom( void )
{
  uint16_t i, j, memaddr = 0;
  /* transfer data */
  for (i = 0; i < LED_NUMBER; i++)
  {
    for (j = 0; j < 8; j++) // GREEN data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][GREEN_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // RED data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][RED_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
    for (j = 0; j < 8; j++) // BLUE data
    {
      LED_DMA_Buffer[memaddr] = ((LED_BYTE_Buffer[i][BLUE_INDEX]<<j) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
      memaddr++;
    }
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Style_Warning
 * Description    : PWM����WS2812������ʾ
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Style_Warning( void )
{
  uint16_t j;

  // �ر���һ����
  uint32_t last_cnt = style_cnt == 0 ? LED_NUMBER-1 : style_cnt-1;
  for (j = 0; j < 24; j++) {
    LED_DMA_Buffer[(last_cnt % LED_NUMBER) * 24 + j] = TIMING_ZERO;
  }
  // ������һ����
  for (j = 0; j < 24; j++) {
    if ( j < 8 ) {  // ���̵���ʾ
      LED_DMA_Buffer[style_cnt * 24 + j] = TIMING_ONE;
    } else {
      LED_DMA_Buffer[style_cnt * 24 + j] = TIMING_ZERO;
    }
  }
  ++style_cnt;
  if (style_cnt >= LED_NUMBER ) {
    WS2812_Change_Style_to(WS2812_Style_Off);
  }
}

/*******************************************************************************
 * Function Name  : WS2812_Change_Style_to
 * Description    : WS2812�л�style��f
 * Input          : f - Ҫ�л���style
 * Return         : None
 *******************************************************************************/
void WS2812_Change_Style_to( WS2812_Style_Func f )
{
    WS2812_status = WS2812_STATUS_CHANGE_STYLE;
    led_style_func = f;
}

/*******************************************************************************
 * Function Name  : WS2812����RGB����
 * Description    : DMA+PWM����WS2812
 * Input          : None
 * Return         : None
 *******************************************************************************/
void WS2812_Send( void )
{
  uint16_t i;

  if ( WS2812_status == WS2812_STATUS_CHANGE_STYLE ) {
    WS2812_status = WS2812_STATUS_UNCHANGEED;
    for (i = 0; i < LED_NUMBER; i++) {  // memory set zero
      LED_BYTE_Buffer[i][0] = LED_BYTE_Buffer[i][1] = LED_BYTE_Buffer[i][2] = 0;
    }
    style_dir = style_cnt = 0;
    WS2812_Style_Off( );
  } else {
    led_style_func( ); // ���ñ仯����
  }

  { // WCH CH582M bug: �����³�ʼ��TMR_PWM����PWM+DMAż�ֵ�һ���ǿ�Byte��ʧ
    TMR1_PWMInit( High_Level, PWM_Times_1 );
    TMR1_PWMCycleCfg( 75 );        // ���� 1.25us
    //TMR1_Enable();
  }
//      TMR1_PWMActDataWidth(3000); // ռ�ձ� 50%, �޸�ռ�ձȱ�����ʱ�رն�ʱ��
      TMR1_PWMEnable();
      TMR1_Enable();
  TMR1_DMACfg( ENABLE, (UINT16) (UINT32) LED_DMA_Buffer, (UINT16) (UINT32) (LED_DMA_Buffer + LED_NUMBER*24 + RESET_FRAME_SIZE), Mode_Single );  // ����DMAת�������ڴ浽����
}


