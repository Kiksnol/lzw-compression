/* FILE NAME  : gif.h
 * PROGRAMMER : ND4
 * DATE       : 07.09.2021
 * PURPOSE    : GIF declaration module.
*/

#ifndef __gif_h_
#define __gif_h_

#include <commondf.h>

#pragma pack(push, 1)

/* GIF image useful namespace */
namespace gif
{
  struct header
  {
    CHAR Sign[6];
    WORD W, H;
    BYTE Info;
    BYTE Bgcolor;
    BYTE Reserved;
  };

  struct image
  {
    BYTE DivSym;
    WORD L, T;
    WORD W, H;
    BYTE Info;
  };

  /* GIF image useful class */
  class gif
  {
  private:
    header fh;
    image img;
    std::fstream f;

    /* GIF Header info */
    UINT BitsPerPixel, 
      ColorBits, 
      IsGlobalColorTable;

    /* Image info */
    UINT IsImageGlobalColorTable, 
      IsForm, 
      ImgBitsPerPixel;
    CHAR InfoBuf[8];
    INT CODE_SIZE, 
      CODE_CLEAR, 
      CODE_END, 
      CODE_FIRST;

  public:
    BYTE Pal[256][3],
      ImagePal[256][3],
      GlobalPal[256][3];

    gif( VOID ) : f("a.gif", std::fstream::in | std::fstream::binary)
    {
      for (INT i = 0; i < 256; i++)
        for (INT c = 0; c < 3; c++)
          GlobalPal[i][c] = fh.Bgcolor;

      f.read((CHAR *)&fh, sizeof(fh));
      BitsPerPixel = (fh.Info & 7) + 1;
      ColorBits = ((fh.Info >> 4) & 7) + 1;
      IsGlobalColorTable = fh.Info >> 7;

      if (IsGlobalColorTable)
        f.read((CHAR *)Pal, 3 * (1 << BitsPerPixel));

      f.read((CHAR *)&img, sizeof(img));

      ImgBitsPerPixel = (img.Info & 7) + 1;
      IsForm = ((img.Info >> 6) & 7);
      IsImageGlobalColorTable = img.Info >> 7;

      if (IsImageGlobalColorTable)
        f.read((CHAR *)ImagePal, 3 * (1 << ImgBitsPerPixel));

      if (img.DivSym == ',')
      {
        CHAR Buf[20];
        INT W0, H0, W, H;

        f.read((CHAR *)&CODE_SIZE, 1);
        CODE_CLEAR = 1 << CODE_SIZE;
        CODE_END = 1 + CODE_CLEAR;
        CODE_FIRST = 2 + CODE_CLEAR;

        if (!IsImageGlobalColorTable)
        {
        }
      }

    }
    ~gif( VOID )
    {
      if (f.is_open())
        f.close();
    }

    VOID OutPutGIF( VOID )
    {

    }

    VOID GifDraw( VOID )
    {
      if (IsImageGlobalColorTable)
        for (INT i = 0; i < 256; i++)
          for (INT c = 0; c < 3; c++)
            GlobalPal[i][c] = ImagePal[i][c];
      else if (IsGlobalColorTable)
        for (INT i = 0; i < 256; i++)
          for (INT c = 0; c < 3; c++)
            GlobalPal[i][c] = Pal[i][c];
    }
  };

}

#pragma pack(pop)

 
#endif /* __gif_h_ */
