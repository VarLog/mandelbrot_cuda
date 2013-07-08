/*
 * GLUT window class
 *
 * Copyright (C) Fedorenko Maxim <varlllog@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef __GLUT_WINDOW_H__
#define __GLUT_WINDOW_H__

#include <GL/freeglut.h>   // Header File For The GLUT Library
#include <GL/gl.h>         // Header File For The OpenGL32 Library
#include <GL/glu.h>        // Header File For The GLu32 Library
#include <GL/glx.h>        // Header file fot the glx libraries.

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

struct GLUTWindow {
  /* ASCII code for the escape key. */
  static const int escape_key = 27;

  /* Size of our GLUT window */
  int m_w_width;
  int m_w_height;
  
  /* The number of our GLUT window */
  int m_window;
  
  /* The number of iteration Mandelbrot algoritm */
  int m_iter_count;

  /* base display list for the font set */
  GLuint m_font_base;

  void BuildFont(void);
  void KillFont(void);
  void glPrint(const char *text);
  void InitGL(int Width, int Height);
  
  /* For callbacks */
public:
  void ReSizeGLScene(int Width, int Height);
  void DrawGLScene();
  void keyPressed(unsigned char key, int x, int y);

public:
  GLUTWindow( int argc, char **argv, int w_width = 800, int w_height = 640, int iter_count = 32 );
};

#endif  // __GLUT_WINDOW_H__
