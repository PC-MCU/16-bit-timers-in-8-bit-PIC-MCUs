 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"

/*
    Main application
*/

//// code at main.c
//variable
extern uint8_t T1G_FLAG ;
extern float   Key_count;

//State machine 
typedef enum{
    STANDBY_MODE,
    KEY_HOLD,
    KEY_RELEASE
            
} main_state_t;
main_state_t main_state = STANDBY_MODE;

void main_count(void){
    switch(main_state){
            case STANDBY_MODE:                
                if(Key_count > 0){
                   main_state = KEY_HOLD;
                   IO_RF3_SetLow();
                }
                break;
				
            case KEY_HOLD:  
                if(T1G_FLAG == 1){
                   T1G_FLAG = 0;
                   main_state = KEY_RELEASE;      
                   IO_RF3_SetHigh();
                }
                break;
                
            case KEY_RELEASE:
				printf("Counter value for pulse: %f \r\n", Key_count);
                Key_count = Key_count/100;
                printf("%.2f Second \r\n", Key_count);
                Key_count = 0;

                printf("New count can be started. \r\n");
                TMR1_StartSinglePulseAcquisition();
                main_state = STANDBY_MODE;
                break;
				
        } 
}



int main(void)
{
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    TMR1_StartSinglePulseAcquisition();
	IO_RF3_SetHigh();
    
    while(1)
    {
        main_count();
    }    
}