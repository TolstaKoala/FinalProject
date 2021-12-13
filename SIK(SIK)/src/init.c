// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 * init.c
 *
 *  Created on: 11 апр. 2017 г.
 *      Author: aod
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_iwdg.h"

extern USART_InitTypeDef 			USART_InitStructure;
extern GPIO_InitTypeDef  			GPIO_InitStructure;
extern TIM_TimeBaseInitTypeDef TIM_CntInit;
extern NVIC_InitTypeDef NVIC_InitStructure;

void initCPUFreq(){
	 ErrorStatus HSEStartUpStatus;
    
    RCC_DeInit();
    RCC_HSEConfig( RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
    if (HSEStartUpStatus == SUCCESS){
        RCC_HCLKConfig( RCC_SYSCLK_Div1);
        RCC_PCLK2Config( RCC_HCLK_Div1);
        RCC_PCLK1Config( RCC_HCLK_Div2);
        RCC_PLLConfig(0x00010000, RCC_PLLMul_9);
        RCC_PLLCmd( ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08) {}
    } else while(1) {}
}

//void ml8088legDeInit(){
//	/* Configure USART2 Rx as input floating */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	//GPIO_InitStructure.GPIO_OE = PORT_OE_OUT;
//	//GPIO_InitStructure.GPIO_FUNC = PORT_FUNC_PORT;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}

//void ml8088legInit(){
//	/* Configure USART2 Rx as input floating */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}


void portUART2Init(uint32_t baudrate){

	/* Fill PortInit structure*/
	//PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
	//PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	//PORT_InitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF;
	//PORT_InitStructure.PORT_PD = PORT_PD_DRIVER;
	//PORT_InitStructure.PORT_GFEN = PORT_GFEN_OFF;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;

	/* RST signal for ml8088s configuration and set */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//GPIO_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
    
		/* Enables the CPU_CLK clock on UART2 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Set the HCLK division factor = 16 for UART2*/

	//USART_BRGInit(USART2, RCC_HCLK_Div16);
	/* Configure UART2 Tx as alternate function push-pull */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure UART2 Rx as input floating */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enables the CPU_CLK clock on UART2 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	/* Set the HCLK division factor = 16 for UART2*/
	//USART_BRGInit(USART2, RCC_HCLK_Div16);


		/* NVIC Configuration.  */
    //NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt.  */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


	/* Enable UART2 NVIC*/
	NVIC_EnableIRQ(USART2_IRQn);

	// ML8080s CONFIG:
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	// ML8080s => UART 2
	USART_Init(USART2, &USART_InitStructure);

	/* Enable ML8088s - UART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable the USB-RS232-UART2 */
	USART_Cmd(USART2, ENABLE);
	return;
}

void portUART1Init(uint32_t baudrate){

	/* Fill PortInit structure*/

	// Leg for 485 (UART1)
	/* Enables the CPU_CLK clock on UART1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	
	/* Configure UART1 Tx as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure UART1 Rx as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


//	/* Enables the CPU_CLK clock on UART1 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);

	/* Set the HCLK division factor = 16 for UART1,UART2*/
	//UART_BRGInit(USART1, USART_HCLKdiv16);
		

		/* NVIC Configuration.  */
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt.  */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	/* Enable UART1 NVIC*/
	//NVIC_EnableIRQ(USART1_IRQn);


	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// RS485 => UART 1
	USART_Init(USART1, &USART_InitStructure);

	/* Enable RS485-UART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* Enable the RS485-UART1 */
	USART_Cmd(USART1, ENABLE);

	return;
}

void timerInit()
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	TIM_TimeBaseStructInit(&TIM_CntInit);
	// 3 sec period of ARR
	TIM_CntInit.TIM_Prescaler = 19999;
	TIM_CntInit.TIM_Period = 12000;
	TIM_CntInit.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit (TIM1,&TIM_CntInit);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	//TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	/* Enable TIMER1 */
	TIM_Cmd(TIM1,ENABLE);
  NVIC_EnableIRQ(TIM1_BRK_IRQn);


  }
