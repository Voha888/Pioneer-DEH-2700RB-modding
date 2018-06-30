#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <EEPROM.h>
IRsend irsend;
 
int RECV_PIN = 38;
int current_mode; // переменная режимов: 0 - usb, 1 - fm, 2- bt
int current_time; //пишем текущее время каждый цикл
int time_to_on; // время для включения bt
int last_mode; // последний режим, для возврата после работы bt
//int key_hold; // переменная удержания кнопки
int usb_need_off; 
int time_to_off_usb;


IRrecv irrecv(RECV_PIN);
 
decode_results results;
int message;
 
 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
 pinMode(20, OUTPUT); digitalWrite(20, HIGH); // "Самоподхват" реле включения
 pinMode(25, OUTPUT); digitalWrite(25, HIGH); // Подадим AAC на магнитолу для её запуска
 pinMode(22, INPUT); //настроим сигнал о включении магнитолы (питание антены)
 pinMode(24, OUTPUT); digitalWrite(25, HIGH); // Настроим пин запуска ATJ2009

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(40, OUTPUT); // пин отправки команд в ATJ2009 по проводу
  pinMode(49, OUTPUT); pinMode(51, OUTPUT); // пины управления мультиплексором GD4052
  pinMode(13, OUTPUT);pinMode(11, OUTPUT);pinMode(9, OUTPUT);pinMode(7, OUTPUT);pinMode(5, OUTPUT);pinMode(3, OUTPUT);pinMode(15, OUTPUT);pinMode(16, OUTPUT); // пины управления Qualcomm CSR86xx
  pinMode(18, INPUT); // csr86xx LED1
  pinMode(59, INPUT); pinMode(18, INPUT); // csr86xx PIO 17, PIO 14
   
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
 
if (EEPROM.read(0)==0){ current_mode =0; last_mode=0; ; digitalWrite(24, HIGH);}  // проверяем последний выбранный режим в eeprom, который был установлен перед выключением питания, и если это 0-usb то запустим ATJ2009 через пин 24
else {current_mode=1;last_mode=1; digitalWrite(49, LOW); digitalWrite(51, HIGH);}
 
 
 
 
   //пины для Pioneer wired control, подтянем их к "+" для реализации обратной логики:
   pinMode(26, OUTPUT); digitalWrite(26, HIGH); // Pioneer wired remote "Source/off 2sec"
   pinMode(28, OUTPUT); digitalWrite(28, HIGH); // Pioneer wired remote "Up"
   pinMode(30, OUTPUT); digitalWrite(30, HIGH); // Pioneer wired remote "Down"
   pinMode(32, OUTPUT); digitalWrite(32, HIGH); // Pioneer wired remote "Vol +"
   pinMode(34, OUTPUT); digitalWrite(34, HIGH); // Pioneer wired remote "Vol -"
   pinMode(36, OUTPUT); digitalWrite(36, HIGH); // Pioneer wired remote WREM2
 
  digitalWrite(16, HIGH);delay(2800);digitalWrite(16, LOW);delay (200); digitalWrite(16, HIGH);delay(2800);digitalWrite(16, LOW); // запустим BT по зажатию VCHGT
}
 
//функции для кнопок:
 
void up() {
 
  if (current_mode==1) {digitalWrite(36, LOW); digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH); digitalWrite(36, HIGH);} // в режиме радио шлём команду в pioneer wired control
  else if (current_mode==0) {irsend.sendNEC(0xFFA25D, 24);} // в режиме usb шлём команду в ATJ2009
  else if (current_mode==2) {digitalWrite(11, HIGH); delay(200); digitalWrite(11, LOW);} // в режиме BT шлём кнопку в CSR86xx
  else {error(2);} //иначе ошибка
  }
 
void right() {
 
  if (current_mode==1) {digitalWrite(28, LOW); delay(200); digitalWrite(28, HIGH);} // в режиме радио шлём команду в pioneer wired control
  else if (current_mode==0) {irsend.sendNEC(0xFFA857, 24);}
  else if (current_mode==2) {digitalWrite(11, HIGH); delay(200); digitalWrite(11, LOW);} // в режиме BT шлём кнопку в CSR86xx
  else {error(3);}
 
  }
 
void down() {  
 
   if (current_mode==1) {digitalWrite(36, LOW); digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH); digitalWrite(36, HIGH);} // в режиме радио шлём команду в pioneer wired control
   else if (current_mode==0) {irsend.sendNEC(0xFF629D, 24);}
   else if (current_mode==2) {digitalWrite(7, HIGH); delay(200); digitalWrite(7, LOW);} // в режиме BT шлём кнопку в CSR86xx
   else {error(4);}
  }
 
void left() {
 
   if (current_mode==1) {digitalWrite(30, LOW); delay(200); digitalWrite(30, HIGH);} // в режиме радио шлём команду в pioneer wired control
   else if (current_mode==0) {irsend.sendNEC(0xFFE01F, 24);}
   else if (current_mode==2) {digitalWrite(9, HIGH); delay(200); digitalWrite(9, LOW);} // в режиме BT шлём кнопку в CSR86xx
   else {error(5);}
  }
 
 void play_pause()  {
   if (current_mode==1) {return;} // здесь нужен код на установку не существующего источника на CD4052 для имитации "паузы" радио
   else if (current_mode==0) {irsend.sendNEC(0xFFF906F, 24);}
   else if (current_mode==2) {digitalWrite(13, HIGH); delay(200); digitalWrite(13, LOW);} // в режиме BT шлём кнопку в CSR86xx
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
 
if (current_mode==2){ return;} //проверяем текущий режим девайса, и если это bt "2" - то просто выходим
else if (current_mode==1) {current_mode=0; last_mode=0; usb_need_off=0; digitalWrite(24, HIGH);digitalWrite(49, LOW); digitalWrite(51, LOW); } //  если это fm "1", то меняем на "0" - usb и пишем в eeprom и включаем ATJ2009 через пин 24
else if (current_mode==0) {current_mode=1; last_mode=1; usb_need_off=1; time_to_off_usb = current_time; digitalWrite(49, LOW); digitalWrite(51, HIGH); } //  если это usb "0", то меняем на "1" - fm и пишем в eeprom и выключаем ATJ2009 через пин 24
else {error(1);} // иначе ошибка 1
 
}
 
 void answer() {
digitalWrite(15, HIGH); delay(200); digitalWrite(15, LOW); // шлём кнопку в CSR86xx
}
 
 void reject() {
digitalWrite(16, HIGH);delay(200);digitalWrite(16, LOW); // шлём кнопку VCHRG в CSR86xx
}
 
 void poweroff() {
  digitalWrite(44, HIGH);
if (last_mode==0) {EEPROM.write(0, 0);} //  запомним последний режим девайса, если это usb "0", то пишем в eeprom 0
else if (last_mode==1) {EEPROM.write(0, 1);} //  запомним последний режим девайса, если это fm "1", то пишем в eeprom 1
else {error(7);} // иначе ошибка
 
 
 //digitalWrite(26, LOW); delay(200); digitalWrite(26, HIGH); // кнопка Source, poweroff 2sec для pioneer wired control
//digitalWrite(13, HIGH); delay(200); digitalWrite(13, LOW); // проверим включение-выключение модуля CSR86xx
delay(200);
digitalWrite(20, LOW); // Отключим всю схему
 
}
 
 void repeat_command() {
 
 
//irsend.sendNEC(0xFFFFFFFF, 32); //команда повтора ATJ2009
 
 
}
 
 
void unknown_command () {
 
 delay(100);
 
}
 
void error(int error_number){
return; // здесь будет код обработки ошибок
 
 
 
}
 
 
 
 
// the loop function runs over and over again forever
void loop() {
 
 if (digitalRead(22)==0){ delay(4000); if (digitalRead(22)==0){ poweroff(); }}
 
  digitalWrite(42, LOW); digitalWrite(44, LOW);   // выключаем светодиоды
 
 
 
  if (irrecv.decode(&results)) {
//Serial.println(results.value, HEX);

 
 message = results.value, HEX; // отправим принятые клавиши в соответствующие им функции
 switch (message){
 case 0xFFE21D: digitalWrite(42, HIGH) ; up() ;   break;
 case 0xFF22DD: digitalWrite(42, HIGH) ; right() ; break;
 case 0xFFA25D: digitalWrite(42, HIGH) ; down() ;  break;
 case 0xFF02FD: digitalWrite(42, HIGH) ; left() ; break;
 case 0xFF906F: digitalWrite(42, HIGH) ; play_pause() ; break;
 case 0xFFA857: digitalWrite(42, HIGH) ; vol_up() ; break;
 case 0xFFE01F: digitalWrite(42, HIGH) ; vol_down() ; break;
 case 0xFFB04F: digitalWrite(42, HIGH) ; usb_fm() ; break;
 case 0xFF9867: digitalWrite(42, HIGH) ; answer() ; break;
 case 0xFF629D: digitalWrite(42, HIGH) ; reject() ; break;
 case 0xFFC23D: digitalWrite(42, HIGH) ; poweroff(); break;
 case 0xFFFFFFFF: digitalWrite(42, HIGH) ;repeat_command() ; break;
 default: digitalWrite(44, HIGH); unknown_command () ;
 }
 
 
irrecv.enableIRIn();
irrecv.resume(); // Receive the next value
 
}
 
else {                                      //этот код выполняется каждый цикл, когда не было команд с пульта
//Serial.println("no command");
 }

current_time=millis();
if (digitalRead(18)==0 && current_mode!=2) {time_to_off_usb = current_time; usb_need_off=1; digitalWrite(49, HIGH); digitalWrite(51, LOW);}
if (digitalRead(18)==0){ current_mode=2; time_to_on = current_time;}  //если сработал LED1 на CSR866xx, нужно включить BT (current_mode=2) на 3 секунды

if (current_mode==2 &&((current_time - time_to_on) > 3000)) 
{ 
  current_mode=last_mode;
    if (current_mode==1){usb_need_off=1; time_to_off_usb = current_time; digitalWrite(49, LOW); digitalWrite(51, HIGH); } // для FM
    else {usb_need_off=0; digitalWrite(24, HIGH); digitalWrite(49, LOW); digitalWrite(51, LOW);} //для USB
}

if (usb_need_off==1 && ((current_time - time_to_off_usb ) > 30000)) {digitalWrite(24, LOW); usb_need_off=0; } // таймер отключения USB


}
