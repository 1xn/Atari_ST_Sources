#if !defined(__MODDEF__)
#define __MODDEF__

#if !defined(uchar)
#define uchar	unsigned char
#endif

#define _G_ mblock.gpfn->
#define _ID_ (mblock.id)

#if !defined(__LAY_AND_MASK__)
#define __LAY_AND_MASK__
typedef struct
{
	int		id;				/* Eine eindeutige ID f�r den Undo-Puffer */
	int		changes;	/* �nderungen vorgenommen (draw/clr etc..) */
	char	name[11];	/* 10 Zeichen Name 0-terminiert */
	int		type;			/* 0=transparent, 1=deckend, 2=deckend incl. wei� */
	int		selected;	/* !=0: Selektiert */
	int		visible;	/* !=0: sichtbar */
	int		draw;			/* !=0: reinzeichnen */
	int		solo;			/* !=0: Nur diese Ebene sichtbar & zeichnen */
	int		width;
	int		height;
	int		word_width;
	void	*red;
	void	*yellow;
	void	*blue;
}LAYER;

typedef struct
{
	int		id;				/* Einmalige Kennung */
	void *mask; 		/* Zeiger auf 8-Bit Plane */
	char	name[11];	/* 10 Zeichen Name 0-terminiert */
	int		col;			/* Farbe der Maske im AES-Objekt-Code */
}MASK;
#endif

typedef struct
{
	/* �bergebenen Bereich in Undo-Buffer */
	int cdecl				(*undo_buf)(GRECT *area, LAYER *layer, MASK *mask, int auto_res);
	/* Neue Dokumentgr��e beantragen */
	int cdecl				(*resize)(int w, int h);
	/* Modul ausf�hren (z.B. "Ausf�hren"-Button) */
	void cdecl			(*do_me)(int id);
	/* Preview aktualisieren (z.B. Echtzeit-Slider) */
	void 						(*update_preview)(void);
	
	/* Button im Dialogfenster desselektieren */
	void cdecl			(*d_unsel)(int id, int ob);
	/* Objekt im Dialogfenster neu zeichnen */
	void cdecl			(*d_objc_draw)(int id, int ob);
	
	/* Modul beendet sich selbst */
	void cdecl			(*mod_quit)(int id);
	
	void	cdecl 		(*slide)(int id, int ob, int min, int max, int now, void cdecl(*newval)(int now2));
	void	cdecl 		(*bar)(int id, int ob, int min, int max, void cdecl(*newval)(int now2));
	void	cdecl			(*set_slide)(int id, int ob, int min, int max, int val);

	int		cdecl			(*get_prev_opt)(void);
	
	void	cdecl			(*set_pal_col)(int index, int cmy[3]);
	void	cdecl			(*set_pal_cols)(int start_index, int end_index, int *cmy_values);

	void	cdecl			(*get_pal_col)(int index, int *cmy3);
	void	cdecl			(*get_pal_cols)(int start_index, int end_index, int *cmy_values);


	/* Set the color-palette for conversion to pal[n][3], where
		pal[0][0]=R[0], pal[0][1]=G[0], pal[0][2]=B[0]
		pal[1][0]=R[1], pal[1][1]=G[1], pal[1][2]=B[1]
		...
	*/
	void cdecl 			(*set_col_pal)(int mode, int num, int *pal);

}GRAPE_FN;
/* get/set_pal definitions */
#define RGB_8				0	/* RGB-values 0-255 */
#define CMY_8				1	/* CMY-values 0-255 */
#define RGB_PM			2	/* RGB-values promille */
#define CMY_PM			3	/* CMY-values promille */
#define RGB_PM_VDI	4	/* RGB-values promille, Atari-XBIOS-Palette */
#define CMY_PM_VDI	5	/* CMY-values promille, Atari-XBIOS-Palette */

typedef struct
{
	void cdecl			(*mod_init)(void);	/* Initialisierung des Moduls */
	void cdecl			(*mod_quit)(void);	/* Deinitialisierung */
	/* Eigentliche Funktion */
	/* Soll 0 liefern wenn nix, 1 wenn etwas gemacht wurde */
	/* F1 wird gerufen, wenn auf Bilddaten ausgef�hrt wird
		 (dann kann �ber l/m undo_buf aufgerufen werden)
		 F2 wird f�r Preview gerufen (c,m,y k�nnen jeweils auch
		 NULL sein) 
		 Parameter prv: Wird 0 �bergeben, ist der Preview f�r die
		 								skalierte Gesamtansicht, bei 1 ist der
		 								Preview f�r die unskalierte Detailansicht.
		 								Ist einer oder beide der Previews durch das
		 								Modul nicht sinnvoll zu bew�ltigen, sollte
		 								einfach 0 zur�ckgegeben werden.
		 
		 Falls das Modul aus F2 aus F2 aufruft, sollte es sich selbst
		 in prv -1 �bergeben, da dieser Parameter von Grape nie �bergeben
		 wird. So kann das Modul falls notwendig zwischen tats�chlicher
		 Operation und Preview unterscheiden.
  */
	int cdecl				(*do_function1)(GRECT *area, LAYER *l, MASK *m);
	int cdecl				(*do_function2)(uchar *c, uchar *m, uchar *y, int w, int h, GRECT *area, int prv);
}MODULE_FN;

typedef struct
{
	char			magic[12]; /* Mu� "GRAPEMODULE"+'0' sein */
	long			version;	 /* diese Version: '0101' */

	/* Daten f�r das Popup */

	char			name[32];					/* Der Name des Moduls */

	CICONBLK	_16x16;						/* 16*16 Farbicondaten */
	int				mono_mask[2*16];
	int				mono_data[2*16];
	char			mono_text[2];			/* Zwei Nullbytes */
	CICON			cicon;
	int				col_data[8*16];		/* In 16 Farben */
	int				col_mask[2*16];
	
	/* Moduldaten */

	int		flags;		/* Bit:	 Bedeutung wenn gesetzt:
												0: do_function1 direkt nach Initialisierung
													 aufrufen (f�r Module, die z.B. nur Info
													 �ber Daten anzeigen, wie Histogram)
									*/
	
	MODULE_FN	*mod_fn; 	/* Funktionspointer */

	/* Dialoginfo */
	OBJECT		*tree;								/* Zeiger auf Dial-tree oder NULL */
	void			cdecl (*dservice)(int ob);	/* Service-Routine f�r Dialog */

	/* Wird von Grape ausgef�llt */
	
	int				*global;			/* global[80] Kann in das eigene global-Feld kopiert werden */
	GRAPE_FN	*gpfn;				/* Div. Funktionspointer */
	int				id;						/* Die von Grape vergebene id */
}MODULEBLK;

#endif