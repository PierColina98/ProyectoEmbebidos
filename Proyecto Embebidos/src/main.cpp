#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "Pier iPhone SE"
#define WIFI_PASSWORD "hola1234"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "espgameproject@gmail.com"
#define AUTHOR_PASSWORD "qwdrxnomqoqfjgvm"
#define RECIPIENT_EMAIL "bompir@hotmail.com"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 18
#define DATA_PIN 23
#define CS_PIN 5


#define PERRO 13
#define GATO 12
#define VACA 14
#define POLLO 27
#define PATO 26
#define RANA 25
#define LEON 33
#define CABALLO 32
#define ELEFANTE 22

#define BATT_VOLT 34

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int analog_value;
float battVoltage;


void batteryLevel()
{
  analog_value = analogRead(BATT_VOLT);
  battVoltage = analog_value * (3.3 / 4096);
  if (battVoltage >= 2.48)
  {
    digitalWrite(17, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
  }
  else if ((battVoltage >= 1.65) && (battVoltage < 2.48))
  {
    digitalWrite(17, LOW);
    digitalWrite(16, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
  }
  else if ((battVoltage >= 0.83) && (battVoltage < 1.65))
  {
    digitalWrite(17, LOW);
    digitalWrite(16, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
  }
  else if (battVoltage < 0.83)
  {
    digitalWrite(17, LOW);
    digitalWrite(16, LOW);
    digitalWrite(4, LOW);
    digitalWrite(2, HIGH);
  }
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  //smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("ESP32_Project");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Device Boot-Up Notification");
  message.addRecipient(F("Everyone"), RECIPIENT_EMAIL);

  // Send raw text message
  String textMsg = "Hello World! - Sent from ESP board";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Connect to the server */
  if (!smtp.connect(&config))
  {
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn())
  {
    Serial.println("\nNot yet logged in.");
  }
  else
  {
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

  P.begin();
  pinMode(PERRO, INPUT_PULLUP);
  pinMode(GATO, INPUT_PULLUP);
  pinMode(VACA, INPUT_PULLUP);
  pinMode(POLLO, INPUT_PULLUP);
  pinMode(PATO, INPUT_PULLUP);
  pinMode(RANA, INPUT_PULLUP);
  pinMode(LEON, INPUT_PULLUP);
  pinMode(CABALLO, INPUT_PULLUP);
  pinMode(ELEFANTE, INPUT_PULLUP);
  pinMode(BATT_VOLT, INPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  
}
void loop()
{
  batteryLevel();
  if (P.displayAnimate() && (digitalRead(GATO) == 0))
  {
    Serial.write("GATO");
    P.displayClear();
    P.displayText("Gato", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(PERRO) == 0))
  {
    Serial.write("PERRO");
    P.displayClear();
    P.displayText("Perro", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(CABALLO) == 0))
  {
    Serial.write("CABALLO");
    P.displayClear();
    P.displayText("Caballo", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(LEON) == 0))
  {
    Serial.write("LEON");
    P.displayClear();
    P.displayText("Leon", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(RANA) == 0))
  {
    Serial.write("RANA");
    P.displayClear();
    P.displayText("Rana", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(PATO) == 0))
  {
    Serial.write("PATO");
    P.displayClear();
    P.displayText("Pato", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(POLLO) == 0))
  {
    Serial.write("POLLO");
    P.displayClear();
    P.displayText("Gallina", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(VACA) == 0))
  {
    Serial.write("VACA");
    P.displayClear();
    P.displayText("Vaca", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  if (P.displayAnimate() && (digitalRead(ELEFANTE) == 0))
  {
    Serial.write("ELEFANTE");
    P.displayClear();
    P.displayText("Elefante", PA_CENTER, P.getSpeed(), 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}
