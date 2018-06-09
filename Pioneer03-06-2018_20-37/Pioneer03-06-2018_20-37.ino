#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;
int message;
String text;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
   pinMode(14, OUTPUT);
   pinMode(3, OUTPUT);


   pinMode(41, OUTPUT); // Pioneer wired remote "Source/off 2sec"
   pinMode(40, OUTPUT); // Pioneer wired remote "Up"
   digitalWrite(40, HIGH);
   pinMode(39, OUTPUT); // Pioneer wired remote "Down"
   pinMode(38, OUTPUT); // Pioneer wired remote "Vol +"
   pinMode(37, OUTPUT); // Pioneer wired remote "Vol -"
  
   
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(14, LOW);    // выключаем светодиод
  
  digitalWrite(3, HIGH);
  delay(100);
digitalWrite(3, LOW);
delay(100);
digitalWrite(3, HIGH);
  
  //digitalWrite(14, HIGH);   // зажигаем светодиод
  //delay(100);    
  //Serial.println(1.23456, 4); // ждем секунду
  //digitalWrite(14, LOW);    // выключаем светодиод
  //delay(100);              // ждем секунду
 
  
  if (irrecv.decode(&results)) {
//Serial.println(results.value, HEX);
digitalWrite(14, HIGH);

 message = results.value, HEX;
 switch (message){
 case 0xFFE21D: text = "up"; digitalWrite(40, LOW); delay(200); digitalWrite(40, HIGH); break;
 case 0xFF22DD: text = "right"; break;
 case 0xFFA25D: text = "down"; break;
 case 0xFF02FD: text = "left"; break;
 case 0xFF906F: text = "play/pause"; break;
 case 0xFFA857: text = "vol+"; break;
 case 0xFFE01F: text = "vol-"; break;
 case 0xFFB04F: text = "usb/fm"; break;
 case 0xFF9867: text = "answer"; break;
 case 0xFF629D: text = "reject"; break;
 case 0xFFC23D: text = "shutdown"; break;
 case 0xFFFFFFFF: text = "repeat_command"; break;
 default: text = "unknown_command";
 }
 
Serial.println(text);

delay(10);



irrecv.resume(); // Receive the next value

}

}

