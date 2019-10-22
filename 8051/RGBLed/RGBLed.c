#define COMMON_CATHODE 0
#define COMMON_ANODE 1

sbit RED= P1^5;
sbit GREEN= P1^4;
sbit BLUE= P1^2;

void RGBLed_Init(int);
void SetColor(unsigned char,unsigned char,unsigned char);
void RGBLed_Stop();
void calculate_timer();

unsigned char DutyCycleFlag,status,led,Blue_flag;
double buffer;
unsigned int temp,common;

unsigned int RED_TimerLow,RED_TimerHigh,temp_red;
unsigned char RED_DutyCycle_HIGH,RED_DutyCycle_LOW;

unsigned int GREEN_TimerLow,GREEN_TimerHigh,temp_green;
unsigned char GREEN_DutyCycle_HIGH,GREEN_DutyCycle_LOW;

unsigned int BLUE_TimerLow,BLUE_TimerHigh,temp_blue;
unsigned char BLUE_DutyCycle_HIGH,BLUE_DutyCycle_LOW;

void RGBLed_Init(int pin_state)
{
	P1=0x00;
	TMOD=0x10;
	IEN0=0xA8;
	TH1=0xDB;
	TL1=0xFF;
	common=pin_state;
}

void SetColor(unsigned char red,unsigned char green,unsigned char blue)
{
	red= (100*red)/255;
	green= (100*green)/255;
	blue= (100*blue)/255; 
	
	if(common==COMMON_CATHODE)
	{
		RED_DutyCycle_HIGH=red;
		GREEN_DutyCycle_HIGH=green;
		BLUE_DutyCycle_HIGH=blue;
	}
	else
	{
		RED_DutyCycle_HIGH=(100-red);
		GREEN_DutyCycle_HIGH=(100-green);
		BLUE_DutyCycle_HIGH=(100-blue);
	}
	
	calculate_timer();
}

void RGBLed_Stop()
{
	TR1=0;
	TR2=0;
}

void et1_isr(void) interrupt 3						
{
//	led=1;
	TF1=0;
	switch(DutyCycleFlag)
	{
		case 0:
			RED=1;GREEN=1;
			if(RED_TimerHigh>GREEN_TimerHigh)
			{
				TH1=RED_TimerHigh>>8;
				TL1=RED_TimerHigh;
				status='r';
				DutyCycleFlag=1;
			}
			else if(RED_TimerHigh<GREEN_TimerHigh)
			{
				TH1=GREEN_TimerHigh>>8;
				TL1=GREEN_TimerHigh;
				status='g';	
				DutyCycleFlag=1;
			}
			else if(RED_TimerHigh==GREEN_TimerHigh)
			{
				TH1=RED_TimerHigh>>8;
				TL1=RED_TimerHigh;
				status='s';                           //similar
				DutyCycleFlag=2;
			}
			break;
		
		case 1:
			if(status=='r')
			{
				RED=0;
				GREEN=1;
			}
			else if(status=='g')
			{
				RED=1;
				GREEN=0;
			}
		
			if(temp_red>temp_green)
				temp=temp_red-temp_green;
			
			else if(temp_red<temp_green)
				temp=temp_green-temp_red;
			
			temp= 65535-temp;
			TH1= temp>>8;
			TL1= temp;
			DutyCycleFlag=2;
			break;
			
		case 2:
			if(status=='r')
			{
				GREEN=0;
				TH1= GREEN_TimerLow>>8;
				TL1= GREEN_TimerLow;
			}
			else if(status=='g')
			{
				RED=0;
				TH1= RED_TimerLow>>8;
				TL1= RED_TimerLow;
			}
			else if(status=='s')
			{
				GREEN=0;
				RED=0;
				TH1= RED_TimerLow>>8;
				TL1= RED_TimerLow;
			}
			DutyCycleFlag=0;
			break;
	}
//	led=0;
}

void et2_isr(void) interrupt 5						
{
	TF2=0;
	switch(Blue_flag)
	{
		case 0:
			BLUE=1;
			TH2=BLUE_TimerHigh>>8;
			TL2=BLUE_TimerHigh;
			Blue_flag=1;
			break;
		
		case 1:
			BLUE=0;
			TH2=BLUE_TimerLow>>8;
			TL2=BLUE_TimerLow;
			Blue_flag=0;
			break;
	}
}

void calculate_timer()
{
	RED_DutyCycle_LOW= 100- RED_DutyCycle_HIGH;
	buffer= ((double)RED_DutyCycle_HIGH*20*10000)/5425;
	temp_red=(unsigned int)buffer;
	RED_TimerHigh= 65535- (unsigned int)buffer;	
	buffer= ((double)RED_DutyCycle_LOW*20*10000)/5425;
	RED_TimerLow= 65535- (unsigned int)buffer;
	
	GREEN_DutyCycle_LOW= 100- GREEN_DutyCycle_HIGH;
	buffer= ((double)GREEN_DutyCycle_HIGH*20*10000)/5425;
	temp_green=(unsigned int)buffer;
	GREEN_TimerHigh= 65535- (unsigned int)buffer;	
	buffer= ((double)GREEN_DutyCycle_LOW*20*10000)/5425;
	GREEN_TimerLow= 65535- (unsigned int)buffer;
	
	BLUE_DutyCycle_LOW= 100- BLUE_DutyCycle_HIGH;
	buffer= ((double)BLUE_DutyCycle_HIGH*20*10000)/5425;
	temp_blue=(unsigned int)buffer;
	BLUE_TimerHigh= 65535- (unsigned int)buffer;	
	buffer= ((double)BLUE_DutyCycle_LOW*20*10000)/5425;
	BLUE_TimerLow= 65535- (unsigned int)buffer;
	
	TR1=1;
	TR2=1;
}
