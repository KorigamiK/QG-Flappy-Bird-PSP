#include <QuickGame.h>

void update(double dt) {

}

void draw() {
    QuickGame_Graphics_Start_Frame();
    QuickGame_Graphics_Clear();

    QuickGame_Graphics_End_Frame(true);
}

int main() {
    if(QuickGame_Init() < 0)
        return 1;

    QGTimer timer;
    QuickGame_Timer_Start(&timer);

    while(QuickGame_Running()) {
        update(QuickGame_Timer_Delta(&timer));
        draw();
    }
    
    QuickGame_Terminate();
    return 0;
}