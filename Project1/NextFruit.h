#pragma once
#include<graphics.h>
#include<vector>
#include"utils.h"
//�����������Ͻ���ʾ��һ��������ɵ�ˮ������ӦͼƬ��ʾ
class NextFruit
{
private:

public:
	std::vector<IMAGE>* fruitIMGs;//�����������ͼƬ
	int next_index;//��һ��ˮ��������
	Vector2 position;//ͼƬ����λ��
	NextFruit(std::vector<IMAGE>* fruitIMGs)
		: fruitIMGs(fruitIMGs)
	{
		randomNext();
	}
	void input(ExMessage msg) {
		//������λ�ںδ���������ʾˮ��λ��
		if (msg.message == WM_MOUSEMOVE) {
			position.x = msg.x - fruitIMGs->at(next_index).getwidth() / 2;
		}
	}
	void draw() {
		//setbkmode(TRANSPARENT);//��ʾ͸������

		//settextcolor(BLUE);//����������ɫΪ��ɫ

		//settextstyle(15, 0, _T("����"));//��������

		//outtextxy(50, 30, _T("��һ��ˮ��"));//�������

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

