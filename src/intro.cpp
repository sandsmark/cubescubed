// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "intro.h"
#include "mzk.h"

//---------------------------------------------------------------------

#define fzn  0.005f
#define fzf  1000.0f

#define DATA_COUNT 4*6*6*(1+6+6*6+6*6*6+6*6*6*6+6*6*6*6*6)
#define INDEX_COUNT  4*6*(1+6+6*6+6*6*6+6*6*6*6+6*6*6*6*6)
#define CUBE_OFFSET 4*6*6

static const int NESTING = 6;

static float myVerts[DATA_COUNT];
static int myIndices[INDEX_COUNT];
static const int counts[] = { 1, 7, 43, 259, 1555 }; 

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

static const float cubeVerts[] = { 

  1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,
  1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
 -1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
 -1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,

 -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
 -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
  1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
  1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,

 -1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,
 -1.0f, -1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
 -1.0f,  1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
 -1.0f,  1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,

  1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
  1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
  1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,

 -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
 -1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
  1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
  1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,

  1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
  1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
 -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
 -1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f };

//----------------------------------------



 int cube_gen( float x, float y, float z, float size, int offset, int timeToLive ) {
	for ( int i=0; i < 4*6; i++ ) {
	// for each vertex. Scale and transform
		for ( int j=0; j<3; j++ ) {
			float translation;
			switch(j) {
			case 0:
				translation = x;
				break;
			case 1:
				translation = y;
				break;
			case 2:
				translation = z;
				break;
			}
			myVerts[i*6+j+offset*CUBE_OFFSET] = cubeVerts[i*6+j]*size+translation; // scale vertex positions
			myVerts[i*6+j+3+offset*CUBE_OFFSET] = cubeVerts[i*6+j+3];  // normals as usual
			myIndices[i*3+j+offset*24] = i*3+j+offset*24;
		}
	}
	if (timeToLive) {
		// for each side, recurse
		int curr_offset = offset;
		curr_offset = cube_gen(x+size, y, z, size/2.0f, curr_offset+1, timeToLive-1);
		curr_offset = cube_gen(x, y+size, z, size/2.0f, curr_offset+1, timeToLive-1);
		curr_offset = cube_gen(x, y, z+size, size/2.0f, curr_offset+1, timeToLive-1);
		curr_offset = cube_gen(x-size, y, z, size/2.0f, curr_offset+1, timeToLive-1);
		curr_offset = cube_gen(x, y-size, z, size/2.0f, curr_offset+1, timeToLive-1);
		curr_offset = cube_gen(x, y, z-size, size/2.0f, curr_offset+1, timeToLive-1);
		// Recurse
		return curr_offset;
	}
	return offset;
 }
int intro_init( void )
{
	cube_gen(0,0,0,1.0f,0, NESTING-1);
	//for ( int i=0; i < 4*6*6; i++ )
	return 1;
}

static int order = 0;

void intro_do( long itime )
{
    const float ftime = 0.001f*(float)itime;

    // animate
    float pos[3] = { 3.0f*cosf( ftime*1.0f ), 
                     3.0f*cosf( ftime*0.6f ), 
                     3.0f*sinf( ftime*1.0f ) };
    float tar[3] = { 0.0f, 0.0f, 0.0f };

    // render
	glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_NORMALIZE );

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( projectionmatrix );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( pos[0], pos[1], pos[2], tar[0], tar[1], tar[2], 0.0f, 1.0f, 0.0f );

	if ( itime % 10 == 0 ) order++;

    // draw cube
    glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), myVerts+0);
    glNormalPointer(    GL_FLOAT, 6*sizeof(float), cubeVerts+3);
	if (itime/1000 < 10l)
		glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, myIndices );
	else
		glDrawElements( GL_QUADS, 24*((itime/5)%(INDEX_COUNT/24)), GL_UNSIGNED_INT, myIndices );
	glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}