#pragma once
#include <windows.h>  // ȷ������ Windows ͷ�ļ�
#pragma comment(lib, "msimg32.lib")  // ���� AlphaBlend ���ڵĿ�
#include<graphics.h>
#include <cmath>
#include <iostream>
// 2D�����࣬���ڱ�ʾλ�á��ٶȺͼ��ٶ�
class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}

    // �����ӷ�
    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    // ��������
    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    // ��������
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // �����������ȵ�ƽ��
    float lengthSquared() const {
        return x * x + y * y;
    }

    // ������������
    float length() const {
        return std::sqrt(lengthSquared());
    }

    // ������һ��
    Vector2 normalize() const {
        float len = length();
        if (len < 0.0001f) return Vector2();
        return Vector2(x / len, y / len);
    }
};

inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int h = img->getheight();
	int w = img->getwidth();
	AlphaBlend(GetImageHDC(GetWorkingImage()), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

