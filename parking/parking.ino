
#include "parking.h"

#include <PubSubClient.h>

LiquidCrystal_I2C lcd(lcd_addr, lcd_cols, lcd_rows);

uint8_t wifiSignal[] = {0x00, 0x00, 0x00, 0x07, 0x08, 0x13, 0x14, 0x15};

const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;

const int distanceInfrared = 3000;

unsigned long previousMillis = 0;
const long samplingInterval = 1000;

const char *ntpServer = "pool.ntp.org";
const long gmtOffsetInSeconds = -14400;
const int daylightOffsetSeconds = 3600;

const char *mqtt_host = "broker.emqx.io";
const char *mqtt_topic = "smartparking-iot";

WiFiClient espClient;
PubSubClient client(espClient);

struct Sensor {
  char* name;
  int pin;
  bool flag;
};

Sensor sensors[] = {
  {"rua_jose_boni_vg1", 35, false},   // rua jose bonifacio - vaga 01
  {"rua_jose_boni_vg2", 34, false},   // rua jose bonifacio - vaga 02
  {"rua_campo_sales_vg1", 33, false}, // rua campo sales - vaga 01
  {"rua_campo_sales_vg2", 32, false}  // rua campo sales - vaga 02
};

struct Street {
  char* name;
  Sensor sensors[2];
  int availableLots;
};

Street streets[] = {
  { "Jose Bon.", {sensors[0], sensors[1]}, 2 },
  { "Campo Sa.", {sensors[2], sensors[3]}, 2 }
};

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.createChar(0, wifiSignal);
  lcd.backlight();
  establishWifiConnection();
  configTime(gmtOffsetInSeconds, daylightOffsetSeconds, ntpServer);
  refreshDisplay();
  displayLocalTime();

  // Configuração dos sensores
  for (int i = 0; i < 4; i++)
  {
    pinMode(sensors[i].pin, INPUT);
  }
}

void loop()
{
  scanParkingSensors();
  verifyWifiConnection();
  handleLocalTime();
}

void verifyBrokerConnection() {
  if (!client.connected())
  {
    client.setServer(mqtt_host, 1883);
    if (client.connect("smartparking-iot"))
    {
      Serial.println("Conectado ao broker MQTT");
    }
    else
    {
      Serial.println("Falha ao conectar ao broker MQTT");
    }
  }
}

void verifyWifiConnection() {
  if (WiFi.status() == WL_CONNECTED)
  {
    lcd.setCursor(19, 0);
    lcd.write(byte(0));
  }
  else
  {
    lcd.setCursor(19, 0);
    lcd.print("X");
  }
}

bool updateSensorStatus(int sensorIndex, bool status)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi desconectado");
    return false;
  }

  verifyBrokerConnection();

  String payload = "{\"sensor\": \"" + String(sensors[sensorIndex].name) + "\", \"status\": " + String(status) + "}";
  client.publish(mqtt_topic, payload.c_str());

  Serial.println("Mensagem enviada: " + payload);

  return true;
}

void handleLocalTime() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= samplingInterval)
  {
    previousMillis = currentMillis;
    displayLocalTime();
  }
}

void refreshDisplay()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SMART PARKING");
  lcd.setCursor(0, 1);

  for (int i = 0; i < 2; i++)
  {
    lcd.setCursor(0, 1 + i);
    lcd.print(streets[i].name);
    lcd.print(": ");

    lcd.setCursor(12, 1 + i);
    lcd.print(streets[i].availableLots);

    lcd.setCursor(14, 1 + i);
    if (streets[i].availableLots == 1)
    {
      lcd.print("vaga");
    }
    else
    {
      lcd.print("vagas");
    }
  }

  lcd.setCursor(0, 3);
  lcd.print("Hora local:");
}

void scanParkingSensors()
{
  for (int i = 0; i < 4; i++)
  {
    int read = analogRead(sensors[i].pin);
    if (read < distanceInfrared && sensors[i].flag == false)
    {
      if (i == 0 || i == 1)
      {
        updateAvailableLots(0, -1);
      }
      else if (i == 2 || i == 3)
      {
        updateAvailableLots(1, -1);
      }
      sensors[i].flag = true;
      updateSensorStatus(i, true);

      refreshDisplay();
    }
    else if (read > distanceInfrared && sensors[i].flag == true)
    {
      if (i == 0 || i == 1)
      {
        updateAvailableLots(0, 1);
      }
      else if (i == 2 || i == 3)
      {
        updateAvailableLots(1, 1);
      }
      sensors[i].flag = false;
      updateSensorStatus(i, false);

      refreshDisplay();
    }

  }
}

void establishWifiConnection()
{
  lcd.setCursor(3, 0);
  lcd.print("SMART PARKING");
  lcd.setCursor(19, 0);
  lcd.print("X");
  lcd.setCursor(0, 1);
  lcd.print("Conectando WiFi");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    lcd.setCursor(15, 1);
    lcd.print(".");
    delay(500);
    lcd.setCursor(15, 1);
    lcd.print("..");
    delay(500);
    lcd.setCursor(15, 1);
    lcd.print("...");
    lcd.setCursor(15, 1);
    delay(500);
    lcd.print("   ");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    lcd.setCursor(19, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 2);
    lcd.print("conectado/IP local:");
    lcd.setCursor(0, 3);
    lcd.print(WiFi.localIP());
    delay(3000);
  }
}

void updateAvailableLots(int streetIndex, int value)
{
  streets[streetIndex].availableLots = streets[streetIndex].availableLots + value;
}

void displayLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Falha ao obter hora");
    lcd.setCursor(12, 3);
    lcd.print("XX:XX:XX");
    return;
  }

  lcd.setCursor(12, 3);
  lcd.print(&timeinfo, "%H:%M:%S");
}
