/*
 * 122a_finalproject_v1.cpp
 *
 * Created: 11/13/2017 9:30:46 AM
 * Author : brandon
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <nokia5110.c>
#include <timer.h>
#include "ws2812_config.h"
#include "light_ws2812.c"

struct cRGB led[40];


//used for the main menu
char roll[] = "roll";
char tradeP1[] = "trade P1";
char tradeP2[] = "trade P2";
char tradeP3[] = "trade P3";
char tradeP4[] = "trade P4";
char mortgage[] = "mortgage";
char unmortgage[] = "unmortgage";
char build[] = "build";

//used for the roll menu
char randomRoll[] = "random";
char camera[] = "camera";

//used for trade
char giving[] = "giving:";
char getting[] = "getting:";

//used for all menus
char done[] = "done";
char cancel[] = "cancel";
unsigned userArrow = 0;

//main menu
char* arrayMenuMain[8] = {roll, tradeP1, tradeP2, tradeP3, tradeP4, mortgage, unmortgage, build};
unsigned char menuMainSize = 8;

//roll menu
char* arrayMenuRoll[3] = {randomRoll, camera, cancel};
unsigned char menuRollSize = 3;

//trade menu
char* arrayMenuTrade[32];
unsigned char menuTradeSize = 0;

//mortgage menu
char* arrayMenuMortgage[30];
unsigned char menuMortgageSize = 0;

//unmortgage menu
char* arrayMenuUnmortgage[30];
unsigned char menuUnmortgageSize = 0;

//build menu
char* arrayMenuBuild[30];
unsigned char menuBuildSize = 0;

//used for the joystick
unsigned char goLeft = 0;
unsigned char goRight = 0;
unsigned char goUp = 0;
unsigned char goDown = 0;

unsigned char adcValueX = 0;
unsigned char adcValueY = 0;
char strX[7];
char strY[7];

//used for game logic
unsigned char board[40];
unsigned char turn = 1;
unsigned char player1 = 1;
unsigned char player2 = 2;
unsigned char player3 = 3;
unsigned char player4 = 4;
unsigned char player1Position = 0;
unsigned char player2Position = 0;
unsigned char player3Position = 0;
unsigned char player4Position = 0;
unsigned short player1Money = 1500;
unsigned short player2Money = 1500;
unsigned short player3Money = 1500;
unsigned short player4Money = 1500;
unsigned char player1Properties[40] = {0};
unsigned char player2Properties[40] = {0};
unsigned char player3Properties[40] = {0};
unsigned char player4Properties[40] = {0};
unsigned char player1PropertiesSize = 0;
unsigned char player2PropertiesSize = 0;
unsigned char player3PropertiesSize = 0;
unsigned char player4PropertiesSize = 0;;

struct properties {
	char* name;
	unsigned char special;
	unsigned char own;
	unsigned short rent;
	unsigned short mortgage;
	unsigned char isMortgage;
	unsigned short value; 
	unsigned char houses;
};

properties propertySpots[40];



// initilize properties

//property names
char namebrown1[] = "brown1";
char namebrown2[] = "brown2";
char namelblue1[] = "lblue1";
char namelblue2[] = "lblue2";
char namelblue3[] = "lblue3";
char namepink1[] = "pink1";
char namepink2[] = "pink2";
char namepink3[] = "pink3";
char nameorange1[] = "orange1";
char nameorange2[] = "orange2";
char nameorange3[] = "orange3";
char namered1[] = "red1";
char namered2[] = "red2";
char namered3[] = "red3";
char nameyellow1[] = "yellow1";
char nameyellow2[] = "yellow2";
char nameyellow3[] = "yellow3";
char namegreen1[] = "green1";
char namegreen2[] = "green2";
char namegreen3[] = "green3";
char namedblue1[] = "dblue1";
char namedblue2[] = "dblue2";
char namewater[] = "water";
char nameelectric[] = "electric";
char namerail1[] = "rail road1";
char namerail2[] = "rail road2";
char namerail3[] = "rail road3";
char namerail4[] = "rail road4";
char namechance[] = "chance";
char namechest[] = "chest";
char nameluxtax[] = "luxury tax";
char nameinctax[] = "income tax";
char namejail[] = "jail";
char namevisiting[] = "visiting";
char nameparking[] = "parking";
char namego[] = "go";

properties brown1;
properties brown2;
properties lblue1;
properties lblue2;
properties lblue3;
properties pink1;
properties pink2;
properties pink3;
properties orange1;
properties orange2;
properties orange3;
properties red1;
properties red2;
properties red3;
properties yellow1;
properties yellow2;
properties yellow3;
properties green1;
properties green2;
properties green3;
properties dblue1;
properties dblue2;
properties water;
properties electric;
properties rail1;
properties rail2;
properties rail3;
properties rail4;
properties luxtax;
properties inctax;
properties chance1;
properties chance2;
properties chance3;
properties chest1;
properties chest2;
properties chest3;
properties go;
properties jail;
properties visiting;
properties parking;

void displayLed() {
	led[player1Position].r = 10; led[player1Position].g = 0; led[player1Position].b = 0;
	led[player2Position].r = 0; led[player2Position].g = 10; led[player2Position].b = 0;
	led[player3Position].r = 0; led[player3Position].g = 0; led[player3Position].b = 10;
	led[player4Position].r = 10; led[player4Position].g = 0; led[player4Position].b = 10;
	
	ws2812_setleds(led,40);
}

void switchTurn() {
	if (turn == 1) {
		turn = 2;
	}
	else if (turn == 2) {
		turn = 3;
	}
	else if (turn == 3) {
		turn = 4;
	}
	else {
		turn = 1;
	}
}

void attemptBuy(unsigned char player) {
	if (player == 1) {
		if (player1Money > propertySpots[player1Position].value) {
			player1Money -= propertySpots[player1Position].value;
			player1Properties[player1Position] = player1Position; 
		}
	}
	else if (player == 1) {
		if (player2Money > propertySpots[player2Position].value) {
			player2Money -= propertySpots[player2Position].value;
			player2Properties[player2Position] = player2Position;
		}
	}
	else if (player == 1) {
		if (player3Money > propertySpots[player3Position].value) {
			player3Money -= propertySpots[player3Position].value;
			player3Properties[player3Position] = player3Position;
		}
	}
	else {
		if (player4Money > propertySpots[player4Position].value) {
			player4Money -= propertySpots[player4Position].value;
			player4Properties[player4Position] = player4Position;
		}
	}
}

void movePlayer(unsigned char move, unsigned char turn) {
	if (turn == 1) {
		led[player1Position].r = 0; led[player1Position].g = 0; led[player1Position].b = 0;
		if (player1Position + move > 39) {
			player1Position = player1Position + move - 39;
		}
		else {
			player1Position += move;
		}
		
		//led[player1Position].r = 10; led[player1Position].g = 0; led[player1Position].b = 0;
		
		if (propertySpots[player1Position].own != 0) {
			
		}
		else {
			attemptBuy(1);
		}
		
		switchTurn();
	}
	else if (turn == 2) {
		led[player2Position].r = 0; led[player2Position].g = 0; led[player2Position].b = 0;

		if (player2Position + move > 39) {
			player2Position = player2Position + move - 39;
		}
		else {
			player2Position += move;
		}
		
		//led[player2Position].r = 0; led[player2Position].g = 10; led[player2Position].b = 0;
		
		if (propertySpots[player2Position].own != 0) {
			
		}
		else {
			attemptBuy(2);
		}
		
		switchTurn();
	}
	else if (turn == 3) {
		led[player3Position].r = 0; led[player3Position].g = 0; led[player3Position].b = 0;

		if (player3Position + move  > 39) {
			player3Position = player3Position + move - 39;
		}
		else {
			player3Position += move;
		}
		
		//led[player3Position].r = 0; led[player3Position].g = 0; led[player3Position].b = 10;

		if (propertySpots[player3Position].own != 0) {
			
		}
		else {
			attemptBuy(3);
		}
		
		switchTurn();
	}
	else {
		led[player4Position].r = 0; led[player4Position].g = 0; led[player4Position].b = 0;

		if (player4Position + move > 39) {
			player4Position = player4Position + move - 39;
		}
		else {
			player4Position += move;
		}
	
		//led[player4Position].r = 10; led[player4Position].g = 0; led[player4Position].b = 10;
		
		if (propertySpots[player4Position].own != 0) {
			
		}
		else {
			attemptBuy(4);
		}
		
		switchTurn();
	}
	displayLed();
	ws2812_setleds(led,40);
}

void initilizeBoard() {
	propertySpots[0] = go;
	propertySpots[1] = brown1;
	propertySpots[2] = chest1;
	propertySpots[3] = brown2;
	propertySpots[4] = inctax;
	propertySpots[5] = rail1;
	propertySpots[6] = lblue1;
	propertySpots[7] = chance1;
	propertySpots[8] = lblue2;
	propertySpots[9] = lblue3;
	propertySpots[10] = visiting;
	propertySpots[11] = pink1;
	propertySpots[12] = electric;
	propertySpots[13] = pink2;
	propertySpots[14] = pink3;
	propertySpots[15] = rail2;
	propertySpots[16] = orange1;
	propertySpots[17] = chest2;
	propertySpots[18] = orange2;
	propertySpots[19] = orange3;
	propertySpots[20] = parking;
	propertySpots[21] = red1;
	propertySpots[22] = chance2;
	propertySpots[23] = red2;
	propertySpots[24] = red3;
	propertySpots[25] = rail3;
	propertySpots[26] = yellow1;
	propertySpots[27] = yellow2;
	propertySpots[28] = water;
	propertySpots[29] = yellow3;
	propertySpots[30] = jail;
	propertySpots[31] = green1;
	propertySpots[32] = green2;
	propertySpots[33] = chest3;
	propertySpots[34] = green3;
	propertySpots[35] = rail4;
	propertySpots[36] = chance3;
	propertySpots[37] = dblue1;
	propertySpots[38] = luxtax;
	propertySpots[39] = dblue2;

	for (unsigned char i = 0; i < 40; ++i) {
		propertySpots[i].name = 0;
		propertySpots[i].special = 0;
		propertySpots[i].own = 0;
		propertySpots[i].rent = 0;
		propertySpots[i].mortgage = 0;
		propertySpots[i].value = 0;
		propertySpots[i].houses = 0;
	}



	brown1.name = namebrown1;
	brown1.rent = 2;
	brown1.mortgage = 30;
	brown1.value = 60;

	brown2.name = namebrown2;
	brown2.rent = 4;
	brown2.mortgage = 30;
	brown2.value = 60;

	lblue1.name = namelblue1;
	lblue1.rent = 6;
	lblue1.mortgage = 50;
	lblue1.value = 100;

	lblue2.name = namelblue2;
	lblue2.rent = 6;
	lblue2.mortgage = 50;
	lblue2.value = 100;

	lblue3.name = namelblue1;
	lblue3.rent = 8;
	lblue3.mortgage = 60;
	lblue3.value = 120;

	pink1.name = namepink1;
	pink1.rent = 10;
	pink1.mortgage = 70;
	pink1.value = 140;

	pink2.name = namepink2;
	pink2.rent = 10;
	pink2.mortgage = 70;
	pink2.value = 140;

	pink3.name = namepink3;
	pink3.rent = 10;
	pink3.mortgage = 80;
	pink3.value = 160;

	orange1.name = nameorange1;
	orange1.rent = 14;
	orange1.mortgage = 90;
	orange1.value = 180;

	orange2.name = nameorange2;
	orange2.rent = 14;
	orange2.mortgage = 90;
	orange2.value = 180;

	orange3.name = nameorange3;
	orange3.rent = 16;
	orange3.mortgage = 100;
	orange3.value = 200;

	red1.name = namered1;
	red1.rent = 18;
	red1.mortgage = 110;
	red1.value = 220;

	red2.name = namered2;
	red2.rent = 18;
	red2.mortgage = 110;
	red2.value = 220;

	red3.name = namered3;
	red3.rent = 20;
	red3.mortgage = 120;
	red3.value = 240;

	yellow1.name = nameyellow1;
	yellow1.rent = 22;
	yellow1.mortgage = 130;
	yellow1.value = 260;

	yellow2.name = nameyellow2;
	yellow2.rent = 22;
	yellow2.mortgage = 130;
	yellow2.value = 260;

	yellow3.name = nameyellow3;
	yellow3.rent = 24;
	yellow3.mortgage = 140;
	yellow3.value = 280;

	green1.name = namegreen1;
	green1.rent = 26;
	green1.mortgage = 150;
	green1.value = 300;

	green2.name = namegreen2;
	green2.rent = 26;
	green2.mortgage = 150;
	green2.value = 300;

	green3.name = namegreen3;
	green3.rent = 28;
	green3.mortgage = 160;
	green3.value = 320;

	dblue1.name = namedblue1;
	dblue1.rent = 35;
	dblue1.mortgage = 175;
	dblue1.value = 350;

	dblue2.name = namedblue2;
	dblue2.rent = 50;
	dblue2.mortgage = 200;
	dblue2.value = 400;

	water.name = namewater;
	water.rent = 0;
	water.mortgage = 75;
	water.value = 150;

	electric.name = nameelectric;
	electric.rent = 0;
	electric.mortgage = 75;
	electric.value = 150;

	rail1.name = namerail1;
	rail1.rent = 25;
	rail1.mortgage = 100;
	rail1.value = 200;

	rail2.name = namerail2;
	rail2.rent = 25;
	rail2.mortgage = 100;
	rail2.value = 200;

	rail3.name = namerail3;
	rail3.rent = 25;
	rail3.mortgage = 100;
	rail3.value = 200;

	rail4.name = namerail4;
	rail4.rent = 25;
	rail4.mortgage = 100;
	rail4.value = 200;

	luxtax.name = nameluxtax;
	luxtax.special = 1;
	luxtax.own = 5;

	inctax.name = nameinctax;
	inctax.special = 1;
	inctax.own = 5;

	chance1.name = namechance;
	chance1.special = 1;
	chance1.own = 5;


	chance2.name = namechance;
	chance2.special = 1;
	chance2.own = 5;

	chance3.name = namechance;
	chance3.special = 1;
	chance3.own = 5;

	chest1.name = namechest;
	chest1.special = 1;
	chest1.own = 5;

	chest2.name = namechest;
	chest2.special = 1;
	chest2.own = 5;

	chest3.name = namechest;
	chest3.special = 1;
	chest3.own = 5;

	go.name = namego;
	go.special = 1;
	go.own = 5;

	jail.name = namejail;
	jail.special = 1;
	jail.own = 5;

	visiting.name = namevisiting;
	visiting.special = 1;
	visiting.own = 5;

	parking.name = nameparking;
	parking.special = 1;
	parking.own = 5;
}

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ADMUX = (ADMUX & 0xE0) | (ch & 0x1F);   //select channel (MUX0-4 bits)
	ADCSRB = (ADCSRB & 0xDF) | (ch & 0x20);   //select channel (MUX5 bit)

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	//Note you may be wondering why we have write one to clear it
	//This is standard way of clearing bits in io as said in datasheets.
	//The code writes '1' but it result in setting bit to '0' !!!

	ADCSRA|=(1<<ADIF);

	return(ADC);
}

void updateNokia(char** menu, unsigned char menuSize) {
	unsigned char startingPoint = 0;
	if (userArrow >= 6) {
		startingPoint = 6 * (userArrow / 6);
	}
	nokia_lcd_clear();
	for (unsigned char j = startingPoint; j < startingPoint + 6 && j < menuSize; ++j) {
		nokia_lcd_write_string(menu[j],1);
		nokia_lcd_set_cursor(0, ((j+1 - startingPoint)*8));
	}
	nokia_lcd_set_cursor(79, (userArrow - startingPoint) * 8);
	nokia_lcd_write_char(0x3C,1);
	nokia_lcd_render();
}

enum joystickStates {center, right, left, up, down} joystickState;

void joystickTick() {
	adcValueX = ReadADC(1) >> 2;
	adcValueY = ReadADC(0) >> 2;
	
	switch(joystickState) {
		case center:
		break;

		case right:
		goRight = 0;
		break;

		case left:
		goLeft = 0;
		break;
		
		case up:
		goUp = 0;
		break;
		
		case down:
		goDown = 0;
		break;
	}

	switch(joystickState) {
		case center:
		if (adcValueX <= 60) {
			joystickState = left;
			goLeft = 1;
		}
		else if (adcValueX >= 200) {
			joystickState = right;
			goRight = 1;
		}
		else if (adcValueY <= 60) {
			joystickState = up;
			goUp = 1;
		}
		else if (adcValueY >= 200) {
			joystickState = down;
			goDown = 1;
		}
		else {
			joystickState = center;
		}
		break;

		case right:
		if (adcValueX < 200) {
			joystickState = center;
		}
		else {
			joystickState = right;
		}
		break;

		case left:
		if (adcValueX > 60) {
			joystickState = center;
		}
		else {
			joystickState = left;
		}
		break;
		
		case down:
		if (adcValueY > 60) {
			joystickState = center;
		}
		else {
			joystickState = up;
		}
		break;
		
		case up:
		if (adcValueY < 200) {
			joystickState = center;
		}
		else {
			joystickState = down;
		}
		break;

		default:
		joystickState = center;
		break;
	}
}

enum menuStates {menuMain, menuRoll, menuTradeP1, menuTradeP2, menuTradeP3, menuTradeP4, menuMortgage, menuUnmortgage, menuBuild} menuState;
	
void menuTick() {
	switch(menuState) {
		case menuMain:
		break;
		
		case menuRoll:
		break;
		
		case menuTradeP1:
		break;
		
		case menuTradeP2:
		break;
		
		case menuTradeP3:
		break;
		
		case menuTradeP4:
		break;
		
		case menuMortgage:
		break;
		
		case menuUnmortgage:
		break;
		
		case menuBuild:
		break;
	}
	
	switch(menuState) {
		case menuMain:
		if(goRight && userArrow == 0) {
			menuState = menuRoll;
			userArrow = 0;
			updateNokia(arrayMenuRoll, menuRollSize);
		}
		else if (goRight && userArrow == 1) {
			menuState = menuTradeP1;
			userArrow = 0;
		}
		else if (goRight && userArrow == 2) {
			menuState = menuTradeP2;
			userArrow = 0;
		}
		else if (goRight && userArrow == 3) {
			menuState = menuTradeP3;
			userArrow = 0;
		}
		else if (goRight && userArrow == 4) {
			menuState = menuTradeP4;
			userArrow = 0;
		}
		else if (goRight && userArrow == 5) {
			menuState = menuMortgage;
			userArrow = 0;
		}
		else if (goRight && userArrow == 6) {
			menuState = menuUnmortgage;
			userArrow = 0;
		}
		else if (goRight && userArrow == 7) {
			menuState = menuBuild;
			userArrow = 0;
		}
		else {
			menuState = menuMain;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					updateNokia(arrayMenuMain, menuMainSize);
				}
			}
			else if (goDown) {
				if (userArrow < menuMainSize - 1) {
					userArrow++;
					updateNokia(arrayMenuMain, menuMainSize);
				}
			}	
		}
		break;
		
		case menuRoll:
		if (goRight && userArrow == 0) {
			menuState = menuMain;
			userArrow = 0;
			movePlayer((rand() % 6) + 1, turn);
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else if (goRight && userArrow == 1) {
			menuState = menuMain;
			userArrow = 0;
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else if (goRight && userArrow == 2) {
			menuState = menuMain;
			userArrow = 0;
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else {
			menuState = menuRoll;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					updateNokia(arrayMenuRoll, menuRollSize);
				}
			}
			else if (goDown) {
				if (userArrow < menuRollSize - 1) {
					userArrow++;
					updateNokia(arrayMenuRoll, menuRollSize);
				}
			}
		}
		break;
		
		case menuTradeP1:
		break;
		
		case menuTradeP2:
		break;
		
		case menuTradeP3:
		break;
		
		case menuTradeP4:
		break;
		
		case menuMortgage:
		break;
		
		case menuUnmortgage:
		break;
		
		case menuBuild:
		break;
		
		
		default:
		menuState = menuMain;
		break;
	}
}

//enum ledStates {ledUpdate} ledState;
	
//void ledTick() {
	//switch (ledState) {
		//case ledUpdate:
		
	//	break;
//	}
	
//	switch (ledState) {
	//	case ledUpdate:
	//	ledState = ledUpdate;
		//break;
		
//		default:
	//	ledState = ledUpdate;
		//break;
//	}
//}

int main(void)
{
    /* Replace with your application code */
	TimerSet(100);
	TimerOn();
	
	initilizeBoard();
	A2D_init();
    nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);
	
	displayLed();
	updateNokia(arrayMenuMain, menuMainSize);
	
	
	while(1) {
		//adcValueX = ReadADC(1) >> 2;
		//itoa(adcValueX, strX, 10);
		//adcValueY = ReadADC(0) >> 2;
		//itoa(adcValueY, strY, 10);
		//nokia_lcd_clear();
		//nokia_lcd_write_string("x: ",1);
		//nokia_lcd_write_string(strX,1);
		//nokia_lcd_write_string(" y: ",1);
		//nokia_lcd_write_string(strY,1);
		//nokia_lcd_render();
		
		joystickTick();
		menuTick();
		//ledTick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	
}

