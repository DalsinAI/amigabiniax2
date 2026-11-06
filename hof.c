/******************************************************************************
BINIAX HALL OF FAME IMPLEMENTATIONS
COPYRIGHT JORDAN TUZSUZOV, (C) 2006-2009

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

LICENSE ORIGIN : http://www.gzip.org/zlib/zlib_license.html

For complete product license refer to LICENSE.TXT file

******************************************************************************/

/******************************************************************************
INCLUDES
******************************************************************************/
#include <string.h>

#include "inc.h"

#if !defined(_WIN32) && !defined(__amigaos__)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <errno.h>
#include <string.h>

#define chCursor		'_'			/* Cursor ON */
#define chSpace			' '			/* Cursor OFF*/

#define csHOFName		"hof.bnx2"	/* File name */
#define cHOFFileSize	504			/* File size */

BNX_HALL	Hof;

/******************************************************************************
FUNCTIONS
******************************************************************************/
void hofAddLetter( BNX_INT16 pos, char a, char *name );
void hofBlinkCursor( BNX_INT16 pos, char *name );
void hofResetCursor( BNX_INT16 pos, char *name );

BNX_BOOL hofOldDebianInit();
BNX_BOOL hofOldDebianSave();

void hofAddLetter( BNX_INT16 pos, char a, char *name )
{
	if ( pos < cHofNameLen-1 )
	{
		name[ pos ] = a;
	}
}

void hofBlinkCursor( BNX_INT16 pos, char *name )
{
	static BNX_INT16 delayer = 0;

	if ( pos < cHofNameLen-1 )
	{
		if ( (delayer & 1) == 0 )
		{
			if ( name[ pos ] == chSpace )
			{
				name[ pos ] = chCursor;
			}
			else
			{
				name[ pos ] = chSpace;
			}
			delayer = 0;
		}
		delayer ++;
	}
}

void hofResetCursor( BNX_INT16 pos, char *name )
{
	if ( pos < cHofNameLen-1 )
	{
		name[ pos ] = chSpace;
	}
}

static const char *hofFileName()
{
#if !defined(_WIN32) && !defined(__amigaos__)
	static char filename[1024];
	char *home;
	struct passwd *passwd;
	if (!getuid())
	{
				fprintf(stderr, "No access to data files for root.\n");
				return BNX_FALSE;
	} else {
		if ((home = getenv("XDG_DATA_HOME")))
		{ /* See http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html */
			if (strlen(home) > 1024 - sizeof("/biniax2/hof"))
			{
				fprintf(stderr, "$XDG_DATA_HOME is excessively long.n");
				return BNX_FALSE;
			}
			snprintf(filename, sizeof(filename), "%s/biniax2", home);
			mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else if ((home = getenv("HOME")))
		{
			passwd = getpwuid (getuid());
			home=passwd->pw_dir;
			if (!home)
			{
				fprintf(stderr, "$HOME is not defined.\n");
				return BNX_FALSE;
			}
			if (strlen(home) > 1024 - sizeof("/.local/share/biniax2/hof"))
			{
				fprintf(stderr, "$HOME is excessively long.n");
				return BNX_FALSE;
			}
			snprintf(filename, sizeof(filename), "%s/.local", home);
			mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			strncat(filename, "/share", sizeof(filename)-1);
			mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			strncat(filename, "/biniax2", sizeof(filename)-1);
			mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else
		{
			fprintf(stderr, "Neither $XDG_DATA_HOME nor $HOME are defined.\n");
			return BNX_FALSE;
		}
	}
	strncat(filename, "/hof", sizeof(filename)-1);
	return filename;
#elif defined(__amigaos__)
	return "PROGDIR:hof.bnx2";	/* in the drawer */
#else
	return "./hof.bnx2";
#endif
}

BNX_BOOL hofInit()
{
	FILE		*file;
	BNX_INT16	i, j;

	if (hofOldDebianInit() == BNX_TRUE)
		return BNX_TRUE;

	for ( i = 0; i < cHofEntries; ++i )
	{
#ifdef __amigaos__
		strcpy( Hof.arcade[ i ].name, "JORDAN                       " );	/* upstream's */
#else
		strcpy( Hof.arcade[ i ].name, "DEBIAN                       " );
#endif
		Hof.arcade[ i ].score = (cHofEntries - i) * cHofInitScore;

#ifdef __amigaos__
		strcpy( Hof.tactic[ i ].name, "JORDAN                       " );	/* upstream's */
#else
		strcpy( Hof.tactic[ i ].name, "DEBIAN                       " );
#endif
		Hof.tactic[ i ].score = (cHofEntries - i) * cHofInitScore;
	}

	if ( sysGetFileLen( hofFileName() ) != cHOFFileSize )
		return BNX_FALSE;
	file = fopen( hofFileName(), "rb" );
	if ( file == (FILE *) NULL )
		return BNX_FALSE;

	for ( i = 0; i < cHofEntries; ++i )
	{
		for ( j = 0; j < cHofNameLen; ++j )
		{
			Hof.arcade[ i ].name[ j ] = sysFGet8( file );
		}
		sysFGet2byte( file );
		Hof.arcade[ i ].score = sysFGet32( file );
	}

	for ( i = 0; i < cHofEntries; ++i )
	{
		for ( j = 0; j < cHofNameLen; ++j )
		{
			Hof.tactic[ i ].name[ j ] = sysFGet8( file );
		}
		sysFGet2byte( file );
		Hof.tactic[ i ].score = sysFGet32( file );
	}

	fclose( file );

	return BNX_TRUE;
}

BNX_BOOL hofSave()
{
	FILE		*file;
	int			i, j;

	file = fopen( hofFileName(), "wb" );

	if ( file == (FILE *) NULL )
		return BNX_FALSE;

	for ( i = 0; i < cHofEntries; ++i )
	{
		for ( j = 0; j < cHofNameLen; ++j )
		{
			sysFPut8( Hof.arcade[ i ].name[ j ], file );
		}
		sysFPut2byte( file );
		sysFPut32( Hof.arcade[ i ].score, file );
	}

	for ( i = 0; i < cHofEntries; ++i )
	{
		for ( j = 0; j < cHofNameLen; ++j )
		{
			sysFPut8( Hof.tactic[ i ].name[ j ], file );
		}
		sysFPut2byte( file );
		sysFPut32( Hof.tactic[ i ].score, file );
	}

	fclose( file );

	return BNX_TRUE;
}

BNX_BOOL hofEnter( BNX_GAME *game )
{
	BNX_INT32		startTime;
	BNX_INT16		viewoption = cGfxHofNoKeyboard;
	BNX_INT16		curPos = 0;
	BNX_INT16		i, j;
	char			cChar = 0;
	BNX_HALL_ENTRY	*recEntry = 0;

	switch ( game->mode )
	{
		case cModeRealtime:
			for ( i = 0; i < cHofEntries; ++i )
			{
				if ( Hof.arcade[ i ].score < game->score[ cPlayer1 ] )
				{
					recEntry = &Hof.arcade[ i ];
					viewoption = cGfxHofKeyboardDn;
					break;
				}
			}
			for ( j = cHofEntries-1; j > i; --j )
			{
				strcpy( Hof.arcade[ j ].name, Hof.arcade[ j-1 ].name );
				Hof.arcade[ j ].score = Hof.arcade[ j-1 ].score;
			}
			break;
		case cModeTurn:
			for ( i = 0; i < cHofEntries; ++i )
			{
				if ( Hof.tactic[ i ].score < game->score[ cPlayer1 ] )
				{
					recEntry = &Hof.tactic[ i ];
					viewoption = cGfxHofKeyboardUp;
					break;
				}
			}
			for ( j = cHofEntries-1; j > i; --j )
			{
				strcpy( Hof.tactic[ j ].name, Hof.tactic[ j-1 ].name );
				Hof.arcade[ j ].score = Hof.tactic[ j-1 ].score;
			}
			break;
		default:
			return BNX_TRUE;
	}

	if ( recEntry == 0 )
	{
		return BNX_FALSE;
	}

	strcpy( recEntry->name, "                             " );
	recEntry->score = game->score[ cPlayer1 ];
	inpInit();
	do
	{
		startTime = sysGetTime();

		gfxGetVirtualChar( game, inpDirect() );
		cChar = inpGetChar();
		if ( cChar > 0 )
		{
			hofAddLetter( curPos, cChar, recEntry->name );
			if ( curPos < cHofNameLen - 1 )
			{
				curPos++;
			}
		}
		if ( inpKeyDel() == BNX_TRUE )
		{
			hofResetCursor( curPos, recEntry->name );
			if ( curPos > 0 )
			{
				curPos--;
			}
		}
		hofBlinkCursor( curPos, recEntry->name );

		inpUpdate();
		gfxRenderHof( &Hof, viewoption );
		gfxUpdate();
		sndUpdate();
		sysUpdate();

		// Synchronize with the clock
		while ( sysGetTime() - startTime < cDeltaTime )
		{
			sysUpdate();
		}
	}
	while ( inpKeyA() == BNX_FALSE && inpKeyB() == BNX_FALSE );
	hofResetCursor( curPos, recEntry->name );

	return BNX_TRUE;
}

void hofView()
{
	BNX_INT32	startTime;

	inpInit();
	do
	{
		startTime = sysGetTime();

		inpUpdate();
		gfxGetHelpPen( inpDirect() );
		gfxRenderHof( &Hof, cGfxHofNoKeyboard );
		gfxUpdate();
		sndUpdate();
		sysUpdate();

		// Synchronize with the clock
		while ( sysGetTime() - startTime < cDeltaTime )
		{
			sysUpdate();
		}
	}
	while ( inpKeyA() == BNX_FALSE );
}

BNX_HALL *hofGet()
{
	return (BNX_HALL *) &Hof;
}

// DEBIAN SPECIFIC

BNX_BOOL hofOldDebianInit()
{
	FILE		*file = NULL;
	int		i;
	uint16_t	id, me, ml;
	const char *filename = NULL;
	char alt_filename[1024+3];

	file = fopen( (filename = hofFileName()), "rb" );

	if ( file == (FILE *) NULL ) // Bug in previous version, see if data can be reached
	{
		char *ptr;
		strncpy(alt_filename, filename, 1024);
		ptr = strrchr(alt_filename, '/');
		if (ptr != NULL)
		{
			ptr++;
			filename = alt_filename;
			strcpy(ptr, "config");
			file = fopen( filename, "rb" );
		}
	}

	if ( file == (FILE *) NULL )
	{
		goto error;
	}

	errno = 0;

	id = sysFGet16(file);
	if (id != 0xB2F1)
		goto error;

	fprintf(stderr, "Loading Hall of Fame data with old format from \"%s\"\n", filename);

	ml = sysFGet16(file);
	if (ml != cHofNameLen)
		goto error;

	me = sysFGet16(file);
	if (me != cHofEntries)
		goto error;

	for ( i = 0; i < cHofEntries; ++i )
	{	
		fread( Hof.arcade[i].name, 1, cHofNameLen, file );
		Hof.arcade[i].score = sysFGet32(file);
	}

	for ( i = 0; i < cHofEntries; ++i )
	{	
		fread( Hof.tactic[i].name, 1, cHofNameLen, file );
		Hof.tactic[i].score = sysFGet32(file);
	}

	id = sysFGet16(file);
	if (id != 0xB2F0)
		goto error;

	fclose( file );

	return BNX_TRUE;

error:
	if (file)
		fclose( file );
	return BNX_FALSE;
}

BNX_BOOL hofOldDebianSave()
{
	FILE		*file;
	int 		i;
	const char *filename = NULL;

	file = fopen( (filename=hofFileName()), "wb" );

	if ( file == (FILE *) NULL )
	{
		if (errno)
			fprintf(stderr, "Could not save Hall of Fame data with old format in \"%s\": %s\n", filename, strerror(errno));
		else
			fprintf(stderr, "Could not save Hall of Fame data with old format in \"%s\"\n", filename);
		return BNX_FALSE;
	}

	fprintf(stderr, "Saving Hall of Fame data with old format in \"%s\"\n", filename);

	sysFPut16( 0xB2F1 , file );
	sysFPut16( cHofNameLen , file );
	sysFPut16( cHofEntries , file );

	for ( i = 0; i < cHofEntries; ++i )
	{
		fwrite( Hof.arcade[i].name, 1, cHofNameLen, file );
		sysFPut32(Hof.arcade[i].score, file);
	}

	for ( i = 0; i < cHofEntries; ++i )
	{
		fwrite( Hof.tactic[i].name, 1, cHofNameLen, file );
		sysFPut32(Hof.tactic[i].score, file);
	}

	sysFPut16( 0xB2F0 , file );

	fclose( file );

	return BNX_TRUE;
}
