#include "jpeglib.h"
#include "stdio.h"
//*************
//* Load JPEG *
//*************
bool LoadJPEG(int Location, LPCTSTR Name,  LPDIRECTDRAWSURFACE7* Surface,LPDIRECTDRAW7 dd,JPEGINFO & jpginfo)
{
   //Error handle
   IJLERR jerr;

   //Create the JPEG object
   JPEG_CORE_PROPERTIES jcprops;
   
   //Initialize the JPEG object
   jerr = ijlInit(&jcprops);
   if (jerr != IJL_OK)
   {
      MessageBox(globalhwnd, "IJLInit problem", "error", MB_OK);
      return false;
   }

   //Load from File or Resource?
   if (Location == FROM_FILE)
   {
      //Set the IJL data source as a filename
      jcprops.JPGFile = Name;
/*char xxx[20];
sprintf(xxx,"%s",Name);
MessageBox(globalhwnd,xxx,"",MB_OK);*/

      //Read JPEG parameters from the file
      jerr = ijlRead(&jcprops, IJL_JFILE_READPARAMS);
   }
   else //(Location == FROM_RESOURCE) or integer ID of resource
   {
      //Resource variables
      BYTE* pmem;
      HGLOBAL hmem;
      DWORD size;
      HRSRC hres;
      
      //Determine whether resource ID is an int or a string
      if (Name == NULL)
         hres = FindResource(NULL, MAKEINTRESOURCE(Location), "JPEG");
      else
         hres = FindResource(NULL, "EXAMPLE", "JPEG");      
      if (hres)
      {
         size = SizeofResource(NULL, hres);
         if (!size)
         {
            MessageBox(globalhwnd, "Error retrieving resource size",
               "error", MB_OK);
            return false;
         }
         hmem = LoadResource(NULL, hres);
         if (hmem == NULL)
         {
            MessageBox(globalhwnd, "Error loading resource", "error", MB_OK);
            return false;
         }
         pmem = (BYTE *)LockResource(hmem);
         if (pmem == NULL)
         {
            MessageBox(globalhwnd, "Error locking resource", "error", MB_OK);
            return false;
         }
      }
      else
      {
         MessageBox(globalhwnd, "JPEG resource not found", "error", MB_OK);
         return false;
      }

      //Set the IJL data source as the resource buffer
      jcprops.JPGFile      = NULL;
      jcprops.JPGBytes     = pmem;
      jcprops.JPGSizeBytes = size;

      //Read JPEG parameters from the buffer
      jerr = ijlRead(&jcprops, IJL_JBUFF_READPARAMS);
   }
//MessageBox(globalhwnd,"ok","",MB_OK);
   //Make sure Parameter read was successful
   if (jerr != IJL_OK)
   {
      MessageBox(globalhwnd, "Error reading JPEG parameters", "error", MB_OK);
      return false;
   }
//MessageBox(globalhwnd,"ok","",MB_OK);
   //Prepare a 24Bit buffer to receive image data
   BYTE *buffer24;
   
   //Determine the required size
   long szbuff24 = (jcprops.JPGWidth * C24BIT + 7) / 8
      * jcprops.JPGHeight;

   //Resize the buffer
   buffer24 = new BYTE [szbuff24];
   if (buffer24 == NULL)
   {
      MessageBox(globalhwnd, "Memory Allocation Error", "error", MB_OK);
      return false;
   }
   
   //Set up the DIB specification for the JPEG decoder
   jcprops.DIBWidth    = jcprops.JPGWidth;
   jcprops.DIBHeight   = jcprops.JPGHeight; //Implies a bottom-up DIB.
   jcprops.DIBChannels = 3;
   jcprops.DIBColor    = IJL_BGR;
   jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.JPGWidth,3);
   jcprops.DIBBytes    = reinterpret_cast<BYTE*>(buffer24);
   jpginfo.width=jcprops.JPGWidth;
   jpginfo.height=jcprops.JPGHeight;
//MessageBox(globalhwnd,"ok","",MB_OK);
   //Set the JPG color space ... this will always be somewhat of an
   //educated guess at best because JPEG is "color blind" (i.e.,
   //nothing in the bit stream tells you what color space the data was
   //encoded from. However, in this example we assume that we are
   //reading JFIF files which means that 3 channel images are in the
   //YCbCr color space and 1 channel images are in the Y color space.
   switch(jcprops.JPGChannels)
   {
      case 1: jcprops.JPGColor = IJL_G;
         break;
      
      case 3: jcprops.JPGColor = IJL_YCBCR;
         break;
      
      default:
         //This catches everything else, but no color twist will be
         //performed by the IJL.
         jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
         jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
         break;
   }
//MessageBox(globalhwnd,"ok","",MB_OK);
   //Read JPEG image data into our 24bit buffer
   if (Location == FROM_FILE)
      jerr = ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE);
   else
      jerr = ijlRead(&jcprops, IJL_JBUFF_READWHOLEIMAGE);

   //Make sure the read was successful
   if (jerr != IJL_OK)
   {
      MessageBox(globalhwnd, "Error reading JPEG image", "error", MB_OK);
      return false;
   }
//MessageBox(globalhwnd,"ok","",MB_OK);
   HBITMAP hbm;

   //Convert to current CLRMODE
   if (CLRMODE == C16BIT)
   {
	   //MessageBox(globalhwnd,"ok","",MB_OK);
      //Create a 16bit buffer
      WORD *buffer16;
      long szbuff16;
      
      //determine the size of our buffer
      szbuff16 = ((jcprops.JPGWidth * C16BIT + 7) / 8)
         * jcprops.JPGHeight;
//MessageBox(globalhwnd,"ok","",MB_OK);  
      //resize the buffer and make sure resize works
      buffer16 = new WORD [szbuff16];
	      
      if (buffer16 == NULL)
      {
         MessageBox(globalhwnd, "Error creating 16Bit buffer", "error", MB_OK);
         return false;
      }

      //Start at the beginning of the buffer
      long j = 0;

      //Step through the 24bit buffer
      //Retrieve 3 channels at a time and convert their values to 16bit
      for (long i = 0; i < szbuff24; i += 3)   
      {
         //Check the pixel format and write the color data
         //to the 16bit buffer. After the write, advance the
         //16bit buffer by one.
         if (PixelFormat == PF_555)
            buffer16[j++] = RGB555(buffer24[i], buffer24[i + 1],
               buffer24[i + 2]);
         else
            buffer16[j++] = RGB565(buffer24[i], buffer24[i + 1],
               buffer24[i + 2]);
      }

      //Create the bitmap using the new 16bit buffer   
      hbm = CreateBitmap (jcprops.JPGWidth, jcprops.JPGHeight, 1,
         C16BIT, buffer16);
      if(hbm == NULL)
      {
         MessageBox(globalhwnd, "Failed to create 16Bit Bitmap", "error", MB_OK);
         return false;
      }

      //remove the new buffer
      delete buffer16;
	  
   }
   else if (CLRMODE == C24BIT)
   {
      //The data we have from the JPEG is already 24bit
      //Just create the new bitmap from our buffer
      hbm = CreateBitmap (jcprops.JPGWidth, jcprops.JPGHeight, 1,
         C24BIT, buffer24);
      if(hbm == NULL)
      {
		  MessageBox(globalhwnd, "Failed to create 24Bit Bitmap", "error", MB_OK);
         return false;
      }
   }
   else if (CLRMODE == C32BIT)
   {

      //Create a 32bit buffer
      BYTE *buffer32;
      long szbuff32;
      
      //determine the size of our buffer
      szbuff32 = ((jcprops.JPGWidth * C32BIT + 7) / 8)
         * jcprops.JPGHeight;

      //resize the buffer and make sure resize works
      buffer32 = new BYTE [szbuff32];
      if (buffer32 == NULL)
      {
         MessageBox(globalhwnd, "Error creating 32Bit buffer", "error", MB_OK);
         return false;
      }
      
      //Start at the beginning of the 24bit buffer
      long j = 0;

      //Step through the 32bit buffer, copying the 3 Channels from
      //the 24bit buffer. However, at the end of each channel, we
      //have to write an extra byte for the 32bit image's alpha
      //channel
      for (long i = 0; i < szbuff32; i += 4)   
      {
         buffer32[i + 0] = buffer24[j++]; //Blue channel
         buffer32[i + 1] = buffer24[j++]; //Green channel
         buffer32[i + 2] = buffer24[j++]; //Red channel
         buffer32[i + 3] = (BYTE)0x00;    //Alpha channel
      }

      //Create the bitmap using the new 32bit buffer   
      hbm = CreateBitmap (jcprops.JPGWidth, jcprops.JPGHeight, 1,
         C32BIT, buffer32);
      if(hbm == NULL)
      {
         MessageBox(globalhwnd, "Failed to create 32Bit Bitmap",
            "error", MB_OK);
         return false;
      }

      //remove the 32bit buffer
      delete buffer32;
   }
   else
   {
      MessageBox(globalhwnd,
         "Palettized modes not supported. Use a 16, 24, or 32 Bit display.",
         "error", MB_OK);
      return false;
   }

   //remove the old buffer
   delete buffer24;
//	  Sleep(300);
//MessageBeep(MB_ICONASTERISK);
   //Create a DirectDraw surface   
   DDSURFACEDESC2 ddsd;

   //If DirectDraw Surface is NULL, create it
   if (*Surface == NULL)
   {
      // Create a DirectDrawSurface for this bitmap
      ZeroMemory(&ddsd, sizeof(ddsd));
      ddsd.dwSize   = sizeof(ddsd);
      ddsd.dwFlags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//Todo为何不要加上DDSCAPS_SYSTEMMEMORY
      ddsd.dwWidth  = jcprops.JPGWidth;
      ddsd.dwHeight = jcprops.JPGHeight;

      //Create the surface from our surface description
      HRESULT Result = dd->CreateSurface(&ddsd, Surface, NULL);
      if(Result != DD_OK)
      {
         MessageBox(globalhwnd, "Could not create surface", "error", MB_OK);
         return false;
      }
   }
	   
   //Copy bitmap data to DirectDraw Surface
   DDCopyBitmapx(*Surface, hbm, 0, 0, 0, 0);
   // clean up and destroy the JPEG Decompressor
   ijlFree(&jcprops);

   return true;
}


//-----------------------------------------------------------------------------
// Name: DDCopyBitmap()
// Desc: Draw a bitmap into a DirectDrawSurface
//-----------------------------------------------------------------------------
extern "C" HRESULT
DDCopyBitmapx(IDirectDrawSurface7* pdds, HBITMAP hbm, int x, int y,
             int dx, int dy)
{
    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hr;
	
    if (hbm == NULL || pdds == NULL)
        return E_FAIL;
    //
    // Make sure this surface is restored.
    //
    pdds->Restore();
    //
    // Select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);
    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    dx = dx == 0 ? bm.bmWidth : dx;     // Use the passed size, unless zero
    dy = dy == 0 ? bm.bmHeight : dy;
    //
    // Get size of surface.
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);
	
    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y,
                   dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    return hr;
}
