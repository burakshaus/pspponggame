#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspdebug.h>

PSP_MODULE_INFO("PongAI", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

static unsigned int __attribute__((aligned(16))) list[262144];
#define BUF_WIDTH 512
#define SCR_WIDTH 480
#define SCR_HEIGHT 272

struct Vertex { unsigned int color; float x, y, z; };

typedef struct { float x, y, w, h, vx, vy; unsigned int color; } Entity;

Entity player = { 20, 110, 10, 50, 0, 0, 0xFFFFFFFF };
Entity cpu = { 450, 110, 10, 50, 0, 0, 0xFFFFFFFF };
Entity ball = { 240, 136, 8, 8, 2.5f, 2.5f, 0xFF00FFFF };

void draw_rect(float x, float y, float w, float h, unsigned int color) {
    struct Vertex* vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(struct Vertex));
    vertices[0].color = color; vertices[0].x = x; vertices[0].y = y; vertices[0].z = 0;
    vertices[1].color = color; vertices[1].x = x + w; vertices[1].y = y + h; vertices[1].z = 0;
    sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
}

int main() {
    pspDebugScreenInit(); // Backup: allows pspDebugScreenPrintf to work
    
    // Setup GU
    sceGuInit();
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void*)0x88000, BUF_WIDTH);
    sceGuDepthBuffer((void*)0x110000, BUF_WIDTH);
    sceGuOffset(2048 - (SCR_WIDTH/2), 2048 - (SCR_HEIGHT/2));
    sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuClearColor(0xFF000000); 
    sceGuDisplay(GU_TRUE);
    sceGuFinish();
    sceGuSync(0,0);

    SceCtrlData pad;
    while (1) {
        sceCtrlPeekBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_START) break;

        // Logic
        if (pad.Buttons & PSP_CTRL_UP && player.y > 0) player.y -= 4.0f;
        if (pad.Buttons & PSP_CTRL_DOWN && player.y < SCR_HEIGHT - player.h) player.y += 4.0f;
        
        if (ball.y > cpu.y + (cpu.h / 2)) cpu.y += 2.0f;
        else cpu.y -= 2.0f;

        ball.x += ball.vx; ball.y += ball.vy;
        if (ball.y <= 0 || ball.y >= SCR_HEIGHT - ball.h) ball.vy = -ball.vy;
        
        if (ball.x <= player.x + player.w && ball.y + ball.h >= player.y && ball.y <= player.y + player.h) ball.vx = -ball.vx;
        if (ball.x + ball.w >= cpu.x && ball.y + ball.h >= cpu.y && ball.y <= cpu.y + cpu.h) ball.vx = -ball.vx;
        if (ball.x < 0 || ball.x > SCR_WIDTH) { ball.x = 240; ball.y = 136; ball.vx = -ball.vx; }

        // Rendering
        sceGuStart(GU_DIRECT, list);
        sceGuClear(GU_COLOR_BUFFER_BIT);

        draw_rect(player.x, player.y, player.w, player.h, player.color);
        draw_rect(cpu.x, cpu.y, cpu.w, cpu.h, cpu.color);
        draw_rect(ball.x, ball.y, ball.w, ball.h, ball.color);

        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }
    sceKernelExitGame();
    return 0;
}
