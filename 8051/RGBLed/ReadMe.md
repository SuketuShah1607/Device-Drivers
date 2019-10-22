Functions-

a) void RGBLed_Init(mode) 

  mode: COMMOM_ANODE or COMMON_CATHODE

  This fuction is used to initialize RGBLed in common anode mode or in common cathode. By default Red pin is connected to P1_5, Green to P1_4 and Blue to P1_2. This library uses timer1 and timer 2 for generation of PWM pulses. 
  
b) void SetColor(RED,GREEN<BLUE)

  This function accepts 3 arguments, that is intensity of each pin(red,green,blue) in range 0-255.It turns ON RGB Led
  
c) void void RGBLed_Stop()

  Turns off RGB Led 
