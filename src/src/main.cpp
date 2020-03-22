#include <Keyboard.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

const int size = 8; // 八个虚拟键
// 用到的脚，按照实际排列
const int buttonPins[size] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7};

const int buttonKeys[size] = {
	KEY_F8, KEY_F6, KEY_F4, KEY_F2,
	KEY_F1, KEY_F3, KEY_F5, KEY_F7};

bool buttonStates[size] = {
	false, false, false, false,
	false, false, false, false};

const int pollRate = 30; // 击键延时(ms)，防止误判连击，可以根据具体情况调大调小
long m;					 // 时钟

void setup()
{
	// 初始化工作灯，并设置默认无击键关闭
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	// 开始
	for (int i = 0; i < size; i++)
		pinMode(buttonPins[i], INPUT_PULLUP); // 启用内置上拉电阻
	m = millis();
	Keyboard.begin();
}

void loop()
{
	// 硬件比较简单，芯片处理速度足够，
	// 所以没有采用复杂的attachInterrupt来写，只是简单的进行快速轮询.
	if (millis() - m > pollRate)
	{
		m = millis();

		for (int i = 0; i < size; i++)
		{
			int state = digitalRead(buttonPins[i]);

			digitalWrite(LED_BUILTIN, state); // 有击键动作，灯亮
			if (state == LOW && !buttonStates[i])
			{
				buttonStates[i] = true;
				Keyboard.press(KEY_LEFT_GUI);
				Keyboard.press(KEY_LEFT_SHIFT);
				Keyboard.press(buttonKeys[i]); // 发出模拟击键事件 SUPER+SHIFT+F?
			}
			else if (state == HIGH && buttonStates[i])
			{
				buttonStates[i] = false;
				Keyboard.release(buttonKeys[i]); // 发出模拟释放键事件
				Keyboard.release(KEY_LEFT_SHIFT);
				Keyboard.release(KEY_LEFT_GUI);
			}
		}
	}
}