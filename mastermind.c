#include <mega32.h>
#include <stdio.h>
#include <stdlib.h>
#include <alcd.h>
#include <delay.h>
#include <stdbool.h>


#define DIGIT 4
bool debug_flag = true;  //*

//-----global variables-----
//LED variables
static int led_arr[8] = {0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00000000, 0b00000000, 0b00000000, 0b00000000};
 
static int happy[8] = {0b00000000, 0b01100110, 0b01100110, 0b00000000,
 0b10000001, 0b01000010, 0b00111100, 0b00000000};  
 
static int sad[8] = {0b00000000, 0b01100110, 0b01100110, 0b00000000,
 0b00111100, 0b01000010, 0b10000001, 0b00000000};

static int apathetic[8] = { 0b00000000, 0b01100110, 0b01100110, 0b00000000,
 0b00000000, 0b11111111, 0b00000000, 0b00000000};
  
int row[4]={0xfe,0xfd,0xfb,0xf7};

//LCD variables
char buffer[16];

//Game variables
int level = 0;
int display_level = 0;
int num = 0;
int guess[8][4];
int goal[9] = { -1, -1, -1, -1, -1, -1, -1, -1 };
int black = 0;
int white = 0;
int tmp;
int random_counter = 0;

//-----functions-----
//LED functions
void led_set_level(int black, int white);
void led_show();
void led_clear();
void led_win();
void led_lose();
void led_warn();

//Game functions
void random_generator();
void check();
void clear_goal();
void show_goal();

//Button functions
void submit();
void next();
void prev();
void rematch();
void welcome();

//LCD functions
void welcome();

void main(void)
{
    int i=0;
    // Input/Output Ports initialization
    // Port A initialization
    // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
    // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTA=0x00;
    DDRA=0x00;

    // Port B initialization
    // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
    // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTB=0x00;
    DDRB=0x00;

    // Port C initialization
    // Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
    // State7=T State6=T State5=T State4=T State3=0 State2=0 State1=0 State0=0 
    PORTC=0xff;
    DDRC=0x0F;

    // Port D initialization
    // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
    // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTD=0x00;
    DDRD=0x00;

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: Timer 0 Stopped
    // Mode: Normal top=0xFF
    // OC0 output: Disconnected
    TCCR0=0x00;
    TCNT0=0x00;
    OCR0=0x00;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: Timer1 Stopped
    // Mode: Normal top=0xFFFF
    // OC1A output: Discon.
    // OC1B output: Discon.
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=0x00;
    TCCR1B=0x00;
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: Timer2 Stopped
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    ASSR=0x00;
    TCCR2=0x00;
    TCNT2=0x00;
    OCR2=0x00;

    // External Interrupt(s) initialization
    // INT0: Off
    // INT1: Off
    // INT2: Off
    MCUCR=0x00;
    MCUCSR=0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=0x00;

    // USART initialization
    // USART disabled
    UCSRB=0x00;

    // Analog Comparator initialization
    // Analog Comparator: Off
    // Analog Comparator Input Capture by Timer/Counter 1: Off
    ACSR=0x80;
    SFIOR=0x00;

    // ADC initialization
    // ADC disabled
    ADCSRA=0x00;

    // SPI initialization
    // SPI disabled
    SPCR=0x00;

    // TWI initialization
    // TWI disabled
    TWCR=0x00;

    // RS - PORTA Bit 0
    // RD - PORTA Bit 1
    // EN - PORTA Bit 2
    // D4 - PORTA Bit 4
    // D5 - PORTA Bit 5
    // D6 - PORTA Bit 6
    // D7 - PORTA Bit 7
    // Characters/line: 16
    lcd_init(16);
    welcome();
    while (1)
          {
             led_show();
            // if(debug_flag){
              //  debug_flag = false; 
                //show_goal();
               // delay_ms(1000);
                //lcd_clear();
             //}
             for(i=0;i<4;i++)
            {
                PORTC=row[i];
                if(i==0)
                {
                    if(display_level == level){
                        if(PINC.4==0) //7
                        {
                            while(PINC.4==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('7');
                                guess[level][num] = 7;
                                num++;
                            }
                            
                        }
                        if(PINC.5==0) //8
                        {
                            while(PINC.5==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('8');
                                guess[level][num] = 8;
                                num++;
                            }
                        }
                        if(PINC.6==0) //9
                        {
                            while(PINC.6==0)
                            {}
                        }
                    }

                    if(PINC.7==0) // /: guide
                    {
                        while(PINC.7==0)
                        {}
                    }     
                }
                else
                {
                if(i==1)
                {  
                    if(display_level == level){
                        if(PINC.4==0) //4
                        {
                            while(PINC.4==0)
                            {} 
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('4');
                                guess[level][num] = 4;
                                num++;
                            }
                        }
                        if(PINC.5==0) //5
                        {
                            while(PINC.5==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('5');
                                guess[level][num] = 5;
                                num++;
                            }
                        }
                        if(PINC.6==0) //6
                        {
                            while(PINC.6==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('6');
                                guess[level][num] = 6;
                                num++;
                            }
                        }
                    }

                    if(PINC.7==0) // *: rematch
                    {
                        while(PINC.7==0)
                        {}
                        rematch();
                    }     
                }
                else
                {
                if(i==2)
                { 
                    if(display_level == level){
                        if(PINC.4==0) //1
                        {
                            while(PINC.4==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('1');
                                guess[level][num] = 1;
                                num++;
                            }
                        }
                        if(PINC.5==0) //2
                        {
                            while(PINC.5==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('2');
                                guess[level][num] = 2;
                                num++;
                            }
                        }
                        if(PINC.6==0) //3
                        {
                            while(PINC.6==0)
                            {}
                            if(num == DIGIT){   
                                led_warn();
                            }
                            else{
                                lcd_putchar('3');
                                guess[level][num] = 3;
                                num++;
                            }
                        }
                    }

                    if(PINC.7==0) //prev
                    {
                        while(PINC.7==0)
                        {}
                        prev();
                    }     
                }
                else
                {
                if(i==3)
                {
                    if(PINC.4==0) //clear
                    {
                        while(PINC.4==0)
                        {}
                        display_level = level;
                        num = 0;
                        lcd_clear();
                    }
                    if(PINC.5==0) //0
                    {
                        while(PINC.5==0)
                        {}
                    }
                    if(PINC.6==0) //submit
                    {
                        while(PINC.6==0)
                        {}
                        submit();
                    }
                    if(PINC.7==0) //next
                    {
                        while(PINC.7==0)
                        {}
                        next();
                    }     
                }
               }
              }
             }    
            }
          }
}

int array_to_number(int a[]){
    int i, k = 0;
    for (i = 0; i < DIGIT; i++)
        k = 10 * k + a[i];
    return k;                                                     
}

void submit()
{  
   int input;
   check();
   level++;  
   display_level++;
   num = 0; 
   lcd_clear();
   lcd_gotoxy(13,0);
   input = level + 1; 
   sprintf(buffer,"%d",input);
   lcd_puts(buffer);
   lcd_puts("/8");
   lcd_gotoxy(0,0);
}

void next(){
   if(display_level < level){
      int input;
      display_level ++;
      if(display_level == level){
         lcd_clear(); 
      }
      else{
          input = array_to_number(guess[display_level]);    
          lcd_clear();  
          sprintf(buffer,"%d", input);
          lcd_puts(buffer);
      }
   }
}

void prev(){
   if(display_level > 0){   
      int input;
      display_level --;
      input = array_to_number(guess[display_level]); 
      lcd_clear(); 
      sprintf(buffer,"%d",input);
      lcd_puts(buffer);
   }
}

void rematch(){
    lcd_clear();
    lcd_puts("  3.. ");
    delay_ms(500);
    lcd_puts("2.. ");
    delay_ms(500);
    lcd_puts("1..  ");
    delay_ms(500);
    lcd_gotoxy(0,1);
    lcd_puts("     Start!     ");
    delay_ms(500);
        
    level = 0;
    display_level = 0;
    lcd_clear(); 
    clear_goal();
    led_clear();
    
    random_generator();
    
    lcd_gotoxy(13,0);
    lcd_puts("1/8");
    lcd_gotoxy(0,0);
}

void welcome(){
    int i;
    lcd_puts("   MASTERMIND");
    lcd_gotoxy(0,1);
    for(i = 0; i < 5; i++){
        lcd_gotoxy(0,1);
        lcd_puts(" *   *   *   *  ");
        delay_ms(125);
        lcd_gotoxy(0,1);
        lcd_puts(" * * * * * * * *");
        delay_ms(125);
    }
   // delay_ms(1000);
    lcd_clear(); 
    lcd_puts("=:submit  +:next");
    lcd_gotoxy(0,1);
    lcd_puts("*:rematch -:prev");
    delay_ms(2000);  
    rematch();
}

void clear_goal(){
    char i;
    for(i = 1; i < 9 ;i++){
        goal[i] = -1;
    }
}

void show_goal(){
    int goal_arr[4] = {0};
    int i;
    for(i = 1; i <= 8; i++){
        if(goal[i] > -1){
            goal_arr[goal[i]] = i;  
        }
    } 
    
}

void check()
{   int j;
    black =0;
    white = 0;
    for (j = 0; j < DIGIT; j++){
        tmp = guess[level][j];
        if(goal[tmp] > -1) {      //goal = -1,3,-1,-1,1,0,-1,2,-1, 
            if (goal[tmp] == j)
                black++;                     
            else{ 
              white++;
            }
        }
    } 
    if(black == 4)
        led_win();
    else if(level == 7){
        lcd_clear();
        show_goal();
        led_lose(); 
    }
    else
        led_set_level(black, white);
}

void random_generator()
{   
    int i;
    int lookup_table[24][4] = { {1,3,5,7}, {8,3,4,5}, {5,6,7,8}, {3,6,7,2}, 
                                {2,8,7,5}, {7,5,1,4}, {4,8,3,5}, {6,7,4,5},
                                {3,4,1,7}, {1,5,6,3}, {7,3,8,4}, {2,7,5,3},
                                {4,2,7,5}, {8,7,4,6}, {6,1,2,3}, {5,3,4,7},
                                {5,1,6,4}, {3,2,7,8}, {8,5,1,2}, {7,2,5,8},
                                {6,3,1,7}, {1,7,4,5}, {2,4,1,6}, {4,1,6,8} }; 
    
    int arr[4] = {1,2,3,4};
    for (i = 0; i < DIGIT; i++){
        arr[i] = lookup_table[random_counter][i]; 
    }
    random_counter++;
    if(random_counter == 24)
        random_counter = 0;
    
    for (i = 0; i < DIGIT; i++) 
    {
        goal[arr[i]] = i;            
    }
    /*
    int array[8];  
    int i;
    srand(2);
    for (i = 1; i <= 8; i++) {  // fill array
        array[i-1] = i;       //array = 1, 2, 3, 4, 5, 6, 7, 8
    }

    for (i = 0; i < 8; i++) {  // shuffle array
        int temp = array[i];
        int randomIndex = rand() % 8;

        array[i]           = array[randomIndex];
        array[randomIndex] = temp;
    }

                                          // 5, 4, 7, 1, 3, 2, 8, 6
    for (i = 0; i < DIGIT; i++) { // set goal
        goal[array[i]] = i;               //goal = -1,3,-1,-1,1,0,-1,2,-1,
    }   
    */
}

void led_set_level(int black, int white){
    char PORT[8] = {128, 64, 32, 16, 8, 4, 2, 1};
     int i;
     for(i = 0; i < black; i++){
         led_arr[level] += PORT[i]; 
     }
     for(i = 0; i < white; i++){
         led_arr[level] += PORT[i + 4]; 
     }
}

void led_show(){
    int n = 0;
    char PORT[8] = {1,2,4,8,16,32,64,128};
    
    DDRB = 0xFF;
    DDRD = 0xFF;
     
    for(n = 0; n < 20; n++){ 
        int j;
        for(j = 0; j < 8; j++){
            PORTD = PORT[j];
            PORTB = ~led_arr[j];
            delay_us(500);
        }            
        PORTD = 0x00;  
    }                  
}

void led_clear(){
     int i;
     for(i=0; i<8;i++){
        led_arr[i] = 0b00000000 ;
     }
}

void led_win()
{
     char PORT[8] = {1,2,4,8,16,32,64,128};
     
     DDRB = 0xFF;
     DDRD = 0xFF;
             
     while (1){ 
        int n = 0;
        for(n = 0; n < 200; n++){ 
            int j;
            for(j = 0; j < 8; j++){
                PORTD = PORT[j];
                PORTB = ~happy[j];
                delay_us(2000);
            }            
            PORTD = 0x00;  
        }
        break;
     }
     rematch();
}

void led_lose()
{
     char PORT[8] = {1,2,4,8,16,32,64,128};
     
     DDRB = 0xFF;
     DDRD = 0xFF;
          
     while (1){ 
        int n = 0;
        for(n = 0; n < 200; n++){ 
            int j;
            for(j = 0; j < 8; j++){
                PORTD = PORT[j];
                PORTB = ~sad[j];
                delay_us(2000);
            }            
            PORTD = 0x00;  
        } 
        break;
     }
     rematch();
}

void led_warn()
{
     char PORT[8] = {1,2,4,8,16,32,64,128};
     
     DDRB = 0xFF;
     DDRD = 0xFF;
          
     while (1){ 
        int n = 0;
        for(n = 0; n < 200; n++){ 
            int j;
            for(j = 0; j < 8; j++){
                PORTD = PORT[j];
                PORTB = ~apathetic[j];
                delay_us(300);
            }            
            PORTD = 0x00;  
        } 
        break;
     }
}