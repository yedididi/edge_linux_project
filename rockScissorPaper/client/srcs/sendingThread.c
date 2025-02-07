#include "../incs/define.h"

void *sendingThread(void *info_)
{
   t_info *info = (t_info *)info_;

   info->touch_fd = open(info->dev_name, O_RDONLY);
   if (info->touch_fd == -1)
   {
      printf("touchtest: %s (%d)\n", strerror(errno), __LINE__);
      exit(1);
   }
   printf("touchtest: %s opened,  this is fd:%d\n", info->dev_name, info->touch_fd);
   sleep(1); // to avoid mixing messages

   while (1)
   {
      printf("info->turn:%d, info->whichClient:%d\n", info->turn, info->whichClient);
      sleep(1);
      if (info->turn == info->whichClient)
      {
         t_gameInfo clickResult;
         clickResult = getClick(info, info->touch_fd);

         printf("(%d, %d) clicked\n", clickResult.i, clickResult.j);
         t_gameInfo gameInfo;
         gameInfo.i = clickResult.i;
         gameInfo.j = clickResult.j;
         gameInfo.gameStatus = PLAYING;

         write(info->serverFD, &gameInfo, sizeof(t_gameInfo));
      }
      //usleep()
   }
}

t_gameInfo getClick(t_info *info, int touch_fd)
{
    t_gameInfo gameInfo;

    struct input_event ev;
    static int x_coordinate = 0;
    static int y_coordinate = 0;
    static int x_scaled = 0;
    static int y_scaled = 0;
    static int x_selected = 0;
    static int y_selected = 0;

    static int x_selected_coordinate = 0;
    static int y_selected_coordinate = 0;

    int x_before_coordinate = 0;
    int y_before_coordinate = 0;
    int x_selected_temp = 0;
    int y_selected_temp = 0;
   
   for (;;)
   {
      int ret = read(touch_fd, &ev, sizeof(struct input_event));
      if (ret == -1)
      {
         printf("touchtest: %s (%d)\n", strerror(errno), __LINE__);
         exit(1);
      }

      if (ev.type == 1 && ev.code == 330 && ev.value == 0)
      {
         printf("pressed\n");
         x_selected = (x_scaled - 220) / 45;
         y_selected = (y_scaled - 60) / 45;
         x_selected_coordinate = 220 + 45 * x_selected;
         y_selected_coordinate = 60 + 45 * y_selected;

         // 바둑알 둘 곳 터치로 선택하기
         if (x_selected >= 0 && x_selected <= 8 && y_selected >= 0 && y_selected <= 8)
         {
            x_selected_temp = x_selected;
            y_selected_temp = y_selected;
            printf("\ninfo->map->clientMap[y_selected][x_selected]:%d\n", info->map->clientMap[y_selected][x_selected]);
            if (info->map->clientMap[y_selected][x_selected] == 0)
            {
               draw_target(info->map, x_selected_temp, y_selected_temp, x_before_coordinate, y_before_coordinate);
               x_before_coordinate = x_selected_coordinate;
               y_before_coordinate = y_selected_coordinate;
            }
            printf("touched point selected, inside board\n");
         }

         // 버튼 누르면 바둑알 두기
         if (x_scaled >= 700 && y_scaled >= 380)
         {
            printf("button pressed\n");
            info->map->clientMap[y_selected_temp][x_selected_temp] = 1;
            gameInfo.i = x_selected_temp;
            gameInfo.j = y_selected_temp;
            gameInfo.gameStatus = PLAYING;
            return (gameInfo);
         }
      }

      else if (ev.type == 3 && ev.code == 0)
      {
         // x 위치 값
         x_coordinate = ev.value; // x 좌표 저장
         // x 좌표 축소 (비율 변환)
         x_scaled = (x_coordinate - ORIGINAL_MIN) * TARGET_WIDTH / (ORIGINAL_MAX - ORIGINAL_MIN);
      }
      else if (ev.type == 3 && ev.code == 1)
      {
         // y 위치 값
         y_coordinate = ev.value; // y 좌표 저장
         // y 좌표 축소 (비율 변환)
         y_scaled = (y_coordinate - ORIGINAL_MIN) * TARGET_HEIGHT / (ORIGINAL_MAX - ORIGINAL_MIN);
      }
   }
   return (gameInfo);
}