sbit LCD_RS at RC2_bit;
sbit LCD_EN at RC3_bit;
sbit LCD_D7 at RC7_bit;
sbit LCD_D6 at RC6_bit;
sbit LCD_D5 at RC5_bit;
sbit LCD_D4 at RC4_bit;
sbit LCD_RS_Direction at TRISC2_bit;
sbit LCD_EN_Direction at TRISC3_bit;
sbit LCD_D7_Direction at TRISC7_bit;
sbit LCD_D6_Direction at TRISC6_bit;
sbit LCD_D5_Direction at TRISC5_bit;
sbit LCD_D4_Direction at TRISC4_bit;
char  keypadPort at PORTB;
unsigned short kp,cnt,state = 0;
char *Password="4567";
char Entered[4]={0};
char txt[7];
char flag=0;
int time,counter=0,i=0;
void initialize_Interrupt(void){
  TMR0IE_bit=1; // Enable timer 0 interrupt
  GIE_bit=1; //Enable Global Interrupt
  INTE_bit=1; //Enable ext interrupts
  T0CS_bit=0; // Select f/4 clock for the TMR0
  PSA_bit=0; // Prescaler is assigned to the Timer0 module
  PS0_bit=0; // Set pre-scaler to 8
  PS1_bit=1; // PS2,PS1,PS0 = 010
  PS2_bit=0; //
  TMR0=6; //counter starting value
}
void main() {
   TRISD.B0=0; // This pin is set as output
   PORTD.B0=0;//turn off the motor
   cnt = 0;                                 // Reset counter
   Keypad_Init();                           // Initialize Keypad
   Lcd_Init();                              // Initialize LCD
   Lcd_Cmd(_LCD_CLEAR);                     // Clear display
   Lcd_Cmd(_LCD_CURSOR_OFF);                // Cursor off
   Lcd_Out(1,1,"Enter Password");
   Lcd_Out(2,1,"");
  do {
    kp = 0;                                // Reset key code variable
    do
      kp = Keypad_Key_Click();             // Store key code in kp variable
    while (!kp);
   // Prepare value for output, transform key to it's ASCII value
    switch (kp) {
      case  1: kp = '1'; break;
      case  2: kp = '2'; break; 
      case  3: kp = '3'; break; 
      case  4: kp = 'A'; break; 
      case  5: kp = '4'; break; 
      case  6: kp = '5'; break; 
      case  7: kp = '6'; break; 
      case  8: kp = 'B'; break; 
      case  9: kp = '7'; break; 
      case 10: kp = '8'; break; 
      case 11: kp = '9'; break; 
      case 12: kp = 'C'; break; 
      case 13: kp = '*'; break; 
      case 14: kp = '0'; break; 
      case 15: kp = '#'; break; 
      case 16: kp = 'D'; break; 
    }
    Lcd_Chr_CP(kp);
    Entered[i++]=kp;
    if(i==4)
    {
    int j;
    for( j=0;j<4;j++)
    {
    if(Entered[j]!=Password[j])
    {
    state=1;
    break;
    }
    }
    if(state!=1)
    {
    state=2;
    }
    if(state==1)
    {
    Lcd_Out(2,1,"Wrong");
    Delay_ms(4000);
    Lcd_Out(2,1,"     ");
    Lcd_Out(2,1,"");
    i=0;
    state=0;
    }
    else if(state==2)
    {
      counter=0;
      time=0;
      Lcd_Out(2,1,"Right");
      Delay_ms(1000);
      Lcd_Cmd(_LCD_CLEAR);
      initialize_Interrupt();
  
      PORTD.B0=1;
      Lcd_Out(1,1,"Motor On Since:");
      while(time<4)
      {
      if(counter==500){ // check if the counter reaches 125
        counter=0; // start counter from the beginning
        time=time++; // increase time one second
        IntToStrWithZeros(time, txt); // convert the number of seconds to string
        Lcd_Out(2,1,txt); // display the number of seconds
      }
      }
      PORTD.B0=0;
      strcpy(txt,"000000");
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,"Enter Password");
      Lcd_Out(2,1,"");
      i=0;
      state=0;
      time=0;
      counter=0;
    }
    }
  } while (1);
}
void interrupt() { // Interrupt handler
  if(INTF_bit==1){ //EXT int flag
    flag=1;
    INTF_bit=0;                //reset the int flag

  }
  if (INTCON.TMR0IF==1) { // check for timer 0 interrupt flag
  counter++; // increment 1 every interrupt
  INTCON.TMR0IF=0; // reset the TMR0IF flag
  TMR0=6; // store 6 in the TMR0 register
  }

}