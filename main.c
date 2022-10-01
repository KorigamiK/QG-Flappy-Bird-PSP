#include <QuickGame.h>
#include <gu2gl.h>
#include <pspctrl.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

QGSprite_t bg, base, gameover, pipe;
QGSprite_t score[10];
QGSprite_t bird[3];

QGTimer timer;

float bird_y;
float vel_y;
bool started, dead;
int current_score;

int curr_anim = 0;
float anim_time = 0.0f;

typedef struct {
    float x, y;
    bool active;
} PipeInfo;
PipeInfo pipes[3];

void flap_wings(double dt) {
    anim_time += dt;

    if(anim_time > 0.125f) {
        curr_anim++;
        anim_time = 0.0f;

        if(curr_anim == 3)
            curr_anim = 0;
    }
}

void draw_base_scroll() {
    glTexOffset(timer.total * 0.5f, 0.0f);

    base->transform.position.x = 128;
    QuickGame_Sprite_Draw(base);
    base->transform.position.x = 384;
    QuickGame_Sprite_Draw(base);

    glTexOffset(0.0f, 0.0f);
}

void reset_game() {
    bird_y = 136.0f;
    dead = false;
    started = false;
    vel_y = 0.0f;
    current_score = 0;

    for(int i = 0; i < 3; i++){
        pipes[i].x = 512 + 192 * i;
        pipes[i].y = 96 + rand() % 128;
        pipes[i].active = true;
    }
}

void animation_update() {
    bird[curr_anim]->transform.position.y = bird_y;
    if(vel_y < -20.0f)
        bird[curr_anim]->transform.rotation = 30.0f;
    else if (vel_y >= -20.0f && vel_y <= 20.0f)
        bird[curr_anim]->transform.rotation = 0.0f;
    else if (vel_y > 20.0f)
        bird[curr_anim]->transform.rotation = -30.0f;
}

void update(double dt) {
    QuickGame_Input_Update();

    if(!dead) {
        flap_wings(dt);
        animation_update();

        if(QuickGame_Button_Pressed(PSP_CTRL_CROSS)){
            started = true;
            vel_y = -96.0f * 2.0f;
        }

        if(started) {
            vel_y += 256.0f * dt * 2.0f;
            bird_y -= vel_y * dt;

            if(bird_y < 48)
                dead = true;
            
            for(int i = 0; i < 3; i++){
                pipes[i].x -= 128.0f * dt;

                if(pipes[i].x < 160 && pipes[i].active){
                    pipes[i].active = false;
                    current_score++;
                }

                if(pipes[i].x < -32){
                    pipes[i].x += 384 + 192;
                    pipes[i].y = 96 + rand() % 128;
                    pipes[i].active = true;
                }
            }
        }
        
    } else {
        QuickGame_Timer_Reset(&timer);

        if(QuickGame_Button_Pressed(PSP_CTRL_CROSS)) 
            reset_game();
    }
}

void draw_score(){
    int s = current_score;

    int digits = 0;
    while(s > 0){
        s /= 10;
        digits++;
    }

    float xoff = -((float)digits-1) / 2.0f;
    xoff *= 24.0f;

    float xn = 0.0f;

    s = current_score;
    while(s > 0){
        int c = s % 10;
        s /= 10;

        score[c]->transform.position.x = -xoff + 240 - xn;
        score[c]->transform.position.y = 192;

        xn += 24.0f;

        QuickGame_Sprite_Draw(score[c]);
    }
}

void draw_pipes(){
    for(int i = 0; i < 3; i++){
        pipe->transform.position.x = pipes[i].x;
        pipe->transform.position.y = pipes[i].y - 128 - 40;
        pipe->transform.rotation = 0.0f;
        QuickGame_Sprite_Draw(pipe);

        if(QuickGame_Sprite_Intersects(pipe, bird[curr_anim])) {
            dead = true;
        }

        pipe->transform.position.y = pipes[i].y + 128 + 40;
        pipe->transform.rotation = 180.0f;
        QuickGame_Sprite_Draw(pipe);

        if(QuickGame_Sprite_Intersects(pipe, bird[curr_anim])) {
            dead = true;
        }
    }
}

void draw() {
    QuickGame_Graphics_Start_Frame();
    QuickGame_Graphics_Clear();

    QuickGame_Sprite_Draw(bg);
    draw_pipes();
    draw_base_scroll();
    QuickGame_Sprite_Draw(bird[curr_anim]);

    if(dead)
        QuickGame_Sprite_Draw(gameover);
    draw_score();

    QuickGame_Graphics_End_Frame(true);
}

void load_sprites() {
    QGTexInfo bgTexInfo = {.filename = "./assets/sprites/bg.png", .flip = true, .vram = 0 };
    bg = QuickGame_Sprite_Create_Contained(240, 128 + 64, 512, 512, bgTexInfo);

    QGTexInfo baseTexInfo = { .filename = "./assets/sprites/slide.png", .flip = true, .vram = 0 };
    base = QuickGame_Sprite_Create_Contained(240, 16, 256, 64, baseTexInfo);

    QGTexInfo b1 = { .filename = "./assets/sprites/bird/down.png", .flip = true, .vram = 0 };
    bird[0] = QuickGame_Sprite_Create_Contained(160, 136, 64, 32, b1);

    QGTexInfo b2 = { .filename = "./assets/sprites/bird/idle.png", .flip = true, .vram = 0 };
    bird[1] = QuickGame_Sprite_Create_Contained(160, 136, 64, 32, b2);

    QGTexInfo b3 = { .filename = "./assets/sprites/bird/up.png", .flip = true, .vram = 0 };
    bird[2] = QuickGame_Sprite_Create_Contained(160, 136, 64, 32, b3);

    for(int i = 0; i < 3; i++){
        bird[i]->aabb_size.x = 32;
        bird[i]->aabb_size.y = 20;
    }

    QGTexInfo go = { .filename = "./assets/sprites/gameover.png", .flip = true, .vram = 0 };
    gameover = QuickGame_Sprite_Create_Contained(240, 136, 256, 64, go);

    QGTexInfo p = { .filename = "./assets/sprites/pipe.png", .flip = true, .vram = 0 };
    pipe = QuickGame_Sprite_Create_Contained(0, 0, 64, 256, p);

    for(int i = 0; i < 10; i++){
        char filename[256];
        sprintf(filename, "./assets/sprites/count/%d.png", i);

        QGTexInfo sc = { .filename = filename, .flip = true, .vram = 0 };
        score[i] = QuickGame_Sprite_Create_Contained(240, 136, 32, 64, sc);
    }
}

int main() {
    if(QuickGame_Init() < 0)
        return 1;

    QuickGame_Graphics_Set2D();
    QuickGame_Timer_Start(&timer);

    load_sprites();
    reset_game();

    srand(time(NULL));

    while(true) {
        update(QuickGame_Timer_Delta(&timer));
        draw();
    }
    
    QuickGame_Terminate();
    return 0;
}