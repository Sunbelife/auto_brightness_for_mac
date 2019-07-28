#include <wiringPi.h>
#include <pcf8591.h>
#include <stdio.h>

#define Address 0x48 // pcf8591 的 I2C 地址
#define BASE 64 // above
#define A0 BASE+0
#define A1 BASE+1
#define A2 BASE+2
#define A3 BASE+3

int main() {
	int read_num;
	// 最大环境亮度阈值
	int max_num = 240;
	// 最小环境亮度阈值
	int min_num = 185;
	int status = 0;
	
	wiringPiSetup();
	pcf8591Setup(A0, Address);
	
	while(1) {
			read_num = analogRead(A0);
			printf("目前读取亮度值：%d\n", read_num); //输出AD转换后的值 delay(1000);
			if (read_num < min_num) {
					if (status == 0) {
							continue;
					}
					printf("环境有点暗，正在调暗屏幕\n");
					system("ssh sunbelife@192.168.123.141 /usr/local/bin/ddcctl -d 1 -b 1");
					status = 0;
					delay(3000);
			} else if (read_num < max_num && read_num > min_num) {
					if (status == 1) {
							continue;
					}
					printf("环境亮度还中，把屏幕亮度调到中\n");
					system("ssh sunbelife@192.168.123.141 /usr/local/bin/ddcctl -d 1 -b 50");
					status = 1;
					delay(3000);
			} else if (read_num > max_num) {
					if (status == 2) {
							continue;
					}
					printf("环境太亮，屏幕亮度全开\n");
					system("ssh sunbelife@192.168.123.141 /usr/local/bin/ddcctl -d 1 -b 100");
					status = 2;
					delay(3000);
			}
			delay(1000);
		}
		return 0;
	}
}