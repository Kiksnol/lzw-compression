/* FILE NAME  : compress.h
 * PROGRAMMER : ND4
 * DATE       : 07.09.2021
 * PURPOSE    : Compress and decomress declaration module.
*/

#ifndef __compress_h_
#define __compress_h_

#include "bits.h"

#include <fstream>
#include <vector>

typedef INT BEBRA;

#define MAX 512

/* LZW Compression useful namespace */
namespace lzw
{
  struct vocabulary
  {
    INT Pref;
    BYTE Suff;
  };

  /* Compression class */
  class compress
  {
  private:
    INT Code;
    vocabulary Voc[MAX];
    std::vector<BEBRA> A;
  public:
    compress( VOID )
    {
      ZeroMemory(Voc, MAX);
      Code = 258;
    }

    ~compress( VOID )
    {
      ZeroMemory(Voc, MAX);
      Code = 0;
    }

    VOID Comp( std::string Str )
    {
      CHAR D;
      INT Length = Str.size(), k = 0;
      INT Active = 258;
      BOOL DoFlag = TRUE;
      INT prepos = 0, sufpos = 1;
      vocabulary V;
      V.Pref = Str[0];
      V.Suff = Str[1];

      for (INT i = 0; i < Length; i++)
      {
        if (prepos >= Length)
          break;
        for (INT j = Code; j < Active; j++)
          if (V.Pref == Voc[j].Pref && V.Suff == Voc[j].Suff)
            DoFlag = FALSE;

        if (DoFlag)
        {
          Voc[Active] = V;
          if (Voc[Active].Pref < 257)
          {
            D = Voc[Active].Pref;
            A.push_back(D);
            std::cout << D;
          }
          else
          {
            std::cout << '[' << Voc[Active].Pref << ']';
            A.push_back(Voc[Active].Pref);
          }
            //std::cout << '[' << Voc[Active].Pref << ']';

          Active++;
        }
        else
        {
          for (INT k = Code; k < Active; k++)
          {
            if (V.Pref == Voc[k].Pref && V.Suff == Voc[k].Suff)
            {
              V.Pref = k;
              if (prepos < Length)
                prepos++;
              if (sufpos < Length)
                sufpos++;
              V.Suff = Str[sufpos];
            }
          }
        }
        if (DoFlag)
        {
          if (sufpos >= Length)
            break;
          V.Pref = Str[++prepos];
          V.Suff = Str[++sufpos];
        }
        DoFlag = TRUE;
      }
      
      std::cout << "[257]\n";

      A.push_back(257);

      std::cout << "\n";
      for (INT i = 257; i < MAX; i++)
        std::cout << i << ": " << Voc[i].Pref << " " << Voc[i].Suff << std::endl;

      std::cout << "\n";

    }

    VOID FileComp( std::string FileName )
    {
      std::fstream F;
      //CHAR *ReadL;
      //INT size;
      std::string S1;


      F.open(FileName);
      
      if(!F.is_open())
      {
        std::cout << "FILE \"" << FileName << "\" HAS BEEN NOT FOUND";
        return;
      }

      while (!F.eof())
        S1 += F.get();

      Comp(S1);
      std::string S = "#COMPRESSED";
      SaveCompBit(FileName + S);

      F.close();


      /*std::fstream F;
      CHAR ReadL[300];
      //INT size;

      F.open(FileName);
      
      if(!F.is_open())
      {
        std::cout << "FILE \"" << FileName << "\" HAS BEEN NOT FOUND";
        return;
      }
     
      F.getline(ReadL, 300);

      Comp(ReadL);
      std::string S = "#COMPRESSED";
      SaveCompBit(FileName + S);

      F.close();*/

    }

    VOID SaveCompBit( std::string SaveName )
    {
      bit::bitsWr Bts;

      Bts.WriteBitInit(SaveName);

      for (INT i = 0; i < A.size(); i++)
        for (INT j = 0; j < 9; j++)
        {
          INT bt;

          bt = (A[i] >> j) & 1;

          Bts.WriteBit(bt);
        }

      Bts.WriteBitClose();
    }

  };

  /* Decompress class */
  class decompress
  {
  private:
    INT Code;
    vocabulary Voc[MAX];

    std::vector<BEBRA> A;
  public:
    decompress(VOID)
    {
      ZeroMemory(Voc, MAX);
      Code = 258;
    }

    ~decompress(VOID)
    {
      ZeroMemory(Voc, MAX);
      Code = 0;
    }

    VOID Decomp( std::string FileName )
    {
      std::fstream F;
      std::string S1;
      BOOL DoFlag = TRUE;
      //vocabulary V = {0};
      INT Active = 258;

      F.open(FileName);

      if (!F.is_open())
      {
        std::cout << "FILE \"" << FileName << "\" HAS BEEN NOT FOUND";
        return;
      }

      while (!F.eof())
        S1 += F.get();

      INT LenC = S1.size(), Len = (INT)(LenC / 8.0 * 9) + 1;

      bit::bitsRe Bts;

      Bts.ReadBitInit(FileName);

      for (INT i = 0; i < Len; i++)
      {
        INT bitsum = 0;
        DoFlag = TRUE;

        for (INT j = 0; j < 9; j++)
          bitsum += Bts.ReadBit() * pow(2, j);

        //if (bitsum == 0)
        //  return;

        if (bitsum > 256)
          DoFlag = FALSE;

        if (DoFlag)
        {
          CHAR Cr = bitsum;
          //BOOL prflag = FALSE;
          std::cout << Cr << " ";
          if (Voc[Active].Pref == 0)
            Voc[Active].Pref = bitsum; // , prflag = TRUE;
          else if (Voc[Active].Suff == 0) // && !prflag)
            Voc[Active++].Suff = bitsum, Voc[Active].Pref = Voc[Active - 1].Suff;
        }
        else if (Voc[bitsum].Pref != 0)
        {
          INT n = bitsum, saved = 0;
          std::vector<BEBRA> a;

          //if (Voc[Active].Suff == 0)
          //  Voc[Active].Suff = Voc[bitsum].Suff;

          while (n > 256)
          {
            if (Voc[n].Suff != 0)
              a.push_back(Voc[n].Suff);
            else
              Voc[n].Suff = Voc[n].Pref, Active++;
            saved = n;
            n = Voc[n].Pref;
          }

          //if (Voc[Active].Suff == 0)
          //  Voc[Active++].Suff = Voc[saved].Pref;//, Voc[Active].Pref = Voc[saved].Suff;
          //if (Voc[Active].Suff == 0)
          //  Voc[Active++].Suff = bitsum;//, Voc[Active].Pref = Voc[saved].Suff;
          //if (Voc[Active].Pref == 0)
          //  Voc[Active].Pref = bitsum;
          if (Voc[Active].Suff == 0)
            Voc[Active++].Suff = n, Voc[Active].Pref = bitsum;

          a.push_back(n);

          for (INT i = a.size() - 1; i >= 0; i--)
          {
            CHAR Cr = a[i];
            std::cout << Cr << " ";
          }
        }
      }

      std::cout << "\n\n\n";

      for (INT i = 257; i < MAX; i++)
        std::cout << i << ": " << Voc[i].Pref << " " << Voc[i].Suff << std::endl;

    }
  };
}

#endif __compress_h_

/* END OF 'compress.h' FILE */
