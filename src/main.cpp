#include <Arduino.h>
#define IR_USE_AVR_TIMER 1
#include <IRremote.hpp>
#define IR_RECEIVE_PIN 2 // zapojit IR data na 2
#define RED_BTN 3091726080 // tlačítko na potvrzení červené barvy, červené CH+
#define GREEN_BTN 3158572800 // tlačítko na potvrzení zelené barvy, zelené tlačítko pause/play
#define BLUE_BTN 3208707840 // tlačítko na potvrzení modré barvy, modré tlačítko next
#define DELETE 3108437760 // smaže hodnotu v paměti, aby se mohla zadat další, červené tlačítko CH
#define POWER_BTN 3125149440 // tlačítko na zapnutí / vypnutí všech led, červené tlačítko CH-

int rgbLed[] = {9,5,6}; // red, green, blue <- v tomhle pořadí na pinech v poli
int hodnoty[3]; // zde si ukládám zapsané hodnoty, protože kvůli pwm nemůžu použít digitalRead

unsigned long codes[] = // Definice kodů kláves
    {
        3910598400, // 0
        4077715200, // 1
        3877175040, // 2
        2707357440, // 3
        4144561920, // 4
        3810328320, // 5
        2774204160, // 6
        3175284480, // 7
        2907897600, // 8
        3041591040, // 9
};

String inputString = "";

void potvrdit(int barva){ // 0 - cervena, 1 - zelena, 2 - modra
  int hodnota = inputString.toInt(); // převede řetězec symbolů čísel na číslo
  if(hodnota > 255 || hodnota < 0){ // kontroluje, jestli hodnota sedí do limitu pwm
    Serial.println("Hodnota musí být menší než 255. Zkus to znova.");
  } else{
    Serial.print(barva == 0 ? "Na červenou" : (barva == 1 ? "Na zelenou" : "Na modrou"));
    Serial.print(" ledku byla zapsána hodnota: ");
    Serial.println(hodnota);
    hodnoty[barva] = hodnota;
    analogWrite(rgbLed[barva], hodnoty[barva]); // rožne pomocí pwm ledku s intenzitou podle zadaného čísla
  }
}

int zapnuto(){
  for(int i = 0; i < 3; i++){
    if(hodnoty[i] != 0){
      return 0; // returnuje 0 když je něco zapnuté
    }
  }
  return 1; //returnuje 1 když není žádna led zapnutá
}

void pridejZnak(const char * znak){
  inputString = String(inputString + znak); // spojí současný řetězec se zadaným znakem
  Serial.println(inputString); // vypíše současnou hodnotu v paměti
}

void setup(){
  for(int i = 0; i < 3; i++){
    pinMode(rgbLed[i], OUTPUT);
  }
  pinMode(IR_RECEIVE_PIN, INPUT);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  Serial.begin(9600);
}

void loop()
{
  if (IrReceiver.decode())
  {
    unsigned long irData = IrReceiver.decodedIRData.decodedRawData;
    //Serial.println(irData, HEX);                 // Print "old" raw data
    //IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
    //IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data

    for(int i = 0; i <= 9; i ++){
      if(irData == codes[i]){
        pridejZnak(String(i).c_str());
      }
    }
    if (irData == POWER_BTN) {
      if(zapnuto()){ // podmínka se splní pokud není žádná barva zapnutá
        Serial.println("Vše zapnuto");
        for(int i = 0; i < 3; i++){ // zapne všechny ledky
          digitalWrite(rgbLed[i], HIGH);
          hodnoty[i] = 255;
        }
      } else{
        Serial.println("Vše vypnuto");
        for(int i = 0; i < 3; i++){ // vypne všechny ledky
          digitalWrite(rgbLed[i], LOW);
          hodnoty[i] = 0;
        }
      }
      
    } else if (irData == RED_BTN) {
      potvrdit(0);
    } else if (irData == GREEN_BTN) {
      potvrdit(1);
    } else if(irData == BLUE_BTN){
      potvrdit(2);
    } else if(irData == DELETE){
      inputString = "";
      Serial.println("Smazal jsem číslo v paměti. Můžeš zadat nové.");
    }
    //delay(20); // předchází opakování signálů z ovladače
    IrReceiver.resume(); // povoluje přijetí dalšího signálu z ovladače
  }
}