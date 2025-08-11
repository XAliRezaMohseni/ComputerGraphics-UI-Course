#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <GL/glut.h>
#include <stdbool.h>
#include <stdint.h>


#define MAKE


typedef struct
{
  GLfloat x;
  GLfloat y;
} Point;

typedef struct
{
  GLfloat x;
  GLfloat y;
} Vector;

typedef enum
{
  Easy,
  Medium,
  Hard,
} Difficulty;

typedef struct
{
  Point center;
  Point body[100];
  Vector movement_direction;
  GLfloat radius;
  GLfloat speed;
  GLfloat colour[3];
} Ball;

typedef struct
{
  Point bottom_left;
  Point bottom_right;
  Point top_left;
  Point top_right;
} RacketBody;

typedef struct
{
  RacketBody body;
  GLfloat movement_speed;
  GLfloat colour[3];
} Racket;

typedef struct
{
  Difficulty difficulty;
  GLuint initial_ball_speed;
  GLuint ball_speed_increment_rate;
  GLuint ball_speed_increment_limit;
  GLuint game_over_limit;
} GameContext;

void display(void);
void init(void);
void reset(bool hard_reset);
void reshapeHandler(const int, const int);
void arrowKeyHandler(const int, const int, const int);
void mouseHandler(const int, const int, const int, const int);
void mainMenuHandler(const int);
void subMenuHandler(const int);
void initMenu(void);
void drawRacket(void);
void drawBall(void);
void makeBallBody(void);
void moveBall(void);
void startBallMovement(Vector *);
void update(const int);
void moveRacket(const bool);
void makeRacketBody(RacketBody *, const GLfloat, const GLfloat, const GLfloat);
void convertLocation(const int *raw_x, const int *raw_y, double *x, double *y);
void traceBallMove(void);
void redirectBall(void);
void writeScreenText(const char*);
void changeGameMode(Difficulty);
void lostCheck();
void lostUpdateFunc(const int);
float sizeOfVector(const Vector *);

#endif
