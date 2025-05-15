#pragma once
#include<graphics.h>
#include<vector>
#include"utils.h"
//本类用于左上角提示下一个随机生成的水果及对应图片显示
class NextFruit
{
private:

public:
	std::vector<IMAGE>* fruitIMGs;//存放所有类型图片
	int next_index;//下一个水果的索引
	Vector2 position;//图片绘制位置
	NextFruit(std::vector<IMAGE>* fruitIMGs)
		: fruitIMGs(fruitIMGs)
	{
		randomNext();
	}
	void input(ExMessage msg) {
		//检测鼠标位于何处并跟新显示水果位置
		if (msg.message == WM_MOUSEMOVE) {
			position.x = msg.x - fruitIMGs->at(next_index).getwidth() / 2;
		}
	}
	void draw() {
		//setbkmode(TRANSPARENT);//显示透明文字

		//settextcolor(BLUE);//设置字体颜色为蓝色

		//settextstyle(15, 0, _T("宋体"));//设置字体

		//outtextxy(50, 30, _T("下一个水果"));//输出文字

		putimage_alpha(position.x, position.y,&fruitIMGs->at(next_index));
	}
	int randomNext() {
		int temp = next_index;
		this->next_index = rand() % (fruitIMGs->size()/2);
		position.x = (getwidth() / 2 - fruitIMGs->at(next_index).getwidth() / 2);
		position.y = 0- fruitIMGs->at(next_index).getheight() / 2;
		return temp;
	}
	void setImgs(std::vector<IMAGE>* fruitIMGs) {
		this->fruitIMGs = fruitIMGs;
	}

	NextFruit() {
		next_index = 0;
	};
};

