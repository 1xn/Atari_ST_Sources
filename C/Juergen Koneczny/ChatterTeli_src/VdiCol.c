#include	<mt_mem.h>
/*
	Tabulatorweite: 3
	Kommentare ab: Spalte 60											*Spalte 60*
*/

#include	<Types2B.h>
#include	<TOS.H>
#include "VDICOL.H"

static int16	contrl[12];
static int16	intin[128];
static int16	intout[128];
static int16	ptsin[128];
static int16	ptsout[128];

static VDIPB	vdi_pb = 
{
	(int *) contrl,
	(int *) intin,
	(int *) ptsin,
	(int *) intout,
	(int *) ptsout
};

static void	vdi_str_to_c( uint16 *src, uint8 *des, int32 len );
static int32	c_str_to_vdi( uint8 *src, uint16 *des );
static int32	wstrlen( uint16 *string );

/* VDI-String in einen C-String umwandeln */
static void	vdi_str_to_c( uint16 *src, uint8 *des, int32 len )
{
	while ( len > 0 )
	{
		*des++ = (uint8) *src++;										/* nur das Low-Byte kopieren */
		len--;
	}
	*des++ = 0;																/* Ende des Strings */
}

/* C-String in einen VDI-String umwandeln */
static int32	c_str_to_vdi( uint8 *src, uint16 *des )
{
	int32	len;

	len = 0;

	while (( *des++ = *src++ ) != 0 )
		len++;

	return( len );															/* L�nge des Strings ohne Null-Byte */
}

/* L�nge einer Zeichenkette f�r Unicode-Strings bestimmen */
static int32	wstrlen( uint16 *string )
{
	int32	len;
	
	len = 0;
	
	while ( *string++ )
		len++;

	return( len );															/* L�nge des Strings ohne Terminator */
}

/*----------------------------------------------------------------------------------------*/ 
/* Druckertreiber �ffnen																						*/
/* Funktionsresultat:	VDI-Handle oder 0 (Fehler)														*/
/*	base_handle:			Handle des AES																		*/
/*	settings:				Druckereinstellung																*/
/*	work_out:				Zeiger auf int16 work_out[57]													*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_opnprn( int16 base_handle, PRN_SETTINGS *settings, int16 *work_out )
{
	int16		work_in[16];
	int16		i;

	vdi_pb.intin = (int *) work_in;
	vdi_pb.intout = (int *) work_out;
	vdi_pb.ptsout = (int *) work_out + 45;

	work_in[0] = settings->driver_id;								/* Treibernummer */
	for ( i = 1; i < 10; i++ )
		work_in[i] = 1;
	work_in[10] = 2;														/* Rasterkoordinaten benutzen */
	work_in[11] = (int16) settings->size_id;						/* Seitenformat aus settings benutzen */
	*(int8 **) &work_in[12] = settings->device;					/* Ausgabekanal aus settings benutzen */
	*(PRN_SETTINGS **) &work_in[14] = settings;					/* Druckereinstellung */

	contrl[0] = 1;															/* Funktionsnummer */
	contrl[1] = 0;
	contrl[3] = 16;														/* erweiterte Parameteranzahl */
	contrl[6] = base_handle;

	vdi( &vdi_pb );														/* Treiber �ffnen */

	vdi_pb.intin = (int *) intin;										/* Zeiger zur�cksetzen */
	vdi_pb.intout = (int *) intout;
	vdi_pb.ptsout = (int *) ptsout;

	return( contrl[6] );
}

/*----------------------------------------------------------------------------------------*/ 
/* OPEN BITMAP (VDI 100, 1)																					*/
/*	Bitmap �ffnen																									*/
/* Funktionsresultat:	-																						*/
/*	work_in:					diverse Einstellungen															*/
/*	bitmap:					Zeiger auf MFDB f�r die Bitmap												*/
/*	handle:					Zeiger auf VDI-Handle 															*/
/* work_out:				Bitmapeigenschaften																*/
/*----------------------------------------------------------------------------------------*/ 
void	v_opnbm( int16 *work_in, MFDB *bitmap, int16 *handle, int16 *work_out )
{
	vdi_pb.intin = (int *) work_in;
	vdi_pb.intout = (int *) work_out;
	vdi_pb.ptsout = (int *) work_out + 45;

	contrl[0] = 100;
	contrl[1] = 0;
	contrl[3] = 20;
	contrl[5] = 1;															/* Bitmap �ffnen */
	contrl[6] = *handle;
	*(MFDB **) &contrl[7] = bitmap;

	vdi( &vdi_pb );

	*handle = contrl[6];
	vdi_pb.intin = (int *) intin;										/* Zeiger zur�cksetzen */
	vdi_pb.intout = (int *) intout;
	vdi_pb.ptsout = (int *) ptsout;
}

/*----------------------------------------------------------------------------------------*/ 
/* RESIZE BITMAP (VDI 100, 2)																					*/
/*	Bitmap �ffnen																									*/
/* Funktionsresultat:	-																						*/
/*	handle:					Handle der Bitmap																	*/
/*	width:					neue Breite in Pixeln															*/
/*	height:					neue H�he in Pixeln																*/
/*	byte_width:				Breite einer Bitmapzeile in Bytes (wenn addr g�ltig ist)				*/
/*	addr:						Zeiger auf eigenen Speicherbereich oder 0 (VDI alloziert)			*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_resize_bm( int16 handle, int16 width, int16 height, int32 byte_width, uint8 *addr )
{
	intin[0] = width;														/* Breite in Pixeln */
	intin[1] = height;													/* H�he in Zeilen */
	*(int32 *) &intin[2] = byte_width;								/* Breite einer Zeile in Bytes (wenn der Speicher vom Aufrufer stammt) */
	*(uint8 **) &intin[4] = addr;										/* Zeiger auf die Bitmap oder 0L (VDI fordert den Speicher an) */
	
	contrl[0] = 100;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 2;															/* Bitmapgr��e ver�ndern */
	contrl[6] = handle;

	vdi( &vdi_pb );
	
	return( intout[0] );
}

/*----------------------------------------------------------------------------------------*/ 
/* OPEN BITMAP (VDI 100, 3)																					*/
/*	Bitmap �ffnen																									*/
/* Funktionsresultat:	Handle der Bitmap oder 0														*/
/*	base_handle:			Handle der �bergeordneten (virtuellen) Workstation						*/
/*	bitmap:					Zeiger auf Bitmapbeschreibung oder 0L										*/
/*	color_flags:			Flags f�r die Farbbehandlung 													*/
/*								1: (inverse) Farbtabelle von <base_handle> mitbenutzen				*/
/*	unit_flags:				-1: Pixelausma�e in dpi															*/
/*								0: Pixelausma�e in Mikrometern												*/
/*								1: Pixelausma�e in 1/10 Mikrometern											*/
/*								2: Pixelausma�e werden in 1/100 Mikrometern								*/
/*								3: Pixelausma�e in 1/1000 Mikrometern										*/
/*	pixel_width:			Pixelbreite																			*/
/*	pixel_height:			Pixelh�he																			*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_open_bm( int16 base_handle, GCBITMAP *bitmap, 
					  int16 color_flags, int16 unit_flags, int16 pixel_width, int16 pixel_height )
{
	intin[0] = color_flags;												/* Behandlung der Farbtabellen */
	intin[1] = unit_flags;												/* Ma�einheit der Pixelausma�e */
	intin[2] = pixel_width;												/* Pixelbreite */
	intin[3] = pixel_height;											/* Pixelh�he */

	contrl[0] = 100;
	contrl[1] = 0;
	contrl[3] = 4;
	contrl[5] = 3;															/* Bitmap �ffnen */
	contrl[6] = base_handle;
	*(GCBITMAP **) &contrl[7] = bitmap;								/* Zeiger auf die Bitmapbeschreibung oder 0L */

	vdi( &vdi_pb );

	return( contrl[6] );
}

/*----------------------------------------------------------------------------------------*/ 
/* CLOSE BITMAP (VDI 101, 1)																					*/
/*	Bitmap schlie�en																								*/
/* Funktionsresultat:	-																						*/
/*	handle:					Handle der Bitmap		 															*/
/*----------------------------------------------------------------------------------------*/ 
void	v_clsbm( int16 handle )
{
	contrl[0] = 101;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 1;															/* Bitmap schlie�en */
	contrl[6] = handle;

	vdi( &vdi_pb );
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE SCREEN INFORMATION (VDI 102, 1)																*/
/*	Informationen �ber das Bitmapformat erfragen															*/
/* Funktionsresultat:	-																						*/
/*	handle:					Handle der Bitmap		 															*/
/*	work_out:				Beschreibung des Bitmapformats												*/
/*----------------------------------------------------------------------------------------*/ 
void	vq_scrninfo( int16 handle, int16 *work_out )
{
	vdi_pb.intout = (int *) work_out;

	intin[0] = 2;
	contrl[0] = 102;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 1;															/* Formatbeschreibung zur�ckliefern */
	contrl[6] = handle;
	
	vdi( &vdi_pb );

	vdi_pb.intout = (int *) intout;
}

/*----------------------------------------------------------------------------------------*/ 
/*	INQUIRE EXTENDED DEVICE STATUS INFORMATION (VDI 248, 4242)         							*/
/* Dateipfad und -namen eines VDI-Treibers und seinen Klartextnamen erfragen					*/
/* Funktionsresultat:	0: Treiber ist geschlossen 1: Treiber ist offen							*/
/*	handle:					VDI-Handle																			*/
/*	id:						Ger�tekennung																		*/
/*	exists:					wird auf 1 gesetzt, wenn der Treiber angemeldet ist					*/
/*	file_path:				String f�r den Dateipfad														*/
/*	file_name:				String f�r den Dateinamen														*/
/*	name:						String f�r den Klartextnamen													*/
/*----------------------------------------------------------------------------------------*/ 
int16	vq_ext_devinfo( int16 handle, int16 id, int16 *exists, int8 *file_path, int8 *file_name, int8 *name )
{
	contrl[0] = 248;
	contrl[1] = 0;
	contrl[3] = 7;
	contrl[5] = 4242;														/* Unterfunktionsnummer */
	contrl[6] = handle;
	
	intin[0] = id;															/* Ger�tekennung */
	*(int8 **)&intin[1] = file_path;
	*(int8 **)&intin[3] = file_name;
	*(int8 **)&intin[5] = name;
	
	vdi( &vdi_pb );
	
	*exists = intout[0];
	return( intout[1] );
}

/*----------------------------------------------------------------------------------------*/ 
/* DRAW BEZIER (VDI 6, 13)																						*/
/* Bezierkurve zeichnen																							*/
/* Funktionsresultat:	-																						*/
/*	handle:					VDI-Handle				 															*/
/*	count:					Anzahl der Koordinatenpaare													*/
/*	xy:						Feld mit Koordinatenpaaren														*/
/*	bezarr:					Feld mit Punkttypen																*/
/*	extent:					Zeiger auf Rechteck (umschlie�endes Rechteck wird zur�ckgeliefert)*/
/*	totpts:					Zeiger auf int16 f�r Anzahl der berechneten Punkte						*/
/*	totmoves:				Zeiger auf int16 f�r Anzahl der Unterbrechungen/Startpunkte			*/
/*----------------------------------------------------------------------------------------*/ 
void	_v_bez( int16 handle, int16 count, int16 *xy, int8 *bezarr,
				  int16 *extent, int16 *totpts, int16 *totmoves )
{
	int8	*tmp;
	int16	i;
	
	vdi_pb.ptsin = (int *) xy;
	vdi_pb.ptsout = (int *) extent;									/* hier wird das umgebende Rechteck zur�ckgeliefert */
	
	contrl[0] = 6;
	contrl[1] = count;
	contrl[3] = ( count + 1 ) >> 1;
	contrl[5] = 13;
	contrl[6] = handle;
	
	tmp = (int8 *) intin;
	
	for( i = 0; i < count; i += 2, bezarr += 2 )					/* Bytes umsortieren wg. !%�$/ Intel */
	{
		*tmp++ = bezarr[1];
		*tmp++ = bezarr[0];
	}
	
	vdi( &vdi_pb );
	
	*totpts = intout[0];													/* Anzahl der berechneten Punkte */
	*totmoves = intout[1];												/* Anzahl der Startpunkten/Unterbrechnungen im Linienzug */
	
	vdi_pb.ptsin = (int *) ptsin;
	vdi_pb.ptsout = (int *) ptsout;
}

/*----------------------------------------------------------------------------------------*/ 
/* DRAW FILLED BEZIER (VDI 9, 13)																			*/
/* Gef�llte Bezierkurve zeichnen																				*/
/* Funktionsresultat:	-																						*/
/*	handle:					VDI-Handle				 															*/
/*	count:					Anzahl der Koordinatenpaare													*/
/*	xy:						Feld mit Koordinatenpaaren														*/
/*	bezarr:					Feld mit Punkttypen																*/
/*	extent:					Zeiger auf Rechteck (umschlie�endes Rechteck wird zur�ckgeliefert)*/
/*	totpts:					Zeiger auf int16 f�r Anzahl der berechneten Punkte						*/
/*	totmoves:				Zeiger auf int16 f�r Anzahl der Unterbrechungen/Startpunkte			*/
/*----------------------------------------------------------------------------------------*/ 
void	_v_bez_fill( int16 handle, int16 count, int16 *xy, int8 *bezarr,
						 int16 *extent, int16 *totpts, int16 *totmoves )
{
	int8	*tmp;
	int16	i;
	
	vdi_pb.ptsin = (int *) xy;
	vdi_pb.ptsout = (int *) extent;									/* hier wird das umgebende Rechteck zur�ckgeliefert */
	
	contrl[0] = 9;
	contrl[1] = count;
	contrl[3] = ( count + 1 ) >> 1;
	contrl[5] = 13;
	contrl[6] = handle;
	
	tmp = (int8 *) intin;
	
	for( i = 0; i < count; i += 2, bezarr += 2 )					/* Bytes umsortieren wg. !%�$/ Intel */
	{
		*tmp++ = bezarr[1];
		*tmp++ = bezarr[0];
	}
	
	vdi( &vdi_pb );
	
	*totpts = intout[0];													/* Anzahl der berechneten Punkte */
	*totmoves = intout[1];												/* Anzahl der Startpunkten/Unterbrechnungen im Linienzug */
	
	vdi_pb.ptsin = (int *) ptsin;
	vdi_pb.ptsout = (int *) ptsout;
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE EXTENDED FACE NAME AND INDEX (VDI 130, 1)													*/
/* Erweiterte Form von vqt_name()																			*/
/* Funktionsresultat:	ID des eingestellten Fonts														*/
/* handle:					VDI-Handle				 															*/
/*	index:					Nummer (1 bis Maximalanzahl)													*/
/*	name:						Zeichensatzname (name[32] gibt an, ob es ein Vektorfont ist)		*/
/*	font_format:			Fontformat 1: Bitmap 2: Speedo 4: TrueType 8: Type1					*/
/*	flags:					Bit 0: �quidistanz-Flag Bit 4: Symbolfont-Flag							*/
/*----------------------------------------------------------------------------------------*/ 
int16	vqt_ext_name( int16 handle, int16 index, int8 *name, uint16 *font_format, uint16 *flags )
{
	intin[0] = index;
	intin[1] = 0;

	contrl[0] = 130;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 1;															/* erweiterter Aufruf */
	contrl[6] = handle;

	vdi( &vdi_pb );

	vdi_str_to_c((uint16 *) &intout[1], (uint8 *) name, 31 );	/* String umwandeln */

	if ( contrl[4] <= 34 )												/* altes VDI? */
	{
		*flags = 0;
		*font_format = 0;
		if ( contrl[4] == 33 )											/* Flag f�r Bitmap/Vektorfonts vorhanden? */
			name[32] = 0;
		else
			name[32] = (int8) intout[33];								/* 0: Bitmapfont sonst: Vektorfont */
	}
	else
	{
		name[32] = (int8) intout[33];									/* 0: Bitmapfont sonst: Vektorfont */
		*flags = (intout[34] >> 8) & 0xff;							/* Flags */
		*font_format = intout[34] & 0xff;							/* Fontformat */
	}

	return( intout[0] );													/* ID des Fonts */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE CHARACTER INDEX (VDI 190, 0)																	*/
/*	Zeichennummer zwischen verschiedenen Mappings umrechnen (ASCII<->direkt<->Unicode)		*/
/* Funktionsresultat:	umgewandelte Zeichennummer														*/
/* handle:					VDI-Handle				 															*/
/*	src_index:				Zeichennummer																		*/
/*	src_mode:				Mapping von <src_index> (0: ASCII 1: direkt 2: Unicode)				*/
/*	dst_mode:				Mapping von <dst_index> (0: ASCII 1: direkt 2: Unicode)				*/
/*----------------------------------------------------------------------------------------*/ 
uint16	vqt_char_index( int16 handle, uint16 src_index, int16 src_mode, int16 dst_mode )
{
	intin[0] = (int16) src_index;										/* Zeichennummer */
	intin[1] = src_mode;													/* Mapping von <src_index> */
	intin[2] = dst_mode;													/* Mapping von <dst_index> (der Ausgabe) */

	contrl[0] = 190;
	contrl[1] = 0;
	contrl[3] = 3;
	contrl[5] = 0;
	contrl[6] = handle;

	vdi( &vdi_pb );

	return( intout[0] );													/* umgewandelte Zeichennummer */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET CHARACTER WIDTH, ABSOLUTE MODE (VDI 231)															*/
/* Zeichenbreite in Pixeln setzen.																			*/
/* Funktionsresultat:	-																						*/
/* handle:					VDI-Handle				 															*/
/*	width:					gew�nschte Zeichenbreite														*/
/*	char_width:				ausgew�hlte Zeichenbreite														*/
/*	char_height:			ausgew�hlte Zeichenh�he															*/
/*	cell_width:				ausgew�hlte Zeichenzellenbreite												*/
/*	cell_height:			ausgew�hlte Zeichenzellenh�he													*/
/*----------------------------------------------------------------------------------------*/ 
void	vst_width( int16 handle, int16 width, int16 *char_width, int16 *char_height, int16 *cell_width, int16 *cell_height )
{
	ptsin[0] = width;

	contrl[0] = 231;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[5] = 0;
	contrl[6] = handle;

	vdi( &vdi_pb );

	*char_width = ptsout[0];
	*char_height = ptsout[1];
	*cell_width = ptsout[2];
	*cell_height = ptsout[3];
}

/*----------------------------------------------------------------------------------------*/ 
/* SET TRACK KERNING OFFSET (VDI 237)																		*/
/* Buchstabenabstand (und somit die Laufweite) �ndern													*/
/* Funktionsresultat:	-																						*/
/* handle:					VDI-Handle				 															*/
/*	offset:					Abstand in 1/65536 Pixeln														*/
/*	pair_mode:				0: Pair-Kerning aus 1: an														*/
/*	track:					das gesetzte Track-Kerning														*/
/*	pairs:					Anzahl der Kerning-Paare														*/
/*----------------------------------------------------------------------------------------*/ 
void	vst_track_offset( int16 handle, fix31 offset, int16 pair_mode, int16 *track, int16 *pairs )
{
	contrl[0] = 237;
	contrl[1] = 0;
	contrl[3] = 4;
	contrl[6] = handle;

	intin[0] = 255;														/* benutzerdefinierten Zeichenabstand einstellen */
	intin[1] = pair_mode;
	*(fix31 *)&intin[2] = offset;										/* Zeichenabstand */

	vdi( &vdi_pb );

	*track = intout[0];
	*pairs = intout[1];
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE OUTLINE FONT TEXT EXTENT (VDI 240, 0), Unicode Binding									*/
/*	Textrechteck *ohne* Ber�cksichtigung der Textattribute zur�ckliefern							*/
/* Funktionsresultat:	-																						*/
/* handle:					VDI-Handle				 															*/
/*	string:					Unicode-Zeichenkette																*/
/*	extent:					Koordinaten des Textrechtecks													*/
/*----------------------------------------------------------------------------------------*/ 
void	vqt_extent_wide( int16 handle, uint16 *string, int16 *extent )
{
	int16	i;

	vdi_pb.intin = (int *) string;									/* Zeiger auf Unicode-String */

	contrl[0] = 240;
	contrl[1] = 0;
	contrl[3] = (int16) wstrlen( string );							/* Zeichenanzahl */
	contrl[5] = 0;															/* �berh�nge und Textattribute nicht beachten */
	contrl[6] = handle;

	vdi( &vdi_pb );

	vdi_pb.intin = (int *) intin;

	for ( i = 0; i < 8; i++ )											/* Textrechteck umkopieren */
		*extent++ = ptsout[i];
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE REAL OUTLINE FONT TEXT EXTENT (VDI 240, 4200)												*/
/*	Umgebendes Viereck unter Ber�cksichtigung aller Textattribute zur�ckliefern				*/
/* Funktionsresultat:	-																						*/
/* handle:					VDI-Handle				 															*/
/*	x:							x-Koordinate des Texts															*/
/*	y:							y-Koordinate des Texts															*/
/*	string:					Zeichenkette																		*/
/*	extent:					Koordinaten des umgebenden Vierecks											*/
/*----------------------------------------------------------------------------------------*/ 
void	vqt_real_extent( int16 handle, int16 x, int16 y, int8 *string, int16 *extent )
{
	int16	len;
	int16	i;

	ptsin[0] = x;
	ptsin[1] = y;

	len = c_str_to_vdi((uint8 *) string, (uint16 *) intin );	/* String wandeln */

	contrl[0] = 240;
	contrl[1] = 1;
	contrl[3] = len;
	contrl[5] = 4200;														/* �berh�nge und Textattribute beachten */
	contrl[6] = handle;

	vdi( &vdi_pb );

	for ( i = 0; i < 8; i++ )											/* umgebendes Viereck umkopieren */
		*extent++ = ptsout[i];
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE REAL OUTLINE FONT TEXT EXTENT (VDI 240, 4200), Unicode Binding						*/
/*	Umgebendes Viereck unter Ber�cksichtigung aller Textattribute zur�ckliefern				*/
/* Funktionsresultat:	-																						*/
/* handle:					VDI-Handle				 															*/
/*	x:							x-Koordinate des Texts															*/
/*	y:							y-Koordinate des Texts															*/
/*	string:					Unicode-Zeichenkette																*/
/*	extent:					Koordinaten des umgebenden Vierecks											*/
/*----------------------------------------------------------------------------------------*/ 
void	vqt_real_extent_wide( int16 handle, int16 x, int16 y, uint16 *string, int16 *extent )
{
	int16	i;

	vdi_pb.intin = (int *) string;									/* Zeiger auf Unicode-String */

	ptsin[0] = x;
	ptsin[1] = y;

	contrl[0] = 240;
	contrl[1] = 1;
	contrl[3] = (int16) wstrlen( string );							/* Zeichenanzahl */
	contrl[5] = 4200;														/* �berh�nge und Textattribute beachten */
	contrl[6] = handle;

	vdi( &vdi_pb );

	vdi_pb.intin = (int *) intin;

	for ( i = 0; i < 8; i++ )											/* umgebendes Viereck umkopieren */
		*extent++ = ptsout[i];
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE EXTENDED FONT INFORMATION (VDI 229)															*/
/* Erweiterte Fonteigenschaften erfragen																	*/
/* Funktionsresultat:	ID des eingestellten Fonts														*/
/*	handle:					VDI-Handle				 															*/
/*	flags:					Art der gew�nschten Information												*/
/*	id:						ID des Fonts oder 0 f�r den eingestellten Font							*/
/*	index:					Index des Fonts oder 0, wenn die ID benutzt werden soll				*/
/*	info:						Zeiger auf Auskunftsstruktur													*/
/*----------------------------------------------------------------------------------------*/ 
int16	vqt_xfntinfo( int16 handle, int16 flags, int16 id, int16 index, XFNT_INFO *info )
{
	info->size = (int32) sizeof( XFNT_INFO );						/* Strukturgr��e eintragen */

	intin[0] = flags;
	intin[1] = id;
	intin[2] = index;
	*(XFNT_INFO **)&intin[3] = info;

	contrl[0] = 229;
	contrl[1] = 0;
	contrl[3] = 5;
	contrl[5] = 0;
	contrl[6] = handle;

	vdi( &vdi_pb );

	return( intout[1] );
}

/*----------------------------------------------------------------------------------------*/ 
/* SET TEXT FACE BY NAME (VDI 230, 0)																		*/
/*	Font anhand des Namens einstellen																		*/
/* Funktionsresultat:	ID des eingestellten Fonts														*/
/*	handle:					VDI-Handle				 															*/
/*	font_format:			Fontformate 1: Bitmap 2: Speedo 4: TrueType 8: Type1					*/
/*	font_name:				gesuchter Fontname																*/
/*	ret_name:				Name des eingestellten Fonts													*/
/*----------------------------------------------------------------------------------------*/ 
int16	vst_name( int16 handle, int16 font_format, int8 *font_name, int8 *ret_name )
{
	int16	len;

	intin[0] = font_format;
	len = c_str_to_vdi((uint8 *) font_name, (uint16 *)&intin[1] );

	contrl[0] = 230;
	contrl[1] = 0;
	contrl[3] = 1 + len;
	contrl[5] = 0;
	contrl[6] = handle;

	vdi( &vdi_pb );

	if ( ret_name )
		vdi_str_to_c((uint16 *) &intout[1], (uint8 *) ret_name, contrl[4] );

	return( intout[0] );
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE FACE NAME AND ID BY NAME (VDI 230, 100)														*/
/*	Auskunftsfunktion zu vst_name																				*/
/* Funktionsresultat:	ID des eingestellten Fonts														*/
/*	handle:					VDI-Handle				 															*/
/*	font_format:			Fontformate 1: Bitmap 2: Speedo 4: TrueType 8: Type1					*/
/*	font_name:				gesuchter Fontname																*/
/*	ret_name:				Name des eingestellten Fonts													*/
/*----------------------------------------------------------------------------------------*/ 
int16	vqt_name_and_id( int16 handle, int16 font_format, int8 *font_name, int8 *ret_name )
{
	int16	len;

	intin[0] = font_format;
	len = c_str_to_vdi((uint8 *) font_name, (uint16 *) intin + 1 );

	contrl[0] = 230;
	contrl[1] = 0;
	contrl[3] = 1 + len;
	contrl[5] = 100;
	contrl[6] = handle;

	vdi( &vdi_pb );

	if ( ret_name )
		vdi_str_to_c( (uint16 *)&intout[1], (uint8 *) ret_name, contrl[4] );

	return( intout[0] );
}

/*----------------------------------------------------------------------------------------*/ 
/* SET CHARACTER MAPPING MODE (VDI 236)																	*/
/*	Zeichenbelegung umschalten																					*/
/* Funktionsresultat:	eingestellte Zeichenbelegung													*/
/*	handle:					VDI-Handle				 															*/
/*	mode:						Zeichenbelegung (0: ASCII 1: direkt 2: Unicode)							*/
/*----------------------------------------------------------------------------------------*/ 
int16	vst_map_mode( int16 handle, int16 mode )
{
	intin[0] = mode;														/* neue Belegung */
	intin[1] = 1;															/* Belegung zur�ckliefern */

	contrl[0] = 236;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[4] = 0;
	contrl[5] = 0;
	contrl[6] = handle;

	vdi( &vdi_pb );

	if ( contrl[4] )														/* R�ckgaben? */
		return( intout[0] );

	if ( mode == 1 )														/* sollte direktes Mapping eingestellt werden? */
		return( 1 );

	return( 0 );															/* sonst kann nur ASCII eingestellt werden */
}

/*----------------------------------------------------------------------------------------*/ 
/* OUTLINE FONT TEXT (VDI 241), Unicode Binding															*/
/*	Unicode-Zeichenkette mit Vektorfont ausgeben															*/
/* Funktionsresultat:	-																						*/
/*	handle:					VDI-Handle				 															*/
/*	x:							x-Koordinate																		*/
/*	y:							y-Koordinate																		*/
/*	string:					Unicode-Zeichenkette (16 Bit pro Zeichen)									*/
/*----------------------------------------------------------------------------------------*/ 
void	v_ftext_wide( int16 handle, int16 x, int16 y, uint16 *string )
{
	vdi_pb.intin = (int *) string;									/* Zeiger auf Unicode-String */

	ptsin[0] = x;
	ptsin[1] = y;

	contrl[0] = 241;
	contrl[1] = 1;
	contrl[3] = (int16) wstrlen( string );							/* Zeichenanzahl */
	contrl[5] = 0;
	contrl[6] = handle;

	vdi( &vdi_pb );

	vdi_pb.intin = (int *) intin;
}

/*----------------------------------------------------------------------------------------*/ 
/* SET RED, GREEN, BLUE (alte GEM/3-Funktion)															*/
/* Funktionsresultat:	-																						*/
/*	handle:					VDI-Handle																			*/
/*	type:						0: Textfarbe 1: F�llfarbe 2: Linienfarbe 3: Markerfarbe				*/
/*	r,g,b:					Farbwerte in Promille															*/
/*----------------------------------------------------------------------------------------*/ 
void	v_setrgb( int16 handle, int16 type, int16 r, int16 g, int16 b )
{
	contrl[0] = 138;
	contrl[1] = 0;
	contrl[3] = 3;
	contrl[5] = type;
	contrl[6] = handle;

	intin[0] = r;
	intin[1] = g;
	intin[2] = b;

	vdi( &vdi_pb );
}

/*----------------------------------------------------------------------------------------*/ 
/* GET CHARACTER OUTLINE (VDI 243, 1)																		*/
/*	Bezierzug f�r ein Zeichen zur�ckliefern, Ausrichtung und Rotation werden beachtet		*/
/* Funktionsresultat:	Anzahl der erzeugten Punkte													*/
/*	handle:					VDI-Handle																			*/
/*	index:					Zeichen-Index																		*/
/*	x_offset:				Offset, der zu jeder x-Koordinate in pts addiert werden soll		*/
/*	y_offset:				Offset, der zu jeder y-Koordinate in pts addiert werden soll		*/
/*	pts:						Feld f�r Koordinaten																*/
/*	flags:					Feld f�r Koordinatenflags (Startpunkt, Sprung, ...)					*/
/*	max_pts:					Gr��e des Felds																	*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_get_outline( int16 handle, uint16 index, int16 x_offset, int16 y_offset,
							int16 *pts, int8 *flags, int16 max_pts )
{
	contrl[0] = 243;
	contrl[1] = 0;
	contrl[3] = 8;
	contrl[5] = 1;
	contrl[6] = handle;

	intin[0] = index;
	intin[1] = max_pts;
	*(int16 **) &intin[2] = pts;
	*(int8 **) &intin[4] = flags;
	intin[6] = x_offset;
	intin[7] = y_offset;
	
	vdi( &vdi_pb );

	return( intout[0] );
}


/*----------------------------------------------------------------------------------------*/ 
/* SET TEXT FOREGROUND COLOR																					*/
/*	Vordergrundfarbe f�r Text setzen																			*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vst_fg_color( int16 handle, int32 color_space, COLOR_ENTRY *fg_color )
{
	contrl[0] = 200;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 0;															/* Textfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *fg_color;						/* Vordergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET FILL FOREGROUND COLOR																					*/
/*	Vordergrundfarbe f�r F�llmuster setzen																	*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsf_fg_color( int16 handle, int32 color_space, COLOR_ENTRY *fg_color )
{
	contrl[0] = 200;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 1;															/* F�llfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *fg_color;						/* Vordergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET LINE FOREGROUND COLOR																					*/
/*	Vordergrundfarbe f�r Linien setzen																		*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsl_fg_color( int16 handle, int32 color_space, COLOR_ENTRY *fg_color )
{
	contrl[0] = 200;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 2;															/* Linienfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *fg_color;						/* Vordergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET MARKER FOREGROUND COLOR																				*/
/*	Vordergrundfarbe f�r Marker setzen																		*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsm_fg_color( int16 handle, int32 color_space, COLOR_ENTRY *fg_color )
{
	contrl[0] = 200;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 3;															/* Markerfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *fg_color;						/* Vordergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET BITMAP FOREGROUND COLOR																				*/
/*	Vordergrundfarbe f�r Bitmaps setzen																		*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsr_fg_color( int16 handle, int32 color_space, COLOR_ENTRY *fg_color )
{
	contrl[0] = 200;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 4;															/* Bitmapfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *fg_color;						/* Vordergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}


/*----------------------------------------------------------------------------------------*/ 
/* SET TEXT BACKGROUND COLOR																					*/
/*	Hintergrundfarbe f�r Text setzen																			*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vst_bg_color( int16 handle, int32 color_space, COLOR_ENTRY *bg_color )
{
	contrl[0] = 201;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 0;															/* Textfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *bg_color;						/* Hintergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET FILL BACKGROUND COLOR																					*/
/*	Hintergrundfarbe f�r F�llmuster setzen																	*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsf_bg_color( int16 handle, int32 color_space, COLOR_ENTRY *bg_color )
{
	contrl[0] = 201;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 1;															/* F�llfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *bg_color;						/* Hintergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET LINE BACKGROUND COLOR																					*/
/*	Hintergrundfarbe f�r Linien setzen																		*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsl_bg_color( int16 handle, int32 color_space, COLOR_ENTRY *bg_color )
{
	contrl[0] = 201;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 2;															/* Linienfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *bg_color;						/* Hintergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET MARKER BACKGROUND COLOR																				*/
/*	Hintergrundfarbe f�r Marker setzen																		*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsm_bg_color( int16 handle, int32 color_space, COLOR_ENTRY *bg_color )
{
	contrl[0] = 201;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 3;															/* Markerfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *bg_color;						/* Hintergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET BITMAP BACKGROUND COLOR																				*/
/*	Hintergrundfarbe f�r Bitmaps setzen																		*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vsr_bg_color( int16 handle, int32 color_space, COLOR_ENTRY *bg_color )
{
	contrl[0] = 201;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 4;															/* Bitmapfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *bg_color;						/* Hintergrundfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}




/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE TEXT FOREGROUND COLOR																				*/
/*	Vordergrundfarbe f�r Text zur�ckliefern																*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqt_fg_color( int16 handle, COLOR_ENTRY *fg_color )
{
	contrl[0] = 202;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 0;															/* Textfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*fg_color = *(COLOR_ENTRY *) &intout[2];						/* Vordergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE FILL FOREGROUND COLOR																				*/
/*	Vordergrundfarbe f�r F�llmuster zur�ckliefern														*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqf_fg_color( int16 handle, COLOR_ENTRY *fg_color )
{
	contrl[0] = 202;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 1;															/* F�llfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*fg_color = *(COLOR_ENTRY *) &intout[2];						/* Vordergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE LINE FOREGROUND COLOR																				*/
/*	Vordergrundfarbe f�r Linien zur�ckliefern																*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vql_fg_color( int16 handle, COLOR_ENTRY *fg_color )
{
	contrl[0] = 202;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 2;															/* Linienfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*fg_color = *(COLOR_ENTRY *) &intout[2];						/* Vordergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE MARKER FOREGROUND COLOR																			*/
/*	Vordergrundfarbe f�r Marker zur�ckliefern																*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqm_fg_color( int16 handle, COLOR_ENTRY *fg_color )
{
	contrl[0] = 202;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 3;															/* Markerfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*fg_color = *(COLOR_ENTRY *) &intout[2];						/* Vordergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE BITMAP FOREGROUND COLOR																			*/
/*	Vordergrundfarbe f�r Bitmaps zur�ckliefern															*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	fg_color:				Zeiger auf COLOR_ENTRY der Vordergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqr_fg_color( int16 handle, COLOR_ENTRY *fg_color )
{
	contrl[0] = 202;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 4;															/* Bitmapfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*fg_color = *(COLOR_ENTRY *) &intout[2];						/* Vordergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}




/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE TEXT BACKGROUND COLOR																				*/
/*	Hintergrundfarbe f�r Text zur�ckliefern																*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqt_bg_color( int16 handle, COLOR_ENTRY *bg_color )
{
	contrl[0] = 203;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 0;															/* Textfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*bg_color = *(COLOR_ENTRY *) &intout[2];						/* Hintergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE FILL BACKGROUND COLOR																				*/
/*	Hintergrundfarbe f�r F�llmuster zur�ckliefern														*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqf_bg_color( int16 handle, COLOR_ENTRY *bg_color )
{
	contrl[0] = 203;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 1;															/* F�llfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*bg_color = *(COLOR_ENTRY *) &intout[2];						/* Hintergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE LINE BACKGROUND COLOR																				*/
/*	Hintergrundfarbe f�r Linien zur�ckliefern																*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vql_bg_color( int16 handle, COLOR_ENTRY *bg_color )
{
	contrl[0] = 203;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 2;															/* Linienfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*bg_color = *(COLOR_ENTRY *) &intout[2];						/* Hintergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE MARKER BACKGROUND COLOR																			*/
/*	Hintergrundfarbe f�r Marker zur�ckliefern																*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqm_bg_color( int16 handle, COLOR_ENTRY *bg_color )
{
	contrl[0] = 203;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 3;															/* Markerfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*bg_color = *(COLOR_ENTRY *) &intout[2];						/* Hintergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE BITMAP BACKGROUND COLOR																			*/
/*	Hintergrundfarbe f�r Bitmaps zur�ckliefern															*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	bg_color:				Zeiger auf COLOR_ENTRY der Hintergrundfarbe								*/
/*----------------------------------------------------------------------------------------*/ 
int32	vqr_bg_color( int16 handle, COLOR_ENTRY *bg_color )
{
	contrl[0] = 203;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 4;															/* Bitmapfarbe erfragen */
	contrl[6] = handle;
	vdi( &vdi_pb );

	*bg_color = *(COLOR_ENTRY *) &intout[2];						/* Hintergrundfarbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}





/*----------------------------------------------------------------------------------------*/ 
/* SET HILITE COLOR																								*/
/*	Hervorhebungsfarbe (f�r T_HILITE) setzen																*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	hilite_color:			Zeiger auf COLOR_ENTRY der Hervorhebungsfarbe							*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_hilite_color( int16 handle, int32 color_space, COLOR_ENTRY *hilite_color )
{
	contrl[0] = 207;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 0;															/* Hervorhebunsfarbe einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *hilite_color;					/* Hervorhebungsfarbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET MINIMUM COLOR																								*/
/*	minimalen Farbwert (f�r T_SUB) setzen																	*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	min_color:				Zeiger auf COLOR_ENTRY des minimalen Farbwerts							*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_min_color( int16 handle, int32 color_space, COLOR_ENTRY *min_color )
{
	contrl[0] = 207;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 1;															/* minimalen Farbwert einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *min_color;						/* minimaler Farbwert */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET MAXIMUM COLOR																								*/
/*	maximalen Farbwert (f�r T_ADD) setzen																	*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	max_color:				Zeiger auf COLOR_ENTRY des maximalen Farbwerts							*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_max_color( int16 handle, int32 color_space, COLOR_ENTRY *max_color )
{
	contrl[0] = 207;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 2;															/* maximalen Farbwert einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *max_color;						/* maximaler Farbwert */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET WEIGHT COLOR																								*/
/*	Gewichtung (f�r T_BLEND) setzen																			*/
/* Funktionsresultat:	-1: falsche Unterfunktionsnummer 1: alles in Ordnung					*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	weight_color:			Zeiger auf COLOR_ENTRY f�r Gewichtung										*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_weight_color( int16 handle, int32 color_space, COLOR_ENTRY *weight_color )
{
	contrl[0] = 207;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 3;															/* Gewichtung einstellen */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *weight_color;					/* Gewichtung */

	vdi( &vdi_pb );

	return( intout[0] );													/* R�ckgabewert */
}



/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE HILITE COLOR																							*/
/*	Hervorhebungsfarbe (f�r T_HILITE) erfragen															*/
/* Funktionsresultat:	Farbraum																				*/
/*	handle:					VDI-Handle																			*/
/*	hilite_color:			Zeiger auf COLOR_ENTRY der Hervorhebungsfarbe							*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_hilite_color( int16 handle, COLOR_ENTRY *hilite_color )
{
	contrl[0] = 209;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 0;															/* Hervorhebunsfarbe erfragen */
	contrl[6] = handle;

	vdi( &vdi_pb );

	*hilite_color = *(COLOR_ENTRY *) &intout[2];					/* Hervorhebungsfarbe */
	return( *(int32 *) intout );										/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE MINIMUM COLOR																						*/
/*	minimalen Farbwert (f�r T_SUB) erfragen																*/
/* Funktionsresultat:	Farbraum																				*/
/*	handle:					VDI-Handle																			*/
/*	min_color:				Zeiger auf COLOR_ENTRY des minimalen Farbwerts							*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_min_color( int16 handle, COLOR_ENTRY *min_color )
{
	contrl[0] = 209;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 1;															/* minimalen Farbwert erfragen */
	contrl[6] = handle;

	vdi( &vdi_pb );

	*min_color = *(COLOR_ENTRY *) &intout[2];						/* minimaler Farbwert */
	return( *(int32 *) intout );										/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE MAXIMUM COLOR																						*/
/*	maximalen Farbwert (f�r T_ADD) erfragen																*/
/* Funktionsresultat:	Farbraum																				*/
/*	handle:					VDI-Handle																			*/
/*	max_color:				Zeiger auf COLOR_ENTRY des maximalen Farbwerts							*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_max_color( int16 handle, COLOR_ENTRY *max_color )
{
	contrl[0] = 209;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 2;															/* maximalen Farbwert erfragen */
	contrl[6] = handle;

	vdi( &vdi_pb );

	*max_color = *(COLOR_ENTRY *) &intout[2];						/* maximaler Farbwert */
	return( *(int32 *) intout );										/* R�ckgabewert */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE WEIGHT COLOR																							*/
/*	Gewichtung (f�r T_BLEND) erfragen																		*/
/* Funktionsresultat:	Farbraum																				*/
/*	handle:					VDI-Handle																			*/
/*	weight_color:			Zeiger auf COLOR_ENTRY f�r Gewichtung										*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_weight_color( int16 handle, COLOR_ENTRY *weight_color )
{
	contrl[0] = 209;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 3;															/* Gewichtung erfragen */
	contrl[6] = handle;

	vdi( &vdi_pb );

	*weight_color = *(COLOR_ENTRY *) &intout[2];					/* Gewichtung */
	return( *(int32 *) intout );										/* R�ckgabewert */
}



/*----------------------------------------------------------------------------------------*/ 
/* TRANSLATE COLOR ENTRY TO PIXEL VALUE																	*/
/*	Pixelwert einer Farbe zur�ckliefern																		*/
/* Funktionsresultat:	Pixelwert																			*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	color:					Zeiger auf COLOR_ENTRY des Farbwerts										*/
/*----------------------------------------------------------------------------------------*/ 
uint32	v_color2value( int16 handle, int32 color_space, COLOR_ENTRY *color )
{
	contrl[0] = 204;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 0;															/* Pixelwert einer Farbe zur�ckliefern */
	contrl[6] = handle;

	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *color;							/* Farbwert */
	vdi( &vdi_pb );

	return( *(uint32*) &intout[0] );									/* Pixelwert */
}

/*----------------------------------------------------------------------------------------*/ 
/* TRANSLATE PIXEL VALUE TO COLOR ENTRY																	*/
/*	Farbe eines Pixelwerts zur�ckliefern																	*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	value:					Pixelwert																			*/
/*	color:					Zeiger auf COLOR_ENTRY f�r die zur�ckgelieferte Farbe					*/
/*----------------------------------------------------------------------------------------*/ 
int32	v_value2color( int16 handle, uint32 value, COLOR_ENTRY *color )
{
	contrl[0] = 204;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 1;															/* Farbe eines Pixelwerts zur�ckliefern */
	contrl[6] = handle;
	
	*(uint32 *) &intin[0] = value;									/* Pixelwert */
	vdi( &vdi_pb );

	*color = *(COLOR_ENTRY *) &intout[2];							/* Farbwert */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE NEAREST COLOR ENTRY																				*/
/*	n�chsten vorhandenen Farbwert zur�ckliefern															*/
/* Funktionsresultat:	Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum (z.Zt. nur 0: eingestellter Farbraum)							*/
/*	color:					Zeiger auf COLOR_ENTRY des Farbwerts										*/
/*	nearest_color:			Zeiger auf COLOR_ENTRY f�r die zur�ckgelieferte Farbe					*/
/*----------------------------------------------------------------------------------------*/ 
int32	v_color2nearest( int16 handle, int32 color_space, COLOR_ENTRY *color, COLOR_ENTRY *nearest_color )
{
	contrl[0] = 204;
	contrl[1] = 0;
	contrl[3] = 6;
	contrl[5] = 2;															/* n�chsten vorhandenen Farbwert zur�ckliefern */
	contrl[6] = handle;
	
	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[2] = *color;							/* gesuchter Farbwert */
	vdi( &vdi_pb );

	*nearest_color = *(COLOR_ENTRY *) &intout[2];				/* n�chster Farbwert */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE COLOR SPACE AND PIXEL FORMAT 																	*/
/* Eingestellten Farbraum und eingestelltes Pixelformat zur�ckliefern							*/
/* Funktionsresultat:	eingestellter Farbraum															*/
/*	handle:					VDI-Handle																			*/
/* px_format:				Zeiger auf Pixelformat der Bitmap/Workstation							*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_px_format( int16 handle, uint32 *px_format )
{
	contrl[0] = 204;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 3;
	contrl[6] = handle;
	
	vdi( &vdi_pb );

	*px_format = *(uint32 *) &intout[2];							/* Pixelformat der Bitmap/Workstation */
	return( *(int32*) &intout[0] );									/* Farbraum der Bitmap */
}



/*----------------------------------------------------------------------------------------*/ 
/* SET COLOR TABLE																								*/
/* Farbtabelle setzen																							*/
/* Funktionsresultat:	Anzahl der gesetzten Farbeintr�ge											*/
/*	handle:					VDI-Handle																			*/
/* ctab_length:			L�nge des Buffers f�r die Farbtabelle										*/
/*	ctab:						Buffer f�r die Farbtabelle														*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_ctab( int16 handle, COLOR_TAB *ctab )
{
	contrl[0] = 205;
	contrl[1] = 0;
	contrl[3] = (int16) ( ctab->length / 2 );
	contrl[5] = 0;															/* Farbtabelle einstellen */
	contrl[6] = handle;
	
	vdi_pb.intin = (int *) ctab;										/* intin zeigt auf die Farbtabelle */
	vdi( &vdi_pb );
	vdi_pb.intin = (int *) intin;										/* intin wieder zur�cksetzen */

	return( intout[0] );													/* Anzahl der gesetzten Farbeintr�ge */
}

/*----------------------------------------------------------------------------------------*/ 
/*	SET COLOR TABLE ENTRY																						*/
/* Farbeintrag setzen																							*/
/* Funktionsresultat:	Anzahl der gesetzten Farbeintr�ge											*/
/*	handle:					VDI-Handle																			*/
/*	index:					Farbtabellenindex																	*/
/*	color_space:			Farbraum																				*/
/*	color:					Zeiger auf COLOR_ENTRY															*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_ctab_entry( int16 handle, int16 index, int32 color_space, COLOR_ENTRY *color )
{
	contrl[0] = 205;
	contrl[1] = 0;
	contrl[3] = 7;
	contrl[5] = 1;															/* Farbeintrag erfragen */
	contrl[6] = handle;
	
	intin[0] = index;														/* Farbtabellenindex */
	*(int32 *) &intin[1] = color_space;								/* Farbraum */
	*(COLOR_ENTRY *) &intin[3] = *color;							/* Farbe */

	vdi( &vdi_pb );

	return( intout[0] );													/* Anzahl der gesetzten Farbeintr�ge */
}

/*----------------------------------------------------------------------------------------*/ 
/* SET DEFAULT COLOR TABLE																						*/
/* Systemfarbtabelle setzen																					*/
/* Funktionsresultat:	Anzahl der gesetzten Farbeintr�ge											*/
/*	handle:					VDI-Handle																			*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_dflt_ctab( int16 handle )
{
	contrl[0] = 205;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 2;															/* Systemfarbtabelle einstellen */
	contrl[6] = handle;
	
	vdi( &vdi_pb );

	return( intout[0] );													/* Anzahl der gesetzten Farbeintr�ge */
}





/*----------------------------------------------------------------------------------------*/ 
/*	INQUIRE CURRENT COLOR TABLE																				*/
/* Eingestellte Farbtabelle erfragen																		*/
/* Funktionsresultat:	0: Fehler 1: alles in Ordnung													*/
/*	handle:					VDI-Handle																			*/
/* ctab_length:			L�nge des Buffers f�r die Farbtabelle										*/
/*	ctab:						Buffer f�r die Farbtabelle														*/
/*----------------------------------------------------------------------------------------*/ 
int16	vq_ctab( int16 handle, int32 ctab_length, COLOR_TAB *ctab )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 0;															/* Farbtabelle erfragen */
	contrl[6] = handle;
	
	*(int32 *) &intin[0] = ctab_length;								/* L�nge der Farbtabelle */
	vdi_pb.intout = (int *) ctab;										/* intout zeigt auf die zur�ckzuliefernde Farbtabelle */
	
	vdi( &vdi_pb );

	vdi_pb.intout = (int *) intout;									/* intout wieder zur�cksetzen */

	if ( contrl[4] )
		return( 1 );														/* alles in Ordnung */
	else
		return( 0 );
}

/*----------------------------------------------------------------------------------------*/ 
/*	INQUIRE COLOR TABLE ENTRY																					*/
/* Eingestellte Farbeintrag erfragen																		*/
/* Funktionsresultat:	Farbraum																				*/
/*	handle:					VDI-Handle																			*/
/*	index:					Farbtabellenindex																	*/
/*	color:					Zeiger auf COLOR_ENTRY															*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_ctab_entry( int16 handle, int16 index, COLOR_ENTRY *color )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 1;															/* Farbeintrag erfragen */
	contrl[6] = handle;
	
	intin[0] = index;														/* Farbtabellenindex */
	vdi( &vdi_pb );

	*color = *(COLOR_ENTRY *) &intout[2];							/* Farbe */
	return( *(int32*) &intout[0] );									/* Farbraum */
}

/*----------------------------------------------------------------------------------------*/ 
/*	INQUIRE CURRENT COLOR TABLE ID																			*/
/* Kennung der Farbtabelle erfragen																			*/
/* Funktionsresultat:	Kennung																				*/
/*	handle:					VDI-Handle																			*/
/*----------------------------------------------------------------------------------------*/ 
int32	vq_ctab_id( int16 handle )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 2;															/* Kennung der Farbtabelle erfragen */
	contrl[6] = handle;
	
	vdi( &vdi_pb );

	return( *(int32*) &intout[0] );									/* Kennung */
}

/*----------------------------------------------------------------------------------------*/ 
/*	TRANSLATE COLOR TABLE INDEX TO VDI COLOR INDEX														*/
/* Farbtabellenindex in VDI-Farbindex wandeln															*/
/* Funktionsresultat:	VDI-Farbindex																		*/
/*	handle:					VDI-Handle																			*/
/*	index:					Farbtabellenindex																	*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_ctab_idx2vdi( int16 handle, int16 index )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 3;															/* Farbtabellenindex in VDI-Farbindex wandeln */
	contrl[6] = handle;
	
	intin[0] = index;
	vdi( &vdi_pb );

	return( intout[0] );													/* VDI-Index */
}

/*----------------------------------------------------------------------------------------*/ 
/*	TRANSLATE VDI COLOR INDEX TO COLOR TABLE INDEX														*/
/* VDI-Farbindex in Farbtabellenindex wandeln															*/
/* Funktionsresultat:	Farbtabellenindex																	*/
/*	handle:					VDI-Handle																			*/
/*	vdi_index:				VDI-Farbindex																		*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_ctab_vdi2idx( int16 handle, int16 vdi_index )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 4;															/* VDI-Farbindex in Farbtabellenindex wandeln */
	contrl[6] = handle;
	
	intin[0] = vdi_index;
	vdi( &vdi_pb );

	return( intout[0] );													/* Farbtabellenindex */
}

/*----------------------------------------------------------------------------------------*/ 
/* INQUIRE COLOR TABLE VALUE																					*/
/* Farbtabellenindex in Pixelwert wandeln																	*/
/* Funktionsresultat:	Pixelwert																			*/
/*	handle:					VDI-Handle																			*/
/*	index:					Farbtabellenindex																	*/
/*----------------------------------------------------------------------------------------*/ 
uint32	v_ctab_idx2value( int16 handle, int16 index )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 5;															/* Farbtabellenindex in Pixelwert wandeln */
	contrl[6] = handle;
	
	intin[0] = index;														/* Farbtabellenindex */

	vdi( &vdi_pb );

	return( *(uint32 *) &intout[0] );								/* Pixelwert */

}

/*----------------------------------------------------------------------------------------*/ 
/* GET UNIQUE COLOR TABLE ID																					*/
/* Eindeutige Kennung f�r eine eigene oder ver�nderte Farbtabelle zur�ckliefern				*/
/* Funktionsresultat:	Farbtabellenkennung																*/
/*	handle:					VDI-Handle																			*/
/*----------------------------------------------------------------------------------------*/ 
int32	v_get_ctab_id( int16 handle )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 6;															/* Farbtabellenkennung zur�ckliefern */
	contrl[6] = handle;

	vdi( &vdi_pb );

	return( *(int32 *) &intout[0] );									/* Kennung */

}

/*----------------------------------------------------------------------------------------*/ 
/*	INQUIRE DEFAULT COLOR TABLE																				*/
/* Bevorzugte Systemfarbtabelle erfragen																	*/
/* Funktionsresultat:	0: Fehler 1: alles in Ordnung													*/
/*	handle:					VDI-Handle																			*/
/* ctab_length:			L�nge des Buffers f�r die Farbtabelle										*/
/*	ctab:						Buffer f�r die Farbtabelle														*/
/*----------------------------------------------------------------------------------------*/ 
int16	vq_dflt_ctab( int16 handle, int32 ctab_length, COLOR_TAB *ctab )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 7;															/* Systemfarbtabelle erfragen */
	contrl[6] = handle;
	
	*(int32 *) &intin[0] = ctab_length;								/* L�nge der Farbtabelle */
	vdi_pb.intout = (int *) ctab;										/* intout zeigt auf die zur�ckzuliefernde Farbtabelle */
	
	vdi( &vdi_pb );

	vdi_pb.intout = (int *) intout;									/* intout wieder zur�cksetzen */

	if ( contrl[4] )
		return( 1 );														/* alles in Ordnung */
	else
		return( 0 );
}

/*----------------------------------------------------------------------------------------*/ 
/*	CREATE COLOR TABLE (VDI 206, 8)																			*/
/*	Speicher f�r Farbtabelle anfordern und initialisieren												*/
/* Funktionsresultat:	Zeiger auf Farbtabelle oder 0L (Fehler)									*/
/*	handle:					VDI-Handle																			*/
/*	color_space:			Farbraum																				*/
/*	px_format:				Pixelformat/Bitanzahl (es reicht die Angabe der Bittiefe)			*/
/*----------------------------------------------------------------------------------------*/ 
COLOR_TAB	*v_create_ctab( int16 handle, int32 color_space, uint32 px_format )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 4;
	contrl[5] = 8;															/* Farbtabelle erzeugen */
	contrl[6] = handle;
	
	*(int32 *) &intin[0] = color_space;								/* Farbraum */
	*(uint32 *) &intin[2] = px_format;								/* Bittiefe (die oberen 24 Bit von px_format werden ignoriert) */

	vdi( &vdi_pb );

	return( *(COLOR_TAB **) &intout[0] );							/* Zeiger auf Farbtabelle */
}

/*----------------------------------------------------------------------------------------*/ 
/*	DELETE COLOR TABLE (VDI 206, 9)																			*/
/*	Speicher einer mit v_create_ctab() erzeugten Farbtabelle freigeben							*/
/* Funktionsresultat:	0: Fehler 1: alles in Ordnung													*/
/*	handle:					VDI-Handle																			*/
/*	ctab:						Zeiger auf Farbtabelle															*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_delete_ctab( int16 handle, COLOR_TAB *ctab )
{
	contrl[0] = 206;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 9;															/* Farbtabelle l�schen */
	contrl[6] = handle;
	
	*(COLOR_TAB **) &intin[0] = ctab;								/* Zeiger auf Farbtabelle */

	vdi( &vdi_pb );

	return( intout[0] );
}



/*----------------------------------------------------------------------------------------*/ 
/*	CREATE INVERSE COLOR TABLE REFERENCE (VDI 208, 0)													*/
/*	Inverse Farbtabelle einer �bergebenen Farbtabelle erstellen										*/
/* Funktionsresultat:	Referenz auf inverse Farbtabelle oder 0L (Fehler)						*/
/*	handle:					VDI-Handle																			*/
/*	ctab:						Zeiger auf Farbtabelle															*/
/*	bits:						bevorzugte Aufl�sung der inversen Farbtabelle							*/
/*								(sinnvollerweise 4 oder 5 Bits [pro Komponente])						*/
/*----------------------------------------------------------------------------------------*/ 
ITAB_REF	v_create_itab( int16 handle, COLOR_TAB *ctab, int16 bits )
{
	contrl[0] = 208;
	contrl[1] = 0;
	contrl[3] = 5;
	contrl[5] = 0;															/* inverse Farbtabelle erzeugen */
	contrl[6] = handle;
	
	*(COLOR_TAB **) &intin[0] = ctab;								/* Zeiger auf Farbtabelle */
	intin[2] = bits;														/* bevorzugte Aufl�sung der inversen Farbtabelle */
	intin[3] = 0;															/* reserviert */
	intin[4] = 0;															/* reserviert */

	vdi( &vdi_pb );

	return( *(ITAB_REF *) &intout[0] );								/* Referenz auf inverse Farbtabelle */
}

/*----------------------------------------------------------------------------------------*/ 
/*	DELETE INVERSE COLOR TABLE REFERENCE (VDI 208, 1)													*/
/*	Speicher und Referenz einer inversen Farbtabelle freigeben										*/
/* Funktionsresultat:	0: Fehler 1: alles in Ordnung													*/
/*	handle:					VDI-Handle																			*/
/*	itab:						Referenz auf inverse Farbtabelle												*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_delete_itab( int16 handle, ITAB_REF itab )
{
	contrl[0] = 208;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 1;															/* inverse Farbtabelle l�schen */
	contrl[6] = handle;
	
	*(ITAB_REF *) &intin[0] = itab;									/* Referenz auf inverse Farbtabelle */

	vdi( &vdi_pb );

	return( intout[0] );
}








/*----------------------------------------------------------------------------------------*/ 
/*	TRANSFER BITMAP																								*/
/* Bitmap ausgeben																								*/
/* Funktionsresultat:	-																						*/
/*	handle:					VDI-Handle																			*/
/*	src_bm:					Quellbitmap oder 0L f�r die Ger�tebitmap									*/
/*	dstbm:					Zielbitmap oder 0L f�r die Ger�tebitmap									*/
/*	src_rect:				Quellrechteck																		*/
/*	dst_rect:				Zielrechteck																		*/
/*	mode:						Verkn�pfung																			*/
/*----------------------------------------------------------------------------------------*/ 
void	vr_transfer_bits( int16 handle, GCBITMAP *src_bm, GCBITMAP *dst_bm,
								int16 *src_rect, int16 *dst_rect,
								int16 mode )
{
	contrl[0] = 170;
	contrl[1] = 4;
	contrl[3] = 4;
	contrl[5] = 0;															/* Unterfunktionsnummer */
	contrl[6] = handle;

	*(GCBITMAP **) &contrl[7] = src_bm;								/* Zeiger auf die Quellbeschreibung */
	*(GCBITMAP **) &contrl[9] = dst_bm;								/* Zeigerauf die Zielbeschreibung */
	*(GCBITMAP **) &contrl[11] = 0L;									/* reserviert */
	
	intin[0] = mode;														/* Transfermodus */
	intin[1] = 0;															/* reserviert */
	intin[2] = 0;
	intin[3] = 0;

	ptsin[0] = src_rect[0];												/* Quellrechteck */
	ptsin[1] = src_rect[1];
	ptsin[2] = src_rect[2];
	ptsin[3] = src_rect[3];

	ptsin[4] = dst_rect[0];												/* Zielrechteck */
	ptsin[5] = dst_rect[1];
	ptsin[6] = dst_rect[2];
	ptsin[7] = dst_rect[3];

	vdi( &vdi_pb );
}




/*----------------------------------------------------------------------------------------*/ 
/* Druckerausrichtung einstellen oder erfragen															*/
/* Funktionsresultat:	Ausrichtung	(0: Hochformat 1: Querformat)									*/
/*	vdi_handle:				Handle des Druckers																*/
/*	orientation:			Ausrichtung oder -1 (nachfragen)												*/
/*																														*/
/*	Bemerkung:																										*/
/*	Wenn die Druckdialoge benutzt werden, sollte v_orient() nur aufgerufen werden, um die	*/
/*	Ausrichtung zu erfragen.																					*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_orient( int16 handle, int16 orientation )
{
	intin[0] = orientation;												/* Ausrichtung */

	contrl[0] = 5;															/* Funktionsnummer */
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[4] = 0;															/* f�r sp�teren Test l�schen */
	contrl[5] = 27;
	contrl[6] = handle;

	vdi( &vdi_pb );
	
	if ( contrl[4] == 1 )												/* wurde die Ausrichtung zur�ckgeliefert? */
		return( intout[0] );
	else
		return( 0 );
}

/*----------------------------------------------------------------------------------------*/ 
/* Anzahl der Exemplare einstellen oder erfragen														*/
/* Funktionsresultat:	Anzahl der Exemplare																*/
/*	vdi_handle:				Handle des Druckers																*/
/*	copies:					Anzahl der Exemplare oder -1 (nachfragen)									*/
/*																														*/
/*	Bemerkung:																										*/
/*	Wenn die Druckdialoge benutzt werden, sollte v_copies() nur aufgerufen werden, um die	*/
/*	Anzahl der Exemplare zu erfragen.																		*/
/*----------------------------------------------------------------------------------------*/ 
int16	v_copies( int16 vdi_handle, int16 copies )
{
	intin[0] = copies;													/* Anzahl der Exemplare */

	contrl[0] = 5;															/* Funktionsnummer */
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[4] = 0;															/* f�r sp�teren Test l�schen */
	contrl[5] = 28;
	contrl[6] = vdi_handle;

	vdi( &vdi_pb );
	
	if ( contrl[4] == 1 )												/* wurde die Anzahl der Kopien zur�ckgeliefert? */
		return( intout[0] );
	else
		return( 0 );
}

/*----------------------------------------------------------------------------------------*/ 
/* Druckerskalierung erfragen																					*/
/* Funktionsresultat:	-1: keine Skalierung != -1: Skalierung (0x10000L entspricht 100 %)*/
/*	vdi_handle:				Handle des Druckers																*/
/*																														*/
/* Bemerkung:																										*/
/*	vq_prn_scaling() sollte aufgerufen werden, um zu erfragen, ob der Druckertreiber die	*/
/* im Druckdialog vorgegebene Skalierung vornimmt oder ob die Applikation die Koordinaten	*/
/*	transformieren mu�.																							*/
/*----------------------------------------------------------------------------------------*/ 
fixed	vq_prn_scaling( int16 handle )
{
	intin[0] = -1;
	intin[1] = -1;

	contrl[0] = 5;															/* Funktionsnummer */
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[4] = 0;															/* f�r sp�teren Test l�schen */
	contrl[5] = 39;
	contrl[6] = handle;

	vdi( &vdi_pb );
	
	if ( contrl[4] == 2 )												/* wurde die Skalierung zur�ckgeliefert? */
		return( *(fixed *) intout );
	else
		return( -1L );
}

/*----------------------------------------------------------------------------------------*/ 
/*	INQUIRE PRINTER MARGINS																						*/
/* Druckerr�nder erfragen																						*/
/* Funktionsresultat:	0: Funktion existiert nicht													*/
/*	handle:					VDI-Handle																			*/
/*	top_margin:				oberer Rand in Pixeln															*/
/*	bottom_margin:			unterer Rand in Pixeln															*/
/*	left_margin:			linker Rand in Pixeln															*/
/*	right_margin:			rechter Rand in Pixeln															*/
/*	hdpi:						horizontale dpi-Aufl�sung														*/
/*	vdpi:						vertikale dpi-Aufl�sung															*/
/*----------------------------------------------------------------------------------------*/ 
int16	vq_margins( int16 handle, int16 *top_margin, int16 *bottom_margin, int16 *left_margin, int16 *right_margin, int16 *hdpi, int16 *vdpi )
{
	intout[0] = 0;
	
	contrl[0] = 5;	 
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 2100;
	contrl[6] = handle;	
	vdi( &vdi_pb );
	
	*top_margin = intout[1];											/* oberer Rand */
	*bottom_margin = intout[2];										/* unterer Rand */
	*left_margin = intout[3];											/* linker Rand */
	*right_margin = intout[4];											/* rechter Rand */
	*hdpi = intout[5];													/* horizontale Aufl�sung */
	*vdpi = intout[6];													/* vertikale Aufl�sung */

	return( intout[0]);
}

/*----------------------------------------------------------------------------------------*/ 
/*	SET DOCUMENT INFO																								*/
/*	Dokumenteninfo setzen																						*/
/* Funktionsresultat:	0: Funktion existiert nicht													*/
/*	handle:					VDI-Handle																			*/
/*	type:						Art des Strings																	*/
/*								0: Name der Applikation															*/
/*								1: Name des Dokuments															*/
/*								2: Name des Bearbeiters															*/
/*								3: Kommentar																		*/
/*								4: Dateiname des Dokuments														*/
/*	s:							Zeiger auf den String															*/
/*	wchar:					0: 8 Bit pro Zeichen 1: 16 Bit pro Zeichen								*/
/*----------------------------------------------------------------------------------------*/ 
int16	vs_document_info( int16 handle, int16 type, void *s, int16 wchar )
{
	uint16	*input;

	intout[0] = 0;
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[5] = 2103;
	contrl[6] = handle;

	intin[0] = type;

	input = (uint16 *) &intin[1];

	if ( wchar )															/* 16 Bit pro Zeichen? */
	{
		uint16	*wstr;
		
		wstr = (uint16 *) s;
		while (( *input++ = *wstr++ ) != 0 );
	}
	else																		/* 8 Bit pro Zeichen */
	{
		uint8	*str;
		
		str = (uint8 *) s;
		while (( *input++ = *str++ ) != 0 );
	}

	contrl[3] = (int16 *) input - intin - 1;						/* Parameteranzahl in intin */

	vdi( &vdi_pb );

	return ( intout[0] );
}
