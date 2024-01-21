#include "device_registers.h"            /* include peripheraldeclarationsS32K144 */
#include "clocks_and_modes.h"
#include "ADC.h"
#include "lcd1602A.h"
#include <stdio.h>
# include <stdlib.h>
#include<time.h>

int lpit0_ch1_flag_counter = 0; /*< LPIT0 timeout counter */

int main_timer = 0;
int i = 0;
int score = 0;
int pre_score = 0;
int pre_key = 0;
int level = 1;


int die = 0;

int current_queue_num = 0;
int next_queue_num = 0;
char queue_position[20] = { 0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
char queue_letter[20] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char msg_array1[1] = { 0x48 };  // 1-row text-char


void enqueue(char position);


void game_one();
int player[5] = { 0,0,0,0,0 }; //플레이어
int obstacle[5] = { 6,6,6,6,6 }; //장애물 6은 장애물이 없는것, 5~0은 5~0초 후 장애물


void game_two();
int LED_num = 4; //현재 불이 켜진 LED

void game_three();
int g3_numbers[5] = { 0,0,0,0,0 }; //게임3 숫자들 *=11 0=10 #=12
int g3count = 0; //게임3 숫자의 개수
int g3_repeated=0; //게임3 반복입력 방지

void game_four();
int g4_numbers[2] = { -1,-1 }; //게임4 숫자2개 -1은 빈 공간
int g4_count = -1; //입력 제한 시간
int g4_repeated = 1; //스위치 반복 입력 방지


void PORT_init(void)
{

	// PTD    LCD--------------------------------------------------------
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;   /* Enable clock forPORTD */
   // PTD->PDDR |= 0xFE00;
	PTD->PDDR |= 1 << 9 | 1 << 10 | 1 << 11 | 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15;

	PCC->PCCn[PCC_PORTD_INDEX] &= ~PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_PCS(0x001);
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_FTM2_INDEX] &= ~PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_FTM2_INDEX] |= (PCC_PCCn_PCS(1) | PCC_PCCn_CGC_MASK);

	//Clock = 80MHz

	  //Pin mux
	PORTD->PCR[9] = PORT_PCR_MUX(1);
	PORTD->PCR[10] = PORT_PCR_MUX(1);
	PORTD->PCR[11] = PORT_PCR_MUX(1);
	PORTD->PCR[12] = PORT_PCR_MUX(1);
	PORTD->PCR[13] = PORT_PCR_MUX(1);
	PORTD->PCR[14] = PORT_PCR_MUX(1);
	PORTD->PCR[15] = PORT_PCR_MUX(1);

	//Output set(set 4bit, 2line - 0b 0010 0101 000x xxxx)

  // PTC스위치--------------------------------------------------------
	  //PORTC Data Clock Set
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;   /* Enable clockfor PORTC */
	//PortC Data Direction Set
	PTC->PDDR &= ~(1 << 11);		/* Port C13 Port Input set,value '0'*/
	PTC->PDDR &= ~(1 << 13);		/* Port C13 Port Input set,value '0'*/
	PTC->PDDR &= ~(1 << 16);		/* Port C16 Port Input set,value '0'*/
	PTC->PDDR &= ~(1 << 17);		/* Port C17 Port Input set,value '0'*/


	PORTC->PCR[11] |= PORT_PCR_MUX(1); // Port C13 mux = GPIO
	PORTC->PCR[11] |= (10 << 16); // Port C12 IRQC : interrupt onFalling-edge

	PORTC->PCR[13] |= PORT_PCR_MUX(1); // Port C13 mux = GPIO
	PORTC->PCR[13] |= (10 << 16); // Port C12 IRQC : interrupt onFalling-edge

	PORTC->PCR[16] |= PORT_PCR_MUX(1); // Port C16 mux = GPIO
	PORTC->PCR[16] |= (10 << 16); // Port C12 IRQC : interrupt onFalling-edge

	PORTC->PCR[17] |= PORT_PCR_MUX(1); // Port C17 mux = GPIO
	PORTC->PCR[17] |= (10 << 16); // Port C12 IRQC : interrupt onFalling-edge


  // PTE키패드--------------------------------------------------------
	PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock forPORT E */


	PTE->PDDR |= 1 << 12 | 1 << 14 | 1 << 15;		/* PortE12,E14-E15:  Data Direction = output */
	PTE->PDDR &= ~(1 << 0);   /* Port E0: Data Direction= input(default) */
	PTE->PDDR &= ~(1 << 1);   /* Port E1: Data Direction= input(default) */
	PTE->PDDR &= ~(1 << 2);   /* Port E2: Data Direction= input(default) */
	PTE->PDDR &= ~(1 << 3);   /* Port E3: Data Direction= input(default) */


	PORTE->PCR[0] = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK | PORT_PCR_PE(1) | PORT_PCR_PS(0); /* Port E0: MUX = GPIO, input filter enabled */
	PORTE->PCR[1] = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK | PORT_PCR_PE(1) | PORT_PCR_PS(0); /* Port E1: MUX = GPIO, input filter enabled */
	PORTE->PCR[2] = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK | PORT_PCR_PE(1) | PORT_PCR_PS(0); /* Port E2: MUX = GPIO, input filter enabled */
	PORTE->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PFE_MASK | PORT_PCR_PE(1) | PORT_PCR_PS(0); /* Port E3: MUX = GPIO, input filter enabled */


	PORTE->PCR[12] = PORT_PCR_MUX(1);	/* Port E12: MUX = GPIO  */
	PORTE->PCR[14] = PORT_PCR_MUX(1);	/* Port E14: MUX = GPIO  */
	PORTE->PCR[15] = PORT_PCR_MUX(1);	/* Port E15: MUX = GPIO  */

  // PTD    LED--------------------------------------------------------
	PTD->PDDR |= 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 8;		/* Port D1~3:  Data Direction = output */
	PORTD->PCR[1] = PORT_PCR_MUX(1);	/* Port D1: MUX = GPIO  */
	PORTD->PCR[2] = PORT_PCR_MUX(1);	/* Port D2: MUX = GPIO  */
	PORTD->PCR[3] = PORT_PCR_MUX(1);	/* Port D3: MUX = GPIO  */
	PORTD->PCR[4] = PORT_PCR_MUX(1);	/* Port D3: MUX = GPIO  */
	PORTD->PCR[5] = PORT_PCR_MUX(1);	/* Port D3: MUX = GPIO  */
	PORTD->PCR[6] = PORT_PCR_MUX(1);	/* Port D3: MUX = GPIO  */
	PORTD->PCR[7] = PORT_PCR_MUX(1);	/* Port D3: MUX = GPIO  */
	PORTD->PCR[8] = PORT_PCR_MUX(1);	/* Port D3: MUX = GPIO  */


}

void WDOG_disable(void)
{
	WDOG->CNT = 0xD928C520;     /* Unlock watchdog       */
	WDOG->TOVAL = 0x0000FFFF;   /* Maximum timeout value    */
	WDOG->CS = 0x00002100;    /* Disable watchdog       */
}



void LPIT0_init(uint32_t Dtime)
{
	/*!
	 * LPIT Clocking:
	 * ==============================
	 */
	PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6(SPLL2_DIV2_CLK)*/
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk toLPIT0regs 		*/
	  /*!
	   * LPIT Initialization:
	   */
	LPIT0->MCR = 0x00000001;  /* DBG_EN-0: Timer chans stop in Debugmode*/
										  /* DOZE_EN=0: Timer chansarestopped in DOZE mode */
										  /* SW_RST=0: SW resetdoesnot reset timer chans, regs */
										  /* M_CEN=1: enable moduleclk(allows writing other LPIT0 regs) */
	LPIT0->MIER = 0x07;  /* TIE0=1: Timer Interrupt Enabled fot Chan0,1,2*/

	LPIT0->TMR[0].TVAL = 40000000;      /* Chan 0 Timeout period:40Mclocks */
	LPIT0->TMR[0].TCTRL = 0x00000001;
	/*T_EN=1:Timer channel is enabled */
	/* CHAIN=0: channel chaining isdisabled*/
	/* MODE=0: 32 periodic counter mode */
	/* TSOT=0: Timer decrementsimmediatelybased on restart */
	/* TSOI=0: Timer does not stopaftertimeout */
	/* TROT=0 Timer will not reloadontrigger */
	/* TRG_SRC=0: External trigger soruce */
	/* TRG_SEL=0: Timer chan 0 triggersourceis selected*/

	LPIT0->TMR[1].TVAL = Dtime * 40000;      /* Chan 1 Timeout period:40Mclocks */
	LPIT0->TMR[1].TCTRL = 0x00000001;
	/*T_EN=1:Timer channel is enabled */
	/* CHAIN=0: channel chaining isdisabled*/
	/* MODE=0: 32 periodic counter mode */
	/* TSOT=0: Timer decrementsimmediatelybased on restart */
	/* TSOI=0: Timer does not stopaftertimeout */
	/* TROT=0 Timer will not reloadontrigger */
	/* TRG_SRC=0: External trigger soruce */
	/* TRG_SEL=0: Timer chan 0 triggersourceis selected*/

	LPIT0->TMR[2].TVAL = Dtime * 40000;      /* Chan 1 Timeout period:40Mclocks */
	LPIT0->TMR[2].TCTRL = 0x00000001;
}

void LPIT0_init_delay(uint32_t Delay_time)
{
	LPIT0->TMR[2].TVAL = Delay_time * 40000;      /* Chan 1 Timeout period:40Mclocks */
	LPIT0->TMR[2].TCTRL = 0x00000001;
}

void delay_ms(volatile int us) {
	LPIT0_init_delay(us);           /* Initialize PIT0 for 1 second timeout  */
	while (0 == (LPIT0->MSR & LPIT_MSR_TIF2_MASK))
		lpit0_ch1_flag_counter++;         /* Increment LPIT0timeoutcounter */
	LPIT0->MSR |= LPIT_MSR_TIF2_MASK;;//............LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timerflag 0 */
}

int KeyScan(void) {

	int Kbuff = 0;

	PTE->PSOR |= 1 << 12;
	delay_ms(1);
	if (PTE->PDIR & (1 << 0))Kbuff = 1;      //1
	if (PTE->PDIR & (1 << 1))Kbuff = 4;      //4
	if (PTE->PDIR & (1 << 2))Kbuff = 7;      //7
	if (PTE->PDIR & (1 << 3))Kbuff = 11;     //*
	PTE->PCOR |= 1 << 12;

	PTE->PSOR |= 1 << 14;
	delay_ms(1);
	if (PTE->PDIR & (1 << 0))Kbuff = 2;      //2
	if (PTE->PDIR & (1 << 1))Kbuff = 5;      //5
	if (PTE->PDIR & (1 << 2))Kbuff = 8;      //8
	if (PTE->PDIR & (1 << 3))Kbuff = 10;      //0
	PTE->PCOR |= 1 << 14;

	PTE->PSOR |= 1 << 15;
	delay_ms(1);
	if (PTE->PDIR & (1 << 0))Kbuff = 3;      //3
	if (PTE->PDIR & (1 << 1))Kbuff = 6;      //6
	if (PTE->PDIR & (1 << 2))Kbuff = 9;      //9
	if (PTE->PDIR & (1 << 3))Kbuff = 12;    //#
	PTE->PCOR |= 1 << 15;

	main_timer = main_timer + 1;

	return Kbuff;
}


int main(void)
{
	WDOG_disable();/* Disable Watchdog in case it is not done in startup code*/
	PORT_init();            /* Configure ports */
	SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus,20 MHz flash */
	LPIT0_init(1);

	ADC_init();            /* Init ADC resolution 12 bit	*/
	int key;



	SystemCoreClockUpdate();
	delay_ms(20);
	PORT_init();            /* Configure ports */

	lcdinit();        /* Initialize LCD1602A module*/

	delay_ms(200);

	PTD->PSOR |= 1 << 8;
	PTD->PSOR |= 1 << 7;
	PTD->PSOR |= 1 << 6;
	PTD->PSOR |= 1 << 5;
	PTD->PSOR |= 1 << 4;
	PTD->PSOR |= 1 << 3;
	PTD->PSOR |= 1 << 2;
	PTD->PSOR |= 1 << 1;

	//~~~~~~~~~~~~~~~~게임 시작 전 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	lcdinput(0x01);	//Clear display
	delay_ms(200);

	lcdinput(0x0C);	//Cursor off
	delay_ms(200);

	msg_array1[0] = 0xFE;
	enqueue(0x40 + 1);//공백

	msg_array1[0] = 0x50;
	enqueue(0x00 + 6);//P
	msg_array1[0] = 0x52;
	enqueue(0x00 + 7);//R
	msg_array1[0] = 0x45;
	enqueue(0x00 + 8);//E
	msg_array1[0] = 0x53;
	enqueue(0x00 + 9);//S
	msg_array1[0] = 0x53;
	enqueue(0x00 + 10);//S

	msg_array1[0] = 0x53;
	enqueue(0x40 + 5);//S
	msg_array1[0] = 0x57;
	enqueue(0x40 + 6);//W
	msg_array1[0] = 0x49;
	enqueue(0x40 + 7);//I
	msg_array1[0] = 0x54;
	enqueue(0x40 + 8);//T
	msg_array1[0] = 0x43;
	enqueue(0x40 + 9);//C
	msg_array1[0] = 0x48;
	enqueue(0x40 + 10);//H

	msg_array1[0] = 0x31;
	enqueue(0x40 + 11);//1

	while (queue_position[current_queue_num] != 0xFF)
	{

		lcdinput(0x80 + queue_position[current_queue_num]); //위치 선정
		delay_ms(3);
		lcdcharinput(queue_letter[current_queue_num]); //글자 출력
		queue_position[current_queue_num] = 0xFF;
		if (current_queue_num == 19) current_queue_num = 0;
		else current_queue_num++;
		delay_ms(10);
	}

	while ((PTC->PDIR & (1 << 11)));

	lcdinput(0x01);	//Clear display
	delay_ms(200);

	lcdinput(0x80 + 0x47);//위치 선정
	delay_ms(3);
	lcdcharinput(0xFE); //빈 공간 출력
	delay_ms(30);

	lcdinput(0x80 + 0x47);//위치 선정
	delay_ms(3);
	lcdcharinput(0x33); //3 출력
	delay_ms(1000);

	lcdinput(0x80 + 0x47);//위치 선정
	delay_ms(3);
	lcdcharinput(0x32); //2 출력
	delay_ms(1000);

	lcdinput(0x80 + 0x47);//위치 선정
	delay_ms(3);
	lcdcharinput(0x31); //1 출력
	delay_ms(1000);

	lcdinput(0x80 + 0x47);//위치 선정
	delay_ms(3);
	lcdcharinput(0xFE); //빈 공간 출력


	//~~~~~~~~~~~~~~~~게임 시작 전~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




	msg_array1[0] = 0xFF;
	enqueue(0x05);//벽
	msg_array1[0] = 0xFF;
	enqueue(0x0A);//벽
	msg_array1[0] = 0xFF;
	enqueue(0x40 + 5);//벽
	msg_array1[0] = 0xFF;
	enqueue(0x40 + 10);//벽

	msg_array1[0] = 0x4C;
	enqueue(0x00 + 6);//L
	msg_array1[0] = 0x56;
	enqueue(0x00 + 7);//V
	msg_array1[0] = 0x30;
	enqueue(0x00 + 8);//0
	msg_array1[0] = 0x31;
	enqueue(0x00 + 9);//1


	srand(time(NULL));
	uint32_t adcResultInMv = 0; //가변저항 변수
						msg_array1[0] = 0x30+level;
						enqueue(0x00 + 9);//2~7
	while (die==0)
	{
		key = KeyScan();
		if ((key != 0)&&(g3_repeated==0|| (pre_key != key)))
		{


			g3_repeated=100;
			pre_key = key;
			int previous_count = g3count;
			int k = 0;
			for (k = 0; k < 5; k++) //입력된 숫자와 같은 것 제거
			{
				if (g3_numbers[k] == key)
				{
					g3_numbers[k] = 0;
					g3count--;
				}
			}
			delay_ms(1);
			if (g3count == previous_count) die = 1; //입력된 숫자와 같은 것이 없었다면 죽음
			for(i=0;i<5;i++)
			{
				for (k = 4; k > -1; k--) //생긴 공백 제거
				{
					if (g3_numbers[k] == 0) //공백일 때만
					{
						int l = 0;
						for (l = 0; l < k; l++)
						{
							g3_numbers[k-l] = g3_numbers[k-l-1];
						}
					}
				}
			}
			delay_ms(1);

			if (g3count < 5) g3_numbers[0] = 0;
			if (g3count < 4) g3_numbers[1] = 0;
			if (g3count < 3) g3_numbers[2] = 0;
			if (g3count < 2) g3_numbers[3] = 0;
			if (g3count < 1) g3_numbers[4] = 0;

			for (k = 0; k < 5; k++)//변경된 숫자들을 LCD에 표시하려고 enqueue
			{
				switch (g3_numbers[k])
				{
				case 0:
					msg_array1[0] = 0xFE;
					enqueue(0x0B + k);//빈 공간
					break;
				case 10:
					msg_array1[0] = 0x30;
					enqueue(0x0B + k);//0
					break;
				case 11:
					msg_array1[0] = 0x2A;
					enqueue(0x0B + k);//*
					break;
				case 12:
					msg_array1[0] = 0x23;
					enqueue(0x0B + k);//#
					break;
				default:
					msg_array1[0] = 0x30 + g3_numbers[k];
					enqueue(0x0B + k);//1~9
					break;
				}
			}
		}



		convertAdcChan(13);                   /* Convert Channel AD13to pot on EVB 	*/
		while (adc_complete() == 0) {}            /* Wait forconversion complete flag 	*/
		adcResultInMv = read_adc_chx();       /* Get channel'sconversion results in mv */

		if (adcResultInMv > 4100)
		{
			if (player[4] == 0)
			{
				player[4] = 1;
				player[3] = 0;

				msg_array1[0] = 0xEF;
				enqueue(0x04);//플레이어
				msg_array1[0] = 0xFE;
				enqueue(0x03);//빈 공간
			}
		}
		else if (adcResultInMv > 3100&&adcResultInMv < 3900)
		{
			if (player[3] == 0)
			{
				player[3] = 1;
				player[2] = 0;
				player[4] = 0;

				msg_array1[0] = 0xEF;
				enqueue(0x03);//플레이어
				msg_array1[0] = 0xFE;
				enqueue(0x02);//빈 공간
				msg_array1[0] = 0xFE;
				enqueue(0x04);//빈 공간
			}
		}
		else if (adcResultInMv > 2100&&adcResultInMv < 2900)
		{
			if (player[2] == 0)
			{
				player[2] = 1;
				player[1] = 0;
				player[3] = 0;
				msg_array1[0] = 0xEF;
				enqueue(0x02);//플레이어
				msg_array1[0] = 0xFE;
				enqueue(0x01);//빈 공간
				msg_array1[0] = 0xFE;
				enqueue(0x03);//빈 공간
			}
		}
		else if (adcResultInMv > 1100&&adcResultInMv < 1900)
		{
			if (player[1] == 0)
			{
				player[1] = 1;
				player[0] = 0;
				player[2] = 0;
				msg_array1[0] = 0xEF;
				enqueue(0x01);//플레이어
				msg_array1[0] = 0xFE;
				enqueue(0x00);//빈 공간
				msg_array1[0] = 0xFE;
				enqueue(0x02);//빈 공간
			}
		}
		else if (adcResultInMv < 900)
		{

			if (player[0] == 0)
			{
				player[0] = 1;
				player[1] = 0;
				msg_array1[0] = 0xEF;
				enqueue(0x00);//플레이어
				msg_array1[0] = 0xFE;
				enqueue(0x01);//빈 공간
			}

		}


		switch (main_timer % 10) { //0.03초 주기
		case 1: //game1
			game_one();
			break;
		case 3: //game2
			game_two();
			break;
		case 5: //game3
			game_three();
			break;
		case 7: //game4
			game_four();
			break;
		case 9: //score,level (10,30,60,100,200,300)
			if (main_timer % 330 == 9)
			{

				pre_score = score;
				score++;
				if (pre_score / 1000 != score / 1000)
				{
					msg_array1[0] = 0x30 + score / 1000;
					enqueue(0x40 + 6);//0~9
				}
				if ((pre_score % 1000) / 100 != (score % 1000) / 100)
				{
					msg_array1[0] = 0x30 + (score % 1000) / 100;
					enqueue(0x40 + 7);//0~9
				}
				if ((pre_score % 100) / 10 != (score % 100) / 10)
				{
					msg_array1[0] = 0x30 + (score % 100) / 10;
					enqueue(0x40 + 8);//0~9
				}
				if (pre_score % 10 != score % 10)
				{
					msg_array1[0] = 0x30 + score % 10;
					enqueue(0x40 + 9);//0~9
				}

				if ((score == 10) || (score == 30) || (score == 60) || (score == 100) || (score == 200) || (score == 300))
				{
					level++;
					msg_array1[0] = 0x30+level;
					enqueue(0x00 + 9);//2~7
				}

			}
			break;
		default: //LCD 갱신
			if (queue_position[current_queue_num] != 0xFF)
			{

				lcdinput(0x80 + queue_position[current_queue_num]); //위치 선정
				delay_ms(3);
				lcdcharinput(queue_letter[current_queue_num]); //글자 출력
				queue_position[current_queue_num] = 0xFF;
				if (current_queue_num == 19) current_queue_num = 0;
				else current_queue_num++;
			}
			break;
		}

	}

	//~~~~~~~~~~~~~~~~게임 오버 후 결과 창 표시~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	while (queue_position[current_queue_num] != 0xFF)
				{

					lcdinput(0x80 + queue_position[current_queue_num]); //위치 선정
					delay_ms(3);
					lcdcharinput(queue_letter[current_queue_num]); //글자 출력
					queue_position[current_queue_num] = 0xFF;
					if (current_queue_num == 19) current_queue_num = 0;
					else current_queue_num++;
					delay_ms(10);
				}

	lcdinput(0x01);	//Clear display
	delay_ms(200);


	msg_array1[0] = 0xFE;
	enqueue(0x40 + 1);//공백

	msg_array1[0] = 0x47;
	enqueue(0x00 + 4);//G
	msg_array1[0] = 0x41;
	enqueue(0x00 + 5);//A
	msg_array1[0] = 0x4D;
	enqueue(0x00 + 6);//M
	msg_array1[0] = 0x45;
	enqueue(0x00 + 7);//E

	msg_array1[0] = 0x4F;
	enqueue(0x00 + 9);//O
	msg_array1[0] = 0x56;
	enqueue(0x00 + 10);//V
	msg_array1[0] = 0x45;
	enqueue(0x00 + 11);//E
	msg_array1[0] = 0x52;
	enqueue(0x00 + 12);//R

	msg_array1[0] = 0x53;
	enqueue(0x40 + 2);//S
	msg_array1[0] = 0x43;
	enqueue(0x40 + 3);//C
	msg_array1[0] = 0x4F;
	enqueue(0x40 + 4);//O
	msg_array1[0] = 0x52;
	enqueue(0x40 + 5);//R
	msg_array1[0] = 0x45;
	enqueue(0x40 + 6);//E

	msg_array1[0] = 0x3A;
	enqueue(0x40 + 8);//:

	msg_array1[0] = 0x30 + score / 1000;
	enqueue(0x40 + 10);//0~9

	msg_array1[0] = 0x30 + (score % 1000) / 100;
	enqueue(0x40 + 11);//0~9

	msg_array1[0] = 0x30 + (score % 100) / 10;
	enqueue(0x40 + 12);//0~9

	msg_array1[0] = 0x30 + score % 10;
	enqueue(0x40 + 13);//0~9
	while (queue_position[current_queue_num] != 0xFF)
	{

		lcdinput(0x80 + queue_position[current_queue_num]); //위치 선정
		delay_ms(3);
		lcdcharinput(queue_letter[current_queue_num]); //글자 출력
		queue_position[current_queue_num] = 0xFF;
		if (current_queue_num == 19) current_queue_num = 0;
		else current_queue_num++;
		delay_ms(10);
	}
	//~~~~~~~~~~~~~~~~게임 오버 후 결과 창 표시~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	delay_ms(50000);


	delay_ms(2000);
	lcdinput(0x08);	//lcd display off
	delay_ms(400);
	lcdinput(0x01);	//Clear display
	delay_ms(200);

}

void enqueue(char position)
{
	queue_position[next_queue_num] = position;
	queue_letter[next_queue_num] = msg_array1[0]; //!
	if (next_queue_num > 18) next_queue_num = 0;
	else next_queue_num++;
}

void game_one()
{
	if (main_timer % (690 - level * 30) == 1) //(2~1.4)초 주기
	{
		int obstacle_position = rand() % 5;
		if (obstacle[obstacle_position] == 6)(obstacle[obstacle_position]) = 5;
	}

	if (main_timer % 300 == 1) //0.9초 주기
	{

		int k = 0;
		for (k = 0; k < 5; k++)
		{
			if (obstacle[k] != 6)
			{
				obstacle[k]--;
				if (obstacle[k] == 0 && player[k] == 1) die = 1;
				else if (obstacle[k] == -1)
				{
					obstacle[k] = 6;

					msg_array1[0] = 0xFE;
					enqueue(0x40 + k);//빈 공간
				}
				else if (obstacle[k] == 0)
				{
					msg_array1[0] = 0x21;
					enqueue(0x40 + k);//!
				}
				else
				{
					msg_array1[0] = 0x30 + obstacle[k];
					enqueue(0x40 + k);//1~5
				}

			}
		}

	}
}

void game_two()
{

	if ((PTC->PDIR & (1 << 11)))
		;
	else //버튼을 눌렀읋때
		if ((main_timer % 100 == 3))LED_num = LED_num + 1;
	if (main_timer % (1200 - level * 90) == 23) LED_num--; //3.4~1.7초 주기

	switch (LED_num)
	{
	case 0:
		die = 1;
		break;
	case 1:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 2;
		PTD->PCOR |= 1 << 1;
		break;
	case 2:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 2;
		break;
	case 3:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 2;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 3;
		break;
	case 4:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 2;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 4;
		break;
	case 5:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 2;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 5;
		break;
	case 6:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 2;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 6;
		break;
	case 7:
		PTD->PSOR |= 1 << 8;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 2;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 7;
		break;
	case 8:
		PTD->PSOR |= 1 << 7;
		PTD->PSOR |= 1 << 6;
		PTD->PSOR |= 1 << 5;
		PTD->PSOR |= 1 << 4;
		PTD->PSOR |= 1 << 3;
		PTD->PSOR |= 1 << 2;
		PTD->PSOR |= 1 << 1;
		PTD->PCOR |= 1 << 8;
		break;
	case 9:
		die = 1;
		break;
	}
}

void game_three()
{
	if(g3_repeated>0) g3_repeated--;
	if (main_timer % (1030 - level * 30) == 5) // 3초~2.5초 주기
	{

		if (g3count == 5) die = 1;
		else
		{
			int k = 0;
			g3count++;
			for (k = 0; k < g3count - 1; k++)//이전에 있던 숫자를 왼쪽으로 한칸씩 옮기기
			{
				g3_numbers[5 - g3count + k] = g3_numbers[6 - g3count + k];
			}
			g3_numbers[4] = (rand() % 12) + 1; //가장 오른쪽에 새로운 숫자추가
			for (k = 0; k < g3count; k++)//변경된 숫자들을 LCD에 표시하려고 enqueue
			{
				switch (g3_numbers[5 - g3count + k])
				{
				case 0:
					msg_array1[0] = 0xFE;
					enqueue(0x0B + 5 - g3count + k);//빈칸
					break;
				case 10:
					msg_array1[0] = 0x30;
					enqueue(0x0B + 5 - g3count + k);//0
					break;
				case 11:
					msg_array1[0] = 0x2A;
					enqueue(0x0B + 5 - g3count + k);//*
					break;
				case 12:
					msg_array1[0] = 0x23;
					enqueue(0x0B + 5 - g3count + k);//#
					break;
				default:
					msg_array1[0] = 0x30 + g3_numbers[5 - g3count + k];
					enqueue(0x0B + 5 - g3count + k);//0~9
					break;
				}
			}
		}
	}
}


void game_four()
{
	if (main_timer % 300 == 7)//0.9초
	{
		g4_count--;
		switch (g4_count)
		{
		case 0:
			die = 1;
			break;
		case 1:
			msg_array1[0] = 0x31;
			enqueue(0x4C);//1
			break;
		case 2:
			msg_array1[0] = 0x32;
			enqueue(0x4C);//2
			break;
		case 3:
			msg_array1[0] = 0x33;
			enqueue(0x4C);//3
			break;
		case 4:
			msg_array1[0] = 0x34;
			enqueue(0x4C);//4
			break;
		}

	}
	if (main_timer % (3210 - level * 210) == 77)//9초~5.2초
	{
		g4_numbers[0] = rand() % 10;
		g4_numbers[1] = rand() % 10;
		g4_count = 5;
		msg_array1[0] = 0x35;
		enqueue(0x4C);//5
		g4_repeated = 0;
		int k = 0;
		for (k = 0; k < 2; k++)//비교 숫자
		{
			msg_array1[0] = 0x30 + g4_numbers[k];
			enqueue(0x4E + k);//0~9
		}
	}

	if ((PTC->PDIR & (1 << 13)))
				{
					;

				}
			else //버튼을 눌렀읋때
			{
				if ((g4_numbers[0]) > (g4_numbers[1]))
				{
					g4_numbers[0] = -1;
					g4_numbers[1] = -1;
					g4_count = -1;
					msg_array1[0] = 0xFE;
					enqueue(0x4C);//빈공간
					enqueue(0x4E);//빈공간
					enqueue(0x4F);//빈공간
					g4_repeated = 1;

				}
				else
				{
					;
				}
			}

	if ((PTC->PDIR & (1 << 16)));
	else //버튼을 눌렀읋때
	{
		if (g4_numbers[0] == g4_numbers[1])
		{
			g4_numbers[0] = -1;
			g4_numbers[1] = -1;
			g4_count = -1;
			msg_array1[0] = 0xFE;
			enqueue(0x4C);//빈공간
			enqueue(0x4E);//빈공간
			enqueue(0x4F);//빈공간
			g4_repeated = 1;
		}
		else
		{
			if (g4_repeated == 0)die = 1;
		}
	}
	if ((PTC->PDIR & (1 << 17)));
	else //버튼을 눌렀읋때
	{
		if (g4_numbers[0] < g4_numbers[1])
		{
			g4_numbers[0] = -1;
			g4_numbers[1] = -1;
			g4_count = -1;
			msg_array1[0] = 0xFE;
			enqueue(0x4C);//빈공간
			enqueue(0x4E);//빈공간
			enqueue(0x4F);//빈공간
			g4_repeated = 1;
		}
		else
		{
			if (g4_repeated == 0)die = 1;
		}
	}

}
