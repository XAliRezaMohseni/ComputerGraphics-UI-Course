#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <stdint.h>

typedef struct
{
  int x;
  int y;
} Point;

enum Difficulty
{
  Easy,
  Medium,
  Hard,
};

typedef struct
{
  Difficulty difficulty;
  uint8_t initial_ball_speed;
  uint8_t ball_speed_increment_rate;
  uint8_t ball_speed_increment_limit;
  uint8_t game_over_limit;
} GameContext;

void display(void);
void reshapeHandler(const int, const int);
void arrowKeyHandler(const int, const int, const int);
void mouseHandler(const int, const int, const int, const int);
void mainMenuHandler(const int);
void subMenuHandler(const int);
void initMenu(void);
void update(void);

#endif
