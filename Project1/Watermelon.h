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
		//根据类型返回图片索引
        //1,2,...,7个等级 3为原始大小
        float scale = n / 3.0;
        int newHeight = height * scale;
        int newWidth = width * scale;
		IMAGE img;
		loadimage(&img, _T("D:\\gameResource\\Watermelon\\watermelon.png"),newWidth,newHeight,true);
		return img;
	}
};
// 西瓜类，继承自PhysicsObject
class Watermelon : public PhysicsObject {
public:
    int type;  // 西瓜类型（决定大小和颜色）
    std::vector<IMAGE>* fruitImgs;
    Watermelon(float x, float y, int type,std::vector<IMAGE>* imgs){
        //根据类型选择图片
        fruitImgs = imgs;
        float radius = fruitImgs->at(type).getwidth()/2.0;
        this->radius = radius;
        // 西瓜越大，弹性越小
        restitution = 0.7f - type * 0.05f;
        this->position.x = x;
        this->position.y = y;
        this->mass = 2.0f * (type + 1);
        this->type = type;
        //设置初始小球速度
        this->velocity.x = 0;
        this->velocity.y = 100;
    }

    // 绘制西瓜
    void draw() {
        //使点击时图片绘制在中心
        IMAGE* img = &fruitImgs->at(type);
        int height = img->getheight();
        putimage_alpha(position.x-height / 2, position.y- height / 2, img);
        ////画一个黑框用于测
        //setlinecolor(BLACK);
        //rectangle(position.x-height/2, position.y-height/2, position.x + img->getwidth() - height / 2, position.y + img->getheight() - height / 2);
        ////画一个圆框看检测的圆
        //circle(position.x, position.y, radius);
    }
    // 升级西瓜类型（合并后）
    void upgrade() {
        type++;

        radius = fruitImgs->at(type).getwidth() / 2.0;
        mass = 2.0f * (type + 1);
        restitution = 0.7f - type * 0.05f;
    }
};

