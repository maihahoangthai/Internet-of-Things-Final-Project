# Internet of things Fundamentals Final Project
Bài làm cuối kỳ môn học IoT cơ bản (502068) của TDTU vào tháng 8 năm 2021.
## Tổng quan về ý tưởng và bài làm
Một trong những ví dụ thiết thực nhất mà công nghệ IoT đem đến, đó chính là kỹ thuật chuyển hóa dữ liệu nhiệt chuyển động thu được từ cảm biến PIR sang tín hiệu điện tử digital, giúp tạo nên những hành vi xử lí tự động thông qua Arduino Uno. Mà trong trường hợp như ví dụ bên dưới đây thì hệ thống sẽ rung còi mỗi khi phát hiện chuyển động (motion) trong tầm nhìn của cảm biến PIR. Một thiết kế vô cùng đơn giản và dễ thực hiện nhưng rất thiết thực, có khả năng giúp hỗ trợ, cảnh báo gia chủ trong công việc phòng chống xâm nhập trái phép của người lạ. Dưới đây là một ví dụ về cảnh báo trộm sử dụng cảm biến PIR và Arduino mà tôi đã tự làm được trong những buổi thực hành môn IoT cơ bản.

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/ee9fa440-e9cc-4e27-8897-fa208cb41f35)

Trong thực tế cuộc sống, không phải ai cũng có khả năng chi trả để thuê nhân viên bảo vệ giúp bảo vệ nhà của họ khỏi những kẻ xâm nhập trái phép. Trên thực tế, trong thời đại của IoT, gia chủ không cần phải làm như vậy nữa. Tất cả những gì họ cần là một PIR sensor giúp theo dõi chuyển động (thông qua nhiệt) kết nối với một bộ vi điều khiển có thể gửi dữ liệu qua Internet. Bất cứ khi nào kẻ xâm nhập cố gắng đột nhập hoặc vì vấn đề gì đó, chuyển động được phát hiện trong ngôi nhà đang vắng chủ, một thông báo sẽ được bật lên, gửi đến điện thoại của chủ nhà, cảnh báo cho họ về sự bất thường. Từ nền tảng ý tưởng ấy, tôi đã xây dựng nên một hệ thống bảo mật thông minh, có tên gọi là *SmartAlert*.

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/3f71891d-f562-46d3-87bc-bfbc87dadc00)

Hệ thống mà tôi xây dựng có hai thành phần xử lý chính có đi kèm lập trình là **ESP32** và ứng dụng trên điện thoại, cũng như có thêm một **PIR sensor** để ghi nhận tín hiệu lạ về nhiệt chuyển động. Còn hình dưới đây là Flowchart của vi xử lý ESP32.

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/cf771b5f-4939-4f1a-bb16-4fdc7970b4f7)

Bằng cách bắt tín hiệu chuyển động thông qua PIR sensor, vi điều khiển ESP32 sẽ nhận và chuyển dữ liệu đó sang tín hiệu điện tử, xử lý nó ở ESP32 rồi truyền tín hiệu báo bất thường qua mạng WiFi vào Server do ESP32 khởi tạo trên mạng này.

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/fdf03085-5645-4135-b17d-aa0e2e745518)

Hình trên là Flowchart của ứng dụng điện thoại xây dựng từ **MIT app inventor**. Về cơ bản, một ứng dụng trên điện thoại thông minh được cài sẵn từ trước sẽ kết nối đến ESP32 Server được tạo ra trước đó và nhận về các dữ liệu phát sinh do các chuyển động nhiệt bất thường được PIR sensor ghi nhận. Thông qua việc kết nối điện thoại của bản thân đến mạng internet thì người dùng có thể nhận được những cảnh báo về nguy cơ có xâm nhập trái phép hoặc hành động lạ trong nhà họ. Để từ đó, gia chủ có hành động hợp lý nhằm bảo vệ gia đình, bản thân và tài sản.
## Tiền điều kiện của sản phẩm cuối
Các phần mềm để lập trình cho ESP32 như **Arduino IDE**, với smartphone thì là **MIT App Inventor**. Phần mềm Arduino IDE nên được cài đặt phiên bản mới nhất và có cập nhật các phần mở rộng, các thư viện dùng cho chip vi xử lý ESP32 trong phần Boards Manager.

Những thành phần linh kiện phần cứng sau:
- 1 vi xử lý NodeMCU ESP32 Wifi BLE.
- 1 cảm biến PIR sensor (HC-SR501).
- 1 Breadboard hay Test board (MB-102 830 lỗ)
- Dây cắm mạch và dây nguồn Micro USB cho ESP32

Trong trường hợp bị hạn chế về ngân sách và muốn tiết kiệm chi phí thì chỉ cần có một ESP32, một PIR sensor và vài sợi dây cắm mạch loại cái – cái để nối ESP32 với PIR là được.

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/2e83be2b-2a90-4029-886b-aad6ccf9cf29)

ESP32 có thể dễ dàng được cấp nguồn nhờ vào USB port của Laptop hoặc máy tính bàn, ổn định nhất là nguồn cấp rơi vào khoảng 3.3v. Còn PIR sensor HC-SR501 có thể cấp nguồn dựa vào chân VIN của ESP32. Tuy nhiên, trong trường hợp muốn thiết bị trở nên tiện gọn và di động thì nên có thêm một nguồn ngoài, chẳng hạn như mạch sạc 5v đầu ra như dưới đây:

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/7ea900f4-113c-4804-80b6-341f0df1845c)

## Sản phẩm thực tế

![image](https://github.com/maihahoangthai/Internet-of-Things-Final-Project/assets/82483112/6710bf0d-1aa5-4ad1-8c58-021c63e2e252)






