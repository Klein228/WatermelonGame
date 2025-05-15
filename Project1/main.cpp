#include<graphics.h>//引用图形库头文件
#include <conio.h>
#include"PhysicsObject.h"
#include"Watermelon.h"
#include"WatermelonGame.h"
int main()
{
    WatermelonGame game(600, 600);

    //// 添加一些初始西瓜

    //// 运行游戏
    game.run();

    return 0;
}
