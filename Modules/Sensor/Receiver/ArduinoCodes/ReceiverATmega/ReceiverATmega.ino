/*
**  ReceiverATmega.ino
**  MakerWear Receiver Module's ATmega Program.
**
**  Receives a signal from sender and sends output based on signal.
**  Description
**  Description
**
**
**  ATmega328 Pin Configurations:
**
**  Pin 1  (Reset):                          Pin 28  (A5):  
**  Pin 2  (D0/RX):                          Pin 27  (A4):
**  Pin 3  (D1/TX):                          Pin 26  (A3):
**  Pin 4  (D2):                             Pin 25  (A2):
**  Pin 5  (D3/PWM):                         Pin 24  (A1):
**  Pin 6  (D4):                             Pin 23  (A0):       Module Input
**  Pin 7  (PWR):                            Pin 22  (GND):
**  Pin 8  (GND):                            Pin 21  (AREF):
**  Pin 9  (XTAL1):                          Pin 20  (AVCC):
**  Pin 10 (XTAL2):                          Pin 19  (D13):
**  Pin 11 (D5/PWM):                         Pin 18  (D12):
**  Pin 12 (D6/PWM):                         Pin 17  (D11/PWM):
**  Pin 13 (D7):                             Pin 16  (D10/PWM):
**  Pin 14 (D8):                             Pin 15  (D9/PWM):
**
**
**  Created on 6/8/16.
**  By Alex Jiao
**  Modified on 6/30/16
**  By Jason McPeak
**
**  MakerWear Link:
**  Github Link:      github.com/myjeeed/MakerWear
**
*/

#include <SignalProcessing.h>
#include <IRremote.h>

int input_pin = A0;       // pin 23 on ATmega328
int filter_size = 15;     // Noise reduction filter size
int ir_pin = 8;           // D8 which is pin 14 on ATmega
int output_pin = 9;       // pin 15 on ATmega

SignalProcessing input(input_pin, filter_size);

IRrecv irrecv(ir_pin);    // uses interrupt to check for incoming tranmissions
decode_results results;   // object where interrupt stores results

void setup() {
  irrecv.enableIRIn();    // Start the receiver
}

int output_val = 0;
int data;

void loop() {  
  if (irrecv.decode(&results)) {
    data = results.value;
    
    // check that data is the correct length
    if ((results.bits > 0) && (results.bits <= 12)) {
      output_val = (data & 0x3FF)/4;
    }
    
    irrecv.resume(); // Receive the next value
  }

  // shouldn't do anything when input voltage is 0
  if(cutAndMap(input.filteredAnalogRead(AVERAGE), 50, 975, 0, 1023) <= 15)
    analogWrite(output_pin, 0);
  else
    analogWrite(output_pin, output_val);
    
}
