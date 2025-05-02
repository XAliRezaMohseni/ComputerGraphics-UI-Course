#include "./DisplayManager.h"
#include <GL/glut.h>


/**
 * @brief This is the main display function of the glut.
 * TODO: Write the doc.
 * */
void
display()
{
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * @brief This function initializes everything for the game.
 */
void init()
{
  initMenu();
}

/**
 * @brief This function initializes the menu and sub-menus
 * for the windows.
 * */
void
initMenu()
{
  const auto master_menu = glutCreateMenu(mainMenuHandler);
}
