#include <REGX52.H>

enum 
{
	row1=0,
	row2=64,
	row3=20,
	row4=84,
}LCD_row;

int i,j;

void LCD_CMND(char);
void LCD_DATA(char);
void wait_for_BusySequence();
void CustomCharLCD();
void DispCursorAtPosition(int ,int);
void LCD_Init();

sbit Enable=P2^7;
sbit RW=P2^6;
sbit RS=P2^5;

//unsigned int date[10]= {0,8,0,3,2,0,2,0};

void main()
{
	Enable=0;
	
	CustomCharLCD();					//Custom character initialisation before LCD_Init
	LCD_Init();
	
	LCD_DATA('A');	
	wait_for_BusySequence();
	LCD_DATA('B');
	wait_for_BusySequence();	
	LCD_DATA('C');	
	wait_for_BusySequence();
	LCD_DATA('0');
	wait_for_BusySequence();
	
//	for(j=0;j<8;j++)
//	{
//		LCD_DATA(date[j]+0x30);	
//		wait_for_BusySequence();
//	}

	LCD_DATA(0);
	wait_for_BusySequence();
	LCD_DATA(1);
	wait_for_BusySequence();
	DispCursorAtPosition(row1,4);  //column no 0 to 19	
	
	while(1)
	{}
}

void LCD_CMND(char command)
{
	RS=0;
	RW=0;
	P3= command;
	Enable=1;
	for(i=0;i<100;i++){}
	Enable=0;
}

void LCD_DATA(char Data)
{
	RS=1;
	RW=0;
	P3=Data;
	Enable=1;
	for(i=0;i<100;i++){}
	Enable=0;
}

void LCD_Init()
{
	LCD_CMND(0x01);							//clear display
	wait_for_BusySequence();
	LCD_CMND(0x02);							//cursor at home
	wait_for_BusySequence();
	LCD_CMND(0x38);							//Function Set
	wait_for_BusySequence();
	LCD_CMND(0x0E);							//Display ON-OFF control
	wait_for_BusySequence();
}

void wait_for_BusySequence()
{
	char buf;
	RS=0;
	RW=1;
	Enable=1;
	for(i=0;i<100;i++){}
	Enable=0;
	buf=P3>>7;
	while(buf)
	{
		Enable=1;
		for(i=0;i<100;i++){}
		Enable=0;
		buf=P3>>7;
	}
}

void CustomCharLCD()
{
	//custom character at CGRAM location 0
	LCD_CMND(0x40);
	wait_for_BusySequence();
	LCD_DATA(0x00);
	wait_for_BusySequence();
	LCD_DATA(0x00);
	wait_for_BusySequence();
	LCD_DATA(0x00);
  wait_for_BusySequence();
	LCD_DATA(0x00);
	wait_for_BusySequence();
	LCD_DATA(0x04);
	wait_for_BusySequence();
	LCD_DATA(0x0E);
	wait_for_BusySequence();
	LCD_DATA(0x1F);
	wait_for_BusySequence();
	
		//custom character at CGRAM location 1
	LCD_CMND(0x48);
	wait_for_BusySequence();
	LCD_DATA(0x00);
	wait_for_BusySequence();
	LCD_DATA(0x02);
	wait_for_BusySequence();
	LCD_DATA(0x02);
  wait_for_BusySequence();
	LCD_DATA(0x02);
	wait_for_BusySequence();
	LCD_DATA(0x0A);
	wait_for_BusySequence();
	LCD_DATA(0x1E);
	wait_for_BusySequence();
	LCD_DATA(0x08);
	wait_for_BusySequence();
}

void DispCursorAtPosition(int RowNo, int ColNo)
{
	char position;
	position= (char)RowNo + (char)ColNo;
	LCD_CMND(0x80 | position);
	wait_for_BusySequence();
}