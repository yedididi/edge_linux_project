#include "../incs/define.h"

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

void draw_target(t_map *map, int x_selected_temp, int y_selected_temp, 
    int x_before_coordinate, int y_before_coordinate)
{
   int xNum[12] = {-15, -10, -15, -15, -15, -10, 5, 10, 10, 5, 10, 10};
   int yNum[12] = {-15, -15, -10, 5, 10, 10, 10, 5, 10, -15, -15, -10};
   int x_selected_coordinate = (x_selected_temp * 45) + 220;
   int y_selected_coordinate = (y_selected_temp * 45) + 60;

   printf("inside draw_target, before if\n");
   // if (map->clientMap[y_selected_temp][x_selected_temp] == 0)
   // {
   //    printf("inside if\n");
   //    if (x_before_coordinate == 0 && y_before_coordinate == 0)
   //    {
   //       printf("it is 0,0\n");
   //       // draw_rect(0, 0, 1, 1, COLOR_BLACK, &(map->vinfo), &(map->finfo), map->mapNum);
   //    }

   //    else if (x_selected_coordinate >= 220 && x_selected_coordinate <= 580 && y_selected_coordinate >= 60 && y_selected_coordinate <= 420)
   //    {
   //       printf("before for, about to print target\n");
   //       for (int i = 0; i < 12; i++)
   //       {
   //          printf("printing target:%d\n", i);
   //          draw_rect(x_before_coordinate + xNum[i], y_before_coordinate + yNum[i], 5, 5, COLOR_BROWN, &(map->vinfo), &(map->finfo), map->mapNum);
   //          draw_rect(x_selected_coordinate + xNum[i], y_selected_coordinate + yNum[i], 5, 5, COLOR_RED, &(map->vinfo), &(map->finfo), map->mapNum);
   //       }
   //    }
   // }
   if (x_selected_coordinate >= 220 && x_selected_coordinate <= 580 && y_selected_coordinate >= 60 && y_selected_coordinate <= 420)
   {
      printf("before for, about to print target\n");
      for (int i = 0; i < 12; i++)
      {
         printf("printing target:%d\n", i);
         if (!(x_before_coordinate == 0 && y_before_coordinate == 0))
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