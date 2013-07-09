/* 
 * main() function - start point of application
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "glut_window.hpp"

void usage( const char *appname ) {
  fprintf( stderr, "Usage:\n\t%s [<iter_number>]\n\tDefault: 64\n", appname );
  exit(EXIT_FAILURE);
}

int main( int argc, char **argv ) {
  if( argc == 1 ) {
    
    // default
    GLUTWindow w(argc, argv, 64);

  } else if( argc > 2 ) {
    usage( argv[0] );
  } else if( argc == 2 && ( !strcmp( argv[1], "-h" ) || !strcmp( argv[1], "--help" ) ) ) {
    usage( argv[0] );
  } else {
    
    int iter_count;
    if( !sscanf( argv[1], "%d", &iter_count ) ) {
      usage( argv[0] );
      exit(EXIT_FAILURE);
    }

    GLUTWindow w(argc, argv, iter_count);
  
  }

  return 0;
}

