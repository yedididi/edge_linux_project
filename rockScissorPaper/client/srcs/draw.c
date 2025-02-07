#include "../incs/define.h"

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

void draw_map(t_map *map)
{
   /* clear screen */
   draw_rect(0, 0, (map->vinfo).xres, (map->vinfo).yres, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);

   /* draw rectangle */
   draw_rect(175, 15, 450, 450, COLOR_BROWN, &(map->vinfo), &(map->finfo), map->mapNum);

   /* draw line */
   int line_spacing = 450 / 10;

   /* draw horizontal lines */
   for (int i = 0; i < 10; i++)
   {
      draw_rect(220, 60 + i * line_spacing, 360, 1, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   }

   /* draw vertical lines */
   for (int j = 0; j < 10; j++)
   {
      draw_rect(220 + j * line_spacing, 60, 1, 360, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   }

   draw_circle(310, 150, 5, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   draw_circle(490, 150, 5, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   draw_circle(400, 240, 5, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   draw_circle(310, 330, 5, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   draw_circle(490, 330, 5, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);

   draw_button(map);
}


void draw_rect(int x, int y, int w, int h, unsigned int color, struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip, char *map)
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
            *(unsigned short *)(map + location) = (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
         }
      }
   }
}

void draw_circle(int cx, int cy, int r, unsigned int color, struct fb_var_screeninfo *vip, struct fb_fix_screeninfo *fip, char *map)
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
               *(unsigned short *)(map + location) = (unsigned short)(((r_color >> 3) << 11) | ((g_color >> 2) << 5) | (b_color >> 3));
            }
         }
      }
   }
}

void draw_turn(t_map *map)
{
   if (map->color == COLOR_BLACK)
   {
      draw_circle(750, 430, 30, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   }

   else if (map->color == COLOR_WHITE)
   {
      draw_circle(750, 430, 30, COLOR_WHITE, &(map->vinfo), &(map->finfo), map->mapNum);
   }
}

void draw_button(t_map *map)
{
   draw_rect(630, 280, 170, 200, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   draw_rect(700, 380, 100, 100, COLOR_BLUE, &(map->vinfo), &(map->finfo), map->mapNum);
   draw_turn(map);
}

void draw_target(t_map *map, int x_selected_temp, int y_selected_temp, int x_before_coordinate, int y_before_coordinate)
{
   int xNum[12] = {-15, -10, -15, -15, -15, -10, 5, 10, 10, 5, 10, 10};
   int yNum[12] = {-15, -15, -10, 5, 10, 10, 10, 5, 10, -15, -15, -10};
   int x_selected_coordinate = (x_selected_temp * 45) + 220;
   int y_selected_coordinate = (y_selected_temp * 45) + 60;

   printf("inside draw_target, before if\n");
   if (x_selected_coordinate >= 220 && x_selected_coordinate <= 580 && y_selected_coordinate >= 60 && y_selected_coordinate <= 420)
   {
      printf("this is y_before_coordinate:%d x_before_coordinate:%d\n", y_before_coordinate, x_before_coordinate);
      int newYBeforeCoor = (y_before_coordinate - 60) / 45;
      int newXBeforeCoor = (x_before_coordinate - 220) / 45;
      printf("this is newXBeforeCoor:%d newYBeforeCoor:%d\n", newXBeforeCoor, newYBeforeCoor);
      for (int i = 0; i < 12; i++)
      {
         if ((!(x_before_coordinate == 0 && y_before_coordinate == 0)) && map->clientMap[newYBeforeCoor][newXBeforeCoor] == 0)
            draw_rect(x_before_coordinate + xNum[i], y_before_coordinate + yNum[i], 5, 5, COLOR_BROWN, &(map->vinfo), &(map->finfo), map->mapNum);
         draw_rect(x_selected_coordinate + xNum[i], y_selected_coordinate + yNum[i], 5, 5, COLOR_RED, &(map->vinfo), &(map->finfo), map->mapNum);
      }
   }
   draw_button(map);
}


void add_stone(t_map *map, int x_selected_temp, int y_selected_temp, int x_selected_coordinate_temp, int y_selected_coordinate_temp)
{
   if (map->clientMap[y_selected_temp][x_selected_temp] != 1)
   {
      printf("map->color:%d\n", map->color);
      
      if (map->color == COLOR_BLACK)
      {
         draw_circle(x_selected_coordinate_temp, y_selected_coordinate_temp, 22, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);

         map->color = COLOR_WHITE;
      }

      else if (map->color == COLOR_WHITE)
      {
         draw_circle(x_selected_coordinate_temp, y_selected_coordinate_temp, 22, COLOR_WHITE, &(map->vinfo), &(map->finfo), map->mapNum);
         map->color = COLOR_BLACK;
      }
      printf("%d, %d\n", x_selected_temp, y_selected_temp);
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

void drawWhoIAm(t_info *info)
{
   char buf1[32];

   if (info->whichClient == C1)
      sprintf(buf1, "PLAYER COLOR:BLACK");
   else
      sprintf(buf1, "PLAYER COLOR:WHITE");

  draw_text(buf1, 650, 0, 4, COLOR_WHITE, &(info->map->vinfo), &(info->map->finfo), info->map->mapNum);
}