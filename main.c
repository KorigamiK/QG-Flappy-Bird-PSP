#include <QuickGame.h>
#include <gu2gl.h>

QGSprite_t bg;
QGSprite_t base;
QGTimer timer;

void update(double dt) {
    
}

void draw_bg() {
    QuickGame_Sprite_Draw(bg);
    bg->transform.position.x = 128;
    QuickGame_Sprite_Draw(bg);
    bg->transform.position.x = 384;
    QuickGame_Sprite_Draw(base);
}

void draw_base() {
    glTexOffset(timer.total * 0.5f, 0.0f);
    base->transform.position.x = 128;
    QuickGame_Sprite_Draw(base);
    base->transform.position.x = 384;
    QuickGame_Sprite_Draw(base);
    glTexOffset(0.0f, 0.0f);
}

void draw() {
    QuickGame_Graphics_Start_Frame();
    QuickGame_Graphics_Clear();

    draw_bg();
    draw_base();

    QuickGame_Graphics_End_Frame(true);
}

int main() {
    if(QuickGame_Init() < 0)
        return 1;

    QuickGame_Graphics_Set2D();

    QuickGame_Timer_Start(&timer);

    QGTexInfo bgTexInfo = {
        .filename = "./assets/sprites/bg.png",
        .flip = true,
        .vram = 0
    };

    bg = QuickGame_Sprite_Create_Contained(240, 128 + 16, 256, 256, bgTexInfo);

    QGTexInfo baseTexInfo = {
        .filename = "./assets/sprites/slide.png",
        .flip = true,
        .vram = 0
    };

    base = QuickGame_Sprite_Create_Contained(240, 16, 256, 64, baseTexInfo);

    while(true) {
        update(QuickGame_Timer_Delta(&timer));
        draw();
    }
    
    QuickGame_Terminate();
    return 0;
}