#pragma once
#include"Watermelon.h"
//��Ϸ��
class WatermelonGame {
private:
    PhysicsEngine physicsEngine;
    ImgSources imgSources;
    NextFruit nextfruitTip;
    std::vector<Watermelon*> watermelons;
    int windowWidth, windowHeight;
    ExMessage msg;
public:
    WatermelonGame(int width, int height) : windowWidth(width), windowHeight(height)
        , physicsEngine(width, height)
    {
        // ��ʼ����������
        physicsEngine.setGravity(Vector2(0, 600.0f));  // ��������
        physicsEngine.setFriction(0.99f);             // ����Ħ��
        
        // ��ʼ������
        initgraph(width, height);
        setbkcolor(WHITE);
        BeginBatchDraw();

    }

    ~WatermelonGame() {
        for (auto melon : watermelons) {
            delete melon;
        }
        EndBatchDraw();
        closegraph();
    }

    // ���������
    void addWatermelon(float x, float y, int type = 0) {
        Watermelon* melon = new Watermelon(x, y, type, imgSources.getAllImg());
        watermelons.push_back(melon);
        physicsEngine.addObject(melon);
    }

    // ��Ⲣ�������Ϻϲ�
    void checkMerges() {
        // �����Ҫ�ϲ�������
        std::vector<std::pair<int, int>> mergePairs;

        for (size_t i = 0; i < watermelons.size(); i++) {
            for (size_t j = i + 1; j < watermelons.size(); j++) {
                Watermelon* melon1 = watermelons[i];
                Watermelon* melon2 = watermelons[j];

                // �����������������ͬ�Ҿ����㹻��������Ϊ�ϲ�
                if (melon1->type == melon2->type) {
                    Vector2 distance = melon2->position - melon1->position;
                    if (distance.length() <= (melon1->radius + melon2->radius) * 1.05f) {
                        mergePairs.push_back({ i, j });
                    }
                }
            }
        }

        // ����ϲ���ע�⣺�Ӻ���ǰɾ���Ա����������⣩
        for (auto it = mergePairs.rbegin(); it != mergePairs.rend(); ++it) {
            int i = it->first;
            int j = it->second;

            // ȷ��������Ч
            if (i < 0 || i >= watermelons.size() || j < 0 || j >= watermelons.size()) {
                continue;
            }

            // �ϲ���������
            Watermelon* melon1 = watermelons[i];
            Watermelon* melon2 = watermelons[j];

            // ������λ�ã��������ϵ����ģ�
            Vector2 newPos = (melon1->position + melon2->position) * 0.5f;
            //�������ٶ�
            Vector2 speed = (melon1->velocity + melon2->velocity) * 0.1f;
            // ɾ��������
            delete melon2;
            watermelons.erase(watermelons.begin() + j);

            // ����ʣ�µ�����
            melon1->position = newPos;
            melon1->velocity = speed;  // �ϲ��������ٶ�
            melon1->upgrade();

        }

    }
    // ��Ϸ��ѭ��
    void run() {
        //��ʼ���¸�������ʾ
        nextfruitTip.setImgs(imgSources.getAllImg());
        nextfruitTip.randomNext();
        float deltaTime = 0.008f;  // Լ60FPS
        boolean gameover = false;
        while (!gameover) {
            if (peekmessage(&msg, EX_MOUSE)) {
                switch (msg.message)
                {
                case WM_MOUSEMOVE:
                    //����ƶ���ʾλ�ø���
                    nextfruitTip.input(msg);
                    break;
                case WM_LBUTTONDOWN:
                    // ��������������������
                    addWatermelon(msg.x, 0, this->nextfruitTip.randomNext());
                    break;
                default:
                    break;
                }
            }

            // ����
            cleardevice();

            // ������������
            physicsEngine.update(deltaTime);

            // ��Ⲣ����ϲ�
            checkMerges();
            // ������������

            for (auto melon : watermelons) {
                melon->draw();
                if (melon->type >= melon->fruitImgs->size() - 1) {
                    std::cout << "�ɹ��ϳɴ����ϣ�" << std::endl;
                    gameover = true;
                }
            }
            //������ʾ��һ������
            nextfruitTip.draw();
            // ���Ʊ߽�
            setlinecolor(BLACK);
            line(0, 0, windowWidth, 0);           // �ϱ߽�
            line(0, 0, 0, windowHeight);          // ��߽�
            line(windowWidth - 1, 0,
                windowWidth - 1, windowHeight);  // �ұ߽�

            // ˢ����ʾ
            FlushBatchDraw();

            // ����֡��
            Sleep(deltaTime * 1000);
        }
    }
};