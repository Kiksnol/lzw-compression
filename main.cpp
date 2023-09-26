/* FILE NAME  : main.cpp
 * PROGRAMMER : ND4
 * DATE       : 07.09.2021
 * PURPOSE    : Main program file.
*/

#include "bits.h"
#include "compress.h"
#include "gif.h"

#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

#define FRAME_W 800
#define FRAME_H 600

BYTE Frame[FRAME_H][FRAME_W][3];

static INT Zoom = 1;

VOID PutPixel( INT X, INT Y, INT R, INT G, INT B )
{
  if (0 <= X && X < FRAME_W && 0 <= Y && Y < FRAME_H)
  {
    Frame[Y][X][0] = B;
    Frame[Y][X][1] = G;
    Frame[Y][X][2] = R;
  }
}

VOID Keyboard( BYTE Key, INT X, INT Y )
{
  if (Key == 27)
   exit(0);
  else if (Key == 'f')
    glutFullScreen();
  else if (Key == '-')
    Zoom--;
  else if (Key == '+')
    Zoom++;
}

VOID Display( VOID )
{
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  gif::gif gf;
  
  gf.GifDraw();

  for (INT i = 0; i < 512; i++)
    for (INT j = 0; j < 256; j++)
      PutPixel(i, j, gf.GlobalPal[j][2], gf.GlobalPal[j][1], gf.GlobalPal[j][0]);

  //PutPixel(10, 10, 255, 255, 255);
  glRasterPos2d(-1, 1);
  glPixelZoom(1, -1);

  glDrawPixels(FRAME_W, FRAME_H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();

  glutSwapBuffers();
  glutPostRedisplay();
}

INT main( INT ArgC, CHAR* Argv[] )
{
  glutInit(&ArgC, Argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(1, 1);
  glutInitWindowSize(FRAME_W, FRAME_H);
  glutCreateWindow("T50LZW");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();
  return 0;
}


/* Main program function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) Prg code.
*/
//INT main( VOID )
//{
//  //gif::gif gf;
//  lzw::compress cmp;
//  lzw::decompress dcmp;
//
//  cmp.FileComp("a.gif");
//  dcmp.Decomp("a.gif#COMPRESSED");
//
//  return 2;
//}
/* End of 'main' function */

/* END OF 'main.cpp' FILE */