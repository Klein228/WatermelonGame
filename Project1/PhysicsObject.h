#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include"utils.h"

// ���������
class PhysicsObject {
public:
    Vector2 position;     // λ��
    Vector2 velocity;     // �ٶ�
    Vector2 acceleration; // ���ٶ�
    float radius;         // �뾶��������ײ��⣩
    float mass;           // ����
    float restitution;    // ����ϵ����0-1֮�䣩
    bool isStatic;        // �Ƿ�Ϊ��̬���󣨲�����Ӱ�죩
    PhysicsObject(float x, float y, float radius, float mass = 1.0f, float restitution = 0.8f)
        : position(x, y), velocity(0, 0), acceleration(0, 0),
        radius(radius), mass(mass), restitution(restitution), isStatic(false) {
    }

    // ���¶���״̬
    void update(float deltaTime) {
        if (isStatic) return;

        // ���ݼ��ٶȸ����ٶ�
        velocity = velocity + acceleration * deltaTime;

        // �����ٶȸ���λ��
        position = position + velocity * deltaTime;

        // ���ü��ٶȣ���Ҫ����һ֡����Ӧ������
        acceleration = Vector2();
    }

    // Ӧ����
    void applyForce(const Vector2& force) {
        if (isStatic) return;
        // F = ma -> a = F/m
        acceleration = acceleration + force * (1.0f / mass);
    }

    PhysicsObject() = default;
};

// ����������
class PhysicsEngine {
private:
    std::vector<PhysicsObject*> objects;
    Vector2 gravity;        // ��������
    float friction;         // Ħ��ϵ��
    int windowWidth;
    int windowHeight;       //���ڴ�С

public:
    PhysicsEngine(int w,int h):windowWidth(w),windowHeight(h){
        friction = 0.99f;
        gravity = Vector2(0, 200.0f);
    }
    // ����������
    void addObject(PhysicsObject* object) {
        objects.push_back(object);
    }

    // ��������
    void setGravity(const Vector2& gravity) {
        this->gravity = gravity;
    }

    // ����Ħ��ϵ��
    void setFriction(float friction) {
        this->friction = friction;
    }

    // ���������������
    void update(float deltaTime) {
        // 1. Ӧ������
        for (auto object : objects) {
            if (!object->isStatic) {
                object->applyForce(gravity * object->mass);
            }
        }

        // 2. �������ж���״̬
        for (auto object : objects) {
            object->update(deltaTime);
        }

        // 3. Ӧ��Ħ����
        for (auto object : objects) {
            if (!object->isStatic) {
                object->velocity = object->velocity * friction;
            }
        }
        // 4. ���ʹ���߽���ײ
        detectAndResolveBoundaryCollisions();
        // 4. ���ʹ�����ײ
        detectAndResolveCollisions();
    }

    // ���ʹ�����ײ
    void detectAndResolveCollisions() {
        // ��ʵ�֣�������ж����֮�����ײ
        for (size_t i = 0; i < objects.size(); i++) {
            for (size_t j = i + 1; j < objects.size(); j++) {
                PhysicsObject* obj1 = objects[i];
                PhysicsObject* obj2 = objects[j];

                // ������������֮��ľ���
                Vector2 distance = obj2->position - obj1->position;
                float distLength = distance.length();

                // ����������巢����ײ
                if (distLength < obj1->radius + obj2->radius) {
                    // ������ײ��Ӧ
                    resolveCollision(obj1, obj2, distance, distLength);
                }
            }
        }
    }
    // ���ʹ���߽���ײ
    void detectAndResolveBoundaryCollisions() {
        for (auto object : objects) {
            if (object->isStatic) continue;

            // ��߽���ײ
            if (object->position.x - object->radius < 0) {
                object->position.x = object->radius;  // ��ֹ��͸�߽�
                object->velocity.x = -object->velocity.x * object->restitution;  // ����
            }

            // �ұ߽���ײ
            if (object->position.x + object->radius > windowWidth) {
                object->position.x = windowWidth - object->radius;
                object->velocity.x = -object->velocity.x * object->restitution;
            }

            // �ϱ߽���ײ
            if (object->position.y - object->radius < 0) {
                object->position.y = object->radius;
                object->velocity.y = -object->velocity.y * object->restitution;
            }

            // �±߽���ײ
            if (object->position.y + object->radius > windowHeight) {
                object->position.y = windowHeight - object->radius;
                object->velocity.y = -object->velocity.y * object->restitution;
            }
        }
    }
    // ������������֮�����ײ
    void resolveCollision(PhysicsObject* obj1, PhysicsObject* obj2,
        const Vector2& distance, float distLength) {
        // ������ײ���ߣ���obj1ָ��obj2�ĵ�λ������
        Vector2 normal = distance.normalize();

        // �����ص���
        float overlap = (obj1->radius + obj2->radius) - distLength;

        // ������������
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

        // ��������ٶ�
        Vector2 relativeVelocity = obj2->velocity - obj1->velocity;

        // ��������ٶ�����ײ���߷����ϵ�ͶӰ
        float normalVelocity = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

        // ����������ڷ��룬�򲻽�����ײ��Ӧ
        if (normalVelocity > 0) return;

        // ���㵯��ϵ����ȡ��������ĵ���ϵ������Сֵ��
        float e = (((obj1->restitution) < (obj2->restitution)) ? (obj1->restitution) : (obj2->restitution));

        // ���������С
        float j = -(1 + e) * normalVelocity;

        // ��������
        if (!obj1->isStatic && !obj2->isStatic) {
            j /= (1 / obj1->mass) + (1 / obj2->mass);
        }
        else if (!obj1->isStatic) {
            j /= (1 / obj1->mass);
        }
        else if (!obj2->isStatic) {
            j /= (1 / obj2->mass);
        }

        // Ӧ�ó���
        Vector2 impulse = normal * j;

        if (!obj1->isStatic) {
            obj1->velocity = obj1->velocity - impulse * (1 / obj1->mass);
        }

        if (!obj2->isStatic) {
            obj2->velocity = obj2->velocity + impulse * (1 / obj2->mass);
        }
    }
};

