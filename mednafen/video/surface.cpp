/* Mednafen - Multi-system Emulator
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "../mednafen.h"
#include "surface.h"

MDFN_PixelFormat::MDFN_PixelFormat()
{
   bpp = 0;
   colorspace = 0;

   Rshift = 0;
   Gshift = 0;
   Bshift = 0;
   Ashift = 0;
}

MDFN_PixelFormat::MDFN_PixelFormat(const unsigned int p_colorspace, const uint8 p_rs, const uint8 p_gs, const uint8 p_bs, const uint8 p_as)
{
#if defined(WANT_16BPP)
   bpp = 16;
#else
   bpp = 32;
#endif
   colorspace = p_colorspace;

   Rshift = p_rs;
   Gshift = p_gs;
   Bshift = p_bs;
   Ashift = p_as;
}

MDFN_Surface::MDFN_Surface()
{
   format     = MDFN_PixelFormat();

   pixels     = NULL;
   pixels16   = NULL;
   pitchinpix = 0;
   w          = 0;
   h          = 0;
}

MDFN_Surface::MDFN_Surface(void *const p_pixels, const uint32 p_width, const uint32 p_height, const uint32 p_pitchinpix, const MDFN_PixelFormat &nf)
{
   Init(p_pixels, p_width, p_height, p_pitchinpix, nf);
}

void MDFN_Surface::Init(void *const p_pixels, const uint32 p_width, const uint32 p_height, const uint32 p_pitchinpix, const MDFN_PixelFormat &nf)
{
   void *rpix = p_pixels;
   assert(nf.bpp == 16 || nf.bpp == 32 || nf.bpp == 8);

   format = nf;

   pixels16 = NULL;
   pixels = NULL;
#if defined(WANT_8BPP)
   palette = NULL;
#endif

   if (!rpix)
   {
      if(!(rpix = calloc(1, p_pitchinpix * p_height * (nf.bpp / 8))))
         throw(1);
   }

   // Dynamically bind pointers based on runtime bpp to ensure video output is never dropped
   if (nf.bpp == 16)
   {
      pixels16 = (uint16 *)rpix;
   }
   else if (nf.bpp == 32)
   {
      pixels = (uint32 *)rpix;
   }
#if defined(WANT_8BPP)
   else if (nf.bpp == 8)
   {
      pixels8 = (uint8 *)rpix;
      palette = (MDFN_PaletteEntry*)calloc(sizeof(MDFN_PaletteEntry), 256);
   }
#endif

   w = p_width;
   h = p_height;
   pitchinpix = p_pitchinpix;
}

void MDFN_Surface::SetFormat(const MDFN_PixelFormat &nf, bool convert)
{
   format = nf;
}

MDFN_Surface::~MDFN_Surface()
{
   void *target_ptr = NULL;

   if (pixels16)
      target_ptr = pixels16;
   else if (pixels)
      target_ptr = pixels;
#if defined(WANT_8BPP)
   else if (pixels8)
      target_ptr = pixels8;
   
   if(palette)
   {
      free(palette);
      palette = NULL;
   }
#endif

   if(target_ptr)
   {
      free(target_ptr);
   }

   pixels = NULL;
   pixels16 = NULL;
}