/* FILE NAME  : t03mandl.cpp
 * PROGRAMMER : ND4
 * LAST UPDATE: 19.07.2021
 * PURPOSE    : Mandelbrot & Julia set drawing program
 */

#include <iostream>
#include <thread>

#include <glut.h>

#include <commondf.h>

#define FRAME_H 100
#define FRAME_W 100

typedef double DBL;

struct cmpl
{
  DBL Re, Im;

  cmpl( VOID ) : Re(0), Im(0)
  {
  }

  cmpl( const DBL R, const DBL I ) : Re(R), Im(I)
  {
  }

  cmpl( const DBL Val ) : Re(Val), Im(Val)
  {
  }

  cmpl operator + (const cmpl &val)
  {
    return cmpl(Re + val.Re, Im + val.Im);
  }

  cmpl operator += (const cmpl &val)
  {
    return cmpl(Re + val.Re, Im + val.Im);
  }

  DBL Norm2( VOID )
  {
    return Re * Re + Im * Im;
  }

  cmpl operator * (const cmpl &val)
  {
    return cmpl(Re * val.Re - Im * val.Im, Re * val.Im + Im * val.Re);
  }
};

UCHAR Frame[FRAME_H][FRAME_W][4];
INT Zoom = 1;

VOID PutPixel( INT x, INT y, INT r, INT g, INT b )
{
  if (x < 0 || y < 0 || x >= FRAME_W || y >= FRAME_H)
    return;
  Frame[y][x][0] = b;
  Frame[y][x][1] = g;
  Frame[y][x][2] = r;
}

INT Julia( cmpl Z, const cmpl C )
{
  INT n = 1;
  
  while (n++ < 255 && (Z.Norm2() < 4))
    Z = Z * Z + C;

  return n;
}

VOID DrawJulia( VOID )
{
  DBL X0 = -2, X1 = 2, Y0 = -2, Y1 = 2;
  LONG t = clock() / (DBL)CLOCKS_PER_SEC;
  cmpl Z;
  INT n, x, y;

  for (y = 0; y < FRAME_H; y++)
    for (x = 0; x < FRAME_W; x++)
    {
      Z = cmpl(x * (X1 - X0) / FRAME_W + X0, y * (Y1 - Y0) / FRAME_H + Y0);
      n = Julia(Z, cmpl(0.30 + 0.24 * sin(t), 0.40 + 0.12 * sin(0.6 * t)));
      PutPixel(x, y, n * 2, n / 4, n * 10);
    }
}

INT Mandelbrot( cmpl Z )
{
  INT n;
  cmpl Z0;

  Z0 = Z;
  n = 1;
  while (n++ < 255 && Z.Norm2() < 4)
    Z = Z * Z + Z0;

  return n;
}

VOID DrawMandelbrot( VOID )
{
  DBL x0 = -2, x1 = 2, y0 = -2, y1 = 2;
  INT xs, ys, n;
  cmpl z;

  for (ys = 0; ys < FRAME_H; ys++)
    for (xs = 0; xs < FRAME_W; xs++)
    {
      z = cmpl(xs * (x1 - x0) / FRAME_W + x0, ys * (y1 - y0) / FRAME_H + y0);
      n = Mandelbrot(z);
      PutPixel(xs, ys, n * 4, n / 100, n * 8);
    }
}

VOID RandFill( VOID )
{
  INT xs, ys;

  for (ys = 0; ys < FRAME_H; ys++)
    for (xs = 0; xs < FRAME_W; xs++)
    {
      PutPixel(xs, ys, rand(), rand(), rand());
    }
}

VOID Keyboard( UCHAR Key, INT X, INT Y )
{
  if (Key == 27)
    exit(0);
  else if (Key == '-')
    Zoom--;
  else if (Key == '+')
    Zoom++;
} 

VOID Display( VOID )
{
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT);

 
  DrawJulia();
  glRasterPos2d(-1, 1);
  glPixelZoom(Zoom, -Zoom);
  glDrawPixels(FRAME_W, FRAME_H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();

  glutSwapBuffers();
  glutPostRedisplay();
}

INT main( INT ArgC, CHAR *ArgV[] )
{
  glutInit(&ArgC, ArgV);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("ND4");
  glutFullScreen();

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glClearColor(0.3, 0.47, 0.7, 1);
  glutMainLoop();
} /* End of 'main' function */

/* END OF 't03mandl.cpp' FILE */