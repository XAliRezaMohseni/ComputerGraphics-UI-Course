/**
 * Author: AliReza Mohseni: al[dot]mohseni[at]yahoo[dot]com
 */

#include "./sources/DisplayManager.h"
#include <GL/glut.h>
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
  init();
  glutTimerFunc(0, update, 0);
  glutMainLoop();
  return EXIT_SUCCESS;
}
