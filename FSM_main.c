
#include <xc.h>
#define _XTAL_FREQ	64000000L

enum {SIMPAN,SIDAI,IDLE}State=IDLE;

unsigned char step_seq[8]={0b1000,0b1100,0b0100,0b0110,0b0010,0b0011,0b0001,0b1001};
signed char index=0;
unsigned char count=0;
unsigned char onceOld,once;
unsigned char last_state;

void setup(void);
void delay(unsigned int x);
void buka_ampaian(void);
void tutup_ampaian(void);

void main(void) {
    setup();
    TRISD=0;        //Pin RD0..RD7 serve as OUTPUT
    TRISA |=0b11;   //Pin RA0 and RA1 serve as INPUT
    TRISB &=0b00;   //Pin RB0 and RB1 serve as OUTPUT
    //Assigned Initial Value
    onceOld=1;
    once = 0;
//    last_state = SIMPAN;
    
    //Your program start here
    while (1) {
        switch(State){
            case IDLE://Check Sensor
                    if((PORTAbits.RA0==0 && PORTAbits.RA1 == 0) && (last_state == SIMPAN)){
                        State = SIDAI;
                    }
                    else if((PORTAbits.RA0 || PORTAbits.RA1) && (last_state == SIDAI)){
                        State = SIMPAN;
                    }                    
                break;
            case SIDAI:buka_ampaian();
                break;
            case SIMPAN:tutup_ampaian();
                break;
        }
  
    }
}

void buka_ampaian(void){
    if(onceOld != once){
        onceOld = once;
        index = 0;
    }    
    if(index>7){
        index = 0;
        count = count + 1;
    }
    //Katakan motor pusing 5 putaran CW,Ampaian Buka sepenuhnya
    if(count == 5){
        count = 0;
        onceOld = 1;
        once = 0;
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;        
        last_state = State;
        State = IDLE;
    }
    LATD = step_seq[index++];
    delay(100);
}

void tutup_ampaian(void){
    if(onceOld != once){
        onceOld = once;
        index = 7;
    }
    if(index<0){
        index = 7;
        count = count + 1;
    }
    //Katakan motor pusing 5 putaran CCW,ampaian tutup sepenuhnya.
    if(count == 5){
        count = 0;
        onceOld = 1;
        once = 0;
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        last_state = State;
        State = IDLE;
    }
    LATD = step_seq[index--];
    delay(100);    
}
//Your program end here

/*
BAHAGIAN INI JANGAN KACAU :)
 */
void setup(void) {

    ANSELA = 0b00000000;
    ANSELB = 0b00000000;
    ANSELC = 0b00000000;
    ANSELD = 0b00000000;
    ANSELE = 0b00000000;
    VREFCON0 = 0b10110000; // Fixed Voltage Reference is enabled, x4 (4.096V)
    ADCON1 = 0b00110000; //0b00110000=4.096V 0b00100000=2.048V 0b00000000=Vdd
    ADCON2 = 0b10000011;

    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
}

void delay(unsigned int x) {
    for (; x > 0; x--)__delay_ms(1);
}

static void interrupt ISR(void) {
}

