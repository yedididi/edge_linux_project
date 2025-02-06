#include "../incs/define.h"

t_map *map_init()
{
   /* open */
   t_map *map = (t_map *)malloc(sizeof(t_map));
   if (map == NULL)
      printf("malloc error\n");
   
   map->fb_fd = open(FBDEV, O_RDWR);
   if (map->fb_fd == -1)
   {
      printf("[%d] error: %s (%d)\n", getpid(), strerror(errno), __LINE__);
      exit(1);
   }
   printf("[%d] %s was opened successfully\n", getpid(), FBDEV);

   /* get fb_var_screeninfo */
   if (ioctl(map->fb_fd, FBIOGET_VSCREENINFO, &(map->vinfo)) == -1)
   {
      printf("[%d] error: %s (%d)\n", getpid(), strerror(errno), __LINE__);
      exit(1);
   }
   printf("[%d] %dx%d %dbpp\n", getpid(), (map->vinfo).xres, (map->vinfo).yres, (map->vinfo).bits_per_pixel);

   /* get fb_fix_screeninfo */
   if (ioctl(map->fb_fd, FBIOGET_FSCREENINFO, &(map->finfo)) == -1)
   {
      printf("[%d] error: %s (%d)\n", getpid(), strerror(errno), __LINE__);
      exit(1);
   }
   printf("[%d] line_length is %d\n", getpid(), (map->finfo).line_length);

   /* mmap */
   map->size = (map->vinfo).xres * (map->vinfo).yres * (map->vinfo).bits_per_pixel / 8;
   map->mapNum = (char *)mmap(0, map->size, PROT_READ | PROT_WRITE, MAP_SHARED, map->fb_fd, 0);
   if (map->mapNum == (char *)-1)
   {
      printf("[%d] error: %s (%d)\n", getpid(), strerror(errno), __LINE__);
      exit(1);
   }
   printf("[%d] %s was mapped to %p\n", getpid(), FBDEV, map->mapNum);
   
   return (map);
}