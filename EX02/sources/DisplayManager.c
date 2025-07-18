#include "./DisplayManager.h"
#include "../lib/lwlog.h"
#include "DisplayManager.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static Racket main_racket;
static Ball main_ball;
static bool first_time_move;
static uint8_t count_of_collisions;
static uint8_t count_of_losts = 0;
static GameContext current_game_mode;
static GameContext game_modes[3];

/**
 * @brief This is the main display function of the glut.
 * TODO: Write the doc.
 */
void
display()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_POINT_SMOOTH);

  glPointSize(10.0);           // Size of the point
  glColor3f(1.0f, 0.0f, 0.0f); // Red color (R=1, G=0, B=0)

  glBegin(GL_POINTS);
  glVertex2f(0.0f, 0.0f); // Draw point at center (0,0)
  glEnd();
  glFlush(); // Force rendering
}

/**
 * @brief This function initializes everything for the game.
 */
void
init()
{
  lwlog_info("Initializing...");

  first_time_move     = true;
  count_of_collisions = 0;

  GameContext easy;
  GameContext medium;
  GameContext hard;

  easy.game_over_limit              = 5;
  easy.initial_ball_speed           = 100.0;
  easy.ball_speed_increment_limit   = 20;
  easy.ball_speed_increment_rate    = 5;
  easy.difficulty                   = Easy;
  game_modes[0]                     = easy;

  medium.game_over_limit            = 3;
  medium.initial_ball_speed         = 90.0;
  medium.ball_speed_increment_limit = 10;
  medium.ball_speed_increment_rate  = 10;
  medium.difficulty                 = Medium;
  game_modes[1]                     = medium;

  hard.game_over_limit              = 2;
  hard.initial_ball_speed           = 80.0;
  hard.ball_speed_increment_limit   = 7;
  hard.ball_speed_increment_rate    = 15;
  hard.difficulty                   = Hard;
  game_modes[2]                     = hard;

  current_game_mode                 = easy;

  RacketBody racket_body;
  makeRacketBody(&racket_body, -0.9, 0.05, 0.5);

  // Defining the racket
  main_racket.body           = racket_body;
  main_racket.colour[0]      = 0.3;
  main_racket.colour[1]      = 0.1;
  main_racket.colour[2]      = 0.5;
  main_racket.movement_speed = 0.15;

  // Defining the ball
  main_ball.colour[0]            = 0.7f;
  main_ball.colour[1]            = 0.4f;
  main_ball.colour[2]            = 0.2f;
  main_ball.center.x             = 0.0f;
  main_ball.center.y             = 0.0f;
  main_ball.radius               = 0.07f;
  main_ball.movement_direction.x = 0.0f;
  main_ball.movement_direction.y = 0.0f;
  main_ball.speed                = current_game_mode.initial_ball_speed;

  makeBallBody();

  // Setting the glut handlers
  glutDisplayFunc(display);
  glutSpecialFunc(arrowKeyHandler);
  glutMouseFunc(mouseHandler);
  initMenu();
}

/**
 * @brief This function initializes the menu and sub-menus
 * for the windows.
 * */
void
initMenu()
{
  int sub_menu = glutCreateMenu(subMenuHandler);
  glutAddMenuEntry("Easy", 0);
  glutAddMenuEntry("Medium", 1);
  glutAddMenuEntry("Hard", 2);

  int master_menu = glutCreateMenu(mainMenuHandler);
  glutAddMenuEntry("Restart", 0);
  glutAddSubMenu("Set level", sub_menu);
  glutAddMenuEntry("Exit", 2);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void
mainMenuHandler(const int handle)
{
  switch(handle)
    {
    case 0:
      init();
      break;
    case 1:
      break;
    case 2:
      exit(0);
      break;
    default:
      break;
    }
}

void
subMenuHandler(const int entry)
{
  switch(entry)
    {
    case 0:
      lwlog_alert("Set to easy");
      changeGameMode(Easy);
      break;
    case 1:
      changeGameMode(Medium);
      break;
    case 2:
      changeGameMode(Hard);
      break;
    default:
      break;
    }
}

void
update(__attribute__((unused)) const int value)
{
  glClear(GL_COLOR_BUFFER_BIT);
  drawRacket();
  drawBall();

  lostCheck();

  moveBall();
  traceBallMove();

  glutSwapBuffers();
  glutTimerFunc(16, update, 0);
}

/**
 * @brief
 * @param body
 */
void
makeRacketBody(RacketBody *body, const GLfloat center, const GLfloat width,
               const GLfloat height)
{
  lwlog_debug("%s", "Making the racket body...");
  const GLfloat x_shift = width / 2;
  const GLfloat y_shift = height / 2;

  assert((center - x_shift) >= -1);
  assert((center - x_shift) <= 1);

  // The top-left point
  body->top_left.x = center - x_shift;
  body->top_left.y = y_shift;

  // The top-right point
  body->top_right.x = center + x_shift;
  body->top_right.y = y_shift;

  // The bottom-right point
  body->bottom_right.x = center + x_shift;
  body->bottom_right.y = -1.0f * y_shift;

  // The bottom-left point
  body->bottom_left.x = center - x_shift;
  body->bottom_left.y = -1.0f * y_shift;

  lwlog_debug("%s", "The racket body made successfully.");
}

/**
 * TODO: WRITE DOC.
 */
void
arrowKeyHandler(const int key, const int x, const int y)
{
  switch(key)
    {
    case GLUT_KEY_UP:
      moveRacket(true);
      lwlog_debug("%s", "Moving the racket up.");
      break;
    case GLUT_KEY_DOWN:
      moveRacket(false);
      lwlog_debug("%s", "Moving the racket down.");
      break;
    default:
      break;
    }
}

/**
 *
 */
void
moveRacket(const bool direction)
{
  lwlog_debug("The racket move function called with args: direction = %d and ",
              direction);

  const GLfloat height
      = main_racket.body.top_right.y - main_racket.body.bottom_right.y;

  // True means up and false means down.
  if(direction)
    {

      // Moving the bottom-left
      main_racket.body.bottom_left.y += main_racket.movement_speed;
      if(main_racket.body.bottom_left.y > (1 - height))
        main_racket.body.bottom_left.y = 1 - height;

      // Moving the bottom-right
      main_racket.body.bottom_right.y += main_racket.movement_speed;
      if(main_racket.body.bottom_right.y > (1 - height))
        main_racket.body.bottom_right.y = 1 - height;

      // Moving the top-right
      main_racket.body.top_right.y += main_racket.movement_speed;
      if(main_racket.body.top_right.y > 1)
        main_racket.body.top_right.y = 1;

      // Moving the top-left
      main_racket.body.top_left.y += main_racket.movement_speed;
      if(main_racket.body.top_left.y > 1)
        main_racket.body.top_left.y = 1;
    }
  else
    {

      // Moving the bottom-left
      main_racket.body.bottom_left.y -= main_racket.movement_speed;
      if(main_racket.body.bottom_left.y < -1)
        main_racket.body.bottom_left.y = -1;

      // Moving the bottom-right
      main_racket.body.bottom_right.y -= main_racket.movement_speed;
      if(main_racket.body.bottom_right.y < -1)
        main_racket.body.bottom_right.y = -1;

      // Moving the top-right
      main_racket.body.top_right.y -= main_racket.movement_speed;
      if(main_racket.body.top_right.y < (-1.0f + height))
        main_racket.body.top_right.y = -1.0f + height;

      // Moving the top-left
      main_racket.body.top_left.y -= main_racket.movement_speed;
      if(main_racket.body.top_left.y < (-1.0f + height))
        main_racket.body.top_left.y = -1.0f + height;
    }
}

/**
 *
 */
void
drawRacket()
{
  glColor3f(main_racket.colour[0], main_racket.colour[1],
            main_racket.colour[2]);
  glBegin(GL_POLYGON);
  glVertex2f(main_racket.body.bottom_left.x, main_racket.body.bottom_left.y);
  glVertex2f(main_racket.body.bottom_right.x, main_racket.body.bottom_right.y);
  glVertex2f(main_racket.body.top_right.x, main_racket.body.top_right.y);
  glVertex2f(main_racket.body.top_left.x, main_racket.body.top_left.y);
  glEnd();
  glFlush();
}

/**
 * TODO: Write the doc.
 */
void
makeBallBody()
{
  const uint8_t number_of_points = 100;
  const float angle              = 360.0f / number_of_points;
  const float radians            = angle * (M_PI / 180.0);

  for(uint16_t i = 0; i < number_of_points; i++)
    {
      float currentAngle  = angle * i;
      float x             = main_ball.radius * cos(currentAngle);
      float y             = main_ball.radius * sin(currentAngle);
      main_ball.body[i].x = x;
      main_ball.body[i].y = y;
    }
}

/**
 * TODO: Write the doc.
 */
void
moveBall()
{
  if(first_time_move)
    return;

  if(count_of_collisions >= current_game_mode.ball_speed_increment_limit)
    {
      count_of_collisions = 0;
      main_ball.speed -= current_game_mode.ball_speed_increment_rate;
      lwlog_info("Increasing ball speed by: %d",
                 current_game_mode.ball_speed_increment_rate);
      lwlog_info("The speed is now %f", main_ball.speed);
    }

  for(uint8_t i = 0; i < 100; i++)
    {
      main_ball.body[i].x += main_ball.movement_direction.x;
      main_ball.body[i].y += main_ball.movement_direction.y;
    }

  main_ball.center.x += main_ball.movement_direction.x;
  main_ball.center.y += main_ball.movement_direction.y;

  // Wall collision detection.
}

/**
 *
 */
void
drawBall()
{
  glColor3fv(main_ball.colour);
  glBegin(GL_POLYGON);

  for(uint8_t i = 0; i < 100; i++)
    {
      glVertex2f(main_ball.body[i].x, main_ball.body[i].y);
    }

  glEnd();
  glFlush();
}

/**
 *
 */
float
sizeOfVector(const Vector *vector)
{
  return sqrt(pow(vector->x, 2) + pow(vector->y, 2));
}

void
mouseHandler(const int button, const int state, const int x, const int y)
{
  if(!first_time_move)
    return;
  if(GLUT_LEFT_BUTTON == button && GLUT_DOWN == state)
    {
      double converted_x, converted_y;
      convertLocation(&x, &y, &converted_x, &converted_y);
      Vector start_vector = { .x = converted_x, .y = converted_y };
      startBallMovement(&start_vector);
      first_time_move = false;
    }
}

/**
 * @brief Converts the location of mouse passed by glut to the normal one
 * used by OpenGL.
 * @param raw_x The x cordinate returned from glut.
 * @param raw_y The y cordinate returned from glut.
 * @param x The converted x cordinate to normal
 * @param y The converted y cordinate to normal
 * */
void
convertLocation(const int *raw_x, const int *raw_y, double *x, double *y)
{
  // Getting the size of the window
  const uint64_t half_window_width  = glutGet(GLUT_WINDOW_WIDTH) / 2;
  const uint64_t half_window_height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

  // Converted cordinates
  int ret_x = (*raw_x - half_window_width);
  int ret_y = (half_window_height - *raw_y);

  // Returning them
  *x = ((double)ret_x) / (double)half_window_width;
  *y = ((double)ret_y) / (double)half_window_height;
}

/** TODO: WRIETE THE DOC.
 * @brief
 */
void
startBallMovement(Vector *start_vector)
{
  if(!first_time_move)
    return;

  const float size_of_start_vector = sizeOfVector(start_vector);
  const float x_unit_vector        = start_vector->x / size_of_start_vector;
  const float y_unit_vector        = start_vector->y / size_of_start_vector;
  main_ball.movement_direction.x   = x_unit_vector / main_ball.speed;
  main_ball.movement_direction.y   = y_unit_vector / main_ball.speed;
  first_time_move                  = false;
}

/**
 * TODO: Write the doc.
 * */
void
traceBallMove()
{
  const GLfloat ball_border_top    = main_ball.center.y + main_ball.radius;
  const GLfloat ball_border_bottom = main_ball.center.y - main_ball.radius;
  const GLfloat ball_border_right  = main_ball.center.x + main_ball.radius;
  const GLfloat ball_border_left   = main_ball.center.x - main_ball.radius;

  // For wall collision detection.
  bool should_be_mirror = (ball_border_top >= 1) | (ball_border_right >= 1)
                          | (ball_border_bottom <= -1);

  // For racket collision detection.
  const float bottom_limit
      = main_racket.body.bottom_right.y - ball_border_bottom;
  const float top_limit = main_racket.body.top_right.y - ball_border_top;
  should_be_mirror |= (ball_border_left <= main_racket.body.bottom_right.x)
                      && (bottom_limit <= 0) && (top_limit >= 0);

  bool is_lost = -1 >= ball_border_left;

  if(is_lost)
    {
      init();
      count_of_losts++;
    }

  if(false == should_be_mirror)
    return;

  count_of_collisions++;

  if(1 <= ball_border_right)
    {
      main_ball.movement_direction.x = -1.0 * main_ball.movement_direction.x;
    }
  else if(1 <= ball_border_top)
    {
      main_ball.movement_direction.y = -1.0 * main_ball.movement_direction.y;
    }
  else if(-1 >= ball_border_bottom)
    {
      main_ball.movement_direction.y = -1.0 * main_ball.movement_direction.y;
    }
  else if(main_racket.body.bottom_right.x >= ball_border_left)
    {
      main_ball.movement_direction.x = -1.0 * main_ball.movement_direction.x;
    }
  else
    {
      main_ball.movement_direction.x = -1.0 * main_ball.movement_direction.x;
    }
}

/**
 * TODO: Write the doc.
 */
void
redirectBall()
{
  const GLfloat ball_border_top    = main_ball.center.y + main_ball.radius;
  const GLfloat ball_border_bottom = main_ball.center.y - main_ball.radius;
  const GLfloat ball_border_right  = main_ball.center.x + main_ball.radius;
  const GLfloat ball_border_left   = main_ball.center.x - main_ball.radius;

  if(1 <= ball_border_right)
    {
      main_ball.movement_direction.x = -1.0 * main_ball.movement_direction.x;
    }
  else if(1 <= ball_border_top)
    {
      main_ball.movement_direction.y = -1.0 * main_ball.movement_direction.y;
    }
  else if(-1 >= ball_border_bottom)
    {
      main_ball.movement_direction.y = -1.0 * main_ball.movement_direction.y;
    }
}

/**
 * @brief
 */
void
writeScreenText(const char *text)
{
  char *c = (char *)malloc(sizeof(char) * strnlen(text, 50));
  strcpy(c, text);
  glColor3f(1.0, 0.0, 0.0);
  glRasterPos2f(-0.9, 0.9);
  for(; *c != '\0'; c++)
    {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

void
changeGameMode(Difficulty difficulty)
{
  switch(difficulty)
    {
    case Easy:
      current_game_mode = game_modes[0];
      break;
    case Medium:
      current_game_mode = game_modes[1];
      break;
    case Hard:
      current_game_mode = game_modes[2];
      break;
    default:
      break;
    }
  main_ball.speed = current_game_mode.initial_ball_speed;
}

void
lostCheck()
{
  if (count_of_losts >= current_game_mode.game_over_limit) 
  {
    init();
    glutTimerFunc(16, lostUpdateFunc, 0);
  }
}

void lostUpdateFunc(__attribute__((unused)) const int rate)
{
  writeScreenText("You Lost!");
  glutTimerFunc(16, lostUpdateFunc, 0);
}