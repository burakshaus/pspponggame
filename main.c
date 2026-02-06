#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <string.h>

PSP_MODULE_INFO("PSPPONG", 0,1,1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER;

static unsigned int __attribute__((aligned(16)) list [262144]);
#define BUF_WIDTH 512
#define SCR_WIDTH 480
#define SCR_HEIGHT 272

struct Vertex {
unsigned int color;
float x,y,z;
}

typedef struct {
float x, y,w,h;
float vx, vy;
unsigned int color ;
} Entity;

Entity paddle = {20,110,10,60,0,0,0xFFFFFFFF};
Entity ball = {240,136,8,8,3.0f,3.0f,0xFF00FFFF};

void init_gu(){
sceGuInit();
sceGuStart(GU_DIRECT,list);
sceGuDrawBuffer(GU_PSM_8888, (void*) BUF_WIDTH);
sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,(void*) 0x88000, BUF_WIDTH);
sceDepthBuffer((void*)0x110000, BUF_WIDTH);
sceGuOffset(2048,2048,SCR_WIDTH,SCR_HEIGHT);
sceGuEnable(GU_BLEND);
sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
sceGuFinish();
sceGuSync(0,0);
sceDisplayWaitVblankstart();
sceGuDisplay(Gu_TRUE);
}
void draw_rect(float x,float y,float w,float h, unsigned int color){
struct Vertex* vertices = (struct Vertex*)sceGuGetMemory(2*(struct Vertex));
vertices[0].color = color;
vertices[1].x = x; vertices[0].y = y; vertices[0].z = 0;
vertices[1].color = color;
vertices[1].x = x+w; vertices[1].y= y+h; vertices[1].z = 0;
sceGuDrawArray(GU_SPRITES, GU_COLOR8888 | GU_VERTEX32BITF | GU_TRANSFORM_2D, 2,0, vertices);

}

int main(){
init_gu();
SceCrtlData pad;
while(1){
sceCrtlPeekBufferPositive(&pad, 1);
if(pad.Buttons & PSP_CRTL_START){
break;
}
if(pad.Buttons & PSP_CRTL_UP && paddle.y>0) paddle.y -= 5.0f;
if(pad.Buttons & PSP_CRTL_DOWN && paddle.y < SCR_HEIGHT -paddle.h) paddle.y += 5.0f;

ball.x += ball.vx;
ball.y += ball.vy;

if (ball.y <= 0 || ball.y >= SCR_HEIGHT - ball.h) ball.vy = -ball.vy;
        if (ball.x >= SCR_WIDTH - ball.w) ball.vx = -ball.vx; // Bounce off right wall

        // Collision: Ball vs Paddle
        if (ball.x <= paddle.x + paddle.w && ball.x >= paddle.x &&
            ball.y + ball.h >= paddle.y && ball.y <= paddle.y + paddle.h) {
            ball.vx = -ball.vx;
            ball.x = paddle.x + paddle.w + 1; // Unstick
        }

        // Reset if missed
        if (ball.x < 0) { ball.x = 240; ball.y = 136; ball.vx = 3.0f; }

        // Rendering
        sceGuStart(GU_DIRECT, list);
        sceGuClearColor(0xFF000000); // Black background
        sceGuClear(GU_COLOR_BUFFER_BIT);

        draw_rect(paddle.x, paddle.y, paddle.w, paddle.h, paddle.color);
        draw_rect(ball.x, ball.y, ball.w, ball.h, ball.color);

        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();

}
sceGuDisplay(GU_FALSE);
sceGuTerm();
sceKernelExitGame();
return 0;
}
