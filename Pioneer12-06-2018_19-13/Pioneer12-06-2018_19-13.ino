#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <EEPROM.h>
IRsend irsend;

int RECV_PIN = 38;
int mode; // переменная режимов: 0 - usb, 1 - fm, 2- bt

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
  pinMode(40, OUTPUT); // пин отправки команд в ATJ2009 по проводу
    
    /*
коды кнопок ATJ2009
FF629D Folder-
FFE01F |<<
FFA857 >>|
FF906F Play-pause
FFB04F +
FF9867 -
FFFFFFFF повтор
*/

if (EEPROM.read(0)==0){ mode =0;}  // проверяем последний выбранный режим в eeprom, который был установлен перед выключением питания
else {mode=1;}





   //пины для Pioneer wired control, подтянем их к "+" для реализации обратной логики:
   pinMode(26, OUTPUT); digitalWrite(26, HIGH); // Pioneer wired remote "Source/off 2sec"
   pinMode(28, OUTPUT); digitalWrite(28, HIGH); // Pioneer wired remote "Up"
   pinMode(30, OUTPUT); digitalWrite(30, HIGH); // Pioneer wired remote "Down"
   pinMode(32, OUTPUT); digitalWrite(32, HIGH); // Pioneer wired remote "Vol +"
   pinMode(34, OUTPUT); digitalWrite(34, HIGH); // Pioneer wired remote "Vol -"
   pinMode(36, OUTPUT); digitalWrite(36, HIGH); // Pioneer wired remote WREM2
  
  
   
  
}

//функции для кнопок:

void up() {
  
  if (mode==1) {digitalWrite(36, LOW); digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH); digitalWrite(36, HIGH);} // в режиме радио шлём команду в pioneer wired control
  else if (mode==0) {irsend.sendNEC(0xFFA25D, 24);} // в режиме usb шлём команду в ATJ2009
  else {error(2);} //иначе ошибка
  }

void right() {
  
  if (mode==1) {digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH);} // в режиме радио шлём команду в pioneer wired control
  else if (mode==0) {irsend.sendNEC(0xFFA857, 24);}
  else {error(3);}
  
  }

void down() {  
  
   if (mode==1) {digitalWrite(36, LOW); digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH); digitalWrite(36, HIGH);} // в режиме радио шлём команду в pioneer wired control
   else if (mode==0) {irsend.sendNEC(0xFF629D, 24);}
   else {error(4);}
  }

void left() { 
  
   if (mode==1) {digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH);} // в режиме радио шлём команду в pioneer wired control
   else if (mode==0) {irsend.sendNEC(0xFFE01F, 24);}
   else {error(5);}
  }
  
 void play_pause()  { 
   if (mode==1) {return;} // здесь нужен код на установку не существующего источника на CD4052 для имитации "паузы" радио
   else if (mode==0) {irsend.sendNEC(0xFFF906F, 24);}
   else {error(6);}
  
  }
  
 void vol_up() { 
  
  digitalWrite(32, LOW); delay(200); digitalWrite(32, HIGH); // добавление громкости через pioneer wired control
//irsend.sendNEC(0xFFB04F, 24); // добавление громкости ATJ2009
}
 void vol_down() { 
  digitalWrite(34, LOW); delay(200); digitalWrite(34, HIGH); // уменьшение громкости через pioneer wired control
//irsend.sendNEC(0xFF9867, 24);  // уменьшение громкости ATJ2009
}

 void usb_fm() { 

if (mode==2){ return;} //проверяем текущий режим девайса, и если это bt "2" - то просто выходим
else if (mode==1) {mode=0; EEPROM.write(0, 0);} //  если это fm "1", то меняем на "0" - usb и пишем в eeprom
else if (mode==0) {mode=1; EEPROM.write(0, 1);} //  если это usb "0", то меняем на "1" - fm и пишем в eeprom
else {error(1);} // иначе ошибка 1

}

 void answer() {

}

 void reject() { 

}

 void poweroff() {
  
  digitalWrite(26, LOW); delay(200); digitalWrite(26, HIGH); // кнопка Source, poweroff 2sec для pioneer wired control


}

 void repeat_command() { 

  
//irsend.sendNEC(0xFFFFFFFF, 32); //команда повтора ATJ2009


}


void unknown_command () {


  
}

void error(int error_number){
return; // здесь будет код обработки ошибок

  
}
  




// the loop function runs over and over again forever
void loop() {
  digitalWrite(42, LOW);    // выключаем светодиод
  
  //digitalWrite(44, HIGH);
  //delay(100);
digitalWrite(44, LOW);
delay(20);
digitalWrite(44, HIGH);
  
 //digitalWrite(40, HIGH);   // зажигаем светодиод
 //delay(2000);    
  //Serial.println(1.23456, 4); // ждем секунду
 //digitalWrite(40, LOW);    // выключаем светодиод
 //delay(2000);              // ждем секунду
 
  
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


irrecv.enableIRIn();
irrecv.resume(); // Receive the next value

}

else {                                      //этот код выполняется каждый цикл, когда не было команд с пульта
Serial.println("no command");
 }

 
}

