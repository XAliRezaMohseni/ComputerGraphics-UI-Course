/*
Author: AliReza Mohseni (4004013069): al[dot]mohseni[at]yahoo[dot]com
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>


#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   600

typedef struct
{
  double x;
  double y;
} Point;

typedef struct
{
  Point a;
  Point b;
  Point c;
} Triangle;

typedef unsigned char byte;

// Global variables
byte points_state = 0;
Point point_buffer[3];

// Function signatures
void display(void);
void mouseInput(int, int, int, int);
void addRawPoint(int, int);
void drawTriangle(const Triangle *);
void convertLocation(const int* raw_x, const int* raw_y, double* x, double* y);
void getRandomColor(GLdouble* red, GLdouble* green, GLdouble* blue);

Triangle flushPointsBuffer();

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Exercise One - AliReza Mohseni (4004013069)");
  glutDisplayFunc(display);
  glutMouseFunc(mouseInput);

  glutMainLoop();

  return EXIT_SUCCESS;
}

/**
 * @brief The glut display function callback.
 * */
void display(void)
{
  glClearColor(0, 0.5, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

/**
 * @brief The glut mouse input function callback.
 * @param button The clicked button.
 * @param The state of the clicked button.
 * @param The x cordinate of clicked position.
 * @param The y cordinate of clicked position.
 * */
void mouseInput(int button, int action, int x, int y)
{

  if ((button == 0) && (action == 0))
  {
    addRawPoint(x, y);
  }
}

/**
 * @brief This function adds the point with the location of mouse to the points buffer
 * when the mouse button is clicked.
 * @param x The x cordinate of the clicked point.
 * @param y The y cordinate of the clicked point.
 */
void addRawPoint(int x, int y)
{

  double a = 0 , b = 0;
  convertLocation(&x, &y, &a, &b);
  printf("The converted location of point is: %f, %f.\n", a, b);

  Point point = {
      .x = a,
      .y = b
  };

  point_buffer[points_state] = point;
  ++points_state;

   if (points_state >= 3)
  {
    Triangle buffer = flushPointsBuffer();
    drawTriangle(&buffer);
    points_state = 0;
  }
}

/**
 * @brief Clears the point buffer and creates a Triangle object
 * Whith them.
 * @returns A Triangle created from current buffer points.
*/
Triangle flushPointsBuffer()
{
  Triangle triangle;
  triangle.a = point_buffer[0];
  triangle.b = point_buffer[1];
  triangle.c = point_buffer[2];
  return triangle;
}

/**
 * @brief Draws a Triangle object on screen.
 * @param triangle The address of triangle which is going
 * to be draw.
 */
void drawTriangle(const Triangle *triangle)
{
  GLdouble red = 0, green = 0, blue = 0;
  getRandomColor(&red, &green, &blue);
  printf("Drawing with color: r = %f, g = %f, b = %f .\n", red, green, blue);

  glColor3d(red, green, blue);
  glBegin(GL_TRIANGLES);
  glVertex2d(triangle->a.x, triangle->a.y);
  glVertex2d(triangle->b.x, triangle->b.y);
  glVertex2d(triangle->c.x, triangle->c.y);
  glEnd();
  glFlush();
  
}

/**
 * @brief Converts the location of mouse passed by glut to the normal one
 * used by OpenGL.
 * @param raw_x The x cordinate returned from glut.
 * @param raw_y The y cordinate returned from glut.
 * @param x The converted x cordinate to normal
 * @param y The converted y cordinate to normal
 * */
void convertLocation(const int* raw_x, const int* raw_y, double* x, double* y)
{
  const uint64_t half_window_width  = glutGet(GLUT_WINDOW_WIDTH)  / 2;
  const uint64_t half_window_height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

  int ret_x = (*raw_x - half_window_width);
  int ret_y = (half_window_height - *raw_y);

  *x = ((double) ret_x) / (double) half_window_width;
  *y = ((double) ret_y) / (double) half_window_height;

}

/** 
* @brief Generates a random color for drawing triangles.
* @param red The value of the red channel of the color.
* @param green The value of the green channel of the color.
* @param blue The value of the blue channel of the color.
*/
void getRandomColor(GLdouble* red, GLdouble* green, GLdouble* blue)
{
  *red   = (GLdouble) (rand() % 255) / 255;
  *green = (GLdouble) (rand() % 255) / 255;
  *blue  = (GLdouble) (rand() % 255) / 255;
}
