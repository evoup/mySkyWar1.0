#include "statusinfo.h"
int showHITS( int hits )
{	 
	HDC				hdc;
	static char		szBuffer[100];
	
	wsprintf( szBuffer, " %3i", hits );
		
	
	//
	// Display the HITS.
	//
	lpddsback->GetDC( &hdc );
	SetTextAlign( hdc, TA_TOP | TA_RIGHT );
	SetBkMode( hdc, TRANSPARENT );
	SelectObject( hdc, status_info_hFont );
	SetTextColor( hdc, RGB( 255, 255, 255 ) );
	TextOut( hdc, 600, 355, szBuffer, strlen( szBuffer ) );
	//MessageBox(globalhwnd,szBuffer,"",MB_OK);
	//lpddswork->ReleaseDC( hdc );
	lpddsback->ReleaseDC( hdc );
	
	return 0;
}

int showHP( int HP ,int maxHP)
{	 
	HDC				hdc;
	static char		szBuffer[100];
	
	//wsprintf( szBuffer, " %3i/%3i", HP,maxHP );
	wsprintf( szBuffer, " LIFE%3i", HP );
	
	
	//
	// Display the HITS.
	//
	lpddsback->GetDC( &hdc );
	SetTextAlign( hdc, TA_TOP | TA_RIGHT );
	SetBkMode( hdc, TRANSPARENT );
	SelectObject( hdc, status_info_hFont );
	SetTextColor( hdc, RGB( 51, 255, 153 ) );
	TextOut( hdc, 575, 285, szBuffer, strlen( szBuffer ) );
	//MessageBox(globalhwnd,szBuffer,"",MB_OK);
	//lpddswork->ReleaseDC( hdc );
	lpddsback->ReleaseDC( hdc );
	
	return 0;
}

