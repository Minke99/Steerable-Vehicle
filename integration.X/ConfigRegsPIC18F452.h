/*!
    @file       ConfigRegsPIC18F452.h

    @brief      PIC18F452 configuration bit settings.

    The config pragma defines configuration register data from within a source
    file. Values set here will override those defined in the MPLABX IDE.
    The labels following the directive are defined in the file P18F452.inc.
    The PIC18FXX2 Data Sheet explains the functions of the configuration bits.

    This file is suitable for both debugging and production. Change it to
    suit your application.

    The macro __DEBUG is automatically defined for a debug build, causing the
    configuration bits to be set as appropriate for development and debugging:
        - Brown-out Reset disabled;
        - Watchdog Timer disabled;
        - Stack Overflow Reset;
        - Low-voltage Programming disabled, Debug mode enabled;

    If the macro __DEBUG is NOT defined, the bits are set as appropriate for
    production code release:
        - Brown-out Reset enabled at 4.2V;
        - Watchdog Timer disabled;
        - Stack Overflow Reset;
        - Low-voltage Programming and Debug mode disabled;

    @example    #include  ConfigRegsPIC18F452.h
    @version    1.1
    @date       1-Sep-2019
    @author     David Rye
    @note   This file was generated by the compiler from the command line:
            mcc18 -p18f452 --help-config > configReg.h
    @todo   Consider if Watchdog Timer should be enabled for RELEASE, and add
            WDT management code if so.
    @todo   Consider if Stack Overflow Reset should be enabled for RELEASE. It
            may be better not to as there shouldn't be any re-entrant, recursive
            code or dynamic memory allocation here. If the code loads statically
            it should run.
*/


#ifndef CONFIG_REG_PIC18F452_H
#define CONFIG_REG_PIC18F452_H

#include <p18cxxx.h>                        // wrapper for all PIC18 headers

#ifndef __18F452
#error  "ERROR: Configuration-header file mismatch. Verify selected processor."
#endif


//  Oscillator Selection bits:
//    OSC = LP          LP
//    OSC = XT          XT
//    OSC = HS          HS                      (use for Minimal Board)
//    OSC = RC          RC
//    OSC = EC          EC-OSC2 as Clock Out    (use for PICDEM 2 Plus board)
//    OSC = ECIO        EC-OSC2 as RA6
//    OSC = HSPLL       HS-PLL Enabled          (alternate for Minimal Board)
//    OSC = RCIO        RC-OSC2 as RA6
//
//  Osc. Switch Enable bits:
//    OSCS = ON         Enabled
//    OSCS = OFF        Disabled
//
//  Power Up Timer:
//    PWRT = ON         Enabled
//    PWRT = OFF        Disabled

#pragma config OSC  = HS
#pragma config OSCS = OFF
#pragma config PWRT = ON


//  Brown Out Reset Enable bits:
//    BOR = OFF         Disabled
//    BOR = ON          Enabled
//
//  Brown Out Reset Voltage bits:
//    BORV = 45         4.5V
//    BORV = 42         4.2V
//    BORV = 27         2.7V
//    BORV = 25         2.5V

#pragma config BORV = 42

#ifdef __DEBUG
    #pragma config BOR = OFF
#else
    //  For a production release, consider BOR = ON.
    #pragma config BOR = OFF
#endif


//  Watchdog Timer Enable bit:
//    WDT = OFF         Disabled
//    WDT = ON          Enabled
//
//  Watchdog Timer Postscale Select bits:
//    WDTPS = 1         1:1
//    WDTPS = 2         1:2
//    WDTPS = 4         1:4
//    WDTPS = 8         1:8
//    WDTPS = 16        1:16
//    WDTPS = 32        1:32
//    WDTPS = 64        1:64
//    WDTPS = 128       1:128

#pragma config WDTPS = 128

#ifdef __DEBUG
    #pragma config WDT = OFF
#else
    //  For a production release, consider WDT = ON, but note that you will
    //  need to configure and 'kick' the watchdog timer from your code.
    #pragma config WDT = OFF
#endif


//  CCP2 Mux bit:
//    CCP2MUX = OFF     Disable (CCP2 is on RB3)
//    CCP2MUX = ON      Enable  (CCP2 is on RC1)

#pragma config CCP2MUX = OFF


//  Stack Overflow Reset:
//    STVR = OFF        Disabled
//    STVR = ON         Enabled
//
//  Low Voltage ICSP:
//    LVP = OFF         Disabled
//    LVP = ON          Enabled
//
//  Background Debugger Enable:
//    DEBUG = ON        Enabled
//    DEBUG = OFF       Disabled

//  NOTE: LVP = ON is not compatible with DEBUG = ON

#pragma config STVR = OFF

// Background Debugger Enable:
//   DEBUG = ON        Enabled
//   DEBUG = OFF       Disabled
// NOTE: LVP = ON is not compatible with DEBUG = ON

#ifdef __DEBUG
    // LVP = ON is not compatible with DEBUG = ON
    #pragma config DEBUG = ON       // debug build
    #pragma config LVP = OFF        // force LVP OFF
#else
    #pragma config DEBUG=OFF        // production build
    #pragma config LVP = OFF	    // can be OFF or ON
#endif


//  Code Protection Block 0:
//    CP0 = ON          Enabled
//    CP0 = OFF         Disabled
//
//  Code Protection Block 1:
//    CP1 = ON          Enabled
//    CP1 = OFF         Disabled
//
//  Code Protection Block 2:
//    CP2 = ON          Enabled
//    CP2 = OFF         Disabled
//
//  Code Protection Block 3:
//    CP3 = ON          Enabled
//    CP3 = OFF         Disabled

#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF


//  Boot Block Code Protection:
//    CPB = ON          Enabled
//    CPB = OFF         Disabled
//
//  Data EEPROM Code Protection:
//    CPD = ON          Enabled
//    CPD = OFF         Disabled

#pragma config CPB = OFF
#pragma config CPD = OFF

//  Write Protection Block 0:
//    WRT0 = ON         Enabled
//    WRT0 = OFF        Disabled
//
//  Write Protection Block 1:
//    WRT1 = ON         Enabled
//    WRT1 = OFF        Disabled
//
//  Write Protection Block 2:
//    WRT2 = ON         Enabled
//    WRT2 = OFF        Disabled
//
//  Write Protection Block 3:
//    WRT3 = ON         Enabled
//    WRT3 = OFF        Disabled

#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF


//  Boot Block Write Protection:
//    WRTB = ON         Enabled
//    WRTB = OFF        Disabled
//
//  Configuration Register Write Protection:
//    WRTC = ON         Enabled
//    WRTC = OFF        Disabled
//
//  Data EEPROM Write Protection:
//    WRTD = ON         Enabled
//    WRTD = OFF        Disabled

#pragma config WRTC = OFF
#pragma config WRTB = OFF
#pragma config WRTD = OFF


//  Table Read Protection Block 0:
//    EBTR0 = ON        Enabled
//    EBTR0 = OFF       Disabled
//
//  Table Read Protection Block 1:
//    EBTR1 = ON        Enabled
//    EBTR1 = OFF       Disabled
//
//  Table Read Protection Block 2:
//    EBTR2 = ON        Enabled
//    EBTR2 = OFF       Disabled
//
//  Table Read Protection Block 3:
//    EBTR3 = ON        Enabled
//    EBTR3 = OFF       Disabled

#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF


//  Boot Block Table Read Protection:
//    EBTRB = ON        Enabled
//    EBTRB = OFF       Disabled

#pragma config EBTRB = OFF


#endif  // #ifndef CONFIG_REG_PIC18F452_H

