#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include"utils.h"

// 物理对象类
class PhysicsObject {
public:
    Vector2 position;     // 位置
    Vector2 velocity;     // 速度
    Vector2 acceleration; // 加速度
    float radius;         // 半径（用于碰撞检测）
    float mass;           // 质量
    float restitution;    // 弹性系数（0-1之间）
    bool isStatic;        // 是否为静态对象（不受力影响）
    PhysicsObject(float x, float y, float radius, float mass = 1.0f, float restitution = 0.8f)
        : position(x, y), velocity(0, 0), acceleration(0, 0),
        radius(radius), mass(mass), restitution(restitution), isStatic(false) {
    }

    // 更新对象状态
    void update(float deltaTime) {
        if (isStatic) return;

        // 根据加速度更新速度
        velocity = velocity + acceleration * deltaTime;

        // 根据速度更新位置
        position = position + velocity * deltaTime;

        // 重置加速度（需要在下一帧重新应用力）
        acceleration = Vector2();
    }

    // 应用力
    void applyForce(const Vector2& force) {
        if (isStatic) return;
        // F = ma -> a = F/m
        acceleration = acceleration + force * (1.0f / mass);
    }

    PhysicsObject() = default;
};

// 物理引擎类
class PhysicsEngine {
private:
    std::vector<PhysicsObject*> objects;
    Vector2 gravity;        // 重力向量
    float friction;         // 摩擦系数
    int windowWidth;
    int windowHeight;       //窗口大小

public:
    PhysicsEngine(int w,int h):windowWidth(w),windowHeight(h){
        friction = 0.99f;
        gravity = Vector2(0, 200.0f);
    }
    // 添加物理对象
    void addObject(PhysicsObject* object) {
        objects.push_back(object);
    }

    // 设置重力
    void setGravity(const Vector2& gravity) {
        this->gravity = gravity;
    }

    // 设置摩擦系数
    void setFriction(float friction) {
        this->friction = friction;
    }

    // 更新所有物理对象
    void update(float deltaTime) {
        // 1. 应用重力
        for (auto object : objects) {
            if (!object->isStatic) {
                object->applyForce(gravity * object->mass);
            }
        }

        // 2. 更新所有对象状态
        for (auto object : objects) {
            object->update(deltaTime);
        }

        // 3. 应用摩擦力
        for (auto object : objects) {
            if (!object->isStatic) {
                object->velocity = object->velocity * friction;
            }
        }
        // 4. 检测和处理边界碰撞
        detectAndResolveBoundaryCollisions();
        // 4. 检测和处理碰撞
        detectAndResolveCollisions();
    }

    // 检测和处理碰撞
    void detectAndResolveCollisions() {
        // 简单实现：检测所有对象对之间的碰撞
        for (size_t i = 0; i < objects.size(); i++) {
            for (size_t j = i + 1; j < objects.size(); j++) {
                PhysicsObject* obj1 = objects[i];
                PhysicsObject* obj2 = objects[j];

                // 计算两个物体之间的距离
                Vector2 distance = obj2->position - obj1->position;
                float distLength = distance.length();

                // 如果两个物体发生碰撞
                if (distLength < obj1->radius + obj2->radius) {
                    // 计算碰撞响应
                    resolveCollision(obj1, obj2, distance, distLength);
                }
            }
        }
    }
    // 检测和处理边界碰撞
    void detectAndResolveBoundaryCollisions() {
        for (auto object : objects) {
            if (object->isStatic) continue;

            // 左边界碰撞
            if (object->position.x - object->radius < 0) {
                object->position.x = object->radius;  // 防止穿透边界
                object->velocity.x = -object->velocity.x * object->restitution;  // 反弹
            }

            // 右边界碰撞
            if (object->position.x + object->radius > windowWidth) {
                object->position.x = windowWidth - object->radius;
                object->velocity.x = -object->velocity.x * object->restitution;
            }

            // 上边界碰撞
            if (object->position.y - object->radius < 0) {
                object->position.y = object->radius;
                object->velocity.y = -object->velocity.y * object->restitution;
            }

            // 下边界碰撞
            if (object->position.y + object->radius > windowHeight) {
                object->position.y = windowHeight - object->radius;
                object->velocity.y = -object->velocity.y * object->restitution;
            }
        }
    }
    // 处理两个物体之间的碰撞
    void resolveCollision(PhysicsObject* obj1, PhysicsObject* obj2,
        const Vector2& distance, float distLength) {
        // 计算碰撞法线（从obj1指向obj2的单位向量）
        Vector2 normal = distance.normalize();

        // 计算重叠量
        float overlap = (obj1->radius + obj2->radius) - distLength;

        // 分离两个物体
        if (!obj1->isStatic && !obj2->isStatic) {
            obj1->position = obj1->position - normal * (overlap * 0.5f);
            obj2->position = obj2->position + normal * (overlap * 0.5f);
        }
        else if (!obj1->isStatic) {
            obj1->position = obj1->position - normal * overlap;
        }
        else if (!obj2->isStatic) {
            obj2->position = obj2->position + normal * overlap;
        }

        // 计算相对速度
        Vector2 relativeVelocity = obj2->velocity - obj1->velocity;

        // 计算相对速度在碰撞法线方向上的投影
        float normalVelocity = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

        // 如果物体正在分离，则不进行碰撞响应
        if (normalVelocity > 0) return;

        // 计算弹性系数（取两个物体的弹性系数的最小值）
        float e = (((obj1->restitution) < (obj2->restitution)) ? (obj1->restitution) : (obj2->restitution));

        // 计算冲量大小
        float j = -(1 + e) * normalVelocity;

        // 考虑质量
        if (!obj1->isStatic && !obj2->isStatic) {
            j /= (1 / obj1->mass) + (1 / obj2->mass);
        }
        else if (!obj1->isStatic) {
            j /= (1 / obj1->mass);
        }
        else if (!obj2->isStatic) {
            j /= (1 / obj2->mass);
        }

        // 应用冲量
        Vector2 impulse = normal * j;

        if (!obj1->isStatic) {
            obj1->velocity = obj1->velocity - impulse * (1 / obj1->mass);
        }

        if (!obj2->isStatic) {
            obj2->velocity = obj2->velocity + impulse * (1 / obj2->mass);
        }
    }
};

