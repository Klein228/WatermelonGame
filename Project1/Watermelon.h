#pragma once
#include<graphics.h>
#include"PhysicsObject.h"
#include"utils.h"
#include"NextFruit.h"
//IMAGE watermelonImg;
class ImgSources
{
public:
    ImgSources() {
        loadResources();
    };
    ~ImgSources() {};
	IMAGE* getImg(int i) {
		return &imgs.at(i);
	}
    std::vector<IMAGE>* getAllImg() {
        return &imgs;
    }
private:
	std::vector<IMAGE> imgs;

	void loadResources() {
		for (size_t i = 1; i <= 7; i++)
		{
			imgs.push_back(getTypeImage(i));
		}
	};
    
	IMAGE getTypeImage(int n) {
        int height = 100;
        int width = 100;
		//�������ͷ���ͼƬ����
        //1,2,...,7���ȼ� 3Ϊԭʼ��С
        float scale = n / 3.0;
        int newHeight = height * scale;
        int newWidth = width * scale;
		IMAGE img;
		loadimage(&img, _T("D:\\gameResource\\Watermelon\\watermelon.png"),newWidth,newHeight,true);
		return img;
	}
};
// �����࣬�̳���PhysicsObject
class Watermelon : public PhysicsObject {
public:
    int type;  // �������ͣ�������С����ɫ��
    std::vector<IMAGE>* fruitImgs;
    Watermelon(float x, float y, int type,std::vector<IMAGE>* imgs){
        //��������ѡ��ͼƬ
        fruitImgs = imgs;
        float radius = fruitImgs->at(type).getwidth()/2.0;
        this->radius = radius;
        // ����Խ�󣬵���ԽС
        restitution = 0.7f - type * 0.05f;
        this->position.x = x;
        this->position.y = y;
        this->mass = 2.0f * (type + 1);
        this->type = type;
        //���ó�ʼС���ٶ�
        this->velocity.x = 0;
        this->velocity.y = 100;
    }

    // ��������
    void draw() {
        //ʹ���ʱͼƬ����������
        IMAGE* img = &fruitImgs->at(type);
        int height = img->getheight();
        putimage_alpha(position.x-height / 2, position.y- height / 2, img);
        ////��һ���ڿ����ڲ�
        //setlinecolor(BLACK);
        //rectangle(position.x-height/2, position.y-height/2, position.x + img->getwidth() - height / 2, position.y + img->getheight() - height / 2);
        ////��һ��Բ�򿴼���Բ
        //circle(position.x, position.y, radius);
    }
    // �����������ͣ��ϲ���
    void upgrade() {
        type++;

        radius = fruitImgs->at(type).getwidth() / 2.0;
        mass = 2.0f * (type + 1);
        restitution = 0.7f - type * 0.05f;
    }
};

