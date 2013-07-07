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

#include "glut_window.hpp"
#include "mandelbrot.h"
#include "timer.hpp"

void GLUTWindow::BuildFont(void) {
    Display *dpy;
    XFontStruct *fontInfo;  // storage for our font.

    m_font_base = glGenLists(96);  // storage for 96 characters.
    
    // load the font.  what fonts any of you have is going
    // to be system dependent, but on my system they are
    // in /usr/X11R6/lib/X11/fonts/*, with fonts.alias and
    // fonts.dir explaining what fonts the .pcf.gz files
    // are.  in any case, one of these 2 fonts should be
    // on your system...or you won't see any text.
    
    // get the current display.  This opens a second
    // connection to the display in the DISPLAY environment
    // value, and will be around only long enough to load 
    // the font. 
    dpy = XOpenDisplay(NULL); // default to DISPLAY env.   

    const char *font_str = "-adobe-helvetica-medium-r-normal--20-*-*-*-p-*-iso8859-1";
    fontInfo = XLoadQueryFont(dpy, font_str);
    if (fontInfo == NULL) {
      fontInfo = XLoadQueryFont(dpy, "fixed");
      if (fontInfo == NULL) {
        printf("no X font available?\n");
      }
    }

    // after loading this font info, this would probably be the time
    // to rotate, scale, or otherwise twink your fonts.  

    // start at character 32 (space), get 96 characters (a few characters past z), and
    // store them starting at base.
    glXUseXFont(fontInfo->fid, 32, 96, m_font_base);

    // free that font's info now that we've got the 
    // display lists.
    XFreeFont(dpy, fontInfo);

    // close down the 2nd display connection.
    XCloseDisplay(dpy);
}

/* delete the font */
void GLUTWindow::KillFont(void) {
    glDeleteLists(m_font_base, 96);  // delete all 96 characters.
}

/* custom gl print routine */
void GLUTWindow::glPrint(const char *text) {
    if (text == NULL) {
      return;
    }
    
    // alert that we're about to offset the display lists with glListBase
    glPushAttrib(GL_LIST_BIT);
    // sets the base character to 32.
    glListBase(m_font_base - 32);
    // draws the display list text. 
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); 
    glPopAttrib();    // undoes the glPushAttrib(GL_LIST_BIT);
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. 
 * We call this right after our OpenGL window is created.     */
void GLUTWindow::InitGL( int w, int h ) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    // This Will Clear The Background Color To Black
  glClearDepth(1.0);                       // Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);                    // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);                 // Enables Depth Testing
  glShadeModel(GL_SMOOTH);                 // Enables Smooth Color Shading
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();             // Reset The Projection Matrix
  
  // Calculate The Aspect Ratio Of The Window 
  gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
  
  glMatrixMode(GL_MODELVIEW);

  BuildFont();  
}

/* The function called when our window is resized */
void GLUTWindow::ReSizeGLScene( int w, int h ) {
  // Prevent A Divide By Zero If The Window Is Too Small
  if( h == 0 ) {
    h = 1;
  }
  // Reset The Current Viewport And Perspective Transformation
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void GLUTWindow::DrawGLScene() {
  static Timer timer;
  
  static float angle = 0.0f;
  unsigned short *dataBuf = ::calculate_mandelbrot( m_w_width, m_w_height, angle );
  angle += 0.001f;

  // Clear The Screen And The Depth Buffer
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, (m_w_width - 1) + 0.5, (m_w_height - 1) + 0.5, -0.5, -1.0, 1.0);

  glPointSize(1.0f);
  glDisable(GL_POINT_SMOOTH);

  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.0f, 0.0f);
  

  // XXX Something wrong in frameBuffer array or in CUDA. Mandelbrot set drawing with artifacts.
  // See: http://storage4.static.itmages.ru/i/13/0707/h_1373222020_7268279_f98f9b9961.png

  unsigned short frameBuffer[ m_w_width * m_w_height ];
  memset( frameBuffer, 0, m_w_width * m_w_height );

  unsigned short *frameBufferPtr = frameBuffer;
  const unsigned short *dataBufPtr = dataBuf;
  for( int j = 0;  j < m_w_height;  j++ ) {
    for( int i = 0;  i < m_w_height;  i++ ) {
      const unsigned char mandelIter = (unsigned char)*dataBufPtr;
      *frameBufferPtr = ( mandelIter << 11 ) | ( mandelIter << 6 ) | mandelIter;
      ++dataBufPtr;
      ++frameBufferPtr;
    }
  }

  glEnd();
  
  glRasterPos2f(0.0f, m_w_height-1.0f);
  glDrawPixels( m_w_width, m_w_height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frameBuffer );
  
  glEnd();
  free(dataBuf);

  // DISABLE DEPTH TEST!
  glDisable(GL_DEPTH_TEST);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Position The Text On The Screen
  glColor3f(0.2f, 0.5f, 0.7f);
  glRasterPos2f(3.0f, 20.0f);

  // print gl text to the screen.
  char str[1024];
  sprintf( str, "Time elapsed: %8.2lf seconds\n", timer.elapsed());
  glPrint(str); 
  
  glRasterPos2f(3.0f, 40.0f);
  static int framesRendered = 0;
  sprintf( str, "Average FPS: %8.2lf\n", ++framesRendered / timer.elapsed());
  glPrint(str);
  
  // swap the buffers to display, since double buffering is used.
  glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void GLUTWindow::keyPressed( unsigned char key, int x, int y ) {
  /* avoid thrashing this call */
  usleep(10);
  
  /* If escape is pressed, kill everything. */
  if( key == escape_key ) { 
    /* shut down our window */
    glutDestroyWindow(m_window); 
    glutLeaveMainLoop();
  }
}

/* For callbacks */
GLUTWindow *g_current_instance;
extern "C" void drawCallback() {
    g_current_instance->DrawGLScene();
}
extern "C" void resizeCallback( int w, int h ) {
    g_current_instance->ReSizeGLScene( w, h );
}
extern "C" void keyCallback( unsigned char key, int x, int y ) {
    g_current_instance->keyPressed( key, x, y );
}

GLUTWindow::GLUTWindow( int argc, char **argv, 
                        int w_width /* = 640 */, int w_height /* = 800 */ )
  : m_w_width(w_width),
    m_w_height(w_height)
{
  g_current_instance = this;
  /* Initialize GLUT state */  
  glutInit(&argc, argv);  
    
  /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffered for automatic clipping */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* get a window */
  glutInitWindowSize( w_width, w_height );  

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  m_window = glutCreateWindow("Mandelbrot set computed on CUDA");  

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(::drawCallback);

  /* Go fullscreen. This is as soon as possible. */
  //  glutFullScreen();

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(::drawCallback);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(::resizeCallback);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(::keyCallback);

  /* Initialize our window. */
  InitGL( w_width, w_height );

  /* This will make sure that GLUT doesn't kill the program 
   * when the window is closed by the OS */
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
      GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  /* Start Event Processing Engine */  
  glutMainLoop();

}


