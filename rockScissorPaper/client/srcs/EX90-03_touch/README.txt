1. 터치 드라이버 설치

cd
git clone https://github.com/goodtft/LCD-show.git
chmod -R 755 LCD-show
cd LCD-show/
sudo ./LCD5-show
자동 reboot 됨

2. 실행
cd /home/willtek/work/practice_device_bd/EX90-03_touch
./touchtest /dev/input/event1
-> 터치를 터치하면 터치 좌표가 출력됨
