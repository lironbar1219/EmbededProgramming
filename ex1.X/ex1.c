#include <xc.h>
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
#pragma config FNOSC =FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
void main()
{ 
    int numTry = 0;
    int j;
    TRISA &= 0xff00;
    PORTA = 0;
    int sw1Helper = 1;
    int sw2HelperL = 16;
    int sw2HelperR = 8;
    int sw0Holder = 1;
    int sw1Holder = 1;
    int sw2Holder = 1;

    
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    TRISBbits.TRISB14 = 0; //The corresponding TRIS bit must be set to 0:
    ANSELBbits.ANSB14 = 0; //The corresponding ANSEL bit should be set to 0:
    while(1)
    {
        if(PORTBbits.RB9) {
            PORTA = 0;
            return;
        }
        if(PORTBbits.RB10) {
            PORTBbits.RB14^=1;
            for(int i = 0; i < 300000; i++) ; 
        }
        if(PORTFbits.RF4) {
            sw2Holder = 1;
            PORTA = sw2HelperL | sw2HelperR;
            while(PORTBbits.RB11) ;
            if(!PORTDbits.RD15) {
                sw2HelperL = sw2HelperL << 1;
                sw2HelperR = sw2HelperR >> 1;
                if(sw2HelperL == 256) {
                    sw2HelperL = 16;
                    sw2HelperR = 8;
                }
            } else {
                sw2HelperL = sw2HelperL >> 1;
                sw2HelperR = sw2HelperR << 1;
                if(sw2HelperL == 8) {
                    sw2HelperL = 128;
                    sw2HelperR = 1;
                }
            }
            if(!PORTDbits.RD14)
                for(int i = 0; i < 300000; i++) ;   
            else 
                for(int i = 0; i < 150000; i++) ;   
        } else if(PORTFbits.RF5) {                  // SW1 - Shifting
            sw1Holder = 1;
            if(!PORTDbits.RD15) {
                if(sw1Helper == 256)
                    sw1Helper = 1;
                PORTA = sw1Helper;
                while(PORTBbits.RB11) ;
                sw1Helper = sw1Helper << 1;
            } else {
                if(sw1Helper == 0) {
                   sw1Helper = 128;
                }
                PORTA = sw1Helper;
                while(PORTBbits.RB11) ;
                sw1Helper = sw1Helper >> 1;
            }
            if(!PORTDbits.RD14)
                for(int i = 0; i < 300000; i++) ;   
            else 
                for(int i = 0; i < 150000; i++) ;          
        } else if(PORTFbits.RF3) {           // SW0 - Binary counter
            sw0Holder = 1;
            if(PORTDbits.RD15) {
                PORTA--;
                while(PORTBbits.RB11) ;
            } else {
                PORTA++;
                while(PORTBbits.RB11) ;
            }
            if(!PORTDbits.RD14)
                for(int i = 0; i < 300000; i++) ;   
            else 
                for(int i = 0; i < 150000; i++) ;          
        } 
        
        if(!PORTFbits.RF4 && sw2Holder) {
            sw2Holder = 0;
            PORTA = 0;
        } 
        if(!PORTFbits.RF5 && sw1Holder) {
            sw1Holder = 0;
            PORTA = 0;
        } 
        if(!PORTFbits.RF3 && sw0Holder) {
            sw0Holder = 0;
            PORTA = 0;
        }
        
        
        
        
    }

}