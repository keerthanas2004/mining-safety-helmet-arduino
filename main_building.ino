#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;

void setup() {
  Serial.begin(9600);
  if (!rf_driver.init())
         Serial.println("init failed");
  pinMode(8,INPUT);
  pinMode(9,OUTPUT);
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    buf[buflen] = '\0';
    Serial.print("Received: ");
    Serial.println((char*)buf);
    if (!(strcmp((char*)buf, "Alert") == 0)) {
    digitalWrite(9,HIGH);
    delay(1000);
    }
  }
  digitalWrite(9,LOW);
  if(digitalRead(8) == HIGH){
    const char *alertMsg = "Alert";
    rf_driver.send((uint8_t *)alertMsg, strlen(alertMsg));
    rf_driver.waitPacketSent();
    delay(1000);
  }
}
