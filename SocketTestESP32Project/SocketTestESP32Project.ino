/*
>>> ขาเชื่อมต่อ <<<
  จอและคีย์แพด
    SCL > 22
    SDA > 21
  PZEM-004T Version 3
    RX > 18
    TX > 19
  GY-NEO6MV2
    RX > 16
    TX > 17
  LDR Sensor
    ตัวที่ 1 > 25
    ตัวที่ 2 > 26
    ตัวที่ 3 > 27
  Relay RCD Test > 13
*/
//--------------------------------------------------------------------------------------------------------------------------------
#include <Wire.h>  /* นำเข้าไรบลารี่ Wire.h */
#include <LiquidCrystal_I2C.h>  /* นำเข้าไรบลารี่ LiquidCrystal_I2C.h */
LiquidCrystal_I2C lcd(0x27, 20, 4);  /* กำหนดค่า lcd ที่ i2C ตำแหน่ง 0x27 ที่ขนาด 20 ตัวอักษร 4 แถว*/

//--------------------------------------------------------------------------------------------------------------------------------
#include <Keypad_I2C.h>  /* นำเข้าไรบลารี่ Keypad_I2C.h */
#include <Keypad.h>  /* นำเข้าไรบลารี่ Keypad */
#define I2CADDR 0x20  /* กำหนด Address ของ i2C (ได้จากการแสกน) */

const byte ROWS = 4;  /* กำหนดจำนวนของ Rows */
const byte COLS = 4;  /* กำหนดจำนวนของ Columns */

/* กำหนด Keypad ที่ใช้งานแบบ (4x4) */
char keys[ROWS][COLS] = { {'1','2','3','A'},
                          {'4','5','6','B'},
                          {'7','8','9','C'},
                          {'*','0','#','D'} };

/*กำหนด Pin ที่ใช้งาน (4x4)*/
byte rowPins[ROWS] = {0, 1, 2, 3};  /* เชื่อมต่อกับ Pin แถวของปุ่มกด (อ้างอิงจากบอร์ขยายขา) */
byte colPins[COLS] = {4, 5, 6, 7}; /* เชื่อมต่อกับ Pin คอลัมน์ของปุ่มกด (อ้างอิงจากบอร์ขยายขา) */

/*
makeKeymap(keys) : กำหนด Keymap
rowPins : กำหนด Pin แถวของปุ่มกด
colPins : กำหนด Pin คอลัมน์ของปุ่มกด
ROWS : กำหนดจำนวนของ Rows
COLS : กำหนดจำนวนของ Columns
I2CADDR : กำหนด Address ขอ i2C
PCF8574 : กำหนดเบอร์ IC
*/
Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574);

//--------------------------------------------------------------------------------------------------------------------------------
#include <PZEM004Tv30.h>  /* นำเข้าไรบลารี่ PZEM004Tv30.h */

#define PZEM_RX_PIN 18  /* กำหนดขา รับข้อมูล */
#define PZEM_TX_PIN 19  /* กำหนดขา ส่งข้อมูล */

#define PZEM_SERIAL Serial2  /* กำหนดให้ PZEM ใช้ Serial2 */
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);  /* เรียกใช้งาน pzem และกำหนดค่าตามตัวแปร */

//--------------------------------------------------------------------------------------------------------------------------------
#include <TinyGPS++.h>  /* นำเข้าไรบลารี่ TinyGPS++.h */
#include <HardwareSerial.h>  /* นำเข้าไรบลารี่ HardwareSerial.h */

#define RXPin (16)  /* กำหนดขา รับข้อมูล */
#define TXPin (17)  /* กำหนดขา ส่งข้อมูล */

static const uint32_t GPSBaud = 9600;  /* กำนดความถี่รับส่งข้อมูลกับ GPS */
TinyGPSPlus GPS;  /* สร้าง GPS ใช้งาน TinyGPSPlus */
HardwareSerial Hardware_Serial(1);  /* สร้าง Hardware_Serial ใช้งาน HardwareSerial(1) */

//--------------------------------------------------------------------------------------------------------------------------------
#include <WiFi.h>  /* นำเข้าไรบลารี่ WiFi++.h */
#include "time.h"  /* นำเข้าไรบลารี่ time++.h */
#include <ESP_Google_Sheet_Client.h>  /* นำเข้าไรบลารี่ ESP_Google_Sheet_Client.h */

#define WIFI_SSID "4GUFI-XX"  /* ชื่อ WiFi */
#define WIFI_PASSWORD "1234567890"  /* รหัส WiFi */

#define PROJECT_ID "33f6e357061aff232e04e9cd02b9a98bdc2d98f0"  /* private key id (ได้จากไฟล์ Json) */
#define CLIENT_EMAIL "elecinspectorkitproject@elecinspectorkitproject.iam.gserviceaccount.com" /* Service Account's client email (ได้จากไฟล์ Json) */

/* Service Account's private key (ได้จากไฟล์ Json) */
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC841Gc/3gAbq0G\nZqmAASAlcwk0w/VUOiqRj2DSSmg75XFuVEqen03Di6l4juOOTjZzLiqse8ATAB6H\nAu8O4UjllMpSI9KWrDloBaYOdIQCdcVMnlT0OpZIImJbVTVFd/Vpwynq3LpiB/1N\nl0yRbZOkmtLeuD816WPsT1YzFjgVqwLhZ8t/EuN65AAntfU6jkL6+5ux8a3eebby\nmabtZ3xJlUuI2lWAOTHmr/NKFgjNuwdl4kohXyhafgYY7vx3Put0meyE/GSp/JAJ\nxbA3GF6mCkxIEleNAw5+//tQfiWnB3gea+JW3wgLXLRgjgCTgQJCamAxBRSFJwzH\nxTHFwAWzAgMBAAECggEASlP1YJ9S3HCw24z4dCt9N99dpwrZdixxZdgS1I19GNy/\n1xeKSYMMeb9yj8NCnlP+rzJijn8f5Rot6Rjpc2SJZ/Y16z63dp0M9fcLEjMdY05u\ntiiz62tGidol9CTCkrSSuOyYiRgUq9DTB35bHfsNlz9pbUT5csCFlo7dr7q1lSiK\nF0XmcE2Fo1rneMZSW8hyjHG7XQwvxYEmqwekBtodrGT2LHb3fo7qrDiqYoFVA5S6\nHcnzNUQaEl9xDFbyD0eu6wfEwfQ3JkM246Y3VZuTSUelbJo5eCAkGtxeD2pZrBMf\nDOC0bnHeqPMwkY1rWUoQcyLuvlUDflwjJ2yW6xuC+QKBgQDiEnQUIfCbpdNfbBXg\nhsBj5vII0QqF17YUBLKFrXzYZdcjsDGUywK2E/G2BXLFEOUJ/s7QBDEANopcpgD8\nQI4Fjxlw0UK6vZ/C47yCVUti7P5yOQFHKs3PRT8iAvuntOBjDtpKp9Rsc2VXpiEx\nNay7p6GMgwGRnJipzyAgc1sKhQKBgQDV5LOXshE3l+k4dLsUiFZH72o7aBHNpZKR\nJhXBTZgasoefYSuCAWqG8x7mTvRUtpMgY6F/nwTE29G34nJKSNbva0cCeEjLx0/5\ne42uYAokkBHKeLC8Bd+i/7ymj0ASS+Hcr+Cs8f5/BDLeIqpi64nyeUcs3l6AV/ek\n9V8cV/Bw1wKBgQCRJyYO0yfo2BUafIyjaAHZYyxeStIF09XdjpKNUDILw3/aM0Py\ndFKeU7nrlB33t/xHaaqiVz6bE2nNSOzzCgPmP9Ym4MWGn0AX4x9jU56euuo4rq9K\nYYpZne2GSgII0RMNpziznITAyWr1Vv6S2RK6Pkx0qpz0QGapCQtoqoh2/QKBgDhY\nNrcPQVnTuqwUk/OktpXci696irYRl7qEvdZeCmN+BBwIDN4CYX2LLeidg9+M+NTf\nafukeC0jeutly5Wl+ni3Wc1cwlBsALVHxtrYkdNv4D/l6yico3zcwWVe9xHay3TH\nH/hvBwuOn5fEqf3E7mHhcM5xBZPpmGLsAFIHGF+hAoGBAM24hT1GY5Zdw6/QsRct\nB9l6UcUppXClL6AJfHgmK0C7IXSA8HuQfMPgMjj/Di4Tb5xrdRNQ1N4E0LHLW1+Q\ns6rzy+qgtGLB5fM/O8AGq2ftXpi0TCKgVYJODiAxtB5sUo4xQ91HYcs3s3qUaT0e\nqIlr7IbD7W005S3b+btx09RX\n-----END PRIVATE KEY-----\n";

/* spreadsheet ID  (ได้จากไฟล์ URL ไฟล์บน Sheets ที่จะบันทึก) */
const char spreadsheetId[] = "1Qs7wgiyjuMRWI2am8PvV_5oslQT7ceF5fZqhiif8m68";

/* ชุดตัวแปรชนิด จำนวนเต็มแบบยาวไม่ติดลบ สำหรับนับเวลาของ Gsheets */
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;

// Token Callback function
/* ฟังก์ชันการตอบกลับเมื่อใช้งาน Gsheets */
void tokenStatusCallback(TokenInfo info);

/* ชุดตัวแปรชนิด จำนวนเต็มแบบยาวไม่ติดลบ สำหรับนับเวลาของ Wifi เมื่อหลุดการเชื่อมต่อ */
unsigned long previousMillis = 0;
unsigned long interval = 30000;

//--------------------------------------------------------------------------------------------------------------------------------
const char* ntpServer = "pool.ntp.org";  /* กำหนด Server สำหรับดึงเวลา */
const long  gmtOffset_sec = 7 * 3600;  /* กำหนด Time Zone */
const long   daylightOffset_sec = 7 * 3600;  /* กำหนด Time Zone */

//--------------------------------------------------------------------------------------------------------------------------------
int ELCB_Pin = 13;  /* ตัวแปรจำนวนเต็ม สำหรับขา Relay เพื่อทดสอบ ELCB */

//--------------------------------------------------------------------------------------------------------------------------------
/* ชุดตัวแปรจำนวนเต็ม สำหรับขา Sensor วัดแสง เพื่อตรวจสอบสถานะเครื่อง Socket Test */
int LED_Pin1 = 25;  
int LED_Pin2 = 26;
int LED_Pin3 = 27;

//--------------------------------------------------------------------------------------------------------------------------------
String Buffer_ID = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ ID ผู้ถูกตรวจสอบ ใช้ชั่วคราว */
String Save_ID ="";  /* ตัวแปรชุดอักขรสำหรับเก็บ ID ผู้ถูกตรวจสอบ ใช้บันทึก */

String Buffer_GPS_Lat = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ ตำแหน่งเส้นรุ้ง ที่ได้จาก GPS ใช้ชั่วคราว */
String Buffer_GPS_Lng = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ ตำแหน่งเส้นแวง ที่ได้จาก GPS ใช้ชั่วคราว */
String Save_GPS_Lat = "";  /* ตัวแปรชุดอักขรสำหรับเก็บ ตำแหน่งเส้นรุ้ง ที่ได้จาก GPS ใช้บันทึก */
String Save_GPS_Lng = "";  /* ตัวแปรชุดอักขรสำหรับเก็บ ตำแหน่งเส้นแวง ที่ได้จาก GPS ใช้บันทึก */

String Buffer_StatusBumblebee = "Fail";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะเครื่อง Bumblebee ใช้ชั่วคราว */
int Buffer_Voltage = 0;  /* ตัวแปรจำนวนเต็ม สำหรับเก็บ แรงดันไฟฟ้า */
int Buffer_Frequency = 0;  /* ตัวแปรจำนวนเต็ม สำหรับเก็บ ความถี่ไฟฟ้า */
String Save_StatusBumblebee = "Fail";  /* ตัวแปรชุดอักขรสำหรับเก็บ สถานะเครื่อง Bumblebee ใช้บันทึก */

String Buffer_SocketTest = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะเครื่อง Socket Test ใช้ชั่วคราว */
String Save_SocketTest = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะเครื่อง Socket Test ใช้บันทึก */

String Save_ELCB = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะเครื่อง ELCB ใช้บันทึกห */
String BufferDate = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะเครื่อง วันที่ ใช้ชั่วคราว */
String BufferTime = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะเครื่อง เวลา ใช้ชั่วคราว */

int CountGSheet = 0;  /* ตัวแปรจำนวนเต็ม สำหรับเก็บ รับการเช็คเวลาก่อนบันทึก */
String StatusWiFi = "";  /* ตัวแปรชุดอักขร สำหรับเก็บ สถานะ WiFi */

//--------------------------------------------------------------------------------------------------------------------------------
String Buffer_IndexMenu[10];  /* ตัวแปรอาเรย์ ชนิดชุดอักขระ สำหรับเก็บชิ้อหน้าแสดงผล */
int Count_IndexMenu = 0;  /* ตัวแปรจำนวนเต็ม สำหรับเก็บ จำนวนหน้าแสดงผล */

char Buffer_IndexID[8];  /* ตัวแปรอาเรย์ ชนิดอักขระ สำหรับเก็บหมายเลข ID */
int Count_IndexID = 0;  /* ตัวแปรจำนวนเต็ม สำหรับเก็บ จำนวนการกด ID */

//--------------------------------------------------------------------------------------------------------------------------------
TaskHandle_t TaskDisplay;  /* กำหนด TaskDisplay เพื่อสร้างงานใน TaskHandle_t */ 
TaskHandle_t TaskKeypad;  /* กำหนด TaskKeypad TaskKeypad TaskHandle_t */
TaskHandle_t TaskGPS; /* กำหนด TaskGPS TaskKeypad TaskHandle_t */
TaskHandle_t TaskPZEM; /* กำหนด TaskPZEM TaskKeypad TaskHandle_t */
TaskHandle_t TaskSocket; /* กำหนด TaskSocket TaskKeypad TaskHandle_t */
TaskHandle_t TaskGetTime; /* กำหนด TaskGetTime TaskKeypad TaskHandle_t */
TaskHandle_t TaskGSheet; /* กำหนด TaskGSheet TaskKeypad TaskHandle_t */

//================================================================================================================================
/* ฟังกชันเชื่อมต่อ WiFi */
void initWiFi()
{
  //--------------------------------------------------------------------------------------------------------------------------------
  /* แสดงผลออก lcd */
  lcd.setCursor(0, 0);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("  Wifi Connecting!  ");
  
  lcd.setCursor(0, 1);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("    Please wait.    ");

  //--------------------------------------------------------------------------------------------------------------------------------
  delay(1000);  /* หน่วงเวลา 1000 มิลลิวินาที*/

  //--------------------------------------------------------------------------------------------------------------------------------
  WiFi.mode(WIFI_STA);  /* กำหนดโหมด WiFi */
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  /* เริ่มเชื่อมต่อ WiFi */

  Serial.print("Connecting to WiFi ..");
  StatusWiFi = "Connecting";  /* เก็บสถานะ WiFi */
  
  //--------------------------------------------------------------------------------------------------------------------------------
  /*  วนซ้ำหากยังเชื่อมต่อไม่ได้ */
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }

  //--------------------------------------------------------------------------------------------------------------------------------
  /* แจ้งข้อมูลการเชื่อมต่อ ออก Serial*/
  Serial.println();
  Serial.println(String("SSID: ") + WIFI_SSID);
  Serial.println(String("IP: ") + (WiFi.localIP().toString()));
  Serial.println(String("RSSI: ") + WiFi.RSSI() + String("dBm."));

  //--------------------------------------------------------------------------------------------------------------------------------
  /* แสดงผลออก lcd */
  lcd.setCursor(0, 0);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("  Wifi Successful.  ");
  lcd.setCursor(0, 1);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print(String("SSID: ") + WIFI_SSID);
  lcd.setCursor(0, 2);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print(String("IP: ") + (WiFi.localIP().toString()));
  lcd.setCursor(0, 3);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print(String("RSSI: ") + WiFi.RSSI() + String("dBm.   "));

  //--------------------------------------------------------------------------------------------------------------------------------
  delay(3000);  /* หน่วงเวลา 3000 มิลลิวินาที */

}

//================================================================================================================================
/* ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
void PrintSerialMenu()
{
  Serial.println("--------------------------------------------");
  Serial.println(String("Count_IndexMenu: ") + Count_IndexMenu);
  Serial.print("Buffer_IndexID: ");
  for (int i = 0;  i < Count_IndexMenu; i++)
  {
    Serial.print(String("|") + Buffer_IndexMenu[i]);
  }
  Serial.println("|");
}

//================================================================================================================================
/* ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */
void PrintSerialID()
{
  Serial.println("--------------------------------------------");
  Serial.println(String("Count_IndexID: ") + Count_IndexID);
  Serial.print("Buffer_IndexID: ");
  for (int i = 0;  i < Count_IndexID; i++)
  {
    Serial.print(String("|") + String(Buffer_IndexID[i]));
  }
  Serial.println("|");
}

//================================================================================================================================
/* ฟังก์ชันตั้งค่า */
void setup() {

  //--------------------------------------------------------------------------------------------------------------------------------
  Serial.begin(115200);  /* กำหนดความถี่ Serial Monitor */
  Serial.println("--------------------------------------------");

  //--------------------------------------------------------------------------------------------------------------------------------
  Wire.begin();  /* เรียกการเชื่อมต่อ Wire */
  keypad.begin(makeKeymap(keys));  /* เรียกการเชื่อมต่อ keypad */

  //--------------------------------------------------------------------------------------------------------------------------------
  /* กำหนด pinMode ให้ขา Sensor แสง */
  pinMode(LED_Pin1, INPUT);
  pinMode(LED_Pin2, INPUT);
  pinMode(LED_Pin3, INPUT);

  //--------------------------------------------------------------------------------------------------------------------------------
  pinMode(ELCB_Pin, OUTPUT);  /* กำหนด pinMode ให้ขา รีเลย์ ELCB */
  digitalWrite(ELCB_Pin, HIGH);  /* digitalWrite ให้ขา รีเลย์ ELCB  เป็น HIGH (Active: :LOW) */

  //--------------------------------------------------------------------------------------------------------------------------------
  lcd.begin();  /* เริ่มใช้งาน lcd */
  lcd.clear();  /* เคลียร์หน้าจอ lcd */

  //--------------------------------------------------------------------------------------------------------------------------------
  /* แสดงผลออก lcd (หน้าแรก) */
  lcd.setCursor(0, 0);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("  ElecInspectorKit  ");
  lcd.setCursor(0, 1);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("By Rung&Ball Project");
  lcd.setCursor(0, 2);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("TL.B.Elec.Technology");
  lcd.setCursor(0, 3);
  // -------"XXXXXXXXXXXXXXXXXXXX"
  lcd.print("     STC iVenr3     ");

  //--------------------------------------------------------------------------------------------------------------------------------
  delay(3000);  /* หน่วงเวลา 3000 มิลลิวินาที*/
  lcd.clear();  /* เคลียร์หน้าจอ lcd */

  //--------------------------------------------------------------------------------------------------------------------------------
  Hardware_Serial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin, false);  /* เริ่มเชื่อมต่อ Hardware_Serial */
  Serial.println(String("GPS Library Version: ") + TinyGPSPlus::libraryVersion());  /* แสดงผล library Version */
  
  //--------------------------------------------------------------------------------------------------------------------------------
  initWiFi();  /* เรียกฟังก์ชัน เชื่อมต่อ WiFi */

  //--------------------------------------------------------------------------------------------------------------------------------
  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);  /* แสดงผล Google Sheet Version */

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  /* กำหนดค่าเพื่อใช้งานเวลาจาก Server */

  /* ตั้งค่าการโทรกลับสำหรับสถานะการสร้างโทเค็นการเข้าถึง Google API (สำหรับการดีบักเท่านั้น) */
  GSheet.setTokenCallback(tokenStatusCallback);

  /* ตั้งค่าวินาทีเพื่อรีเฟรชโทเค็นการรับรองความถูกต้องก่อนที่จะหมดอายุ (60 ถึง 3540 ค่าเริ่มต้นคือ 300 วินาที) */
  GSheet.setPrerefreshSeconds(10 * 60);

  /* เริ่มต้นการสร้างโทเค็นการเข้าถึงสำหรับการตรวจสอบสิทธิ์ Google API */
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

  //--------------------------------------------------------------------------------------------------------------------------------
  /*
  Task1code, ฟังก์ชั่นงาน
  "Task1", ชื่อของงาน
  10,000, ขนาดสแต็กของงาน
  NULL, พารามิเตอร์ของงาน
  1, ลำดับความสำคัญของงาน
  &Task1, ตัวจัดการงานเพื่อติดตามงานที่สร้างขึ้น
  0); ปักหมุดงานไว้ที่ core 0
   */

  xTaskCreatePinnedToCore(Task_Display, "TaskDisplay/Core0", 10000, NULL, 1, &TaskDisplay, 0);
  xTaskCreatePinnedToCore(Task_Keypad, "TaskKeypad/Core0", 10000, NULL, 1, &TaskKeypad, 0);
  xTaskCreatePinnedToCore(Task_GPS, "TaskGPS/Core1", 10000, NULL, 1, &TaskGPS, 1);
  xTaskCreatePinnedToCore(Task_PZEM, "TaskPZEM/Core1", 10000, NULL, 1, &TaskPZEM, 1);
  xTaskCreatePinnedToCore(Task_Socket, "TaskSocket/Core1", 10000, NULL, 1, &TaskSocket, 1);
  xTaskCreatePinnedToCore(Task_GetTime, "TaskGetTime/Core0", 10000, NULL, 1, &TaskGetTime, 1);
  xTaskCreatePinnedToCore(Task_GSheet, "TaskGSheet/Core0", 10000, NULL, 1, &TaskGSheet, 1);

  //--------------------------------------------------------------------------------------------------------------------------------
  vTaskSuspend(TaskPZEM);  /* TaskPZEM หยุดงาน */
  vTaskSuspend(TaskSocket); /* TaskSocketหยุดงาน */

}

//================================================================================================================================
/* ฟังก์ชันงาน แสดงผล */
void Task_Display(void* pvParameters)
{
  for (;;)  /* วนซ้ำ */
  {
    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd ตรวจไม่พบโมดูล GPS */
    if (
    Buffer_IndexMenu[0] == "Detected Fail" and 
    Buffer_IndexMenu[1] == "" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Status GPS:     ");
      lcd.setCursor(0, 1);
      // --------------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(String("   ") + Buffer_IndexMenu[0]);
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("   Please Turn OFF  ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("And Checking Module!");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd กำลังพยายามเชื่อมต่อดาวเทียม */
    else if (
    Buffer_IndexMenu[0] == "Satellite Checking" and 
    Buffer_IndexMenu[1] == "" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Status: GPS     ");
      lcd.setCursor(0, 1);
      // --------------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(String(" ") + Buffer_IndexMenu[0]);
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please wait.    ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("                    ");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd ตำแหน่งที่ได้รับจากดาวเทียม */
    else if (
    Buffer_IndexMenu[0] == "Load Location" and 
    Buffer_IndexMenu[1] == "" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(String("   ") + Buffer_IndexMenu[0]);
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Lat: " + Buffer_GPS_Lat + "      ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Long: " + Buffer_GPS_Lng + "    ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("             'A'Save"); 
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้าแรกป้อน ID */
    else if (
    Count_IndexID == 0 and 
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please Enter    ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    ID:________     ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'B'Back             ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("                    ");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้าป้อน ID ยังไม่ครบ */
    else if (
    Count_IndexID > 0 and Count_IndexID < int(sizeof(Buffer_IndexID)) and 
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please Enter    ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    ID:" + Buffer_ID + "_");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'B'Back    'D'Delete");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'C'Clear            ");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้าป้อน ID ครบ */
    else if (
    Count_IndexID >= int(sizeof(Buffer_IndexID)) and 
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please Enter    ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    ID:" + Buffer_ID);
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'B'Back    'D'Delete");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'C'Clear     'A'Save");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้าสถานะ Bumblebee */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("      EKT Test      ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("VAC:" + String(Buffer_Voltage) + "V. Fq:" + String(Buffer_Frequency) + "Hz.");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Status:" + Buffer_StatusBumblebee + "         ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'B'Back      'A'Save");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้าสถานะ Socket Test */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("     Socket Test    ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Status:" + Buffer_SocketTest);
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'B'Back      'A'Save");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้าก่อนทดสอบ ELCB */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "Socket Test" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("RCD Test   :Warning:");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(" After this, cannot ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("  be edited again.  ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'B'Back      'A'Save");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้า ELCB ทำงาน */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "Socket Test" and 
    Buffer_IndexMenu[4] == "ELCB Working" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
           
      //--------------------------------------------------------------------------------------------------------------------------------
      Serial.println("RCD Test / Time Countdown: 3");
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("      RCD Test      ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("   Time Countdown   ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("          3         ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please wait.    ");

      //--------------------------------------------------------------------------------------------------------------------------------
      delay(1000);  /* หน่วงเวลา 1000 มิลลิวินาที*/

      //--------------------------------------------------------------------------------------------------------------------------------
      Serial.println("RCD Test / Time Countdown: 2");
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("      RCD Test      ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("   Time Countdown   ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("          2         ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please wait.    ");

      //--------------------------------------------------------------------------------------------------------------------------------
      delay(1000);  /* หน่วงเวลา 1000 มิลลิวินาที*/

      //--------------------------------------------------------------------------------------------------------------------------------
      Serial.println("RCD Test / Time Countdown: 1");
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("      RCD Test      ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("   Time Countdown   ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("          1         ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please wait.    ");

      //--------------------------------------------------------------------------------------------------------------------------------
      delay(1000);  /* หน่วงเวลา 1000 มิลลิวินาที*/

      //--------------------------------------------------------------------------------------------------------------------------------
      Serial.println("RCD Test / Working");
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("      RCD Test      ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("       Working      ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("                   ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please wait.    ");

      //--------------------------------------------------------------------------------------------------------------------------------
      digitalWrite(ELCB_Pin, LOW);  /* digitalWrite ให้ขา รีเลย์ ELCB  เป็น LOW (Active: :LOW) */
      delay(2000);  /* หน่วงเวลา 2000 มิลลิวินาที*/
      digitalWrite(ELCB_Pin, HIGH);  /* digitalWrite ให้ขา รีเลย์ ELCB  เป็น LOW (Active: :LOW) */

      //--------------------------------------------------------------------------------------------------------------------------------
      Buffer_IndexMenu[Count_IndexMenu] = "ELCB Finish";  /* บันทึกหน้า ELCB Finish ตำแหน่ง Count_IndexMenu */
      Count_IndexMenu++;  /* เพิ่มค่าตัวแปร Count_IndexMenu ขึ้น 1 */

      //--------------------------------------------------------------------------------------------------------------------------------
      PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

      //--------------------------------------------------------------------------------------------------------------------------------
      vTaskResume(TaskSocket);  // TaskSocket หยุดทำงาน
    
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้า สถานะก่อนอัพโหลด */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "Socket Test" and 
    Buffer_IndexMenu[4] == "ELCB Working" and 
    Buffer_IndexMenu[5] == "ELCB Finish" and
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      // -------"     ID:"XXXXXXX    "
      lcd.print("     ID:" + Save_ID + "    ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("RCD:" + Save_ELCB + "     ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'*' Info    '#'Reset");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("'A'Upload           ");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้า ข้อมูลก่อนอัพโหลด */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "Socket Test" and 
    Buffer_IndexMenu[4] == "ELCB Working" and 
    Buffer_IndexMenu[5] == "ELCB Finish" and
    Buffer_IndexMenu[6] == "Information" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Lat:" + Save_GPS_Lat + "     ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Lng:" + Save_GPS_Lng + "      ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("EKT:" + Save_StatusBumblebee + "             ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("Sock:" + Save_SocketTest);
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้า ดึงเวลาจาก Server */
    else if (
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "Socket Test" and 
    Buffer_IndexMenu[4] == "ELCB Working" and 
    Buffer_IndexMenu[5] == "ELCB Finish" and
    Buffer_IndexMenu[6] == "Getting Time" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {  
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(" Getting Date Time  ");
      lcd.setCursor(0, 1);
      // --------------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(String("      ") + BufferTime + String("      "));
      // lcd.print(String(" ") + BufferDate);
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Please wait.    ");

    }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้า กำลังอัพโหลด */
    if (Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "Getting Time" and 
      Buffer_IndexMenu[7] == "Uploading" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {

        lcd.setCursor(0, 0);
        // -------"XXXXXXXXXXXXXXXXXXXX"
        lcd.print("      Progress      ");
        lcd.setCursor(0, 1);
        // -------"XXXXXXXXXXXXXXXXXXXX"
        lcd.print(" Uploading to cloud ");
        lcd.setCursor(0, 2);
        // -------"XXXXXXXXXXXXXXXXXXXX"
        lcd.print("with Google Sheets. ");
        lcd.setCursor(0, 3);
        // -------"XXXXXXXXXXXXXXXXXXXX"
        lcd.print("    Please wait.    ");

      }

    //--------------------------------------------------------------------------------------------------------------------------------
    /* แสดงผล lcd หน้า จบการทำงาน */
    else if (
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "Getting Time" and 
      Buffer_IndexMenu[7] == "Uploading" and 
      Buffer_IndexMenu[8] == "Uploaded" and 
      Buffer_IndexMenu[9] == "")
      {
      lcd.setCursor(0, 0);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    Successfully    ");
      lcd.setCursor(0, 1);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(" Uploaded to cloud  ");
      lcd.setCursor(0, 2);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print(" with Google Sheets ");
      lcd.setCursor(0, 3);
      // -------"XXXXXXXXXXXXXXXXXXXX"
      lcd.print("    '#' New Test    ");
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    delay(1);  /* หน่วงเวลา 1 มิลลิวินาที*/

   }
}

//================================================================================================================================
/* ฟังก์ชันงาน Keypad */
void Task_Keypad(void* pvParameters) {

  for (;;)  /* วนซ้ำ */
  {
    //--------------------------------------------------------------------------------------------------------------------------------
    char Custom_Key = keypad.getKey();  /* สร้างตัวแปรชื่อ Custom_Key ชนิด char เพื่อเก็บตัวอักขระที่กด */

    //--------------------------------------------------------------------------------------------------------------------------------
    if (Custom_Key)  /* ถ้าหากตัวแปร Custom_Key มีอักขระ */
    { 
      Serial.println("--------------------------------------------");
      //--------------------------------------------------------------------------------------------------------------------------------
      Serial.println(String("Key: ") + String(Custom_Key));  /* แสดงผล Custom_Key ออก Serial */

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Load Location 
      เหตุการ: กด Save Location
      */
      if (
      Custom_Key == 'A' and 
      Buffer_IndexMenu[0] == "Load Location" and 
      Buffer_IndexMenu[1] == "" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        
        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskSuspend(TaskGPS);  /* หยุด TaskGPS */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Save Location";  /* บันทึกหน้า Save Location ตำแหน่ง Count_IndexMenu */
        Count_IndexMenu++;  /* เพิ่มค่าตัวแปร Count_IndexMenu ขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        Save_GPS_Lat = Buffer_GPS_Lat;  /* ให้ Save_GPS_Lat เท่ากับ Buffer_GPS_Lat */
        Save_GPS_Lng = Buffer_GPS_Lng;  /* ให้ Save_GPS_Lng เท่ากับ Buffer_GPS_Lng */

        //--------------------------------------------------------------------------------------------------------------------------------
        /* แสดงผล ตำแหน่ง GPS ออกทาง Serial */
        Serial.println(String("Save_GPS_Lat: ") + Save_GPS_Lat);
        Serial.println(String("Save_GPS_Lng: ") + Save_GPS_Lng);
  
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: ID 
      เหตุการ: กดกลับหน้า Load Location (เคลียร์ค่า ID)
      */
      else if ( 
      Custom_Key == 'B' and 
      Buffer_IndexMenu[0] == "Save Location" and
      Buffer_IndexMenu[1] == "" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        
        //--------------------------------------------------------------------------------------------------------------------------------
        Count_IndexMenu--;  /* เพิ่มค่าตัวแปร Count_IndexMenu ลดลง 1 */
        Buffer_IndexMenu[Count_IndexMenu] = "";  /* เคลียร์บันทึกหน้า ตำแหน่ง Count_IndexMenu */
        
        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        Save_GPS_Lat = "";  /* ให้ Save_GPS_Lat เท่ากับ ว่าง */
        Save_GPS_Lng = "";  /* ให้ Save_GPS_Lng เท่ากับ ว่าง */

        //--------------------------------------------------------------------------------------------------------------------------------
        /* แสดงผล ตำแหน่ง GPS ออกทาง Serial */
        Serial.println(String("Save_GPS_Lat: ") + Save_GPS_Lat);
        Serial.println(String("Save_GPS_Lng: ") + Save_GPS_Lng);

        //--------------------------------------------------------------------------------------------------------------------------------
        /* วนเคลียร์ค่าใน Buffer_IndexID */
        for (int i = Count_IndexID;  i > 0; i--)
        {
          Buffer_IndexID[i] = NULL;  /* ให้ Buffer_IndexID ตำแหน่ง i เท่ากับ ว่าง */
          Count_IndexID--;  /* ให้ Count_IndexID ลดลง 1 */
        }

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_ID = "";  /* ให้ Buffer_ID เท่ากับ ว่าง */
        Serial.println(String("Buffer_ID: ") + Buffer_ID);  /* แสดงผล ตำแหน่ง Buffer_ID ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskResume(TaskGPS);  /* TaskGPS กลับมาทำงาน */

      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: ID 
      เหตุการ: กดหมายเลข ID
      */
      else if ((
      Custom_Key == '0' or Custom_Key == '1' or 
      Custom_Key == '2' or Custom_Key == '3' or
      Custom_Key == '4' or Custom_Key == '5' or 
      Custom_Key == '6' or Custom_Key == '7' or
      Custom_Key == '8' or Custom_Key == '9') and
      Count_IndexID < int(sizeof(Buffer_IndexID)) and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        
        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexID[Count_IndexID] = Custom_Key;  /* บันทึก Custom_Key ตำแหน่ง Count_IndexID บนตัวแปร Buffer_IndexID */
        Count_IndexID++;  /* เพิ่มค่าตัวแปร Count_IndexID ขึ้น 1 */
        
        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_ID = Buffer_IndexID;  /* ให้ Buffer_ID เท่ากับ Buffer_IndexID */
        Serial.println(String("Buffer_ID: ") + Buffer_ID);  /* แสดงผล Buffer_ID ออกทาง Serial */

      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: ID 
      เหตุการ: กดลบ ID
      */
      else if (
      Count_IndexID != 0 and 
      Custom_Key == 'D' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        Count_IndexID--;  /* ให้ Count_IndexID ลดลง 1*/
        Buffer_IndexID[Count_IndexID] = NULL;  /* ให้ Buffer_IndexID ตำแหน่ง Count_IndexID เท่ากับ ว่าง */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_ID = String(Buffer_IndexID);  /* ให้ Buffer_ID เท่ากับ Buffer_IndexID แปลงเป็นชุดอักขระ */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: ID
      เหตุการ: กดเคลียร์ ID
      */
      else if (
      Count_IndexID != 0 and 
      Custom_Key == 'C' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        /* วนเคลียร์ค่าใน Buffer_IndexID */
        for (int i = Count_IndexID;  i > 0; i--)
        {
          Buffer_IndexID[i] = NULL;  /* ให้ Buffer_IndexID ตำแหน่ง i เท่ากับ ว่าง */
          Count_IndexID--;  /* ให้ Count_IndexID ลดลง 1 */
        }

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_ID = "";  /* ให้ Buffer_ID เท่ากับ ว่าง */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: ID ไปหน้า
      เหตุการ: บันทึก ID ไปหน้า Bumblebee Test
      */
      else if (
      Count_IndexID == int(sizeof(Buffer_IndexID)) and 
      Custom_Key == 'A' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {

        Serial.println("--------------------------------------------");
        Save_ID = Buffer_ID;  /* ให้ Save_ID เท่ากับ Buffer_ID */
        Serial.println(String("Save_ID: ") + Save_ID);  /* แสดงผล Save_ID ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Save ID";  /* บันทึกหน้า Save ID ไปที่ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu */
        Count_IndexMenu++;  /* ให้ Count_IndexMenu เพิ่มขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskResume(TaskPZEM);  /* TaskPZEM ทำงาน*/
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: Bumblebee Test
      เหตุการ์: กลับหน้า ID
      */
      else if (
      Custom_Key == 'B' and  
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        Count_IndexMenu--;  /* ให้ Count_IndexMenu ลดลง 1 */
        Buffer_IndexMenu[Count_IndexMenu] = "";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับ ว่าง */
        
        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskSuspend(TaskPZEM);  /* TaskPZEM หยุด */

      }
      
      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Bumblebee
      เหตุการณ์: กดบันทึก Bumblebee Test ไปหน้า Socket Test
      */
      else if (
      Custom_Key == 'A' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and
      Buffer_IndexMenu[2] == "" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        Serial.println("--------------------------------------------");
        Save_StatusBumblebee = Buffer_StatusBumblebee;  /* ให้ Save_StatusBumblebee เท่ากับ Buffer_StatusBumblebee */
        Serial.println(String("Save_StatusBumblebee: ") + Save_StatusBumblebee);  /* แสดงผล Save_StatusBumblebee ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Bumblebee Test";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า Bumblebee Test */
        Count_IndexMenu++;  /* ให้ Count_IndexMenu เพิ่มขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskSuspend(TaskPZEM);  /* TaskPZEM หยุด */
        vTaskResume(TaskSocket);  /* TaskSocket ทำงาน */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Socket Test
      เหตุการณ์: กลับไปหน้า Bumblebee Test
      */
      else if (
      Custom_Key == 'B' and
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        Count_IndexMenu--;  /* ให้ Count_IndexMenu ลดลง 1 */
        Buffer_IndexMenu[Count_IndexMenu] = "";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เป็นหน้า ว่าง */
        
        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskResume(TaskPZEM);  /* TaskPZEM ทำงาน */
        vTaskSuspend(TaskSocket);  /* TaskSocket หยุดทำงาน */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Socket Test
      เหตุการณ์: กดบันทึก Socket Test ไปหน้า ELCB Test
      */
      else if (
      Custom_Key == 'A' and
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {

        //--------------------------------------------------------------------------------------------------------------------------------
        Serial.println("--------------------------------------------");
        Save_SocketTest = Buffer_SocketTest;  /* ให้ Save_SocketTest เท่ากับ Buffer_SocketTest */
        Serial.println(String("Save_SocketTest: ") + Save_SocketTest);  /* แสดงผล Save_SocketTest ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Socket Test";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า Socket Test*/
        Count_IndexMenu++;  /* ให้ Count_IndexMenu เพิ่มขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskSuspend(TaskSocket);  /* TaskSocket หยุดทำงาน */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า ELCB Test
      เหตุการณ์: กลับไปหน้า Socket Test
      */
      else if ( 
      Custom_Key == 'B' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        Count_IndexMenu--;  /* ให้ Count_IndexMenu ลดลง 1 */
        Buffer_IndexMenu[Count_IndexMenu] = "";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า ว่าง */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า ELCB Test
      บันทึก ELCB Test ไปหน้า ELCB Working
      */
      else if ( 
      Custom_Key == 'A' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "" and 
      Buffer_IndexMenu[5] == "" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "ELCB Working";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า ELCB Working */
        Count_IndexMenu++;  /* ให้ Count_IndexMenu เพิ่มขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Pre Upload
      เหตุการ กดไปหน้า Information
      */
      else if ( 
      Custom_Key == '*' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        lcd.clear();  /* เคลียหน้าจอ lcd */
        delay(100);  /* หน่วงเวลา 100 มิลลิวินาที */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Information";  /* บันทึกหน้าตำแหน่ง */
        Count_IndexMenu++;  /* ให้ Count_IndexMenu เพิ่มขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */ 
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Information
      เหตุการ กดกลับหน้า Pre Upload
      */
      else if (  
      Custom_Key == '*' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "Information" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        lcd.clear();  /* เคลียหน้าจอ lcd */
        delay(100);  /* หน่วงเวลา 100 มิลลิวินาที */

        //--------------------------------------------------------------------------------------------------------------------------------
        Count_IndexMenu--;  /* ให้ Count_IndexMenu ลดลง 1 */
        Buffer_IndexMenu[Count_IndexMenu] = "";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า ว่าง */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: Pre Upload
      เหตุการ์: รีเซตค่า
      */
      else if (
      Custom_Key == '#' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        /* เคลียค่าในตัวแปร */
        Buffer_ID = "";
        Save_ID = "";  

        Buffer_GPS_Lat = "";
        Buffer_GPS_Lng = "";
        Save_GPS_Lat = "";
        Save_GPS_Lng = "";

        Buffer_StatusBumblebee = "Fail";
        Buffer_Voltage = 0;
        Buffer_Frequency = 0;
        Save_StatusBumblebee = "Fail";

        Buffer_SocketTest = "";
        Save_SocketTest = "";

        Save_ELCB = "";

        //--------------------------------------------------------------------------------------------------------------------------------
        /* วนเคลียร์ค่าใน Buffer_IndexID */
        for (int i = Count_IndexID;  i > 0; i--)
        {
          Buffer_IndexID[i] = NULL;
          // Count_IndexID--;
          delay(1);
        }

        Count_IndexID = 0;
        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

        // --------------------------------------------------------------------------------------------------------------------------------
        /* เคลียร์ค่าใน Buffer_IndexMenu */
        Count_IndexMenu = 0;
        Buffer_IndexMenu[0] = "Load Location"; 
        Buffer_IndexMenu[1] = ""; 
        Buffer_IndexMenu[2] = ""; 
        Buffer_IndexMenu[3] = "";  
        Buffer_IndexMenu[4] = ""; 
        Buffer_IndexMenu[5] = ""; 
        Buffer_IndexMenu[6] = ""; 
        Buffer_IndexMenu[7] = ""; 
        Buffer_IndexMenu[8] = ""; 
        Buffer_IndexMenu[9] = "";

        //-------------------------------------------------------------------------------------------------------------------------------- 
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
 
        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskResume(TaskGPS);  /* TaskGPS ทำงาน */
        vTaskSuspend(TaskPZEM);  /* TaskPZEM หยุดงาน */
        vTaskSuspend(TaskSocket); /* TaskSocketหยุดงาน */

      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า Pre Upload
      เหตุการณ์ กด Pre Upload ไปหน้า Successfully
      */
      else if ( 
      Custom_Key == 'A' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {
        //-------------------------------------------------------------------------------------------------------------------------------- 
        /* แสดงผลค่าก่อนบันทึก ออกทาง Serial */
        Serial.println("--------------------------------------------");
        Serial.println(">>>>>>>>> Info Upload to GSeets <<<<<<<<<<<<");
        Serial.println(String("Location Lat: ") + Save_GPS_Lat + ("    lng: ") + Save_GPS_Lng);
        Serial.println(String("ID: ") + Save_ID);
        Serial.println(String("Bumblebee: ") + Save_StatusBumblebee);
        Serial.println(String("SocketTest: ") + Save_SocketTest);
        Serial.println(String("ELCB: ") + Save_ELCB);

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Getting Time";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า Getting Time */
        Count_IndexMenu++;  /* ให้ Count_IndexMenu เพิ่มขึ้น 1 */

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      /*
      หน้า: Successfully
      เหตุการ์: เริ่มใหม่
      */
      else if (
      Custom_Key == '#' and 
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "Getting Time" and 
      Buffer_IndexMenu[7] == "Uploading" and 
      Buffer_IndexMenu[8] == "Uploaded" and 
      Buffer_IndexMenu[9] == "")
      {
        //--------------------------------------------------------------------------------------------------------------------------------
        /* เคลียค่าในตัวแปร */
        Buffer_ID = "";
        Save_ID = "";  

        Buffer_GPS_Lat = "";
        Buffer_GPS_Lng = "";
        Save_GPS_Lat = "";
        Save_GPS_Lng = "";

        Buffer_StatusBumblebee = "Fail";
        Buffer_Voltage = 0;
        Buffer_Frequency = 0;
        Save_StatusBumblebee = "Fail";

        Buffer_SocketTest = "";
        Save_SocketTest = "";

        Save_ELCB = "";

        //--------------------------------------------------------------------------------------------------------------------------------
        /* วนเคลียร์ค่าใน Buffer_IndexID */
        for (int i = Count_IndexID;  i > 0; i--)
        {
          Buffer_IndexID[i] = NULL;
          // Count_IndexID--;
          delay(1);
        }

        Count_IndexID = 0;
        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

        // --------------------------------------------------------------------------------------------------------------------------------
        /* เคลียร์ค่าใน Buffer_IndexMenu */
        Count_IndexMenu = 0;
        Buffer_IndexMenu[0] = "Load Location"; 
        Buffer_IndexMenu[1] = ""; 
        Buffer_IndexMenu[2] = ""; 
        Buffer_IndexMenu[3] = "";  
        Buffer_IndexMenu[4] = ""; 
        Buffer_IndexMenu[5] = ""; 
        Buffer_IndexMenu[6] = ""; 
        Buffer_IndexMenu[7] = ""; 
        Buffer_IndexMenu[8] = ""; 
        Buffer_IndexMenu[9] = "";

        //-------------------------------------------------------------------------------------------------------------------------------- 
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */
 
        //--------------------------------------------------------------------------------------------------------------------------------
        vTaskResume(TaskGPS);  /* TaskGPS ทำงาน */
        vTaskSuspend(TaskPZEM);  /* TaskPZEM หยุดงาน */
        vTaskSuspend(TaskSocket); /* TaskSocketหยุดงาน */
        
        // //--------------------------------------------------------------------------------------------------------------------------------
        // /* วนเคลียร์ค่าใน Count_IndexID */
        // for (int i = Count_IndexID;  i > 0; i--)
        // {
        //   Buffer_IndexID[i] = NULL;
        //   Count_IndexID--;
        //   delay(1);
        // }

        // //--------------------------------------------------------------------------------------------------------------------------------
        // PrintSerialID();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ ID ออกทาง Serial */

        // //--------------------------------------------------------------------------------------------------------------------------------
        // /* วนเคลียร์ค่าใน Buffer_IndexMenu */
        // for (int i = Count_IndexMenu;  i > 0; i--)
        // {
        //   Buffer_IndexMenu[i] = "";
        //   Count_IndexMenu--;
        //   delay(1);
        // }

        // //--------------------------------------------------------------------------------------------------------------------------------
        // PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

        // //--------------------------------------------------------------------------------------------------------------------------------
        // /* เคลียค่าในตัวแปร */
        // Buffer_ID = "";
        // Save_ID = "";  

        // Buffer_GPS_Lat = "";
        // Buffer_GPS_Lng = "";
        // Save_GPS_Lat = "";
        // Save_GPS_Lng = "";

        // Buffer_StatusBumblebee = "Fail";
        // Buffer_Voltage = 0;
        // Buffer_Frequency = 0;
        // Save_StatusBumblebee = "Fail";

        // Buffer_SocketTest = "";
        // Save_SocketTest = "";

        // Save_ELCB = "";

        // //--------------------------------------------------------------------------------------------------------------------------------
        // vTaskResume(TaskGPS);  /* TaskGPS ทำงาน */
        // vTaskSuspend(TaskPZEM);  /* TaskPZEM หยุดงาน */
        // vTaskSuspend(TaskSocket); /* TaskSocketหยุดงาน */

        // // //--------------------------------------------------------------------------------------------------------------------------------
        // delay(1000);  /* หน่วงเวลา 1000 มิลลิวินาที */
        // // lcd.clear();  /* เคลียร์หน้าจอ lcd */

      }

    //--------------------------------------------------------------------------------------------------------------------------------
    delay(100);  /* หน่วงเวลา 100 มิลลิวินาที */

    }
  }
}

//================================================================================================================================
/* ฟังก์ชันงาน GPS */
void Task_GPS(void* pvParameters) {

  for (;;)  /* วนซ้ำ */ 
  {
    //--------------------------------------------------------------------------------------------------------------------------------
    while (Hardware_Serial.available() > 0)  /* วนซ้ำถ้าหาก Hardware_Serial มากกว่่า 0 */

    //--------------------------------------------------------------------------------------------------------------------------------
    if (GPS.encode(Hardware_Serial.read()))  /* ถ้าหาก GPS ถอดรหัสจากการอ่าน Hardware_Serial */
    {
      //--------------------------------------------------------------------------------------------------------------------------------
      if (GPS.location.isValid())  /* ถ้าหากได้รับ location */
      {
        Buffer_IndexMenu[0] = "Load Location";  /* ให้ Buffer_IndexMenu ลำดับที่ 0 เท่ากับหน้า Load Location */
        Buffer_GPS_Lat = String(GPS.location.lat(), 6);  /* ให้ Buffer_GPS_Lat เท่ากับ lat, 6 ตำแหน่ง แปลงเป็นชุดอักขระ */
        Buffer_GPS_Lng = String(GPS.location.lng(), 6);  /* ให้ Buffer_GPS_Lng เท่ากับ lng, 6 ตำแหน่ง แปลงเป็นชุดอักขระ */

        // Serial.print(Status_GPS + " : ");
        // Serial.print(Buffer_GPS_Lat);
        // Serial.print(F(","));
        // Serial.println(Buffer_GPS_Lng);

      }

      //--------------------------------------------------------------------------------------------------------------------------------
      else  /* เป็นอื่น */
      {
        Buffer_IndexMenu[0] = "Satellite Checking";  /* ให้ Buffer_IndexMenu ลำดับที่ 0 เท่ากับหน้า Satellite Checking*/
        Serial.println(Buffer_IndexMenu[0]);  /* แสดงผล Buffer_IndexMenu ลำดับที่ 0 ออกทาง Serial */

      }
    }
    
    //--------------------------------------------------------------------------------------------------------------------------------
    /* ถ้าหาก เวลาสัญญาณนาฬิกาของ MCU  มากกว่า 5000 และ อักขระที่ได้จาก && GPS น้อยกว่า 10*/
    if (millis() > 5000 && GPS.charsProcessed() < 10)
    {
      Buffer_IndexMenu[0] = "Detected Fail";  /* ให้ Buffer_IndexMenu ลำดับที่ 0 เท่ากับหน้า Satellite Checking*/
      Serial.println(Buffer_IndexMenu[0]);  /* แสดงผล Buffer_IndexMenu ลำดับที่ 0 ออกทาง Serial */
      while(true);  /* วนซ้ำหากเงื่อนไขเป็นจริง */
      
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    delay(100);  /* หน่วงเวลา 100 มิลลิวินาที */

  }
}

//================================================================================================================================
/* ฟังก์ชันงาน PZEM */
void Task_PZEM(void* pvParameters)
{
  for (;;)  /* วนซ้ำ */
  {
    //--------------------------------------------------------------------------------------------------------------------------------
    /*
    แสดงผล Address เพื่อตวจสอบการเชื่อมต่อ
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);
    */

    //--------------------------------------------------------------------------------------------------------------------------------
    float voltage = pzem.voltage();  /* สร้างตัวแปร voltage ชนิดทศนิยม เท่ากับ voltage จาก pzem */
    float frequency = pzem.frequency();  /* สร้างตัวแปร frequency ชนิดทศนิยม เท่ากับ frequency จาก pzem */

    //--------------------------------------------------------------------------------------------------------------------------------
    if (isnan(voltage) or isnan(frequency))  /* ถ้าหาก ค่าใน voltage หรือ frequency เท่ากับ ว่าง */
    {
      // Serial.println("Error reading voltage or frequency");
      Buffer_Voltage = 0;  /* ให้ Buffer_Voltage ท่ากับ 0 */
      Buffer_Frequency = 0;  /* ให้ Buffer_Frequency ท่ากับ 0 */
      Buffer_StatusBumblebee = "Fail";  /* ให้ Buffer_StatusBumblebee ท่ากับ Fail */
    } 
    else  /* เป็นอื่น */
    {
      Buffer_StatusBumblebee = "Pass";  /* ให้ Buffer_StatusBumblebee ท่ากับ Pass */
      Buffer_Voltage = voltage;  /* ให้ Buffer_Voltage ท่ากับ voltage */
      Buffer_Frequency = frequency;  /* ให้ Buffer_Frequency ท่ากับ frequency */

      // Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
      // Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
      // Serial.println();

    }
   
    delay(100);  /* หน่วงเวลา 100 มิลลิวินาที */
    
  }
}

//================================================================================================================================
/* ฟังก์ชันงาน Socket */
void Task_Socket(void* pvParameters) {

  for (;;)  /* วนซ้ำ */
  {
    /* สร้างตัวแปรชนิดบูลลิน เก็บค่าตรงข้ามที่ได้จาก digitalRead ขาเซนเซอร์วัดแสง */
    bool LED_Pin1Value = not(digitalRead(LED_Pin1));
    bool LED_Pin2Value = not(digitalRead(LED_Pin2));
    bool LED_Pin3Value = not(digitalRead(LED_Pin3));

    //--------------------------------------------------------------------------------------------------------------------------------
    if (
    /* ถ้าหากหน้าที่ 2 คือ Bumblebee Test */
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "" and 
    Buffer_IndexMenu[4] == "" and 
    Buffer_IndexMenu[5] == "" and 
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      /* ถ้าหากหลอดที่ 1 ติด และ 2 ติด และ 3 ดับ */
      if (LED_Pin1Value == 1 and LED_Pin2Value == 1 and LED_Pin3Value == 0)
      {
        Buffer_SocketTest = "Correct";
      }

      /* ถ้าหากหลอดที่ 1 ติด และ 2 ดับ และ 3 ดับ */
      else if  (LED_Pin1Value == 1 and LED_Pin2Value == 0 and LED_Pin3Value == 0)
      {
        Buffer_SocketTest = "Open Ground";
      }

      /* ถ้าหากหลอดที่ 1 ดับ และ 2 ติด และ 3 ดับ */
      else if  (LED_Pin1Value == 0 and LED_Pin2Value == 1 and LED_Pin3Value == 0)
      {
        Buffer_SocketTest = "Open Neutral";
      }

      /* ถ้าหากหลอดที่ 1 ดับ และ 2 ดับ และ 3 ดับ */
      else if  (LED_Pin1Value == 0 and LED_Pin2Value == 0 and LED_Pin3Value == 0)
      {
        Buffer_SocketTest = "Open Live";
      }

      /* ถ้าหากหลอดที่ 1 ดับ และ 2 ติด และ 3 ติด */
      else if  (LED_Pin1Value == 0 and LED_Pin2Value == 1 and LED_Pin3Value == 1)
      {
        Buffer_SocketTest = "Live/GRD Reverse";
      }

      /* ถ้าหากหลอดที่ 1 ติด และ 2 ดับ และ 3 ติด */
      else if  (LED_Pin1Value == 1 and LED_Pin2Value == 0 and LED_Pin3Value == 1)
      {
        Buffer_SocketTest = "Live/NEU Reverse";
      }

      /* ถ้าหากหลอดที่ 1 ติด และ 2 ติด และ 3 ติด */
      else if  (LED_Pin1Value == 1 and LED_Pin2Value == 1 and LED_Pin3Value == 1)
      {
        Buffer_SocketTest = "Live/GRD Reverse,Missing GRD";
      }

      // Serial.println("Buffer_SocketTest: " + Buffer_SocketTest);
      // Serial.println("1:" + String(LED_Pin1Value) + " 2:" + String(LED_Pin2Value) + " 3:" + String(LED_Pin3Value));

    }

    else if(
    /* ถ้าหากหน้าที่ 5 คือ ELCB Finish */
    Buffer_IndexMenu[0] == "Save Location" and 
    Buffer_IndexMenu[1] == "Save ID" and 
    Buffer_IndexMenu[2] == "Bumblebee Test" and 
    Buffer_IndexMenu[3] == "Socket Test" and 
    Buffer_IndexMenu[4] == "ELCB Working" and 
    Buffer_IndexMenu[5] == "ELCB Finish" and
    Buffer_IndexMenu[6] == "" and 
    Buffer_IndexMenu[7] == "" and 
    Buffer_IndexMenu[8] == "" and 
    Buffer_IndexMenu[9] == "")
    {
      /* ถ้าหากหลอดที่ 1 ติด หรือ 2 ติด หรือ 3 ติด */
      if (LED_Pin1Value == 1 or LED_Pin2Value == 1 or LED_Pin3Value == 1)
      {
        Save_ELCB = "Fail";
      }

      /* ถ้าหากหลอดที่ 1 ติด และ 2 ติด และ 3 ติด */
      else if (LED_Pin1Value == 1 and LED_Pin2Value == 1 and LED_Pin3Value == 1)
      {
        Save_ELCB = "Fail";
      }

      /* เป็นอื่น */
      else
      {
        Save_ELCB = "Pass";
      }

      // Serial.println("Save_ELCB: " + Save_ELCB);
    }

    delay(100);  /* หน่วงเวลา 100 มิลลิวินาที */

  }
}

//================================================================================================================================
/* ฟังก์ชันงาน GetTime */
void Task_GetTime(void* pvParameters)
{
  for (;;)  /* วนซ้ำ */
  {
    if (Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "Getting Time" and 
      Buffer_IndexMenu[7] == "" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {

      //--------------------------------------------------------------------------------------------------------------------------------
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo))  /* ถ้าหากรับค่าเวลาไม่ได้ */
      {
        Serial.println("Failed to obtain time");
        return;
        
      }

      //--------------------------------------------------------------------------------------------------------------------------------
      char cDate[30];  /* สร้างตัวแปร cDate ชนิดอักขระ แบบอาเรย์ */
      char cTime[30];  /* สร้างตัวแปร cTime ชนิดอักขระ แบบอาเรย์ */
      
      strftime(cDate,30, "%A, %B %d %Y", &timeinfo);  /* เก็บวันใว้ใน cDate */
      strftime(cTime,30, "%H:%M:%S", &timeinfo);  /* เก็บเวลาใว้ใน cTime */
      
      BufferDate = String(cDate);  /* แปลงค่า cDate เป็นชุดอักขระ ไว้ใน BufferDate */
      BufferTime = String(cTime);  /* แปลงค่า cTime เป็นชุดอักขระ ไว้ใน BufferTime */

      Serial.println(BufferDate);
      Serial.println(BufferTime);

      //--------------------------------------------------------------------------------------------------------------------------------
      CountGSheet++;  /* ให้ CountGSheet เพิ่มขึ้น 1 */

      //--------------------------------------------------------------------------------------------------------------------------------
        /* แสดงผลสถานะ WiFi และ CountGSheet ออกทาง Serial */
      Serial.println("Status WiFi: " + StatusWiFi + " | CountGSheet: " + String(CountGSheet));

      //--------------------------------------------------------------------------------------------------------------------------------
      delay(1000);  /* หน่วงเวลา 1000 มิลลิวินาที */

      //--------------------------------------------------------------------------------------------------------------------------------
      if (CountGSheet >= 5)  /* ถ้าหาก CountGSheet มากกว่าเท่ากับ 5 */
      {

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Uploading";  /* ให้ Buffer_IndexMenu ตำแหน่ง Count_IndexMenu เท่ากับหน้า Uploading */
        Count_IndexMenu++;

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

      }
    }

  delay(1);  /* หน่วงเวลา 1 มิลลิวินาที */

  }
}

//================================================================================================================================
/* ฟังก์ชันงาน GSheet */
void Task_GSheet(void* pvParameters)
{
  for (;;)  /* วนซ้ำ */
  {
    if (
      Buffer_IndexMenu[0] == "Save Location" and 
      Buffer_IndexMenu[1] == "Save ID" and 
      Buffer_IndexMenu[2] == "Bumblebee Test" and 
      Buffer_IndexMenu[3] == "Socket Test" and 
      Buffer_IndexMenu[4] == "ELCB Working" and 
      Buffer_IndexMenu[5] == "ELCB Finish" and 
      Buffer_IndexMenu[6] == "Getting Time" and 
      Buffer_IndexMenu[7] == "Uploading" and 
      Buffer_IndexMenu[8] == "" and 
      Buffer_IndexMenu[9] == "")
      {

      //--------------------------------------------------------------------------------------------------------------------------------
      /* เรียก ready() วนซ้ำๆ เพื่อตรวจสอบและประมวลผลการรับรองความถูกต้อง */
      bool ready = GSheet.ready();

      //--------------------------------------------------------------------------------------------------------------------------------
      /* ถ้าหาก พร้อม และ เวลาปัจจุบันโดยสัญาณนาฬิกาบอร์ด ลบด้วย เวลาที่ผ่านมา มากกว่า เวลาที่หน่วง */
      if (ready && millis() - lastTime > timerDelay)
      {
        lastTime = millis();  /* ให้ lastTime เท่ากับเวลาปัจจุบันโดยสัญาณนาฬิกาบอร์ด */

        FirebaseJson response;  /* เรียกใช้ออปเจ็ค response โดย FirebaseJson */

        Serial.println("\nAppend spreadsheet values...");
        Serial.println("----------------------------");

        FirebaseJson valueRange;  /* เรียกใช้ออปเจ็ค valueRange โดย FirebaseJson */

        /* 
        เพิ่มแถวใหม่เข้าไปยังสเปรดชีต valueRange.add("อิงตามขนาด", "คอลัมน์");
        จัดเรียงค่าเข้าสเปรดชีต valueRange.set("values/[คอลัมน์]/[แถว]", ตัวแปร) 
        */
        valueRange.add("majorDimension", "COLUMNS");
        valueRange.set("values/[0]/[0]", BufferDate);
        valueRange.set("values/[1]/[0]", BufferTime);
        valueRange.set("values/[2]/[0]", Save_GPS_Lat);
        valueRange.set("values/[3]/[0]", Save_GPS_Lng);
        valueRange.set("values/[4]/[0]", Save_ID);
        valueRange.set("values/[5]/[0]", Save_StatusBumblebee);
        valueRange.set("values/[6]/[0]", Save_SocketTest);
        valueRange.set("values/[7]/[0]", Save_ELCB);

        /* ผนวกค่าเข้ากับสเปรดชีต */
        bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "DataLog!A1" /* range to append */, &valueRange /* data range to append */);
        
        if (success)
        {
          response.toString(Serial, true);
          valueRange.clear();
        }
        else
        {
          Serial.println(GSheet.errorReason());
        }

        Serial.println();
        Serial.println(ESP.getFreeHeap());

        CountGSheet = 0;  /* ให้ CountGSheet เท่ากับ 0 เพื่อรอนับใหม่ */

        //--------------------------------------------------------------------------------------------------------------------------------
        Buffer_IndexMenu[Count_IndexMenu] = "Uploaded";  /* บันทึกหน้าตำแหน่ง */
        Count_IndexMenu++;

        //--------------------------------------------------------------------------------------------------------------------------------
        PrintSerialMenu();  /* เรียกใช้ฟังก์ชัน แสดงผลลำดับ Menu ออกทาง Serial */

    }
      }

    delay(1);  /* หน่วงเวลา 1 มิลลิวินาที */

  }
}

//================================================================================================================================
/* ฟังก์ชัน สถาตอบกลับของ GSheet */
void tokenStatusCallback(TokenInfo info)
{
  if (info.status == token_status_error)
  {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  }
  else
  {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}

//================================================================================================================================
/* ฟังก์ชัน วนซ้ำ */
void loop() 
{
  unsigned long currentMillis = millis();  /* ให้ currentMillis เท่ากับ เวลาจากสัญญาณนาฬิกาบอร์ด */
  /* หาก WiFi ไม่ทำงาน ให้ลองเชื่อมต่อใหม่ทุกๆ และ เวลาปัจจุบัญ ลบด้วย เวลาก่อนหน้า มากกว่า interval (มิลลิวินาที) */
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
  {
    StatusWiFi = "Reconnect";  /* ให้ StatusWiFi เท่ากับ Reconnect */
    Serial.print(millis());  /* แสดงผลเวลาที่ขาดการเชื่อมต่อ */
    Serial.println("Reconnecting to WiFi...");  /* แสดงผล กำลังกลับมาเชื่อมต่อ ออกทาง Serial */
    WiFi.disconnect();  /* ตัดการเชื่อมต่อปัจจุบัน */
    WiFi.reconnect();  /* เชื่อมต่อใหม่ */
    previousMillis = currentMillis;  /* ให้ เวลาก่อนหน้า เท่ากับ เวลาปัจจุบัน (มิลลิวินาที) */
  }
  else  /* เป็นอื่น */
  {
    StatusWiFi = "Connected";  /* ให้ StatusWiFi เท่ากับ Connected */
  }
}