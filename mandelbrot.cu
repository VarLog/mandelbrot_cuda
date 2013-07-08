/*
 * Mandelbrot set on CUDA.
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

#include <math.h>

#include "utils.h"
#include "cuComplex.h"
#include "mandelbrot.h"

//#define DIMX (64*4*4)
//#define DIMY (64*4*3)

#define ITER_COUNT 31

__device__ unsigned char mandelbrot( int x, int y, float angle ) {
    // project the screen coordinate into the complex plane
    const float cx = -2.0f + 3.0f * x / gridDim.x;
    const float cy = -1.125f + 2.25f * y / gridDim.y;
    
    // rotate
    const float angleCos = cosf( angle );
    const float angleSin = sinf( angle );

    const float rotationCenterX = -0.5f;
    const float rcx = rotationCenterX + angleCos * (cx - rotationCenterX) - angleSin * cy;
    const float rcy = angleCos * cy + angleSin * (cx - rotationCenterX);

    cuComplex z(0.0, 0.0);
    cuComplex c(rcx, rcy);

    // Mandelbrot set
    // z_0 = 0
    // z_n = z_(n-1)^2 + c
    // |z_n| <= 4
    unsigned char i = 0;
    for (i=0; i<ITER_COUNT; i++) {
        z = z * z + c;
        if (z.magnitude2() > 4)
            return i+1;
    }

    return 0;
}

__global__ void kernel( unsigned char *ptr, float angle ) {
    // map from blockIdx to pixel position
    int x = blockIdx.x;
    int y = blockIdx.y;
    int offset = x + y * gridDim.x;

    // now calculate the value at that position
    unsigned char mandelbrotValue = mandelbrot( x, y, angle );
    ptr[offset] = mandelbrotValue;
}

unsigned char* calculate_mandelbrot( int w, int h, float angle ) {
    unsigned char *data_buf = (unsigned char*)calloc( w*h, sizeof(unsigned char) );
    
    unsigned char *dev_data_buf;

    HANDLE_ERROR( cudaMalloc( (void**)&dev_data_buf, w*h*sizeof(unsigned char) ) );

    dim3 grid(w,h);
    kernel<<<grid,1>>>( dev_data_buf, angle );

    HANDLE_ERROR( cudaMemcpy( data_buf, dev_data_buf,
                              w*h*sizeof(unsigned char),
                              cudaMemcpyDeviceToHost ) );
    HANDLE_ERROR( cudaFree( dev_data_buf ) );
  
    return data_buf;
}

