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
    pinMode(40, OUTPUT); // пин отправки команд в модулятор по проводу
    
    /*
     коды кнопок ATJ2009
    FFA25D Folder+
FF629D Folder-
FFE01F |<<
FFA857 >>|
FF906F Play-pause
FFB04F +
FF9867 -
FFFFFFFF повтор
*/
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

//функции для кнопок:

void up() {
  
  digitalWrite(36, LOW); digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH); digitalWrite(36, HIGH);
  
  }

void right() {
  
  digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH);
  
  }

void down() {  
  
  digitalWrite(36, LOW); digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH); digitalWrite(36, HIGH); 
  
  }
void left() { 
  
  digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH); 
  
  }
  
 void play_pause() 
 
 { 
  
  }
  
 void vol_up() { 
  
  digitalWrite(32, LOW); delay(200); digitalWrite(32, HIGH);

}
 void vol_down() { 
  digitalWrite(34, LOW); delay(200); digitalWrite(34, HIGH);

}

 void usb_fm() { 

}

 void answer() {

}

 void reject() { 

}

 void poweroff() {
  
  digitalWrite(26, LOW); delay(200); digitalWrite(26, HIGH); 


}

 void repeat_command() { 



}


void unknown_command () {


  
}






// the loop function runs over and over again forever
void loop() {
  digitalWrite(42, LOW);    // выключаем светодиод
  
  //digitalWrite(44, HIGH);
  //delay(100);
digitalWrite(44, LOW);
delay(20);
digitalWrite(44, HIGH);
  
 // digitalWrite(40, HIGH);   // зажигаем светодиод
 // delay(100);    
  //Serial.println(1.23456, 4); // ждем секунду
 // digitalWrite(40, LOW);    // выключаем светодиод
 // delay(100);              // ждем секунду
 
  
  if (irrecv.decode(&results)) {
Serial.println(results.value, HEX);
digitalWrite(42, HIGH);

 message = results.value, HEX; // отправим принятые клавиши в соответствующие им функции
 switch (message){
 case 0xFFE21D: up() ;   break;
 case 0xFF22DD: right() ; break;
 case 0xFFA25D: down() ;  break;
 case 0xFF02FD: left() ; break;
 case 0xFF906F: play_pause() ; break;
 case 0xFFA857: vol_up() ; break;
 case 0xFFE01F: vol_down() ; break;
 case 0xFFB04F: usb_fm() ; break;
 case 0xFF9867: answer() ; break;
 case 0xFF629D: reject() ; break;
 case 0xFFC23D: poweroff(); break;
 case 0xFFFFFFFF: repeat_command() ; break;
 default: unknown_command () ;
 }
 
Serial.println(text);

//delay(100);



irrecv.resume(); // Receive the next value

}

}

