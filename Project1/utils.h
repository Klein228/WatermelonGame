#pragma once
#include <windows.h>  // 确保包含 Windows 头文件
#pragma comment(lib, "msimg32.lib")  // 链接 AlphaBlend 所在的库
#include<graphics.h>
#include <cmath>
#include <iostream>
// 2D向量类，用于表示位置、速度和加速度
class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}

    // 向量加法
    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    // 向量减法
    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    // 向量数乘
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // 计算向量长度的平方
    float lengthSquared() const {
        return x * x + y * y;
    }

    // 计算向量长度
    float length() const {
        return std::sqrt(lengthSquared());
    }

    // 向量归一化
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

