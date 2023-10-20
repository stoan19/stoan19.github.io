// Sử dụng Arduino đọc nhiệt độ từ cảm biến DS18B20. Hiển thị kết quả lên màn hình LCD. Từ kết quả thu được từ cảm biến tử đó điển khiển quạt được kết nối với RELAY
#include<LiquidCrystal.h>
int DSPIN = 9;
#define Relay 8
int CAO=30;
int THAP=26;
const int rs=12, en=11, d4=5, d5= 4, d6=3, d7=2; // vss> âm nguồn, vdd>dương nguồn, v0 biến trở,rw âm nguồn, A> dương, K>âm
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 // pinMode(PC13, OUTPUT);
pinMode(Relay, OUTPUT);
digitalWrite(Relay, LOW);
lcd.begin(16,2);

}

void loop()
{
  // put your main code here, to run repeatedly:
  double temp = TempRead();
  temp  = temp * 0.0625; // conversion accuracy is 0.0625 / LSB

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.println("");

  delay(300);
  lcd.setCursor(0,0);
  lcd.print("temp:");
  lcd.print(temp);
  lcd.print("oC_N17");
  delay(100);


  //if (temp<26){
    //digitalWrite(Relay,LOW);
    //digitalWrite(PC13, HIGH);
    //delay(200);
   // digitalWrite(PC13, LOW);
    //delay(200);
  // lcd.setCursor(0,1); // cot 0 hang 1
  //lcd.print("duoimucchophep**");
   //}
//delay (100);

if(temp<CAO&&temp>THAP){
    digitalWrite(Relay, LOW);
    delay(50);
     lcd.setCursor(0,1);
  lcd.print("binh thuong*****");
  }

delay(100);
  if(temp>CAO){
    digitalWrite(Relay, HIGH);
    delay(50);
     lcd.setCursor(0,1);
  lcd.print("trenmucchophep**");



  }
  delay(50);


}

boolean DS18B20_Init() // ket noi voi cam bien
{
  bool ret = 0;
  digitalWrite(DSPIN, LOW);     // Send reset pulse to the DS18B20 sensor
  pinMode(DSPIN, OUTPUT);
  delayMicroseconds(500);             // Wait 500 us
  pinMode(DSPIN, INPUT);
  delayMicroseconds(100);             //wait to read the DS18B20 sensor response
  if (!digitalRead(DSPIN)) {         //       neu =0
    ret = 1;                          // DS18B20 sensor is present
    delayMicroseconds(400);           // Wait 400 us
  }
  return(ret);
}

void DS18B20_Write(byte data)
{
  pinMode(DSPIN, OUTPUT);
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(10);
    if (data & 1) digitalWrite(DSPIN, HIGH);
    else digitalWrite(DSPIN, LOW);
    data >>= 1;
    delayMicroseconds(50);
    digitalWrite(DSPIN, HIGH);
  }
}

byte DS18B20_Read()
{
  pinMode(DSPIN, OUTPUT); // thiet lap chan dspin la output
  digitalWrite(DSPIN, HIGH);// bat dspin
  delayMicroseconds(2);
  byte data = 0;
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(1);
    digitalWrite(DSPIN, HIGH);
    pinMode(DSPIN, INPUT);// chan dspin la input
    delayMicroseconds(5);
    data >>= 1; // dich bit logic phai
    if (digitalRead(DSPIN)) data |= 0x80; // doc tin hieu tu digital dspin!=0 thi . 0x80 nghia la 1000 0000, |= là phep or
    delayMicroseconds(55);
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
  }
  return data;
}

int TempRead()
{
  if (!DS18B20_Init()) return 0; // neu ham nay ko dung thi tra ve 0
  DS18B20_Write (0xCC); // Send skip ROM command (cho phep bus truy cap nhanh cac chuc nang cua bo nho ma ko can cung cap ma ROM 64 bit)
  DS18B20_Write (0x44); // RAM Send reading start conversion command (bat dau chuyen doi nhiet do, no se tra ve 1 khi qua trinh chuyen doi nhiet do hoan tat )
  if (!DS18B20_Init()) return 0;
  DS18B20_Write (0xCC); // Send skip ROM command
  DS18B20_Write (0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
  int temp = DS18B20_Read (); // Low byte
  temp |= DS18B20_Read () << 8; // High byte
  return temp;
}
