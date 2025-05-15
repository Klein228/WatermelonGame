#pragma once
#include"Watermelon.h"
//游戏类
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
        // 初始化物理引擎
        physicsEngine.setGravity(Vector2(0, 600.0f));  // 设置重力
        physicsEngine.setFriction(0.99f);             // 设置摩擦
        
        // 初始化窗口
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

    // 添加新西瓜
    void addWatermelon(float x, float y, int type = 0) {
        Watermelon* melon = new Watermelon(x, y, type, imgSources.getAllImg());
        watermelons.push_back(melon);
        physicsEngine.addObject(melon);
    }

    // 检测并处理西瓜合并
    void checkMerges() {
        // 标记需要合并的西瓜
        std::vector<std::pair<int, int>> mergePairs;

        for (size_t i = 0; i < watermelons.size(); i++) {
            for (size_t j = i + 1; j < watermelons.size(); j++) {
                Watermelon* melon1 = watermelons[i];
                Watermelon* melon2 = watermelons[j];

                // 如果两个西瓜类型相同且距离足够近，则标记为合并
                if (melon1->type == melon2->type) {
                    Vector2 distance = melon2->position - melon1->position;
                    if (distance.length() <= (melon1->radius + melon2->radius) * 1.05f) {
                        mergePairs.push_back({ i, j });
                    }
                }
            }
        }

        // 处理合并（注意：从后向前删除以避免索引问题）
        for (auto it = mergePairs.rbegin(); it != mergePairs.rend(); ++it) {
            int i = it->first;
            int j = it->second;

            // 确保索引有效
            if (i < 0 || i >= watermelons.size() || j < 0 || j >= watermelons.size()) {
                continue;
            }

            // 合并两个西瓜
            Watermelon* melon1 = watermelons[i];
            Watermelon* melon2 = watermelons[j];

            // 计算新位置（两个西瓜的中心）
            Vector2 newPos = (melon1->position + melon2->position) * 0.5f;
            //计算新速度
            Vector2 speed = (melon1->velocity + melon2->velocity) * 0.1f;
            // 删除旧西瓜
            delete melon2;
            watermelons.erase(watermelons.begin() + j);

            // 升级剩下的西瓜
            melon1->position = newPos;
            melon1->velocity = speed;  // 合并后重置速度
            melon1->upgrade();

        }

    }
    // 游戏主循环
    void run() {
        //初始化下个西瓜提示
        nextfruitTip.setImgs(imgSources.getAllImg());
        nextfruitTip.randomNext();
        float deltaTime = 0.008f;  // 约60FPS
        boolean gameover = false;
        while (!gameover) {
            if (peekmessage(&msg, EX_MOUSE)) {
                switch (msg.message)
                {
                case WM_MOUSEMOVE:
                    //鼠标移动提示位置更新
                    nextfruitTip.input(msg);
                    break;
                case WM_LBUTTONDOWN:
                    // 处理鼠标点击，添加新西瓜
                    addWatermelon(msg.x, 0, this->nextfruitTip.randomNext());
                    break;
                default:
                    break;
                }
            }

            // 清屏
            cleardevice();

            // 更新物理引擎
            physicsEngine.update(deltaTime);

            // 检测并处理合并
            checkMerges();
            // 绘制所有西瓜

            for (auto melon : watermelons) {
                melon->draw();
                if (melon->type >= melon->fruitImgs->size() - 1) {
                    std::cout << "成功合成大西瓜！" << std::endl;
                    gameover = true;
                }
            }
            //绘制提示下一个类型
            nextfruitTip.draw();
            // 绘制边界
            setlinecolor(BLACK);
            line(0, 0, windowWidth, 0);           // 上边界
            line(0, 0, 0, windowHeight);          // 左边界
            line(windowWidth - 1, 0,
                windowWidth - 1, windowHeight);  // 右边界

            // 刷新显示
            FlushBatchDraw();

            // 控制帧率
            Sleep(deltaTime * 1000);
        }
    }
};