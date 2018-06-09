#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

int RECV_PIN = 38;

IRrecv irrecv(RECV_PIN);

decode_results results;
int message;
String text;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
   pinMode(42, OUTPUT);
   pinMode(44, OUTPUT);

//пины для Pioneer wired control:
   pinMode(26, OUTPUT); // Pioneer wired remote "Source/off 2sec"
   digitalWrite(26, HIGH);
   pinMode(28, OUTPUT); // Pioneer wired remote "Up"
   digitalWrite(28, HIGH);
   pinMode(30, OUTPUT); // Pioneer wired remote "Down"
   digitalWrite(30, HIGH);
   pinMode(32, OUTPUT); // Pioneer wired remote "Vol +"
   digitalWrite(32, HIGH);
   pinMode(34, OUTPUT); // Pioneer wired remote "Vol -"
   digitalWrite(34, HIGH);
   pinMode(36, OUTPUT); // Pioneer wired remote WREM2
   digitalWrite(36, HIGH);
  
   
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(42, LOW);    // выключаем светодиод
  
  //digitalWrite(44, HIGH);
  //delay(100);
digitalWrite(44, LOW);
delay(20);
digitalWrite(44, HIGH);
  
  //digitalWrite(14, HIGH);   // зажигаем светодиод
  //delay(100);    
  //Serial.println(1.23456, 4); // ждем секунду
  //digitalWrite(14, LOW);    // выключаем светодиод
  //delay(100);              // ждем секунду
 
  
  if (irrecv.decode(&results)) {
//Serial.println(results.value, HEX);
digitalWrite(42, HIGH);

 message = results.value, HEX;
 switch (message){
 case 0xFFE21D: text = "up"; digitalWrite(36, LOW); digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH); digitalWrite(36, HIGH);  break;
 case 0xFF22DD: text = "right"; digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH);  break;
 case 0xFFA25D: text = "down";  digitalWrite(36, LOW); digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH); digitalWrite(36, HIGH); break;
 case 0xFF02FD: text = "left"; digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH); break;
 case 0xFF906F: text = "play/pause"; break;
 case 0xFFA857: text = "vol+"; digitalWrite(32, LOW); delay(200); digitalWrite(32, HIGH); break;
 case 0xFFE01F: text = "vol-"; digitalWrite(34, LOW); delay(200); digitalWrite(34, HIGH); break;
 case 0xFFB04F: text = "usb/fm"; break;
 case 0xFF9867: text = "answer"; break;
 case 0xFF629D: text = "reject"; break;
 case 0xFFC23D: text = "shutdown"; digitalWrite(26, LOW); delay(200); digitalWrite(26, HIGH); break;
 case 0xFFFFFFFF: text = "repeat_command"; break;
 default: text = "unknown_command";
 }
 
Serial.println(text);

//delay(100);



irrecv.resume(); // Receive the next value

}

}

