/* FILE NAME  : bits.h
 * PROGRAMMER : ND4
 * DATE       : 07.09.2021
 * PURPOSE    : Bits class declaration module.
*/

#ifndef __bits_h_
#define __bits_h_

#include "commondf.h"
#include <iostream>
#include <fstream>
#include <string.h>

namespace bit
{
  /* Useful bits write class */
  class bitsWr
  {
  private:
    INT BitAccum, BitPos;
    std::fstream BitF;
  public:

    /* Bits construction function.
     * ARGUMENTS: None.
     * RETURNS: None.
    */
    bitsWr( VOID )
    {
      BitAccum = 0;
      BitPos = 7;
    }
    /* End of 'bitsWr' function */

    /* Bits destruction function.
     * ARGUMENTS: None.
     * RETURNS: None.
    */
    ~bitsWr( VOID )
    {
      if (BitF.is_open())
        BitF.close();

      BitAccum = 0;
      BitPos = 0;
    }
    /* End of '~bitsWr' function */

    /* Bit writing initialization.
     * ARGUMENTS: 
     *   - File name to open:
     *       std::string Name;
     * RETURNS: None.
    */
    VOID WriteBitInit( std::string Name )
    {
      BitF.open(Name);

      if(!BitF.is_open())
      {
       BitF.clear();
       BitF.open(Name, std::ios::out);
       BitF.close();
       BitF.open(Name);
      }

      BitAccum = 0;
      BitPos = 7;
    }
    /* End of 'WriteBitInit' function */

    /* Bit writing.
     * ARGUMENTS: 
     *   - Bit num:
     *       INT Bit;
     * RETURNS: None.
    */
    VOID WriteBit( INT Bit )
    {
      BitAccum |= Bit << BitPos--;

      if (BitPos < 0)
      {
        BitF.put(BitAccum);
        BitAccum = 0;
        BitPos = 7;
      }
    }
    /* End of 'WriteBit' function */

    /* Bit writing deinitialization.
     * ARGUMENTS: None.
     * RETURNS: None.
    */
    VOID WriteBitClose( VOID )
    {
      if (BitPos < 7)
        BitF.put(BitAccum);

      BitF.close();
    }
    /* End of 'WriteBitClose' function */

    VOID WriteDword( DWORD NewD )
    {
      for (INT i = 32; i > 0; i--)
        WriteBit((NewD >> i) & 1);
    }

  };

  /* Useful bits read class */
  class bitsRe
  {
  private:
    INT BitAccum, BitPos;
    std::fstream BitF;
  public:

    /* Bits construction function.
     * ARGUMENTS: None.
     * RETURNS: None.
    */
    bitsRe( VOID )
    {
      BitAccum = 0;
      BitPos = 7;
    }
    /* End of 'bitsRe' function */

    /* Bits destruction function.
     * ARGUMENTS: None.
     * RETURNS: None.
    */
    ~bitsRe( VOID )
    {
      if (BitF.is_open())
        BitF.close();

      BitAccum = 0;
      BitPos = 0;
    }
    /* End of '~bitsRe' function */

    VOID ReadBitInit( std::string RName )
    {
      if (BitF.is_open())
        BitF.close();

      BitF.open(RName);
      BitPos = -1;
    }
    /* End of 'ReadBitInit' function */

    INT ReadBit( VOID )
    {
      if (BitPos < 0)
        BitAccum = BitF.get(), BitPos = 7;


      //if (BitAccum == -1)
      //  return 0;

      return (BitAccum >> BitPos--) & 1;
    }
    /* End of 'ReadBit' function */
  };

};

#endif /* __bits_h_ */

/* END OF 'bits.h' FILE */
