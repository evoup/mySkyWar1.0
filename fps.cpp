#include "fps.h"
int ShowFrameRate( int currentfps )
{	 
	HDC				hdc;
	static char		szBuffer[100];
	static DWORD	dwTime;
	static int		iFramecount = 0;

	// Count every frame ( this function must 
	// be called in every frame ).
	iFramecount++;

	// If another second has passed ...
	if ( timeGetTime() - dwTime > 999 )
	{
		// ... write the frame rate to the string buffer, ...
		wsprintf( szBuffer, "FPS: %3i/%3i", iFramecount,currentfps );

		// ... reset the frame counter ...
		iFramecount = 0;

		// ... and the timer.
		dwTime = timeGetTime();
	}

	//
	// Display the frame rate.
	//
	lpddsback->GetDC( &hdc );
	SetTextAlign( hdc, TA_TOP | TA_RIGHT );
	SetBkMode( hdc, TRANSPARENT );
	SelectObject( hdc, g_hFont );
	SetTextColor( hdc, RGB( 255, 255, 255 ) );
	TextOut( hdc, 500, 4, szBuffer, strlen( szBuffer ) );
	//MessageBox(globalhwnd,szBuffer,"",MB_OK);
	//lpddswork->ReleaseDC( hdc );
	lpddsback->ReleaseDC( hdc );

	//return 0;
	return iFramecount;
}