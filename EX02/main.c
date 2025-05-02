/**
 * Author: AliReza Mohseni: al[dot]mohseni[at]yahoo[dot]com
 */

#include "./sources/DisplayManager.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  const int window = glutCreateWindow("Ping Pong Game!"
                                      " "
                                      "AliReza Mohseni"
                                      " "
                                      "4004013069");
  glutDisplayFunc(display);
  glutMainLoop();
  return EXIT_SUCCESS;
}
