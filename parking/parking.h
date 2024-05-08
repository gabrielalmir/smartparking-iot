#ifndef PARKING_H
#define PARKING_H

#include "WiFiCredentials.h"
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#define lcd_addr 0x27
#define lcd_cols 20
#define lcd_rows 4

extern uint8_t wifiSignal[];

extern const char* ssid;
extern const char* pass;

void refreshDisplay();
void establishWifiConnection();
void displayLocalTime();

#endif
