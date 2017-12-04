/*
 * 122a_finalproject_v1.cpp
 *
 * Created: 11/13/2017 9:30:46 AM
 * Author : brandon
 */ 
/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <time.h>
#include <nokia5110.c>
#include <timer.h>
#include "usart_ATmega1284.h"
#include "ws2812_config.h"
#include "light_ws2812.c"



struct cRGB led[40];

unsigned char tradingWith = 0;
unsigned char usartData = 0;

//used for the main menu
char roll[] = "roll";
char tradeP1[] = "trade P1";
char tradeP2[] = "trade P2";
char tradeP3[] = "trade P3";
char tradeP4[] = "trade P4";
char mortgage[] = "mortgage";
char unmortgage[] = "unmortgage";
char build[] = "build";
char playerProperty[] = "properties";

//used for the roll menu
char randomRoll[] = "random";
char camera[] = "camera";

//used for trade
char givingProperty[] = "give property";
char givingMoney[] = "give money";
char gettingProperty[] = "get property";
char gettingMoney[] = "get money";
char tenThousand[] = "10,000";
char thousand[] = "1,000";
char hundred[] = "100";
char ten[] = "10";
char one[] = "1";

//used for all menus
char done[] = "done";
char cancel[] = "cancel";
unsigned userArrow = 0;

//main menu
char* arrayMenuMain[9] = {roll, tradeP1, tradeP2, tradeP3, tradeP4, mortgage, unmortgage, build, playerProperty};
unsigned char menuMainSize = 9;

//roll menu
char* arrayMenuRoll[3] = {randomRoll, camera, cancel};
unsigned char menuRollSize = 3;

//trade menu
char* arrayMenuTrade[6] = {gettingProperty, gettingMoney, givingProperty, givingMoney, done, cancel};
unsigned char menuTradeSize = 6;

char* arrayMenuMoney[7] = {tenThousand, thousand, hundred, ten, one, done, cancel};
unsigned char menuMoneySize = 7;


//mortgage menu
char* arrayMenuMortgage[30];
unsigned char menuMortgageSize = 0;

//unmortgage menu
char* arrayMenuUnmortgage[30];
unsigned char menuUnmortgageSize = 0;

//build menu
char* arrayMenuBuild[30];
unsigned char menuBuildSize = 0;

//properties menu
char* arrayMenuProperties1[41] = {0};
unsigned char menuPropertiesSize1 = 0;


char* arrayMenuProperties2[41] = {0};
unsigned char menuPropertiesSize2 = 0;

char* arrayMenuProperties3[41] = {0};
unsigned char menuPropertiesSize3 = 0;

char* arrayMenuProperties4[41] = {0};
unsigned char menuPropertiesSize4 = 0;

//used for the joystick
unsigned char goLeft = 0;
unsigned char goRight = 0;
unsigned char goUp = 0;
unsigned char goDown = 0;

unsigned char adcValueX = 0;
unsigned char adcValueY = 0;
char strX[7];
char strY[7];
char temp[16];

struct properties {
	char* name;
	unsigned char special;
	unsigned char own;
	unsigned short rent;
	unsigned short mortgage;
	unsigned char isMortgage;
	unsigned short value;
	unsigned char houses;
	unsigned char number;
};

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

long player1Money = 1500;
long player2Money = 1500;
long player3Money = 1500;
long player4Money = 1500;
long player1MoneyTemp = 0;
long player2MoneyTemp = 0;
long player3MoneyTemp = 0;
long player4MoneyTemp = 0;

long moneyGiving = 0;
long moneyGetting = 0;

properties* player1Properties[40] = {0};
properties* player2Properties[40] = {0};
properties* player3Properties[40] = {0};
properties* player4Properties[40] = {0};
unsigned char player1PropertiesSize = 40;
unsigned char player2PropertiesSize = 40;
unsigned char player3PropertiesSize = 40;
unsigned char player4PropertiesSize = 40;

properties* player1Trade[30] = {0};
properties* player2Trade[30] = {0};
properties* player3Trade[30] = {0};
properties* player4Trade[30] = {0};
unsigned char player1TradeSize = 2;
unsigned char player2TradeSize = 2;
unsigned char player3TradeSize = 2;
unsigned char player4TradeSize = 2;

properties* player1Mortgage[30] = {0};
properties* player2Mortgage[30] = {0};
properties* player3Mortgage[30] = {0};
properties* player4Mortgage[30] = {0};
unsigned char player1MortgageSize = 2;
unsigned char player2MortgageSize = 2;
unsigned char player3MortgageSize = 2;
unsigned char player4MortgageSize = 2;

properties* player1Unmortgage[30] = {0};
properties* player2Unmortgage[30] = {0};
properties* player3Unmortgage[30] = {0};
properties* player4Unmortgage[30] = {0};
unsigned char player1UnmortgageSize = 2;
unsigned char player2UnmortgageSize = 2;
unsigned char player3UnmortgageSize = 2;
unsigned char player4UnmortgageSize = 2;

unsigned char arrayPropertiesMortgaging[28] = {0};
unsigned char arrayPropertiesUnmortgaging[28] = {0};
unsigned char propertiesMortgagingSize = 28;
unsigned char propertiesUnmortgagingSize = 28;

unsigned char arrayPropertiesGetting[28] = {0};
unsigned char arrayPropertiesGiving[28] = {0};
unsigned char arrayMoneyGetting[5] = {0};
unsigned char arrayMoneyGiving[5] = {0};
unsigned char propertiesGettingSize = 28;
unsigned char propertiesGivingSize = 28;
unsigned char moneyGettingSize = 5;
unsigned char moneyGivingSize = 5;

properties* propertySpots[40];


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

//this is only used for menu
properties propertyDone;
properties propertyCancel;

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

void doMortgage() {
	if (turn == 1) {
		for (unsigned char i = 0; i < propertiesMortgagingSize; ++i) {
			if (arrayPropertiesMortgaging[i] != 0) {
				player1Properties[arrayPropertiesMortgaging[i]]->isMortgage = 1;
				player1Money += player1Properties[arrayPropertiesMortgaging[i]]->mortgage;
			}
		}
	}
	else if (turn == 2) {
		for (unsigned char i = 0; i < propertiesMortgagingSize; ++i) {
			if (arrayPropertiesMortgaging[i] != 0) {
				player2Properties[arrayPropertiesMortgaging[i]]->isMortgage = 1;
				player2Money += player2Properties[arrayPropertiesMortgaging[i]]->mortgage;
			}
		}
	}
	else if (turn == 3) {
		for (unsigned char i = 0; i < propertiesMortgagingSize; ++i) {
			if (arrayPropertiesMortgaging[i] != 0) {
				player3Properties[arrayPropertiesMortgaging[i]]->isMortgage = 1;
				player3Money += player3Properties[arrayPropertiesMortgaging[i]]->mortgage;
			}
		}
	}
	else {
		for (unsigned char i = 0; i < propertiesMortgagingSize; ++i) {
			if (arrayPropertiesMortgaging[i] != 0) {
				player4Properties[arrayPropertiesMortgaging[i]]->isMortgage = 1;
				player4Money += player4Properties[arrayPropertiesMortgaging[i]]->mortgage;
			}
		}
	}
}

void doUnmortgage() {
	if (turn == 1) {
		for (unsigned char i = 0; i < propertiesUnmortgagingSize; ++i) {
			if (arrayPropertiesUnmortgaging[i] != 0) {
				player1Properties[arrayPropertiesUnmortgaging[i]]->isMortgage = 0;
				player1Money -= player1Properties[arrayPropertiesUnmortgaging[i]]->mortgage;
			}
		}
	}
	else if (turn == 2) {
		for (unsigned char i = 0; i < propertiesUnmortgagingSize; ++i) {
			if (arrayPropertiesUnmortgaging[i] != 0) {
				player2Properties[arrayPropertiesUnmortgaging[i]]->isMortgage = 0;
				player2Money -= player2Properties[arrayPropertiesUnmortgaging[i]]->mortgage;
			}
		}
	}
	else if (turn == 3) {
		for (unsigned char i = 0; i < propertiesUnmortgagingSize; ++i) {
			if (arrayPropertiesUnmortgaging[i] != 0) {
				player3Properties[arrayPropertiesUnmortgaging[i]]->isMortgage = 0;
				player3Money -= player3Properties[arrayPropertiesUnmortgaging[i]]->mortgage;
			}
		}
	}
	else {
		for (unsigned char i = 0; i < propertiesUnmortgagingSize; ++i) {
			if (arrayPropertiesUnmortgaging[i] != 0) {
				player4Properties[arrayPropertiesUnmortgaging[i]]->isMortgage = 0;
				player4Money -= player4Properties[arrayPropertiesUnmortgaging[i]]->mortgage;
			}
		}
	}
}

void attemptBuy(unsigned char player) {
	if (player == 1) {
		if (player1Money > propertySpots[player1Position]->value && !propertySpots[player1Position]->own) {
			player1Money -= propertySpots[player1Position]->value;
			player1Properties[player1Position] = propertySpots[player1Position];
			//arrayMenuProperties1[player1Position] = propertySpots[player1Position]->name;
			propertySpots[player1Position]->own = 1; 
		}
	}
	else if (player == 2) {
		if (player2Money > propertySpots[player2Position]->value && !propertySpots[player2Position]->own) {
			player2Money -= propertySpots[player2Position]->value;
			player2Properties[player2Position] = propertySpots[player2Position];
			//arrayMenuProperties2[player2Position] = propertySpots[player2Position]->name;
			propertySpots[player2Position]->own = 2;
		}
	}
	else if (player == 3) {
		if (player3Money > propertySpots[player3Position]->value && !propertySpots[player3Position]->own) {
			player3Money -= propertySpots[player3Position]->value;
			player3Properties[player3Position] = propertySpots[player3Position];
			//arrayMenuProperties3[player3Position] = propertySpots[player3Position]->name;
			propertySpots[player3Position]->own = 3;
		}
	}
	else {
		if (player4Money > propertySpots[player4Position]->value && !propertySpots[player4Position]->own) {
			player4Money -= propertySpots[player4Position]->value;
			player4Properties[player4Position] = propertySpots[player4Position];
			//arrayMenuProperties4[player4Position] = propertySpots[player4Position]->name;
			propertySpots[player4Position]->own = 4;
		}
	}
}

void movePlayer(unsigned char move, unsigned char turn) {
	if (turn == 1) {
		led[player1Position].r = 0; led[player1Position].g = 0; led[player1Position].b = 0;
		if (player1Position + move > 39) {
			player1Position = player1Position + move - 39;
			if (player1Money >= 0) {
				player1Money += 200;
			}
		}
		else {
			player1Position += move;
		}
		
		if (player1Position == 4) {
			player1Money -= 200;
		}
		
		if (player1Position == 38) {
			player1Money -= 100;
		}
		
		//led[player1Position].r = 10; led[player1Position].g = 0; led[player1Position].b = 0;
		
		if (propertySpots[player1Position]->own != 0) {
			if (player1Money > 0) {
			if (propertySpots[player1Position]->own == 2) {
				player1Money -= propertySpots[player1Position]->value;
				player2Money += propertySpots[player1Position]->value;
			}
			else if (propertySpots[player1Position]->own == 3) {
				player1Money -= propertySpots[player1Position]->value;
				player3Money += propertySpots[player1Position]->value;
			}
			else if (propertySpots[player1Position]->own == 4) {
				player1Money -= propertySpots[player1Position]->value;
				player4Money += propertySpots[player1Position]->value;
			}
			}
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
			
			if (player2Money >= 0) {
				player2Money += 200;
			}
		}
		else {
			player2Position += move;
		}
		
		if (player2Position == 4) {
			player2Money -= 200;
		}
		
		if (player2Position == 38) {
			player2Money -= 100;
		}
		
		//led[player2Position].r = 0; led[player2Position].g = 10; led[player2Position].b = 0;
		
		if (propertySpots[player2Position]->own != 0) {
			if (player2Money > 0) {
			if (propertySpots[player1Position]->own == 1) {
				player2Money -= propertySpots[player2Position]->value;
				player1Money += propertySpots[player2Position]->value;
			}
			else if (propertySpots[player2Position]->own == 3) {
				player2Money -= propertySpots[player2Position]->value;
				player3Money += propertySpots[player2Position]->value;
			}
			else if (propertySpots[player2Position]->own == 4) {
				player2Money -= propertySpots[player2Position]->value;
				player4Money += propertySpots[player2Position]->value;
			}
			}
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
			if (player3Money >= 0) {
				player3Money += 200;
			}
		}
		else {
			player3Position += move;
		}
		
		if (player3Position == 4) {
			player3Money -= 200;
		}
		
		if (player3Position == 38) {
			player3Money -= 100;
		}
		
		//led[player3Position].r = 0; led[player3Position].g = 0; led[player3Position].b = 10;

		if (propertySpots[player3Position]->own != 0) {
			if (player3Money > 0) {
			if (propertySpots[player1Position]->own == 1) {
				player3Money -= propertySpots[player3Position]->value;
				player1Money += propertySpots[player3Position]->value;
			}
			else if (propertySpots[player3Position]->own == 2) {
				player3Money -= propertySpots[player3Position]->value;
				player2Money += propertySpots[player3Position]->value;
			}
			else if (propertySpots[player3Position]->own == 4) {
				player3Money -= propertySpots[player3Position]->value;
				player4Money += propertySpots[player3Position]->value;
			}
			}
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
			if (player4Money >= 0) {
				player4Money += 200;
			}
		}
		else {
			player4Position += move;
		}
	
		if (player4Position == 4) {
			player4Money -= 200;
		}
		
		if (player4Position == 38) {
			player4Money -= 100;
		}
		
		//led[player4Position].r = 10; led[player4Position].g = 0; led[player4Position].b = 10;
		
		if (propertySpots[player4Position]->own != 0) {
			if (player4Money >0) {
			if (propertySpots[player4Position]->own == 1) {
				player4Money -= propertySpots[player4Position]->value;
				player1Money += propertySpots[player4Position]->value;
			}
			else if (propertySpots[player4Position]->own == 2) {
				player4Money -= propertySpots[player4Position]->value;
				player2Money += propertySpots[player4Position]->value;
			}
			else if (propertySpots[player4Position]->own == 3) {
				player4Money -= propertySpots[player4Position]->value;
				player3Money += propertySpots[player4Position]->value;
			}
			}
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
	
	arrayMenuProperties1[40] = cancel;
	arrayMenuProperties2[40] = cancel;
	arrayMenuProperties3[40] = cancel;
	arrayMenuProperties4[40] = cancel;
	
	propertySpots[0] = &go;
	propertySpots[1] = &brown1;
	propertySpots[2] = &chest1;
	propertySpots[3] = &brown2;
	propertySpots[4] = &inctax;
	propertySpots[5] = &rail1;
	propertySpots[6] = &lblue1;
	propertySpots[7] = &chance1;
	propertySpots[8] = &lblue2;
	propertySpots[9] = &lblue3;
	propertySpots[10] = &visiting;
	propertySpots[11] = &pink1;
	propertySpots[12] = &electric;
	propertySpots[13] = &pink2;
	propertySpots[14] = &pink3;
	propertySpots[15] = &rail2;
	propertySpots[16] = &orange1;
	propertySpots[17] = &chest2;
	propertySpots[18] = &orange2;
	propertySpots[19] = &orange3;
	propertySpots[20] = &parking;
	propertySpots[21] = &red1;
	propertySpots[22] = &chance2;
	propertySpots[23] = &red2;
	propertySpots[24] = &red3;
	propertySpots[25] = &rail3;
	propertySpots[26] = &yellow1;
	propertySpots[27] = &yellow2;
	propertySpots[28] = &water;
	propertySpots[29] = &yellow3;
	propertySpots[30] = &jail;
	propertySpots[31] = &green1;
	propertySpots[32] = &green2;
	propertySpots[33] = &chest3;
	propertySpots[34] = &green3;
	propertySpots[35] = &rail4;
	propertySpots[36] = &chance3;
	propertySpots[37] = &dblue1;
	propertySpots[38] = &luxtax;
	propertySpots[39] = &dblue2;

	for (unsigned char i = 0; i < 40; ++i) {
		propertySpots[i]->name = 0;
		propertySpots[i]->special = 0;
		propertySpots[i]->own = 0;
		propertySpots[i]->rent = 0;
		propertySpots[i]->mortgage = 0;
		propertySpots[i]->value = 0;
		propertySpots[i]->houses = 0;
	}



	brown1.name = namebrown1;
	brown1.rent = 2;
	brown1.mortgage = 30;
	brown1.value = 60;
	brown1.number = 1;

	brown2.name = namebrown2;
	brown2.rent = 4;
	brown2.mortgage = 30;
	brown2.value = 60;
	brown2.number = 3;

	lblue1.name = namelblue1;
	lblue1.rent = 6;
	lblue1.mortgage = 50;
	lblue1.value = 100;
	lblue1.number = 6;

	lblue2.name = namelblue2;
	lblue2.rent = 6;
	lblue2.mortgage = 50;
	lblue2.value = 100;
	lblue2.number = 8;

	lblue3.name = namelblue3;
	lblue3.rent = 8;
	lblue3.mortgage = 60;
	lblue3.value = 120;
	lblue3.number = 9;

	pink1.name = namepink1;
	pink1.rent = 10;
	pink1.mortgage = 70;
	pink1.value = 140;
	pink1.number = 11;

	pink2.name = namepink2;
	pink2.rent = 10;
	pink2.mortgage = 70;
	pink2.value = 140;
	pink2.number = 13;

	pink3.name = namepink3;
	pink3.rent = 10;
	pink3.mortgage = 80;
	pink3.value = 160;
	pink3.number = 14;

	orange1.name = nameorange1;
	orange1.rent = 14;
	orange1.mortgage = 90;
	orange1.value = 180;
	orange1.number = 16;

	orange2.name = nameorange2;
	orange2.rent = 14;
	orange2.mortgage = 90;
	orange2.value = 180;
	orange2.number = 18;

	orange3.name = nameorange3;
	orange3.rent = 16;
	orange3.mortgage = 100;
	orange3.value = 200;
	orange3.number = 19;

	red1.name = namered1;
	red1.rent = 18;
	red1.mortgage = 110;
	red1.value = 220;
	red1.number = 21;

	red2.name = namered2;
	red2.rent = 18;
	red2.mortgage = 110;
	red2.value = 220;
	red2.number = 23;

	red3.name = namered3;
	red3.rent = 20;
	red3.mortgage = 120;
	red3.value = 240;
	red3.number = 24;

	yellow1.name = nameyellow1;
	yellow1.rent = 22;
	yellow1.mortgage = 130;
	yellow1.value = 260;
	yellow1.number = 26;

	yellow2.name = nameyellow2;
	yellow2.rent = 22;
	yellow2.mortgage = 130;
	yellow2.value = 260;
	yellow2.number = 27;

	yellow3.name = nameyellow3;
	yellow3.rent = 24;
	yellow3.mortgage = 140;
	yellow3.value = 280;
	yellow3.number = 29;

	green1.name = namegreen1;
	green1.rent = 26;
	green1.mortgage = 150;
	green1.value = 300;
	green1.number = 31;

	green2.name = namegreen2;
	green2.rent = 26;
	green2.mortgage = 150;
	green2.value = 300;
	green2.number = 32;

	green3.name = namegreen3;
	green3.rent = 28;
	green3.mortgage = 160;
	green3.value = 320;
	green3.number = 34;

	dblue1.name = namedblue1;
	dblue1.rent = 35;
	dblue1.mortgage = 175;
	dblue1.value = 350;
	dblue1.number = 37;

	dblue2.name = namedblue2;
	dblue2.rent = 50;
	dblue2.mortgage = 200;
	dblue2.value = 400;
	dblue2.number = 39;

	water.name = namewater;
	water.rent = 0;
	water.mortgage = 75;
	water.value = 150;
	water.number = 28;

	electric.name = nameelectric;
	electric.rent = 0;
	electric.mortgage = 75;
	electric.value = 150;
	electric.number = 12;

	rail1.name = namerail1;
	rail1.rent = 25;
	rail1.mortgage = 100;
	rail1.value = 200;
	rail1.number = 5;

	rail2.name = namerail2;
	rail2.rent = 25;
	rail2.mortgage = 100;
	rail2.value = 200;
	rail2.number = 15;

	rail3.name = namerail3;
	rail3.rent = 25;
	rail3.mortgage = 100;
	rail3.value = 200;
	rail3.number = 25;

	rail4.name = namerail4;
	rail4.rent = 25;
	rail4.mortgage = 100;
	rail4.value = 200;
	rail4.number = 35;

	luxtax.name = nameluxtax;
	luxtax.special = 1;
	luxtax.own = 5;
	luxtax.number = 38;

	inctax.name = nameinctax;
	inctax.special = 1;
	inctax.own = 5;
	inctax.number = 4; 

	chance1.name = namechance;
	chance1.special = 1;
	chance1.own = 5;
	chance1.number = 7;

	chance2.name = namechance;
	chance2.special = 1;
	chance2.own = 5;
	chance2.number = 22;

	chance3.name = namechance;
	chance3.special = 1;
	chance3.own = 5;
	chance3.number = 36;

	chest1.name = namechest;
	chest1.special = 1;
	chest1.own = 5;
	chest1.number = 2;

	chest2.name = namechest;
	chest2.special = 1;
	chest2.own = 5;
	chest2.number = 17;

	chest3.name = namechest;
	chest3.special = 1;
	chest3.own = 5;
	chest3.number = 33;

	go.name = namego;
	go.special = 1;
	go.own = 5;
	go.number = 0;

	jail.name = namejail;
	jail.special = 1;
	jail.own = 5;
	jail.number = 30;

	visiting.name = namevisiting;
	visiting.special = 1;
	visiting.own = 5;
	visiting.number = 10;

	parking.name = nameparking;
	parking.special = 1;
	parking.own = 5;
	parking.number = 20;
	
	propertyDone.name = done;
	
	propertyCancel.name = cancel;
}

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

uint16_t ReadADC(unsigned char ch)
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

void clearArray(unsigned char* arr, unsigned char arraySize) {
	for (unsigned char i = 0; i < arraySize; ++i) {
		arr[i] = 0;
	}
}

void updateMortgageArray() {
	unsigned char marker = 0;
	if (turn == 1) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player1Properties[i] != 0) {
				if (!(player1Properties[i]->isMortgage)) {
					player1Mortgage[marker] = player1Properties[i];
					++marker;
				}
			}
		}
		player1Mortgage[marker] = &propertyDone;
		player1Mortgage[marker+1] = &propertyCancel;
		player1MortgageSize = marker + 2;
	}
	else if (turn == 2) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player2Properties[i] != 0) {
				if (!(player2Properties[i]->isMortgage)) {
					player2Mortgage[marker] = player2Properties[i];
					++marker;
				}
			}
		}
		player2Mortgage[marker] = &propertyDone;
		player2Mortgage[marker+1] = &propertyCancel;
		player2MortgageSize = marker + 2;
	}
	else if (turn == 3) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player3Properties[i] != 0) {
				if (!(player3Properties[i]->isMortgage)) {
					player3Mortgage[marker] = player3Properties[i];
					++marker;
				}
			}
		}
		player3Mortgage[marker] = &propertyDone;
		player3Mortgage[marker+1] = &propertyCancel;
		player3MortgageSize = marker + 2;
	}
	else {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player4Properties[i] != 0) {
				if (!(player4Properties[i]->isMortgage)) {
					player4Mortgage[marker] = player4Properties[i];
					++marker;
				}
			}
		}
		player4Mortgage[marker] = &propertyDone;
		player4Mortgage[marker+1] = &propertyCancel;
		player4MortgageSize = marker + 2;
	}
}

void updateUnmortgageArray() {
	unsigned char marker = 0;
	if (turn == 1) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player1Properties[i] != 0) {
				if ((player1Properties[i]->isMortgage)) {
					player1Unmortgage[marker] = player1Properties[i];
					++marker;
				}
			}
		}
		player1Unmortgage[marker] = &propertyDone;
		player1Unmortgage[marker+1] = &propertyCancel;
		player1UnmortgageSize = marker + 2;
	}
	else if (turn == 2) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player2Properties[i] != 0) {
				if ((player2Properties[i]->isMortgage)) {
					player2Unmortgage[marker] = player2Properties[i];
					++marker;
				}
			}
		}
		player2Unmortgage[marker] = &propertyDone;
		player2Unmortgage[marker+1] = &propertyCancel;
		player2UnmortgageSize = marker + 2;
	}
	else if (turn == 3) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player3Properties[i] != 0) {
				if ((player3Properties[i]->isMortgage)) {
					player3Unmortgage[marker] = player3Properties[i];
					++marker;
				}
			}
		}
		player3Unmortgage[marker] = &propertyDone;
		player3Unmortgage[marker+1] = &propertyCancel;
		player3UnmortgageSize = marker + 2;
	}
	else {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player4Properties[i] != 0) {
				if ((player4Properties[i]->isMortgage)) {
					player4Unmortgage[marker] = player4Properties[i];
					++marker;
				}
			}
		}
		player4Unmortgage[marker] = &propertyDone;
		player4Unmortgage[marker+1] = &propertyCancel;
		player4UnmortgageSize = marker + 2;
	}
}

void updatePropertiesArray() {
	unsigned char marker = 0;
	if (turn == 1) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player1Properties[i] != 0) {
				arrayMenuProperties1[marker] = player1Properties[i]->name;
				++marker;
			}
		}
		arrayMenuProperties1[marker] = cancel;
		menuPropertiesSize1 = marker + 1;
	}
	else if (turn == 2) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player2Properties[i] != 0) {
				arrayMenuProperties2[marker] = player2Properties[i]->name;
				++marker;
			}
		}
		arrayMenuProperties2[marker] = cancel;
		menuPropertiesSize2 = marker + 1;
	}
	else if (turn == 3) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player3Properties[i] != 0) {
				arrayMenuProperties3[marker] = player3Properties[i]->name;
				++marker;
			}
		}
		arrayMenuProperties3[marker] = cancel;
		menuPropertiesSize3 = marker + 1;
	}
	else {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player4Properties[i] != 0) {
				arrayMenuProperties4[marker] = player4Properties[i]->name;
				++marker;
			}
		}
		arrayMenuProperties4[marker] = cancel;
		menuPropertiesSize4 = marker + 1;
	}
}

void updateTradeArray(unsigned char trader) {
	unsigned char marker = 0;
	if (turn == 1) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player1Properties[i] != 0) {
				player1Trade[marker] = player1Properties[i];
				++marker;
			}
		}
		player1Trade[marker] = &propertyDone;
		player1Trade[marker+1] = &propertyCancel;
		player1TradeSize = marker + 2;
	}
	else if (turn == 2) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player2Properties[i] != 0) {
				player2Trade[marker] = player2Properties[i];
				++marker;
			}
		}
		player2Trade[marker] = &propertyDone;
		player2Trade[marker+1] = &propertyCancel;
		player2TradeSize = marker + 2;
	}
	else if (turn == 3) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player3Properties[i] != 0) {
				player3Trade[marker] = player3Properties[i];
				++marker;
			}
		}
		player3Trade[marker] = &propertyDone;
		player3Trade[marker+1] = &propertyCancel;
		player3TradeSize = marker + 2;
	}
	else {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player4Properties[i] != 0) {
				player4Trade[marker] = player4Properties[i];
				++marker;
			}
		}
		player4Trade[marker] = &propertyDone;
		player4Trade[marker+1] = &propertyCancel;
		player4TradeSize = marker + 2;
	}
	
	//update the trade array for the trader
	marker = 0;
	if (trader == 1) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player1Properties[i] != 0) {
				player1Trade[marker] = player1Properties[i];
				++marker;
			}
		}
		player1Trade[marker] = &propertyDone;
		player1Trade[marker+1] = &propertyCancel;
		player1TradeSize = marker + 2;
	}
	else if (trader == 2) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player2Properties[i] != 0) {
				player2Trade[marker] = player2Properties[i];
				++marker;
			}
		}
		player2Trade[marker] = &propertyDone;
		player2Trade[marker+1] = &propertyCancel;
		player2TradeSize = marker + 2;
	}
	else if (trader == 3) {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player3Properties[i] != 0) {
				player3Trade[marker] = player3Properties[i];
				++marker;
			}
		}
		player3Trade[marker] = &propertyDone;
		player3Trade[marker+1] = &propertyCancel;
		player3TradeSize = marker + 2;
	}
	else {
		for (unsigned char i = 0; i < 40; ++i) {
			if (player4Properties[i] != 0) {
				player4Trade[marker] = player4Properties[i];
				++marker;
			}
		}
		player4Trade[marker] = &propertyDone;
		player4Trade[marker+1] = &propertyCancel;
		player4TradeSize = marker + 2;
	}
}

void updateNokia(char** menu, unsigned char menuSize) {
	unsigned char startingPoint = 0;
	if (userArrow >= 5) {
		startingPoint = 5 * (userArrow / 5);
	}
	nokia_lcd_clear();
	if(turn == 1) {
		nokia_lcd_write_string("Player 1: ", 1);
		nokia_lcd_write_string(itoa(player1Money, temp, 10), 1);
	}
	else if(turn == 2) {
		nokia_lcd_write_string("Player 2: ", 1);
		nokia_lcd_write_string(itoa(player2Money, temp, 10), 1);
	}
	else if(turn == 3) {
		nokia_lcd_write_string("Player 3: ", 1);
		nokia_lcd_write_string(itoa(player3Money, temp, 10), 1);
	}
	if(turn == 4) {
		nokia_lcd_write_string("Player 4: ", 1);
		nokia_lcd_write_string(itoa(player4Money, temp, 10), 1);
	}
	nokia_lcd_set_cursor(0, 8);
	for (unsigned char j = startingPoint; j < startingPoint + 5 && j < menuSize; ++j) {
		nokia_lcd_write_string(menu[j],1);
		nokia_lcd_set_cursor(0, ((j+2 - startingPoint)*8));
	}
	nokia_lcd_set_cursor(79, (userArrow - startingPoint + 1) * 8);
	nokia_lcd_write_char(0x3C,1);
	nokia_lcd_render();
}

void updateNokia(properties** menu, unsigned char menuSize) {
	unsigned char startingPoint = 0;
	if (userArrow >= 5) {
		startingPoint = 5 * (userArrow / 5);
	}
	nokia_lcd_clear();
	if(turn == 1) {
		nokia_lcd_write_string("Player 1: ", 1);
		nokia_lcd_write_string(itoa(player1Money, temp, 10), 1);
	}
	else if(turn == 2) {
		nokia_lcd_write_string("Player 2: ", 1);
		nokia_lcd_write_string(itoa(player2Money, temp, 10), 1);
	}
	else if(turn == 3) {
		nokia_lcd_write_string("Player 3: ", 1);
		nokia_lcd_write_string(itoa(player3Money, temp, 10), 1);
	}
	if(turn == 4) {
		nokia_lcd_write_string("Player 4: ", 1);
		nokia_lcd_write_string(itoa(player4Money, temp, 10), 1);
	}
	nokia_lcd_set_cursor(0, 8);
	for (unsigned char j = startingPoint; j < startingPoint + 5 && j < menuSize; ++j) {
		nokia_lcd_write_string(menu[j]->name,1);
		nokia_lcd_set_cursor(0, ((j+2 - startingPoint)*8));
	}
	nokia_lcd_set_cursor(79, (userArrow - startingPoint + 1) * 8);
	nokia_lcd_write_char(0x3C,1);
	nokia_lcd_render();
}

void updateNokia(unsigned char* menu, unsigned char menuSize) {
	unsigned char startingPoint = 0;
	if (userArrow >= 5) {
		startingPoint = 5 * (userArrow / 5);
	}
	
	nokia_lcd_set_cursor(73, 8);
	for (unsigned char j = startingPoint; j < startingPoint + 5 && j < menuSize; ++j) {
		if (menu[j] != 0) {
			nokia_lcd_write_char(0x31,1);	
		}
		else {
			nokia_lcd_write_char(' ',1);
		}
		nokia_lcd_set_cursor(73, ((j+2 - startingPoint)*8));
	}
	nokia_lcd_render();
}

void updateNokiaMoney(unsigned char* menu, unsigned char menuSize) {
	unsigned char startingPoint = 0;
	if (userArrow >= 5) {
		startingPoint = 5 * (userArrow / 5);
	}
	
	nokia_lcd_set_cursor(73, 8);
	for (unsigned char j = startingPoint; j < startingPoint + 5 && j < menuSize; ++j) {
		if (menu[j] != 0) {
			nokia_lcd_write_char(menu[j] + '0',1);
		}
		else {
			nokia_lcd_write_char(' ',1);
		}
		nokia_lcd_set_cursor(73, ((j+2 - startingPoint)*8));
	}
	nokia_lcd_render();
}

void clearTradeValues() {
	clearArray(arrayMoneyGetting, moneyGettingSize);
	clearArray(arrayMoneyGiving, moneyGivingSize);
	clearArray(arrayPropertiesGetting, propertiesGettingSize);
	clearArray(arrayPropertiesGiving, propertiesGivingSize);
	moneyGiving = 0;
	moneyGetting = 0;
	player1MoneyTemp = 0;
	player2MoneyTemp = 0;
	player3MoneyTemp = 0;
	player4MoneyTemp = 0;
}

enum joystickStates {center, right, left, up, down} joystickState;

void joystickTick() {
	
	if (turn == 1) {
		adcValueX = ReadADC(1) >> 2;
		adcValueY = ReadADC(0) >> 2;
	}
	else if (turn == 2) {
		adcValueX = ReadADC(3) >> 2;
		adcValueY = ReadADC(2) >> 2;
	}
	else if (turn == 3) {
		adcValueX = ReadADC(5) >> 2;
		adcValueY = ReadADC(4) >> 2;
	}
	else {
		adcValueX = ReadADC(7) >> 2;
		adcValueY = ReadADC(6) >> 2;
	}
	
	
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

enum menuStates {menuMain, menuRoll, menuTradeP1, menuTradeP2, menuTradeP3, menuTradeP4, menuMortgage, menuUnmortgage, menuBuild, menuProperties, menuGettingProperty, menuGettingMoney, menuGivingProperty, menuGivingMoney, menuWaitForImage} menuState;
	
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
		
		case menuProperties:
		break;
		
		case menuGettingProperty:
		break;
		
		case menuGettingMoney:
		break;
		
		case menuGivingProperty:
		break;
		
		case menuGivingMoney:
		break;
		
		case menuWaitForImage:
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
			updateTradeArray(1);
			updateNokia(arrayMenuTrade, menuTradeSize);
		}
		else if (goRight && userArrow == 2) {
			menuState = menuTradeP2;
			userArrow = 0;
			updateTradeArray(2);
			updateNokia(arrayMenuTrade, menuTradeSize);
		}
		else if (goRight && userArrow == 3) {
			menuState = menuTradeP3;
			userArrow = 0;
			updateTradeArray(3);
			updateNokia(arrayMenuTrade, menuTradeSize);
		}
		else if (goRight && userArrow == 4) {
			menuState = menuTradeP4;
			userArrow = 0;
			updateTradeArray(4);
			updateNokia(arrayMenuTrade, menuTradeSize);
		}
		else if (goRight && userArrow == 5) {
			menuState = menuMortgage;
			userArrow = 0;
			updateMortgageArray();
			if (turn == 1) {
				updateNokia(player1Mortgage, player1MortgageSize);
			}
			else if (turn == 2) {
				updateNokia(player2Mortgage, player2MortgageSize);
			}
			else if (turn == 3) {
				updateNokia(player3Mortgage, player3MortgageSize);
			}
			else {
				updateNokia(player4Mortgage, player4MortgageSize);
			}
		}
		else if (goRight && userArrow == 6) {
			menuState = menuUnmortgage;
			userArrow = 0;
			updateUnmortgageArray();
			if (turn == 1) {
				updateNokia(player1Unmortgage, player1UnmortgageSize);
			}
			else if (turn == 2) {
				updateNokia(player2Unmortgage, player2UnmortgageSize);
			}
			else if (turn == 3) {
				updateNokia(player3Unmortgage, player3UnmortgageSize);
			}
			else {
				updateNokia(player4Unmortgage, player4UnmortgageSize);
			}
		}
		else if (goRight && userArrow == 7) {
			//menuState = menuBuild;
			//userArrow = 0;
		}
		else if (goRight && userArrow == 8) {
			menuState = menuProperties;
			userArrow = 0;
			updatePropertiesArray();
			if (turn == 1) {
				updateNokia(arrayMenuProperties1, menuPropertiesSize1);
			}
			else if (turn == 2) {
				updateNokia(arrayMenuProperties2, menuPropertiesSize2);
			}
			else if (turn == 3) {
				updateNokia(arrayMenuProperties3, menuPropertiesSize3);
			}
			else {
				updateNokia(arrayMenuProperties4, menuPropertiesSize4);
			}
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
			menuState = menuWaitForImage;
			userArrow = 0;
			nokia_lcd_clear();
			nokia_lcd_write_string("Waiting for raspberry pi",1);
			nokia_lcd_render();
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
		tradingWith = 1;
		if (goRight && userArrow == 0 && turn != 1) {
			menuState = menuGettingProperty;
			userArrow = 0;
			updateNokia(player1Trade, player1TradeSize);
		}
		else if (goRight && userArrow == 1 && turn != 1) {
			menuState = menuGettingMoney;
			userArrow = 0;
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 2 && turn != 1) {
			menuState = menuGivingProperty;
			userArrow = 0;
			if(turn == 2) {
				updateNokia(player2Trade, player2TradeSize);
			}
			else if (turn == 3) {
				updateNokia(player3Trade, player3TradeSize);
			}
			else if (turn == 4) {
				updateNokia(player4Trade, player4TradeSize);
			}
			else {
				
			}
		}
		else if (goRight && userArrow == 3 && turn != 1) {
			menuState = menuGivingMoney;
			userArrow = 0;
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 4 && turn != 1) {
			menuState = menuMain;
			userArrow = 0;
			//transfer the values
			//CLEAR THE ARRAYS!!!
			
			if (turn == 2) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player2Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player1Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 2;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player1Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player2Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 1;
					}
				}
				
				//transfer money
				player2Money += moneyGetting;
				player1Money -= moneyGetting;
				player2Money -= moneyGiving;
				player1Money += moneyGiving;
				
			}
			else if (turn == 3) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player3Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player1Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 3;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player1Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player3Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 1;
					}
				}
				
				//transfer money
				player3Money += moneyGetting;
				player1Money -= moneyGetting;
				player3Money -= moneyGiving;
				player1Money += moneyGiving;
			}
			else {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player4Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player1Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 4;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player1Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player4Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 1;
					}
				}
				
				//transfer money
				player4Money += moneyGetting;
				player1Money -= moneyGetting;
				player4Money -= moneyGiving;
				player1Money += moneyGiving;
			}
			
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else if (goRight && userArrow == 5) {
			menuState = menuMain;
			userArrow = 0;
			//CLEAR THE ARRAYS!!!!
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else {
			menuState = menuTradeP1;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goDown) {
				if (userArrow < menuTradeSize - 1) {
					userArrow++;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
		}
		break;
		
		case menuTradeP2:
		tradingWith = 2;
		if (goRight && userArrow == 0 && turn != 2) {
			menuState = menuGettingProperty;
			userArrow = 0;
			updateNokia(player2Trade, player2TradeSize);
		}
		else if (goRight && userArrow == 1 && turn != 2) {
			menuState = menuGettingMoney;
			userArrow = 0;
			//update array
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 2 && turn != 2) {
			menuState = menuGivingProperty;
			userArrow = 0;
			if(turn == 1) {
				updateNokia(player1Trade, player1TradeSize);
			}
			else if (turn == 3) {
				updateNokia(player3Trade, player3TradeSize);
			}
			else if (turn == 4) {
				updateNokia(player4Trade, player4TradeSize);
			}
			else {
				
			}
		}
		else if (goRight && userArrow == 3 && turn != 2) {
			menuState = menuGivingMoney;
			userArrow = 0;
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 4 && turn != 2) {
			menuState = menuMain;
			userArrow = 0;
			//transfer the values
			//CLEAR THE ARRAYS!!!
			
			if (turn == 1) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player1Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player2Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 1;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player2Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player1Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 2;
					}
				}
				
				//transfer money
				player1Money += moneyGetting;
				player2Money -= moneyGetting;
				player1Money -= moneyGiving;
				player2Money += moneyGiving;
				
			}
			else if (turn == 3) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player3Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player2Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 3;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player2Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player3Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 2;
					}
				}
				
				//transfer money
				player3Money += moneyGetting;
				player2Money -= moneyGetting;
				player3Money -= moneyGiving;
				player2Money += moneyGiving;
			}
			else {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player4Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player2Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 4;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player2Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player4Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 2;
					}
				}
				
				//transfer money
				player4Money += moneyGetting;
				player2Money -= moneyGetting;
				player4Money -= moneyGiving;
				player2Money += moneyGiving;
			}
			
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else if (goRight && userArrow == 5) {
			menuState = menuMain;
			userArrow = 0;
			//CLEAR THE ARRAYS!!!!
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else {
			menuState = menuTradeP2;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goDown) {
				if (userArrow < menuTradeSize - 1) {
					userArrow++;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
		}
		break;
		
		case menuTradeP3:
		tradingWith = 3;
		if (goRight && userArrow == 0 && turn != 3) {
			menuState = menuGettingProperty;
			userArrow = 0;
			updateNokia(player3Trade, player3TradeSize);
		}
		else if (goRight && userArrow == 1 && turn != 3) {
			menuState = menuGettingMoney;
			userArrow = 0;
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 2 && turn != 3) {
			menuState = menuGivingProperty;
			userArrow = 0;
			if(turn == 1) {
				updateNokia(player1Trade, player1TradeSize);
			}
			else if (turn == 2) {
				updateNokia(player2Trade, player2TradeSize);
			}
			else if (turn == 4) {
				updateNokia(player4Trade, player4TradeSize);
			}
			else {
				
			}
		}
		else if (goRight && userArrow == 3 && turn != 3) {
			menuState = menuGivingMoney;
			userArrow = 0;
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 4 && turn != 3) {
			menuState = menuMain;
			userArrow = 0;
			//transfer the values
			//CLEAR THE ARRAYS!!!
			
			if (turn == 1) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player1Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player3Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 1;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player3Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player1Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 3;
					}
				}
				
				//transfer money
				player1Money += moneyGetting;
				player3Money -= moneyGetting;
				player1Money -= moneyGiving;
				player3Money += moneyGiving;
				
			}
			else if (turn == 2) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player2Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player3Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 2;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player3Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player2Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 3;
					}
				}
				
				//transfer money
				player2Money += moneyGetting;
				player3Money -= moneyGetting;
				player2Money -= moneyGiving;
				player3Money += moneyGiving;
			}
			else {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player4Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player3Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 4;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player3Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player4Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 3;
					}
				}
				
				//transfer money
				player4Money += moneyGetting;
				player3Money -= moneyGetting;
				player4Money -= moneyGiving;
				player3Money += moneyGiving;
			}
			
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else if (goRight && userArrow == 5) {
			menuState = menuMain;
			userArrow = 0;
			//CLEAR THE ARRAYS!!!!
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else {
			menuState = menuTradeP3;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goDown) {
				if (userArrow < menuTradeSize - 1) {
					userArrow++;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
		}
		break;
		
		case menuTradeP4:
		tradingWith = 4;
		if (goRight && userArrow == 0 && turn != 4) {
			menuState = menuGettingProperty;
			userArrow = 0;
			updateNokia(player4Trade, player4TradeSize);
		}
		else if (goRight && userArrow == 1 && turn != 4) {
			menuState = menuGettingMoney;
			userArrow = 0;
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 2 && turn != 4) {
			menuState = menuGivingProperty;
			userArrow = 0;
			if(turn == 1) {
				updateNokia(player1Trade, player1TradeSize);
			}
			else if (turn == 2) {
				updateNokia(player2Trade, player2TradeSize);
			}
			else if (turn == 3) {
				updateNokia(player3Trade, player3TradeSize);
			}
			else {
				
			}
		}
		else if (goRight && userArrow == 3 && turn != 4) {
			menuState = menuGivingMoney;
			userArrow = 0;
			//update array
			updateNokia(arrayMenuMoney, menuMoneySize);
		}
		else if (goRight && userArrow == 4 && turn != 4) {
			menuState = menuMain;
			userArrow = 0;
			//transfer the values
			//CLEAR THE ARRAYS!!!
			
			if (turn == 1) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player1Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player4Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 1;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player4Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player1Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 4;
					}
				}
				
				//transfer money
				player1Money += moneyGetting;
				player4Money -= moneyGetting;
				player1Money -= moneyGiving;
				player4Money += moneyGiving;
				
			}
			else if (turn == 2) {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player2Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player4Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 2;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player4Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player2Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 4;
					}
				}
				
				//transfer money
				player2Money += moneyGetting;
				player4Money -= moneyGetting;
				player2Money -= moneyGiving;
				player4Money += moneyGiving;
			}
			else {
				//transfer properties
				for (unsigned char i = 0; i < propertiesGettingSize; i++) {
					if (arrayPropertiesGetting[i] != 0) {
						player3Properties[arrayPropertiesGetting[i]] = propertySpots[arrayPropertiesGetting[i]];
						player4Properties[arrayPropertiesGetting[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 3;
					}
				}
				
				for (unsigned char i = 0; i < propertiesGivingSize; i++) {
					if (arrayPropertiesGiving[i] != 0) {
						player4Properties[arrayPropertiesGiving[i]] = propertySpots[arrayPropertiesGiving[i]];
						player3Properties[arrayPropertiesGiving[i]] = 0;
						propertySpots[arrayPropertiesGetting[i]]->own = 4;
					}
				}
				
				//transfer money
				player3Money += moneyGetting;
				player4Money -= moneyGetting;
				player3Money -= moneyGiving;
				player4Money += moneyGiving;
			}
			
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else if (goRight && userArrow == 5) {
			menuState = menuMain;
			userArrow = 0;
			//CLEAR THE ARRAYS!!!!
			clearTradeValues();
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else {
			menuState = menuTradeP4;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goDown) {
				if (userArrow < menuTradeSize - 1) {
					userArrow++;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
		}
		break;
		
		case menuMortgage:
		if (goRight && turn == 1) {
			if (userArrow < player1MortgageSize - 2) {
				if (arrayPropertiesMortgaging[userArrow] == 0) {
					arrayPropertiesMortgaging[userArrow] = player1Mortgage[userArrow]->number;
				}
				else {
					arrayPropertiesMortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player1MortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				doMortgage();
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else if (goRight && turn == 2) {
			if (userArrow < player2MortgageSize - 2) {
				if (arrayPropertiesMortgaging[userArrow] == 0) {
					arrayPropertiesMortgaging[userArrow] = player2Mortgage[userArrow]->number;
				}
				else {
					arrayPropertiesMortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player2MortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				doMortgage();
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else if (goRight && turn == 3) {
			if (userArrow < player3MortgageSize - 2) {
				if (arrayPropertiesMortgaging[userArrow] == 0) {
					arrayPropertiesMortgaging[userArrow] = player3Mortgage[userArrow]->number;
				}
				else {
					arrayPropertiesMortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player3MortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				doMortgage();
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else if (goRight && turn == 4) {
			if (userArrow < player4MortgageSize - 2) {
				if (arrayPropertiesMortgaging[userArrow] == 0) {
					arrayPropertiesMortgaging[userArrow] = player4Mortgage[userArrow]->number;
				}
				else {
					arrayPropertiesMortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player4MortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				doMortgage();
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				clearArray(arrayPropertiesMortgaging, propertiesMortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else {
			menuState = menuMortgage;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					if (turn == 1) {
						updateNokia(player1Mortgage, player1MortgageSize);
					}
					else if (turn == 2) {
						updateNokia(player2Mortgage, player2MortgageSize);
					}
					else if (turn == 3) {
						updateNokia(player3Mortgage, player3MortgageSize);
					}
					else {
						updateNokia(player4Mortgage, player4MortgageSize);
					}
				}
			}
			else if (goDown) {
				if (turn == 1) {
					if (userArrow < player1MortgageSize - 1) {
						userArrow++;
						updateNokia(player1Mortgage, player1MortgageSize);
					}
				}
				else if (turn == 2) {
					if (userArrow < player2MortgageSize - 1) {
						userArrow++;
						updateNokia(player2Mortgage, player2MortgageSize);
					}
				}
				else if (turn == 3) {
					if (userArrow < player3MortgageSize - 1) {
						userArrow++;
						updateNokia(player3Mortgage, player3MortgageSize);
					}
				}
				else {
					if (userArrow < player4MortgageSize - 1) {
						userArrow++;
						updateNokia(player4Mortgage, player4MortgageSize);
					}
				}
			}
			updateNokia(arrayPropertiesMortgaging, propertiesMortgagingSize);
		}
		break;
		
		case menuUnmortgage:
		if (goRight && turn == 1) {
			if (userArrow < player1UnmortgageSize - 2) {
				if (arrayPropertiesUnmortgaging[userArrow] == 0) {
					if (player1MoneyTemp + propertySpots[player1Unmortgage[userArrow]->number]->mortgage < player1Money) {
						arrayPropertiesUnmortgaging[userArrow] = player1Unmortgage[userArrow]->number;
						player1MoneyTemp += propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					}
				}
				else {
					player1MoneyTemp -= propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					arrayPropertiesUnmortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player1UnmortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				player1MoneyTemp = 0;
				doUnmortgage();
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				player1MoneyTemp = 0;
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else if (goRight && turn == 2) {
			if (userArrow < player2UnmortgageSize - 2) {
				if (arrayPropertiesUnmortgaging[userArrow] == 0) {
					if (player2MoneyTemp + propertySpots[player2Unmortgage[userArrow]->number]->mortgage < player2Money) {
						arrayPropertiesUnmortgaging[userArrow] = player2Unmortgage[userArrow]->number;
						player2MoneyTemp += propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					}
				}
				else {
					player2MoneyTemp -= propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					arrayPropertiesUnmortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player2UnmortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				player2MoneyTemp = 0;
				doUnmortgage();
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				player2MoneyTemp = 0;
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else if (goRight && turn == 3) {
			if (userArrow < player3UnmortgageSize - 2) {
				if (arrayPropertiesUnmortgaging[userArrow] == 0) {
					if (player3MoneyTemp + propertySpots[player3Unmortgage[userArrow]->number]->mortgage < player3Money) {
						arrayPropertiesUnmortgaging[userArrow] = player3Unmortgage[userArrow]->number;
						player3MoneyTemp += propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					}
				}
				else {
					player3MoneyTemp -= propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					arrayPropertiesUnmortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player3UnmortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				player3MoneyTemp = 0;
				doUnmortgage();
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				player3MoneyTemp = 0;
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else if (goRight && turn == 4) {
			if (userArrow < player4UnmortgageSize - 2) {
				if (arrayPropertiesUnmortgaging[userArrow] == 0) {
					if (player4MoneyTemp + propertySpots[player4Unmortgage[userArrow]->number]->mortgage < player4Money) {
						arrayPropertiesUnmortgaging[userArrow] = player4Unmortgage[userArrow]->number;
						player4MoneyTemp += propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					}
				}
				else {
					player4MoneyTemp -= propertySpots[arrayPropertiesUnmortgaging[userArrow]]->mortgage;
					arrayPropertiesUnmortgaging[userArrow] = 0;
				}
			}
			else if (userArrow == player4UnmortgageSize - 2) {
				//done
				menuState = menuMain;
				userArrow = 0;
				player4MoneyTemp = 0;
				doUnmortgage();
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
			else {
				//cancel
				menuState = menuMain;
				userArrow = 0;
				player4MoneyTemp = 0;
				clearArray(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
				updateNokia(arrayMenuMain, menuMainSize);
			}
		}
		else {
			menuState = menuUnmortgage;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					if (turn == 1) {
						updateNokia(player1Unmortgage, player1UnmortgageSize);
					}
					else if (turn == 2) {
						updateNokia(player2Unmortgage, player2UnmortgageSize);
					}
					else if (turn == 3) {
						updateNokia(player3Unmortgage, player3UnmortgageSize);
					}
					else {
						updateNokia(player4Unmortgage, player4UnmortgageSize);
					}
				}
			}
			else if (goDown) {
				if (turn == 1) {
					if (userArrow < player1UnmortgageSize - 1) {
						userArrow++;
						updateNokia(player1Unmortgage, player1UnmortgageSize);
					}
				}
				else if (turn == 2) {
					if (userArrow < player2UnmortgageSize - 1) {
						userArrow++;
						updateNokia(player2Unmortgage, player2UnmortgageSize);
					}
				}
				else if (turn == 3) {
					if (userArrow < player3UnmortgageSize - 1) {
						userArrow++;
						updateNokia(player3Unmortgage, player3UnmortgageSize);
					}
				}
				else {
					if (userArrow < player4UnmortgageSize - 1) {
						userArrow++;
						updateNokia(player4Unmortgage, player4UnmortgageSize);
					}
				}
			}
			updateNokia(arrayPropertiesUnmortgaging, propertiesUnmortgagingSize);
		}
		break;
		
		case menuBuild:
		break;
		
		case menuProperties:
		if((goRight &&  turn == 1 && userArrow == menuPropertiesSize1 - 1) || (goRight &&  turn == 2 && userArrow == menuPropertiesSize2 - 1) || (goRight &&  turn == 3 && userArrow == menuPropertiesSize3 - 1) || (goRight &&  turn == 4 && userArrow == menuPropertiesSize4 - 1)) {
			menuState = menuMain;
			userArrow = 0;
			updateNokia(arrayMenuMain, menuMainSize);
		}
		else {
			menuState = menuProperties;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					if (turn == 1) {
						updateNokia(arrayMenuProperties1, menuPropertiesSize1);
					}
					else if (turn == 2) {
						updateNokia(arrayMenuProperties2, menuPropertiesSize2);
					}
					else if (turn == 3) {
						updateNokia(arrayMenuProperties3, menuPropertiesSize3);
					}
					else {
						updateNokia(arrayMenuProperties4, menuPropertiesSize4);
					}
				}
			}
			else if (goDown) {
				if (turn == 1) {
					if (userArrow < menuPropertiesSize1 - 1) {
						userArrow++;
						updateNokia(arrayMenuProperties1, menuPropertiesSize1);
					}
				}
				else if (turn == 2) {
					if (userArrow < menuPropertiesSize2 - 1) {
						userArrow++;
						updateNokia(arrayMenuProperties2, menuPropertiesSize2);
					}
				}
				else if (turn == 3) {
					if (userArrow < menuPropertiesSize3 - 1) {
						userArrow++;
						updateNokia(arrayMenuProperties3, menuPropertiesSize3);
					}
				}
				else {
					if (userArrow < menuPropertiesSize4 - 1) {
						userArrow++;
						updateNokia(arrayMenuProperties4, menuPropertiesSize4);
					}
				}
			}
		}
		break;
		
		case menuGettingProperty:
		if (goRight && tradingWith == 1) {
			if (userArrow < player1TradeSize - 2) {
				if (arrayPropertiesGetting[userArrow] == 0) {
					arrayPropertiesGetting[userArrow] = player1Trade[userArrow]->number;	
				}
				else {
					arrayPropertiesGetting[userArrow] = 0;
				}
			}
			else if (userArrow == player1TradeSize - 2) {
				//done
				menuState = menuTradeP1;
				userArrow = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else {
				//cancel
				menuState = menuTradeP1;
				userArrow = 0;
				clearArray(arrayPropertiesGetting, propertiesGettingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && tradingWith == 2) {
			if (userArrow < player2TradeSize - 2) {
				if (arrayPropertiesGetting[userArrow] == 0) {
					arrayPropertiesGetting[userArrow] = player2Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGetting[userArrow] = 0;
				}
			}
			else if (userArrow == player2TradeSize - 2) {
				//done
				menuState = menuTradeP2;
				userArrow = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else {
				//cancel
				menuState = menuTradeP2;
				userArrow = 0;
				clearArray(arrayPropertiesGetting, propertiesGettingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && tradingWith == 3) {
			if (userArrow < player3TradeSize - 2) {
				if (arrayPropertiesGetting[userArrow] == 0) {
					arrayPropertiesGetting[userArrow] = player3Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGetting[userArrow] = 0;
				}
			}
			else if (userArrow == player3TradeSize - 2) {
				//done
				menuState = menuTradeP3;
				userArrow = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else {
				//cancel
				menuState = menuTradeP3;
				userArrow = 0;
				clearArray(arrayPropertiesGetting, propertiesGettingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && tradingWith == 4) {
			if (userArrow < player4TradeSize - 2) {
				if (arrayPropertiesGetting[userArrow] == 0) {
					arrayPropertiesGetting[userArrow] = player4Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGetting[userArrow] = 0;
				}
			}
			else if (userArrow == player4TradeSize - 2) {
				//done
				menuState = menuTradeP4;
				userArrow = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else {
				//cancel
				menuState = menuTradeP4;
				userArrow = 0;
				clearArray(arrayPropertiesGetting, propertiesGettingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else {
			menuState = menuGettingProperty;
			if (goUp) {
				if (userArrow > 0) {
					userArrow--;
					if (tradingWith == 1) {
						updateNokia(player1Trade, player1TradeSize);
					}
					else if (tradingWith == 2) {
						updateNokia(player2Trade, player2TradeSize);
					}
					else if (tradingWith == 3) {
						updateNokia(player3Trade, player3TradeSize);
					}
					else {
						updateNokia(player4Trade, player4TradeSize);
					}
				}
			}
			else if (goDown) {
				if (tradingWith == 1) {
					if (userArrow < player1TradeSize - 1) {
						userArrow++;
						updateNokia(player1Trade, player1TradeSize);
					}
				}
				else if (tradingWith == 2) {
					if (userArrow < player2TradeSize - 1) {
						userArrow++;
						updateNokia(player2Trade, player2TradeSize);
					}
				}
				else if (tradingWith == 3) {
					if (userArrow < player3TradeSize - 1) {
						userArrow++;
						updateNokia(player3Trade, player3TradeSize);
					}
				}
				else {
					if (userArrow < player4TradeSize - 1) {
						userArrow++;
						updateNokia(player4Trade, player4TradeSize);
					}
				}
			}
			updateNokia(arrayPropertiesGetting, propertiesGettingSize);
		}
		break;
		
		case menuGettingMoney:
		if (goRight && tradingWith == 1) {
			if (goRight && userArrow == 0) {
				menuState = menuGettingMoney;
				if ((player1MoneyTemp + 10000 < player1Money) && arrayMoneyGetting[0] < 9) {
					arrayMoneyGetting[0]++;
					player1MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGettingMoney;
				if ((player1MoneyTemp + 1000 < player1Money) && arrayMoneyGetting[1] < 9) {
					arrayMoneyGetting[1]++;
					player1MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGettingMoney;
				if ((player1MoneyTemp + 100 < player1Money) && arrayMoneyGetting[2] < 9) {
					arrayMoneyGetting[2]++;
					player1MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGettingMoney;
				if ((player1MoneyTemp + 10 < player1Money) && arrayMoneyGetting[3] < 9) {
					arrayMoneyGetting[3]++;
					player1MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGettingMoney;
				if ((player1MoneyTemp + 1 < player1Money) && arrayMoneyGetting[4] < 9) {
					arrayMoneyGetting[4]++;
					player1MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 5) {
				menuState = menuTradeP1;
				userArrow = 0;
				moneyGetting = player1MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				menuState = menuTradeP1;
				userArrow = 0;
				player1MoneyTemp = 0;
				moneyGetting = 0;
				clearArray(arrayMoneyGetting, moneyGettingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && tradingWith == 2) {
			if (goRight && userArrow == 0) {
				menuState = menuGettingMoney;
				if ((player2MoneyTemp + 10000 < player2Money) && arrayMoneyGetting[0] < 9) {
					arrayMoneyGetting[0]++;
					player2MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGettingMoney;
				if ((player2MoneyTemp + 1000 < player2Money) && arrayMoneyGetting[1] < 9) {
					arrayMoneyGetting[1]++;
					player2MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGettingMoney;
				if ((player2MoneyTemp + 100 < player2Money) && arrayMoneyGetting[2] < 9) {
					arrayMoneyGetting[2]++;
					player2MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGettingMoney;
				if ((player2MoneyTemp + 10 < player2Money) && arrayMoneyGetting[3] < 9) {
					arrayMoneyGetting[3]++;
					player2MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGettingMoney;
				if ((player2MoneyTemp + 1 < player2Money) && arrayMoneyGetting[4] < 9) {
					arrayMoneyGetting[4]++;
					player2MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 5) {
				menuState = menuTradeP2;
				userArrow = 0;
				moneyGetting = player2MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				menuState = menuTradeP2;
				userArrow = 0;
				player2MoneyTemp = 0;
				moneyGetting = 0;
				clearArray(arrayMoneyGetting, moneyGettingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && tradingWith == 3) {
			if (goRight && userArrow == 0) {
				menuState = menuGettingMoney;
				if ((player3MoneyTemp + 10000 < player3Money) && arrayMoneyGetting[0] < 9) {
					arrayMoneyGetting[0]++;
					player3MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGettingMoney;
				if ((player3MoneyTemp + 1000 < player3Money) && arrayMoneyGetting[1] < 9) {
					arrayMoneyGetting[1]++;
					player3MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGettingMoney;
				if ((player3MoneyTemp + 100 < player3Money) && arrayMoneyGetting[2] < 9) {
					arrayMoneyGetting[2]++;
					player3MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGettingMoney;
				if ((player3MoneyTemp + 10 < player3Money) && arrayMoneyGetting[3] < 9) {
					arrayMoneyGetting[3]++;
					player3MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGettingMoney;
				if ((player3MoneyTemp + 1 < player3Money) && arrayMoneyGetting[4] < 9) {
					arrayMoneyGetting[4]++;
					player3MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 5) {
				menuState = menuTradeP3;
				userArrow = 0;
				moneyGetting = player3MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				menuState = menuTradeP3;
				userArrow = 0;
				moneyGetting = 0;
				clearArray(arrayMoneyGetting, moneyGettingSize);
				player3MoneyTemp = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && tradingWith == 4) {
			if (goRight && userArrow == 0) {
				menuState = menuGettingMoney;
				if ((player4MoneyTemp + 10000 < player4Money) && arrayMoneyGetting[0] < 9) {
					arrayMoneyGetting[0]++;
					player4MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGettingMoney;
				if ((player4MoneyTemp + 1000 < player4Money) && arrayMoneyGetting[1] < 9) {
					arrayMoneyGetting[1]++;
					player4MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGettingMoney;
				if ((player4MoneyTemp + 100 < player4Money) && arrayMoneyGetting[2] < 9) {
					arrayMoneyGetting[2]++;
					player4MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGettingMoney;
				if ((player4MoneyTemp + 10 < player4Money) && arrayMoneyGetting[3] < 9) {
					arrayMoneyGetting[3]++;
					player4MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGettingMoney;
				if ((player4MoneyTemp + 1 < player4Money) && arrayMoneyGetting[4] < 9) {
					arrayMoneyGetting[4]++;
					player4MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
			else if (goRight && userArrow == 5) {
				menuState = menuTradeP4;
				userArrow = 0;
				moneyGetting = player4MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				menuState = menuTradeP4;
				userArrow = 0;
				clearArray(arrayMoneyGetting, moneyGettingSize);
				player4MoneyTemp = 0;
				moneyGetting = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goUp) {
			menuState = menuGettingMoney;
			if (userArrow > 0) {
				userArrow--;
				updateNokia(arrayMenuMoney, menuMoneySize);
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
		}
		else if (goDown) {
			menuState = menuGettingMoney;
			if (userArrow < menuMoneySize - 1) {
				userArrow++;
				updateNokia(arrayMenuMoney, menuMoneySize);
				updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
			}
		}
		else {
			menuState = menuGettingMoney;
			updateNokiaMoney(arrayMoneyGetting, moneyGettingSize);
		}
		break;
		
		case menuGivingProperty:
		if (turn == 1) {
			if (goRight && userArrow < player1TradeSize - 2) {
				if (arrayPropertiesGiving[userArrow] == 0) {
					arrayPropertiesGiving[userArrow] = player1Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGiving[userArrow] = 0;
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
			else if (goRight && userArrow == player1TradeSize - 2) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goRight && userArrow == player1TradeSize - 1) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				clearArray(arrayPropertiesGiving, propertiesGivingSize);
			}
			else {
				menuState = menuGivingProperty;
				if (goUp) {
					if (userArrow > 0) {
						userArrow--;
						updateNokia(player1Trade, player1TradeSize);
					}
				}
				else if (goDown) {
					if (userArrow < player1TradeSize - 1) {
						userArrow++;
						updateNokia(player1Trade, player1TradeSize);
					}
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
		}
		else if (turn == 2) {
			if (goRight && userArrow < player2TradeSize - 2) {
				if (arrayPropertiesGiving[userArrow] == 0) {
					arrayPropertiesGiving[userArrow] = player2Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGiving[userArrow] = 0;
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
			else if (goRight && userArrow == player2TradeSize - 2) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goRight && userArrow == player2TradeSize - 1) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				clearArray(arrayPropertiesGiving, propertiesGivingSize);
			}
			else {
				menuState = menuGivingProperty;
				if (goUp) {
					if (userArrow > 0) {
						userArrow--;
						updateNokia(player2Trade, player2TradeSize);
					}
				}
				else if (goDown) {
					if (userArrow < player2TradeSize - 1) {
						userArrow++;
						updateNokia(player2Trade, player2TradeSize);
					}
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
		}
		else if (turn == 3) {
			if (goRight && userArrow < player3TradeSize - 2) {
				if (arrayPropertiesGiving[userArrow] == 0) {
					arrayPropertiesGiving[userArrow] = player3Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGiving[userArrow] = 0;
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
			else if (goRight && userArrow == player3TradeSize - 2) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goRight && userArrow == player3TradeSize - 1) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				clearArray(arrayPropertiesGiving, propertiesGivingSize);
			}
			else {
				menuState = menuGivingProperty;
				if (goUp) {
					if (userArrow > 0) {
						userArrow--;
						updateNokia(player3Trade, player3TradeSize);
					}
				}
				else if (goDown) {
					if (userArrow < player3TradeSize - 1) {
						userArrow++;
						updateNokia(player3Trade, player3TradeSize);
					}
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
		}
		else {
			if (goRight && userArrow < player4TradeSize - 2) {
				if (arrayPropertiesGiving[userArrow] == 0) {
					arrayPropertiesGiving[userArrow] = player4Trade[userArrow]->number;
				}
				else {
					arrayPropertiesGiving[userArrow] = 0;
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
			else if (goRight && userArrow == player4TradeSize - 2) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
			}
			else if (goRight && userArrow == player4TradeSize - 1) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				else {
					menuState = menuTradeP4;
					userArrow = 0;
					updateNokia(arrayMenuTrade, menuTradeSize);
				}
				clearArray(arrayPropertiesGiving, propertiesGivingSize);
			}
			else {
				menuState = menuGivingProperty;
				if (goUp) {
					if (userArrow > 0) {
						userArrow--;
						updateNokia(player4Trade, player4TradeSize);
					}
				}
				else if (goDown) {
					if (userArrow < player4TradeSize - 1) {
						userArrow++;
						updateNokia(player4Trade, player4TradeSize);
					}
				}
				updateNokia(arrayPropertiesGiving, propertiesGivingSize);
			}
		}
		break;
		
		case menuGivingMoney:
		if (goRight && turn == 1) {
			if (goRight && userArrow == 0) {
				menuState = menuGivingMoney;
				if ((player1MoneyTemp + 10000 < player1Money) && arrayMoneyGiving[0] < 9) {
					arrayMoneyGiving[0]++;
					player1MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGivingMoney;
				if ((player1MoneyTemp + 1000 < player1Money) && arrayMoneyGiving[1] < 9) {
					arrayMoneyGiving[1]++;
					player1MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGivingMoney;
				if ((player1MoneyTemp + 100 < player1Money) && arrayMoneyGiving[2] < 9) {
					arrayMoneyGiving[2]++;
					player1MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGivingMoney;
				if ((player1MoneyTemp + 10 < player1Money) && arrayMoneyGiving[3] < 9) {
					arrayMoneyGiving[3]++;
					player1MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGivingMoney;
				if ((player1MoneyTemp + 1 < player1Money) && arrayMoneyGiving[4] < 9) {
					arrayMoneyGiving[4]++;
					player1MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 5) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				
				userArrow = 0;
				moneyGiving = player1MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				player1MoneyTemp = 0;
				moneyGiving = 0;
				clearArray(arrayMoneyGiving, moneyGivingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && turn == 2) {
			if (goRight && userArrow == 0) {
				menuState = menuGivingMoney;
				if ((player2MoneyTemp + 10000 < player2Money) && arrayMoneyGiving[0] < 9) {
					arrayMoneyGiving[0]++;
					player2MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGivingMoney;
				if ((player2MoneyTemp + 1000 < player2Money) && arrayMoneyGiving[1] < 9) {
					arrayMoneyGiving[1]++;
					player2MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGivingMoney;
				if ((player2MoneyTemp + 100 < player2Money) && arrayMoneyGiving[2] < 9) {
					arrayMoneyGiving[2]++;
					player2MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGivingMoney;
				if ((player2MoneyTemp + 10 < player2Money) && arrayMoneyGiving[3] < 9) {
					arrayMoneyGiving[3]++;
					player2MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGivingMoney;
				if ((player2MoneyTemp + 1 < player2Money) && arrayMoneyGiving[4] < 9) {
					arrayMoneyGiving[4]++;
					player2MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 5) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				moneyGiving = player2MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				player2MoneyTemp = 0;
				moneyGiving = 0;
				clearArray(arrayMoneyGiving, moneyGivingSize);
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && turn == 3) {
			if (goRight && userArrow == 0) {
				menuState = menuGivingMoney;
				if ((player3MoneyTemp + 10000 < player3Money) && arrayMoneyGiving[0] < 9) {
					arrayMoneyGiving[0]++;
					player3MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGivingMoney;
				if ((player3MoneyTemp + 1000 < player3Money) && arrayMoneyGiving[1] < 9) {
					arrayMoneyGiving[1]++;
					player3MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGivingMoney;
				if ((player3MoneyTemp + 100 < player3Money) && arrayMoneyGiving[2] < 9) {
					arrayMoneyGiving[2]++;
					player3MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGivingMoney;
				if ((player3MoneyTemp + 10 < player3Money) && arrayMoneyGiving[3] < 9) {
					arrayMoneyGiving[3]++;
					player3MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGivingMoney;
				if ((player3MoneyTemp + 1 < player3Money) && arrayMoneyGiving[4] < 9) {
					arrayMoneyGiving[4]++;
					player3MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 5) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				moneyGiving = player3MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				moneyGiving = 0;
				clearArray(arrayMoneyGiving, moneyGivingSize);
				player3MoneyTemp = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goRight && turn == 4) {
			if (goRight && userArrow == 0) {
				menuState = menuGivingMoney;
				if ((player4MoneyTemp + 10000 < player4Money) && arrayMoneyGiving[0] < 9) {
					arrayMoneyGiving[0]++;
					player4MoneyTemp += 10000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 1) {
				menuState = menuGivingMoney;
				if ((player4MoneyTemp + 1000 < player4Money) && arrayMoneyGiving[1] < 9) {
					arrayMoneyGiving[1]++;
					player4MoneyTemp += 1000;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 2) {
				menuState = menuGivingMoney;
				if ((player4MoneyTemp + 100 < player4Money) && arrayMoneyGiving[2] < 9) {
					arrayMoneyGiving[2]++;
					player4MoneyTemp += 100;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 3) {
				menuState = menuGivingMoney;
				if ((player4MoneyTemp + 10 < player4Money) && arrayMoneyGiving[3] < 9) {
					arrayMoneyGiving[3]++;
					player4MoneyTemp += 10;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 4) {
				menuState = menuGivingMoney;
				if ((player4MoneyTemp + 1 < player4Money) && arrayMoneyGiving[4] < 9) {
					arrayMoneyGiving[4]++;
					player4MoneyTemp += 1;
				}
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
			else if (goRight && userArrow == 5) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				moneyGiving = player4MoneyTemp;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
			else if (goRight && userArrow == 6) {
				if (tradingWith == 1) {
					menuState = menuTradeP1;
				}
				else if (tradingWith == 2) {
					menuState = menuTradeP2;
				}
				else if (tradingWith == 3) {
					menuState = menuTradeP3;
				}
				else {
					menuState = menuTradeP4;
				}
				userArrow = 0;
				clearArray(arrayMoneyGiving, moneyGivingSize);
				player4MoneyTemp = 0;
				moneyGiving = 0;
				updateNokia(arrayMenuTrade, menuTradeSize);
			}
		}
		else if (goUp) {
			menuState = menuGivingMoney;
			if (userArrow > 0) {
				userArrow--;
				updateNokia(arrayMenuMoney, menuMoneySize);
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
		}
		else if (goDown) {
			menuState = menuGivingMoney;
			if (userArrow < menuMoneySize - 1) {
				userArrow++;
				updateNokia(arrayMenuMoney, menuMoneySize);
				updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
			}
		}
		else {
			menuState = menuGivingMoney;
			updateNokiaMoney(arrayMoneyGiving, moneyGivingSize);
		}
		break;
		
		case menuWaitForImage:
		menuState = menuWaitForImage;
		if(USART_HasReceived(0)) {
			usartData = USART_Receive(0);
			if (usartData > 0 && usartData < 13) {
				menuState = menuMain;
				userArrow = 0;
				movePlayer(usartData, turn);
				updateNokia(arrayMenuMain, menuMainSize);
				//nokia_lcd_write_char(usartData,1);
				//nokia_lcd_render();
			}
			
		}
		USART_Flush(0);
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
	srand(time(NULL));
	TimerSet(100);
	TimerOn();
	initUSART(0);
	
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

