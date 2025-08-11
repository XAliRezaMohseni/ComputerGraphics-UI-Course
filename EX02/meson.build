project('002', 'c',
        version : '0.1',
        default_options : ['warning_level=3'],
       )

# Used for storing dependencies
dependency_list = []


# Finding the freeglut (glut) library.
glut_dept = dependency('glut', required: false)
if glut_dept.found()
  dependency_list += glut_dept
else
  freeglut_dept = dependency('freeglut', required: false)
  if freeglut_dept.found()
    dependency_list += freeglut_dept
  else
    error('Can not found the freeglut or glut library. Please install them first.')
  endif
endif

dependency_list += dependency('gl', required: true)

cc = meson.get_compiler('c')
dependency_list += cc.find_library('m', required : true)


executable('002',
           [
             'main.c',
             'sources/DisplayManager.c',
             'sources/Displaymanager.h',
           ],
           dependencies: dependency_list,
           c_args: ['-DFREEGLUT_STATIC'],
           link_args:['-static'],
          )
