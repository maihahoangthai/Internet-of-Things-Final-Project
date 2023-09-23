// Tải thư viện cần thiết:
#include <WiFi.h>
#include "time.h"

// Thay đổi cho phù hợp với Wifi của nhà bạn
const char* ssid = "Len_VT";
const char* password = "55555555";
// Hãy chỉnh sửa sao cho đúng với Wifi mà ESP32 sẽ kết nối tới để vào Internet

WiFiServer server(80); // Set web server port number to 80
String header; // Tạo biến để lưu HTTP request
String outputLEDState = "off"; // Biến dùng để biểu thị trạng thái của đèn LED (tích hợp sẵn) trên mạch ESP32
String outputMotionState = "No Motion Detected..."; // Biến dùng để hiển thị tin nhắn phát hiện

// Khai báo biến dùng cho PIR sensor:
const int inputPIR = 26;
int pirState = LOW;
int val = 0;

// Biến cho việc xác định thời gian tại thời điểm hiện tại
const char* ntpServer = "pool.ntp.org"; // Gửi yêu cầu (để nhận về thời gian) đến NTP Server, "Pool.ntp.org" sẽ tự động chọn máy chủ thời gian gần với ta nhất
const long gmtOffset_sec = 25200; // Do Việt Nam có UTC+07:00 nên 7*60*60 = 25200
const int daylightOffset_sec = 0; // Thay đổi độ lệch ánh sáng ban ngày theo mili giây, nếu Quốc gia của bạn không sử dụng thì thiết lập 0

// Biến toàn cục để lưu thông tin Thời gian
char timeWeekDay[10];
char timeMonth[10];
char timeDateOfMonth[3];
char timeYear[5];
char timeHour[3];
char timeMinute[3];
char timeSecond[3];

// Lập bộ đếm Timer (thay vì dùng deplay();)
unsigned long currentTime = millis(); // Thời gian lúc này
unsigned long previousTime = 0; // Thời gian lúc trước đó
const long timeoutTime = 3000; // 3000ms = 3s, Sau mỗi 3 giây thì ESP32 sẽ bắt tín hiệu mới

// Tạo Timer 2 cho LED onboard:
unsigned long LEDpreviousMillis = 0;
const long LEDinterval = 15000; //Cách 15s thì chuyển trạng thái của LED tùy vào val đọc được từ PIR sensor

void setup() {
  Serial.begin(115200);
  
  // Thiết lập Input/Output cho PIR sensor và LED Onboard:
  pinMode(inputPIR, INPUT);
  pinMode (LED_BUILTIN, OUTPUT); // Sử dụng LED onboard của ESP32 làm OUTPUT
  digitalWrite(LED_BUILTIN, LOW); // LED onboard sẽ luôn bắt đầu ở trạng thái tắt

  // Kết nối đến Wi-Fi network bằng SSID và password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Sau khi kết nối Wifi thành công thì hiện ra màn hình Serial Monitor local IP Address (của ESP32) và tạo Web Server
  Serial.println("");
  Serial.println("->WiFi connected.");
  Serial.println("Your IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("OK!");
  Serial.println("->First Boot's time...");

  // Gọi tới NTP Server để nhận thời gian, In nó ra Serial Monitor và lưu vào biến
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void loop(){
  unsigned long LEDcurrentMillis = millis();
  val = digitalRead(inputPIR);

  if((LEDcurrentMillis - LEDpreviousMillis) >= LEDinterval){ // Cứ sau mỗi 15s thì:
    LEDpreviousMillis = LEDcurrentMillis;
  
    // Bắt được tín hiệu, chỉnh pirState và cho LED sáng
    if (val == HIGH) {
      if (pirState == LOW) {
        // Serial.println("->ESP32's LED on!");
        outputLEDState = "on";
        digitalWrite(LED_BUILTIN, HIGH);
        
        Serial.println("Motion detected!");
        pirState = HIGH;
        outputMotionState = "Motion Detected!";
        
        printLocalTime();
      }
     }
  
     // Không có gì, chỉnh lại pirState và cho LED tắt
     else {
      if (pirState == HIGH){
        // Serial.println("->ESP32's LED off...");
        outputLEDState = "off";
        digitalWrite(LED_BUILTIN, LOW);
        
        Serial.println("...Motion ended");
        pirState = LOW;
        outputMotionState = "No Motion Detected...";
      }
     }
  }
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects:
    currentTime = millis();
    previousTime = currentTime; // Bắt đầu đếm giờ cho Timer
    Serial.println("New Client.");
    String currentLine = "";                // Tạo một biến String để giữ dữ liệu gửi tới từ client
    
    while (client.connected() && (currentTime - previousTime <= timeoutTime)) {  // loop while the client's connected
      currentTime = millis();
      
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html lang=en>");
            client.println("<head><meta charset=utf-8><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style:
            client.println("<style></style></head>");
            
            // Web Page Heading
            client.println("<body><h1>SMART ALERT</h1>");
            
            // Hiển thị thời gian lần Quét cuối:
            client.println("<p>Lastest Alert at: ");
            client.println(timeWeekDay);
            client.println(",");
            client.println(timeDateOfMonth);
            client.println(timeMonth);
            client.println(",");
            client.println(timeYear);
            client.println(",");
            client.println(timeHour);
            client.println(":");
            client.println(timeMinute);
            client.println(":");
            client.println(timeSecond);
            client.println("</p>");
            
            // Display current state, and ON/OFF buttons for ESP32's built in LED 
            client.println("<p>LED - State: " + outputLEDState + "</p>");
            if (outputMotionState == "No Motion Detected...") {
              client.println("<p><a>NO MOTION</a></p>");
            } else {
              client.println("<p><a>MOTION DETECTED!</a></p>");
            } 
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void printLocalTime(){
  // KHI GỌI, nếu không lỗi thì trả về màn hình Serial Monitor: Thứ mấy, Tháng mấy Ngày mấy Năm mấy Giờ:Phút:Giây
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("->Error: Failed to obtain time...");
    return;
  }
  Serial.println(&timeinfo, "->Lastest Alert at: %A, %B %d %Y %H:%M:%S");

  // ĐỒNG THỜI, Lưu data thời gian vào các biến chỉ định sẵn
  Serial.println("->->...Processing to save date & time to variables");
  strftime(timeWeekDay, 10, "%A", &timeinfo); //Thứ mấy trong tuần
  strftime(timeMonth, 10, "%B", &timeinfo); //Tháng thứ mấy
  strftime(timeDateOfMonth, 3, "%d", &timeinfo); //Ngày mấy trong tháng
  strftime(timeYear, 5, "%Y", &timeinfo); //Năm bao nhiêu
  strftime(timeHour, 3, "%H", &timeinfo); //Mấy giờ/24h
  strftime(timeMinute, 3, "%M", &timeinfo); //Phút
  strftime(timeSecond, 3, "%S", &timeinfo); //Giây
  // Ngắt dòng Serial Monitor (để nhận biết đã kết thúc chuỗi lệnh):
  Serial.println("-x-x-");
}
