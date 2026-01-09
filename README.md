Phần mềm mô phỏng một cuộc đua vịt (giống Duck Race), viết bằng SFML.  

Cách hoạt động thì khá là đơn giản:  
- Đưa tên vào mảng *ducksNameList*, sau đó hệ thống sẽ tìm trong avatars có hình ảnh nào có tên giống không, nếu không thì sẽ lấy ảnh con vịt mặc định
- Những hình ảnh này sẽ được dùng để tạo sprite, sau đó hệ thống vẽ cảnhh, đặt các vật thể vào đúng vị trí của nó
- Khi người dùng muốn bắt đầu thì chỉ cần bấm dấu Cách là các con vịt sẽ bắt đầu đua
- Vì để kịch tính thì các tốc độ của các con vịt sẽ ngẫu nhiên thay đổi sau một khoảng thời gian ngắn
- Khi cán đích thì tất cả hoạt động sẽ dừng lại và hệ thống sẽ chúc mừng người chiến thắng

Nhận xét:  
- Tự nhận xét là cái này khá sơ sài và khó để tự sử dụng (do chưa học cách làm một giao diện tử tế).
- Hàm main() quá lớn và lan man, khi vọc các dự án khác thì tôi thấy hàm main() của họ rất nhỏ, đồng thời các file của họ được quản lí rất gọn, nên đây sẽ là điểm mà tôi cần cải thiện
