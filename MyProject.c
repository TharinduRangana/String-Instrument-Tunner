//Sourcecode for Smart Tuning Robot

unsigned short kp; //store KeyPad Key Click value

const dl=30;       //Use for constant time delays
const dl2=60;
const dl3=100;

// Keypad module connections
unsigned short keypadPort at PORTA;
// End of keypad module connections

//--------LCD module connections---------------
// LCD Pin out settings
sbit LCD_RS at RD4_bit;
sbit LCD_EN at RD5_bit;
sbit LCD_D7 at RD3_bit;
sbit LCD_D6 at RD2_bit;
sbit LCD_D5 at RD1_bit;
sbit LCD_D4 at RD0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISD4_bit;
sbit LCD_EN_Direction at TRISD5_bit;
sbit LCD_D7_Direction at TRISD3_bit;
sbit LCD_D6_Direction at TRISD2_bit;
sbit LCD_D5_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD0_bit;
//--------End LCD module connections------------

//use flash memory to store the string constants
char* codetxt_to_ramtxt(const char* ctxt)
{
    static char txt[20];
    char i;

    for(i =0; txt[i] = ctxt[i]; i++);
    return txt;
}

//Get Keypad Key Click
keyclick()
{
    kp=0;
    while(!kp)
    {
        kp = Keypad_Key_Click();
        delay_ms(dl);
    }
    return kp;
}

//Show Main menu in LCD Display & Get User Selection
void menu()
{
    Lcd_Cmd(_LCD_CLEAR);
    LCD_Out(1, 1,codetxt_to_ramtxt("  SELECT METHOD"));
    LCD_Out(2, 1,codetxt_to_ramtxt("1)Frequency Detect"));
    LCD_Out(3, 1,codetxt_to_ramtxt("2)Guitar Tune"));
    LCD_Out(4, 1,codetxt_to_ramtxt("3)Other"));
    delay_ms(dl);
    keyclick();
}

//Rotate Stepper Motor Clockwise
void stepr_clkwise()
{
    unsigned short cnt; // Loop variable
    TRISB = 0x00;       // Configure PORTB as output
    PORTB = 0x00;       // Initial PORTB value

    for(cnt=0; cnt<20; cnt++)
    {
        portb=0b00000011;
        delay_ms(dl);
        portb=0b00000110;
        delay_ms(dl);
        portb=0b00001100;
        delay_ms(dl);
        portb=0b00001001;
        delay_ms(dl);
    }
}

//Rotate Stepper Motor Anticlockwise
void stepr_anticlkwise()
{
    unsigned short cnt;

    TRISB=0;        // Configure PORTB as output
    PORTB=0x00;     // Initial PORTB value

    for(cnt=0; cnt<20; cnt++)
    {
        portb=0b00001001;
        delay_ms(dl);
        portb=0b00001100;
        delay_ms(dl);
        portb=0b00000110;
        delay_ms(dl);
        portb=0b00000011;
        delay_ms(dl);
    }
}

//Detect Frequency of the Sound
freqout()
{
    unsigned int c=0,i=0,z=0;
    short cnt;

    for(cnt=0; cnt<500; cnt++)
    {
        TRISE=0;        // Configure PORTE as output
        TRISC=1;        // Configure PORTC as Input
        TRISB=0;        // Configure PORTB as output
        PORTB=0x00;     // Initial PORTB value
        TRISD=0;        // Configure PORTD as output
        PORTD=0;        // Initial PORTD value
        TMR1L=0;        //Resets the timer values low byte
        TMR1H=0;        //Resets the timer values high byte
        delay_ms(dl);
        T1CON= 0B00000011;  //starts the timer
        delay_ms(1000);
        T1CON= 0B00000010;  //stop the timer
        c= TMR1L;
        i= TMR1H * 256;
        z= (c+i)/3+1;
        return z;           //Return frequency of the sound
    }
}

//Show Sub menu in LCD Display & Get User Selection
void submenu()
{
    unsigned short kp;      //store KeyPad Key Click value
    unsigned int frq;       //store sound frequency value
    char txt2[8];

    Lcd_Cmd(_LCD_CLEAR);    // Clear display
    LCD_Out(1, 1,codetxt_to_ramtxt("  Tunning Method"));
    LCD_Out(2, 1,codetxt_to_ramtxt("1)E,B,G,D,A,E"));
    LCD_Out(3, 1,codetxt_to_ramtxt("2)E#,B#,G#,D#,A#,E#"));
    LCD_Out(4, 1,codetxt_to_ramtxt("3)Eb,Bb,Gb,Db,Ab,Eb"));
    delay_ms(dl);
    kp=keyclick();
    delay_ms(dl);

    if(kp==1)       // Tune 1st(E1st) String in Guitar
    {
        Lcd_Cmd(_LCD_CLEAR);    // Clear display
        LCD_Out(1, 1,codetxt_to_ramtxt("Tunning E1st..."));
        delay_ms(dl);

        while(1)
        {
            frq=freqout();
            InttoStr(frq,txt2);
            delay_ms(dl);
            LCD_Out(2, 1,codetxt_to_ramtxt("Frequency(Hz):"));
            LCD_Out(2, 15,txt2);
            delay_ms(dl);

            if(frq>360)
            {
                stepr_clkwise();
                LCD_Out(3,1,codetxt_to_ramtxt("stepper clockwise"));
                delay_ms(1000);
            }

            else if(frq<300)
            {
                stepr_anticlkwise();
                LCD_Out(3, 1,codetxt_to_ramtxt("stepper antclkwis"));
                delay_ms(1000);
            }

            else
            {
                Lcd_Cmd(_LCD_CLEAR);
                LCD_Out(1, 1,codetxt_to_ramtxt("E1st successful"));
                delay_ms(100);
                break;
            }
        }

// Tune 2nd(B2nd) String in Guitar
        Lcd_Cmd(_LCD_CLEAR);        // Clear display
        LCD_Out(1, 1,codetxt_to_ramtxt("Tunning B2nd..."));
        delay_ms(dl);

        while(1)
        {
            frq=freqout();
            InttoStr(frq,txt2);
            delay_ms(dl);
            LCD_Out(2, 1,codetxt_to_ramtxt("Frequency(Hz):"));
            LCD_Out(2, 15,txt2);
            delay_ms(dl);

            if(frq>270)
            {
                stepr_clkwise();
                LCD_Out(3,1,codetxt_to_ramtxt("stepper clockwise"));
                delay_ms(1000);
            }

            else if(frq<210)
            {
                stepr_anticlkwise();
                LCD_Out(3, 1,codetxt_to_ramtxt("stepper antclkwis"));
                delay_ms(1000);
            }

            else
            {
                Lcd_Cmd(_LCD_CLEAR);
                LCD_Out(1, 1,codetxt_to_ramtxt("B2nd successful"));
                delay_ms(100);
            }
            break;
        }

        // Tune 3rd(G3rd) String in Guitar
        Lcd_Cmd(_LCD_CLEAR);        // Clear display
        LCD_Out(1, 1,codetxt_to_ramtxt("Tunning G3rd..."));
        delay_ms(dl);

        while(1)
        {
            frq=freqout();
            InttoStr(frq,txt2);
            delay_ms(dl);
            LCD_Out(2, 1,codetxt_to_ramtxt("Frequency(Hz):"));
            LCD_Out(2, 15,txt2);
            delay_ms(dl);

            if(frq>220)
            {
                stepr_clkwise();
                LCD_Out(3,1,codetxt_to_ramtxt("stepper clockwise"));
                delay_ms(1000);
            }

            else if(frq<170)
            {
                stepr_anticlkwise();
                LCD_Out(3, 1,codetxt_to_ramtxt("stepper antclkwis"));
                delay_ms(1000);
            }

            else
            {
                Lcd_Cmd(_LCD_CLEAR);
                LCD_Out(1, 1,codetxt_to_ramtxt("G3rd successful"));
                delay_ms(100);
            }
            break;
        }

        //D4th
        // Tune 4th(D4th) String in Guitar
        Lcd_Cmd(_LCD_CLEAR);        // Clear display
        LCD_Out(1, 1,codetxt_to_ramtxt("Tunning D4th..."));
        delay_ms(dl);

        while(1)
        {
            frq=freqout();
            InttoStr(frq,txt2);
            delay_ms(dl);
            LCD_Out(2, 1,codetxt_to_ramtxt("Frequency(Hz):"));
            LCD_Out(2, 15,txt2);
            delay_ms(dl);

            if(frq>170)
            {
                stepr_clkwise();
                LCD_Out(3,1,codetxt_to_ramtxt("stepper clockwise"));
                delay_ms(1000);
            }

            else if(frq<120)
            {
                stepr_anticlkwise();
                LCD_Out(3, 1,codetxt_to_ramtxt("stepper antclkwis"));
                delay_ms(1000);
            }

            else
            {
                Lcd_Cmd(_LCD_CLEAR);
                LCD_Out(1, 1,codetxt_to_ramtxt("D4th successful"));
                delay_ms(100);
            }
            break;
        }
    }
}

//Detect & Display Frequency of the sound Simultaneously(In menu selection 1)
void freqdetct()
{
    unsigned int c=0,i=0,z=0;
    char txt[5];
    while(1)
    {
        Lcd_Cmd(_LCD_CLEAR);
        delay_ms(dl);
        delay_ms(dl);
        TRISE=0;                // Configure PORTE as output
        TRISC=1;                // Configure PORTC as Input
        TRISB=0;                // Configure PORTB as output
        PORTB=0x00;         // Initial PORTB value
        TRISD=0;                // Configure PORTD as output
        PORTD=0;               // Initial PORTD value
        TMR1L=0;               //Resets the timer values low byte
        TMR1H=0;               //Resets the timer values High byte
        lcd_out(1,1,codetxt_to_ramtxt("FREQUENCY IS:"));
        delay_ms(dl);
        T1CON= 0B00000011;          //starts the timer
        delay_ms(1000);
        T1CON= 0B00000010;          //stop the timer
        c= TMR1L;
        i= TMR1H * 256;
        z= (c+i)/3+1;
        IntToStr(z,txt);
        delay_ms(dl);
        lcd_out(2,0,txt);
        lcd_out(2,7,codetxt_to_ramtxt("Hz"));
        delay_ms(500);
    }
}

void main()
{

    ANSEL = 0;           // Configure AN pins as digital I/O
    ANSELH = 0;
    delay_ms(dl);
    Lcd_Init();         // Initialize LCD
    delay_ms(dl);
    Keypad_Init();      // Initialize Keypad
    delay_ms(dl);
    lcd_cmd(_LCD_CURSOR_OFF);

    menu();                  //Call menu Function

    while(1)
    {
        delay_ms(dl);
        keyclick();          //Call keyclick Function

        if(kp==1)
        {
            freqdetct();    //Call Frequency Detect Function
        }

        else if(kp==2)
        {
            submenu();      //Call Sub menu Function
        }

        else if(kp==3)      //Select Other Option in menu
        {
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1,codetxt_to_ramtxt("Other"));
            delay_ms(dl);
        }

    }
}
