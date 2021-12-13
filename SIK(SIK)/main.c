#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_iwdg.h"

#include "minmea.h"
#include <string.h>
#include <math.h>
#include "init.h"
#include "path.h"


#define SIZE_OF_LINE		200								// max length obrabativaemogo NMEA message
#define SIZE_OF_BUFS		1000							// UARTs Buffer length
#define OUTDATA_0X01_SIZE	39								// Redefining the request 0x01
#define OUTDATA_0X02_SIZE	1								// Redefining the request 0x03

#define CMD_GGA_GET			0x01       // Pereopredelenie zaprosa 0x01
#define PATH						0x02	
#define CMD_VTG_GET			0x03							// Pereopredelenie zaprosa 0x03

#define BR_115200			115200							// UART Speed
#define BR_9600				9600							// ~~~~||~~~~
#define BR_38400			38400							// ~~~~||~~~~

USART_InitTypeDef 			USART_InitStructure;
GPIO_InitTypeDef  			GPIO_InitStructure;
TIM_ICInitTypeDef 			TIM_ChnInit;
TIM_TimeBaseInitTypeDef TIM_CntInit;
TIM_OCInitTypeDef 			TIM_ChnOutInit;
NVIC_InitTypeDef        NVIC_InitStructure;

struct minmea_sentence_vtg frame1; //frame vector velocity and moving
struct minmea_sentence_gga frame; //frame vector gga

char outData0x01[OUTDATA_0X01_SIZE];						// ????? ?????????? ????????? ?? ??????? 0x01
unsigned char outData0x02[OUTDATA_0X02_SIZE];				// ????? ?????????? ????????? ?? ??????? 0x03

UartDataReg RxBuffer1[SIZE_OF_BUFS];						// USART1
uint32_t writeRxCounter1;									
uint32_t readRxCounter1;		

uint8_t TxBuffer1[SIZE_OF_BUFS];							// ???????? ????????? ????? ??? UART1-RS485
uint32_t writeTxCounter1;									// ??????? ???? ?? ?????? ? ?????
uint32_t readTxCounter1;									// ??????? ???? ?? ?????? ?? ??????
uint32_t sizeTx1;											// ????? ?????????? ?????????

uint8_t RxBuffer2[SIZE_OF_BUFS];							// ??????? ????????? ????? ??? UART2-ML8088
uint32_t writeRxCounter2;									// ??????? ???? ?? ?????? ? ?????
uint32_t readRxCounter2;									// ??????? ???? ?? ?????? ?? ??????

UartDataReg input485;

unsigned int gnss_data_started = 0;
unsigned int ml8088_reseted = 0;

char line[SIZE_OF_LINE];									  // ????? ??? ????????? NMEA-?????????
unsigned int i = 0, j = 0;									//
unsigned int start = 0, lineCount = 0;			//
unsigned int sending = 0, prev_time = 0, stop = 0;			//
int temp = 0;												//
double tempo = 0.0;											//
float tempik = 0.0, tt = 0.0;								//
uint16_t tmp;												//

float llat_cur = 0;
float llong_cur = 0;
float llat_prev = 0;
float llong_prev = 0;
float dist1 = 0;
float height_cur = 0;

char time_secs = -1;										//

double pi = 3.14159265358979;						//
int number_of_resets = 0;	//

int WGS84toSK42(float llat_cur ,float llong_cur, float height_cur);
void write_float(float f);

int main(void) {
  initCPUFreq();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	//UART1(STM32F10x1) & UART2(STM32F10x2)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  portUART1Init(BR_115200);
	portUART2Init(BR_115200);
	
	//priemoperedatchik na priem
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	
	//Nastroika taimera na 3 sec
	//timerInit();
	//TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	uint32_t step;
	
	outData0x02[0] = 0xFF;
	while (1) {
		/* ???????? ????????? ?? ML8088S */
		if (writeRxCounter2 != readRxCounter2) {
			/* ???? ?????? ????????? ?? NMEA ????????? ($) */
			if (RxBuffer2[readRxCounter2] == '$') {
				start = 1;
				lineCount = 0;
				line[lineCount] = 0x24;
				lineCount++;
				readRxCounter2++;
				if (readRxCounter2 == SIZE_OF_BUFS)
					readRxCounter2 = 0;
				continue;
			}
			/* ???? ?????? ????????? ???????, ?? ??????? ?? ????? ????????? */
			if (start) {
				if (RxBuffer2[readRxCounter2] != '\r') { //didn't equal '\r'
					line[lineCount] = RxBuffer2[readRxCounter2];
					readRxCounter2++;
					if (readRxCounter2 == SIZE_OF_BUFS) // ???? ????? ?? ????? ??????,?? ??????? ??????????
						readRxCounter2 = 0;
					lineCount++;
					if (lineCount == SIZE_OF_LINE) {
						lineCount = 0;
						continue;
					}
				}
				/* ????? ?? ????? ????????? ?????? ??? */
				else {
					start = 0;
					line[lineCount] = '\r'; // '\r'
					readRxCounter2++;
					if (readRxCounter2 == SIZE_OF_BUFS)
					readRxCounter2 = 0;
					lineCount++;
					line[lineCount] = '\n'; //
					readRxCounter2++;
					if (readRxCounter2 == SIZE_OF_BUFS)
						readRxCounter2 = 0;
					lineCount++;
					for (j = lineCount; j < SIZE_OF_LINE; j++)
						line[j] = 0;

					start = 0;
					switch (minmea_sentence_id(line, 0)) {
						/* ???? ??? GGA, ?? ????????? ????????? ????????? ?? 0x01 ??????*/
						case MINMEA_SENTENCE_GGA: {
							
							if (minmea_parse_gga(&frame, line)) {
								tt = (float) frame.longitude.value
										/ frame.longitude.scale;
								temp = (int) (tt / 100); // ??? ??? ? ?????? ??? ????????
								tempik = tt - (float) (temp * 100);
								tempik = ((tempik / 60 + (float) temp) * pi / 180)
										* 100000000;
								temp = (int) tempik;
								
								llong_cur = tempik;
								
								outData0x01[7] = (char) (temp >> 24); //?????? ????? ??????
								outData0x01[6] = (char) (temp >> 16);
								outData0x01[5] = (char) (temp >> 8);
								outData0x01[4] = (char) (temp);
                
								
								// Lattitude
								tt = (float) frame.latitude.value
										/ frame.latitude.scale;
								temp = (int) (tt / 100);
								tempik = tt - (float) (temp * 100);
								tempik = ((tempik / 60 + (float) temp) * pi / 180)
										* 100000000;
								temp = (int) tempik;
								
								llat_cur = tempik;
								
								outData0x01[3] = (char) (temp >> 24);
								outData0x01[2] = (char) (temp >> 16);
								outData0x01[1] = (char) (temp >> 8);
								outData0x01[0] = (char) (temp);
								
                
                if((llat_prev != 0)&&(llong_prev != 0))
								{
									dist1 += path(llat_prev, llong_prev, llat_cur, llong_cur);
								}
								
								llat_prev = llat_cur;
								llong_prev = llong_cur;


								// Magn variat
								//temp = (int)(0);
								//outData0x01[9] = (char)(temp >> 8);
								//outData0x01[8] = (char)(temp);
								time_secs = (char) (frame.time.seconds);
								outData0x01[9] = (char) (number_of_resets);
								outData0x01[8] = (char) (frame.time.seconds);

								// Elevation
								temp = (int) ((frame.altitude.value
										/ frame.altitude.scale) * 100);
								outData0x01[13] = (char) (temp >> 24); //?? ?????? ?????? ????????? ?????
								outData0x01[12] = (char) (temp >> 16);
								outData0x01[11] = (char) (temp >> 8);
								outData0x01[10] = (char) (temp);	//	outData0x01[10] = sinf(0.012);
								// actual?
								//temp = (float) height_cur;
								height_cur = (float) temp*1e-2;
								
								if (frame.fix_quality > 0) {
									gnss_data_started = 1;
									outData0x01[14] = (char) (1);
									prev_time = frame.time.seconds;
								} else {
									outData0x01[14] = (char) (0);
								}
							} else {
								j = 1;
							}
						}
						break;
						/* ???? ??? VTG, ?? ????????? ????????? ????????? ?? 0?03 ??????*/
						case MINMEA_SENTENCE_VTG: {
							
							if (minmea_parse_vtg(&frame1, line)) {
								tt = (float) frame1.speed_kph.value
										/ frame1.speed_kph.scale;
								if (tt > 0.1) {
									j = 1;
								}
								//temp = (unsigned char)(tt*100/36);
								//outData0x02[3] = (char)(temp >> 24);
								//outData0x02[2] = (char)(temp >> 16);
								//outData0x02[1] = (char)(temp >> 8);
								if(outData0x01[14]) outData0x02[0] = (unsigned char) (tt * 100 / 36);
								else outData0x02[0] = (unsigned char) (0xFF);
							} else {

							}
						}
						break;
					}
					lineCount = 0;
				}
			} else {
				readRxCounter2++;
				if (readRxCounter2 == SIZE_OF_BUFS)
					readRxCounter2 = 0;
			}
		}
		/* ???????? ????????? ?? ??????????? RS485*/
		if (writeRxCounter1 != readRxCounter1) {
			switch (RxBuffer1[readRxCounter1].bit.data) {
			case 0x41:
           // RxBuffer1[readRxCounter1].bit.PE = 1;
				if (RxBuffer1[readRxCounter1].bit.PE)
					step = 1;
				break;
			case PATH:
				if (step == 1 && RxBuffer1[readRxCounter1].bit.PE == 0) {
					write_float(dist1);
				}
			case CMD_GGA_GET:
				if (step == 1 && RxBuffer1[readRxCounter1].bit.PE == 0) {
					step = 0;
					WGS84toSK42(llat_cur ,llong_cur, height_cur);
					
					for (j = 0; j < OUTDATA_0X01_SIZE; j++) { //?????? ?????? ? ?????? ???????????
						TxBuffer1[writeTxCounter1] = outData0x01[j];
						writeTxCounter1++;
						if (writeTxCounter1 == SIZE_OF_BUFS)
							writeTxCounter1 = 0;
					}
					sizeTx1 = OUTDATA_0X01_SIZE;
					if (!sending) {
						sending = 1;
						//for (i = 0; i < 1000; i++){};
						//GPIO_SetBits(GPIOA, GPIO_Pin_10);
						USART_SendData(USART1, 0x41);
					}

				}
				break;
			case CMD_VTG_GET:
				if (step == 1 && RxBuffer1[readRxCounter1].bit.PE == 0) {
					step = 0;
					for (j = 0; j < OUTDATA_0X02_SIZE; j++) {
						TxBuffer1[writeTxCounter1] = outData0x02[j];
						writeTxCounter1++;
						if (writeTxCounter1 == SIZE_OF_BUFS)
							writeTxCounter1 = 0;
					}
					sizeTx1 = OUTDATA_0X02_SIZE;
					if (!sending) {
						sending = 1;
						//GPIO_SetBits(GPIOA, GPIO_Pin_10);
						USART_SendData(USART1, 0x41);
					}
				}
				break;
			default:
				step = 0;
				break;
			}
			readRxCounter1++;
			if (readRxCounter1 == SIZE_OF_BUFS)
				readRxCounter1 = 0;
		}
	}
}

void write_float(float f)
{
unsigned char *ptr;
char i;
ptr = (unsigned char *)&f;
for (i=0;i<4;i++)
USART_SendData(USART1, (*(ptr++)) );
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1 , USART_IT_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		tmp = USART_ReceiveData(USART1);
		memcpy(&input485, &tmp, sizeof(uint16_t));
		RxBuffer1[writeRxCounter1] = input485;
		writeRxCounter1++;
		if (writeRxCounter1 == SIZE_OF_BUFS)
			writeRxCounter1 = 0;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

	//if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
	if ((USART1->SR & USART_FLAG_TXE) != (u16)RESET){
        if (sending) {
			/* Write one byte to the transmit data register */
			sizeTx1--;
			USART_SendData(USART1, TxBuffer1[readTxCounter1]);
			readTxCounter1++;
			if (readTxCounter1 == SIZE_OF_BUFS)
				readTxCounter1 = 0;
			if (sizeTx1 == 0) {
				sending = 0;
                //USART_ClearITPendingBit(USART1, USART_IT_TXE);
			}
		} else {
			for (i = 0; i < 1000; i++) {
			};
			
			//GPIO_ResetBits(GPIOA, GPIO_Pin_3);
            //USART_ClearITPendingBit(USART1, USART_IT_TXE);
		}
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
}
void USART2_IRQHandler(void) {
	// Receive
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		RxBuffer2[writeRxCounter2] = (uint8_t) (USART_ReceiveData(USART2));
		writeRxCounter2++;
		if (writeRxCounter2 == SIZE_OF_BUFS) {
			writeRxCounter2 = 0;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		__ISB();
	}
	// Transmitte
	else if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_TXE);
		__ISB();
	} else {
		USART_ClearITPendingBit(USART2, ((uint32_t) 0x000007FF));
		__ISB();
	}
   
}

