#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h> // 시스템 호출 (close, usleep 등)

#define FBDEV "/dev/fb0"

#define COLOR_RED 0xff0000
#define COLOR_GREEN 0x00ff00
#define COLOR_BLUE 0x0000ff
#define COLOR_BLACK 0x00000f
#define COLOR_WHITE 0xffffff
#define COLOR_BROWN 0xc68a12
#define COLOR_GOLD 0xFFD700

pid_t pid;

int fb_fd;
int size;
char *map;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int touch_fd;
int ret;
char *dev_name;

struct input_event ev;

// 입력 장치의 좌표 범위 (160 ~ 3900)
const int original_min = 150;
const int original_max = 4000;

// 목표 해상도 (800x480)
const int target_width = 800;
const int target_height = 480;

static int x_coordinate = 0;
static int y_coordinate = 0;
static int x_scaled = 0;
static int y_scaled = 0;
static int x_selected = 0;
static int y_selected = 0;
static int x_selected_coordinate = 0;
static int y_selected_coordinate = 0;
int x_selected_coordinate_temp = 0;
int y_selected_coordinate_temp = 0;
int x_before_coordinate = 0;
int y_before_coordinate = 0;
int x_selected_temp = 0;
int y_selected_temp = 0;

int color = COLOR_BLACK;
void draw_button();

int selected[9][9] = {0};

// 영어 정의 배열열
const int FONT_5x7[26][7] = {
    {0b010, 0b101, 0b111, 0b101, 0b101, 0b101, 0b101}, // A
    {0b110, 0b101, 0b110, 0b101, 0b101, 0b101, 0b110}, // B
    {0b011, 0b100, 0b100, 0b100, 0b100, 0b100, 0b011}, // C
    {0b110, 0b101, 0b101, 0b101, 0b101, 0b101, 0b110}, // D
    {0b111, 0b100, 0b100, 0b110, 0b100, 0b100, 0b111}, // E
    {0b111, 0b100, 0b100, 0b110, 0b100, 0b100, 0b100}, // F
    {0b011, 0b100, 0b100, 0b101, 0b101, 0b101, 0b011}, // G
    {0b101, 0b101, 0b101, 0b111, 0b101, 0b101, 0b101}, // H
    {0b111, 0b010, 0b010, 0b010, 0b010, 0b010, 0b111}, // I
    {0b111, 0b010, 0b010, 0b010, 0b010, 0b110, 0b100}, // J
    {0b101, 0b101, 0b110, 0b100, 0b110, 0b101, 0b101}, // K
    {0b100, 0b100, 0b100, 0b100, 0b100, 0b100, 0b111}, // L
    {0b101, 0b111, 0b101, 0b101, 0b101, 0b101, 0b101}, // M
    {0b101, 0b111, 0b111, 0b111, 0b111, 0b111, 0b101}, // N
    {0b010, 0b101, 0b101, 0b101, 0b101, 0b101, 0b010}, // O
    {0b110, 0b101, 0b101, 0b110, 0b100, 0b100, 0b100}, // P
    {0b011, 0b100, 0b100, 0b101, 0b101, 0b101, 0b011}, // Q
    {0b110, 0b101, 0b101, 0b110, 0b101, 0b101, 0b101}, // R
    {0b011, 0b100, 0b100, 0b010, 0b001, 0b001, 0b110}, // S
    {0b111, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010}, // T
    {0b101, 0b101, 0b101, 0b101, 0b101, 0b101, 0b011}, // U
    {0b101, 0b101, 0b101, 0b101, 0b101, 0b010, 0b010}, // V
    {0b101, 0b101, 0b101, 0b101, 0b111, 0b111, 0b101}, // W
    {0b101, 0b101, 0b010, 0b010, 0b010, 0b101, 0b101}, // X
    {0b101, 0b101, 0b101, 0b101, 0b010, 0b010, 0b010}, // Y
    {0b111, 0b001, 0b010, 0b010, 0b100, 0b100, 0b111}, // Z
};

const int FONT_3x5[10][5] = {
    {0b111, 0b101, 0b101, 0b101, 0b111}, // 0
    {0b010, 0b110, 0b010, 0b010, 0b111}, // 1
    {0b111, 0b001, 0b111, 0b100, 0b111}, // 2
    {0b111, 0b001, 0b111, 0b001, 0b111}, // 3
    {0b101, 0b101, 0b111, 0b001, 0b001}, // 4
    {0b111, 0b100, 0b111, 0b001, 0b111}, // 5
    {0b111, 0b100, 0b111, 0b101, 0b111}, // 6
    {0b111, 0b001, 0b001, 0b010, 0b010}, // 7
    {0b111, 0b101, 0b111, 0b101, 0b111}, // 8
    {0b111, 0b101, 0b111, 0b001, 0b111}  // 9
};

void draw_letter(char letter, int x, int y, int scale, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map);
void draw_number(int num, int x, int y, int scale, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map);
void draw_rect(int x, int y, int w, int h, unsigned int color,
               struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
               char *map);

void draw_rect(int x, int y, int w, int h, unsigned int color,
               struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
               char *map)
{
  int xx, yy;
  int location = 0;

  for (yy = y; yy < (y + h); yy++)
  {
    for (xx = x; xx < (x + w); xx++)
    {
      location = xx * (vip->bits_per_pixel / 8) + yy * fip->line_length;
      if (vip->bits_per_pixel == 32)
      { /* 32 bpp */
        *(unsigned int *)(map + location) = color;
      }
      else
      { /* 16 bpp */
        int r = color >> 16;
        int g = (color >> 8) & 0xff;
        int b = color & 0xff;
        *(unsigned short *)(map + location) =
            (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
      }
    }
  }
}

void draw_letter(char letter, int x, int y, int scale, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map)
{
  if (letter < 'A' || letter > 'Z')
    return; // A~Z가 아닐 경우 무시
  int index = letter - 'A';

  for (int row = 0; row < 7; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      if ((FONT_5x7[index][row] >> (2 - col)) & 1)
      {
        // 비트가 1이면 픽셀 그리기
        draw_rect(x + col * scale, y + row * scale, scale, scale, color, vip,
                  fip, map);
      }
    }
  }
}

void draw_number(int num, int x, int y, int scale, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map)
{
  if (num < 0 || num > 9)
    return; // 숫자가 0~9 범위를 벗어나면 무시

  for (int row = 0; row < 5; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      if ((FONT_3x5[num][row] >> (2 - col)) & 1)
      {
        // 비트가 1이면 색칠
        draw_rect(x + col * scale, y + row * scale, scale, scale, color, vip,
                  fip, map);
      }
    }
  }
}

void draw_circle(int cx, int cy, int r, unsigned int color,
                 struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
                 char *map)
{
  int xx, yy;
  int location = 0;

  // 원을 그리기 위해 y축을 기준으로 x축을 조정
  for (yy = cy - r; yy <= cy + r; yy++)
  {
    for (xx = cx - r; xx <= cx + r; xx++)
    {
      // 원의 방정식 (x - cx)^2 + (y - cy)^2 <= r^2
      if ((xx - cx) * (xx - cx) + (yy - cy) * (yy - cy) <= r * r)
      {
        location = xx * (vip->bits_per_pixel / 8) + yy * fip->line_length;
        if (vip->bits_per_pixel == 32)
        {
          *(unsigned int *)(map + location) = color;
        }
        else
        {
          int r_color = color >> 16;
          int g_color = (color >> 8) & 0xff;
          int b_color = color & 0xff;
          *(unsigned short *)(map + location) =
              (unsigned short)(((r_color >> 3) << 11) | ((g_color >> 2) << 5) |
                               (b_color >> 3));
        }
      }
    }
  }
}

void draw_text(const char *text, int x, int y, int scale, unsigned int color,
               struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip,
               char *map)
{
  int spacing = scale * 4; // 글자 간 간격 설정

  while (*text)
  {
    if (*text == ' ')
    {
      x += spacing; // 공백일 경우 x 좌표만 이동
    }
    else if (*text >= 'A' && *text <= 'Z')
    {
      draw_letter(*text, x, y, scale, color, vip, fip, map);
      x += spacing;
    }
    else if (*text >= '0' && *text <= '9')
    {
      draw_number(*text - '0', x, y, scale * 1.5, color, vip, fip, map);
      x += spacing * 1.5;
    }
    text++;
  }
}

void draw_target()
{
  if (selected[x_selected_temp][y_selected_temp] != 1)
  {
    if (x_before_coordinate == 0 && y_before_coordinate == 0)
    {
      draw_rect(0, 0, 1, 1, COLOR_BLACK, &vinfo, &finfo, map);
    }

    else if (x_selected_coordinate >= 220 && x_selected_coordinate <= 580 &&
             y_selected_coordinate >= 60 && y_selected_coordinate <= 420)
    {
      draw_rect(x_before_coordinate - 15, y_before_coordinate - 15, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate - 10, y_before_coordinate - 15, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate - 15, y_before_coordinate - 10, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);

      draw_rect(x_before_coordinate - 15, y_before_coordinate + 5, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate - 15, y_before_coordinate + 10, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate - 10, y_before_coordinate + 10, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);

      draw_rect(x_before_coordinate + 5, y_before_coordinate + 10, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate + 10, y_before_coordinate + 5, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate + 10, y_before_coordinate + 10, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);

      draw_rect(x_before_coordinate + 5, y_before_coordinate - 15, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate + 10, y_before_coordinate - 15, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);
      draw_rect(x_before_coordinate + 10, y_before_coordinate - 10, 5, 5,
                COLOR_BROWN, &vinfo, &finfo, map);

      draw_rect(x_selected_coordinate - 15, y_selected_coordinate - 15, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate - 10, y_selected_coordinate - 15, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate - 15, y_selected_coordinate - 10, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);

      draw_rect(x_selected_coordinate - 15, y_selected_coordinate + 5, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate - 15, y_selected_coordinate + 10, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate - 10, y_selected_coordinate + 10, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);

      draw_rect(x_selected_coordinate + 5, y_selected_coordinate + 10, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate + 10, y_selected_coordinate + 5, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate + 10, y_selected_coordinate + 10, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);

      draw_rect(x_selected_coordinate + 5, y_selected_coordinate - 15, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate + 10, y_selected_coordinate - 15, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
      draw_rect(x_selected_coordinate + 10, y_selected_coordinate - 10, 5, 5,
                COLOR_RED, &vinfo, &finfo, map);
    }
  }
  draw_button();
}

void add_stone()
{
  if (selected[x_selected_temp][y_selected_temp] != 1)
  {
    if (color == COLOR_BLACK)
    {
      draw_circle(x_selected_coordinate_temp, y_selected_coordinate_temp, 22,
                  COLOR_BLACK, &vinfo, &finfo, map);

      color = COLOR_WHITE;


    }

    else if (color == COLOR_WHITE)
    {
      draw_circle(x_selected_coordinate_temp, y_selected_coordinate_temp, 22,
                  COLOR_WHITE, &vinfo, &finfo, map);
      color = COLOR_BLACK;

    }
    printf("%d, %d\n", x_selected_temp, y_selected_temp);
  }
}

int map_init()
{
  /* open */
  fb_fd = open(FBDEV, O_RDWR);
  if (fb_fd == -1)
  {
    printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
    return EXIT_FAILURE;
  }
  printf("[%d] %s was opened successfully\n", pid, FBDEV);

  /* get fb_var_screeninfo */
  if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) == -1)
  {
    printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
    return EXIT_FAILURE;
  }
  printf("[%d] %dx%d %dbpp\n", pid, vinfo.xres, vinfo.yres,
         vinfo.bits_per_pixel);

  /* get fb_fix_screeninfo */
  if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) == -1)
  {
    printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
    return EXIT_FAILURE;
  }
  printf("[%d] line_length is %d\n", pid, finfo.line_length);

  /* mmap */
  size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
  map = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
  if (map == (char *)-1)
  {
    printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
    return EXIT_FAILURE;
  }
  printf("[%d] %s was mapped to %p\n", pid, FBDEV, map);
  return 0;
}

void timer()
{
  for (int i = 0; i <= 15; i++)
  {
    char buf[32];
    sprintf(buf, "%02d", 15 - i);
    draw_text(buf, 670, 20, 11, COLOR_WHITE, &vinfo, &finfo, map);

    sleep(1);
    draw_rect(625, 15, 175, 150, COLOR_BLACK, &vinfo, &finfo, map);
  }
  // draw_rect(625, 15, 175, 150, COLOR_BLACK, &vinfo, &finfo, map);
  // draw_rect(315, 195, 150, 95, COLOR_GOLD, &vinfo, &finfo, map);
  // draw_text("TIME OVER", 320, 200, 4, COLOR_WHITE, &vinfo, &finfo, map);
  
}

void draw_turn()
{
  if (color == COLOR_BLACK)
  {
    draw_circle(750, 430, 30, COLOR_BLACK, &vinfo, &finfo, map);
    
  }

  else if (color == COLOR_WHITE)
  {
    draw_circle(750, 430, 30, COLOR_WHITE, &vinfo, &finfo, map);
    
  }
  
}

void draw_button()
{
  draw_rect(630, 280, 170, 200, COLOR_BLACK, &vinfo, &finfo, map);

  char buf1[32];
  
  if(ev.value == 0)
  {
    sprintf(buf1, "ME  BLACK");
  }
  else{
    sprintf(buf1, "ME  WHITE");

  }

  draw_text(buf1, 650, 330, 4, COLOR_WHITE, &vinfo, &finfo, map);
  

  draw_rect(700, 380, 100, 100, COLOR_BLUE, &vinfo, &finfo, map);
  //draw_turn();
  
}

// void gmaeover_msg()
// {
// if (gameInfo.gameStatus == 1)
// {

//   draw_rect(315, 195, 150, 95, COLOR_GOLD, &vinfo, &finfo, map);
//   draw_text("GAME OVER", 320, 200, 4, COLOR_WHITE, &vinfo, &finfo, map);
//   draw_text("BLACK WIN", 320, 250, 4, COLOR_WHITE, &vinfo, &finfo, map);

// }
//}

//}

void draw_map()
{
  /* clear screen */
  draw_rect(0, 0, vinfo.xres, vinfo.yres, COLOR_BLACK, &vinfo, &finfo, map);

  /* draw rectangle */
  draw_rect(175, 15, 450, 450, COLOR_BROWN, &vinfo, &finfo, map);

  /* draw line */
  int line_spacing = 450 / 10;

  /* draw horizontal lines */
  for (int i = 0; i < 10; i++)
  {
    draw_rect(220, 60 + i * line_spacing, 360, 1, COLOR_BLACK, &vinfo, &finfo,
              map);
  }

  /* draw vertical lines */
  for (int j = 0; j < 10; j++)
  {
    draw_rect(220 + j * line_spacing, 60, 1, 360, COLOR_BLACK, &vinfo, &finfo,
              map);
  }

  draw_circle(310, 150, 5, COLOR_BLACK, &vinfo, &finfo, map);
  draw_circle(490, 150, 5, COLOR_BLACK, &vinfo, &finfo, map);
  draw_circle(400, 240, 5, COLOR_BLACK, &vinfo, &finfo, map);
  draw_circle(310, 330, 5, COLOR_BLACK, &vinfo, &finfo, map);
  draw_circle(490, 330, 5, COLOR_BLACK, &vinfo, &finfo, map);

  draw_button();
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("touchtest: there is no input device\n");
    return EXIT_FAILURE;
  }

  dev_name = argv[1];

  touch_fd = open(dev_name, O_RDONLY);
  if (touch_fd == -1)
  {
    printf("touchtest: %s (%d)\n", strerror(errno), __LINE__);
    return EXIT_FAILURE;
  }
  printf("touchtest: %s opened\n", dev_name);
  sleep(1); // to avoid mixing messages

  // if (argc != 1)
  // {
  //    printf("usage: %s\n", argv[0]);
  //    return EXIT_FAILURE;
  // }
  // printf("[%d] running %s\n", pid = getpid(), argv[0]);

  map_init();
  draw_map();

  for (;;)
  {
    
    //draw_turn();
    ret = read(touch_fd, &ev, sizeof(struct input_event));
    if (ret == -1)
    {
      printf("touchtest: %s (%d)\n", strerror(errno), __LINE__);
      return EXIT_FAILURE;
    }

    // 터치 시작 시 (x, y 좌표 출력)
    if (ev.type == 1 && ev.code == 330 && ev.value == 0)
    {
      // 터치 좌표 출력 (현재 좌표를 바로 출력)
      printf(
          "Touch started at coordinates: x=%d, y=%d -> Scaled x: %d, Scaled y: "
          "%d\n",
          x_coordinate, y_coordinate, x_scaled, y_scaled);
      x_selected = (x_scaled - 220) / 45;
      y_selected = (y_scaled - 60) / 45;
      x_selected_coordinate = 220 + 45 * x_selected;
      y_selected_coordinate = 60 + 45 * y_selected;

      // 바둑알 둘 곳 터치로 선택하기
      if (x_selected >= 0 && x_selected <= 8 && y_selected >= 0 &&
          y_selected <= 8)
      {
        x_selected_coordinate_temp = x_selected_coordinate;
        y_selected_coordinate_temp = y_selected_coordinate;
        x_selected_temp = x_selected;
        y_selected_temp = y_selected;
        draw_target();
        printf("%d, %d\n", x_selected, y_selected);
      }

      // 버튼 누르면 바둑알 두기
      if (x_scaled >= 700 && y_scaled >= 380)
      {
        add_stone();
        selected[x_selected_temp][y_selected_temp] = 1;
      }
    }

    else if (ev.type == 1 && ev.code == 330 && ev.value == 1)
    {
      x_before_coordinate = x_selected_coordinate;
      y_before_coordinate = y_selected_coordinate;
    }

    else if (ev.type == 3 && ev.code == 0)
    {
      // x 위치 값
      x_coordinate = ev.value; // x 좌표 저장
      // x 좌표 축소 (비율 변환)
      x_scaled = (x_coordinate - original_min) * target_width /
                 (original_max - original_min);
    }
    else if (ev.type == 3 && ev.code == 1)
    {
      // y 위치 값
      y_coordinate = ev.value; // y 좌표 저장
      // y 좌표 축소 (비율 변환)
      y_scaled = (y_coordinate - original_min) * target_height /
                 (original_max - original_min);
    }
  }

  /* close */
  munmap(map, size);
  close(fb_fd);
  close(touch_fd);

  return 0;
}
