In den einzelnen Ordnern befinden sich folgende
Programme:

Ordner			Datei
----------------------------------------------------------
Hello			Hello1.exe
			Hello2.exe
			Hello1.gfw
			Hello2.gfw

1. GFA-Basic Fenster mit einer CALLBACK-Funktion zur
   Steuerung des Fensters
2. GFA-Basic Fenster mit zwei CALLBACK-Funktionen.
   Die erste f�r das Hauptfenster. Zweite �bernimmt
   die Steuerung des Client-Fensters. In Windows 95
   kann man so den vertieften Clientbereich darstellen. 

Ordner			Datei
----------------------------------------------------------
Checker			Checker.exe
			Checker.gfw

Beispiel zur Steuerung und Abfrage von Mausereignissen
im Clientbereich eines Fensters.


Ordner			Datei
----------------------------------------------------------
OwnerDraw		Ownerdraw.exe
			Ownerdraw.gfw

Fenster wie gehabt mit zwei Buttons im Clientbereich im
BS_OWNERDRAW-Stil. Mittels einer OWNERDRAWSTRUCT-Struktur
wird in der Message WM_OWNERRAW das Zeichnen und Steuern
eigener Buttons �bernommen.


Ordner			Datei
----------------------------------------------------------
Colors			Colors.exe
			Colors.gfw

Beispiel zur Funktionsweise von ScrollBars. Mittels dieser
werden Farben ver�ndert und dargestellt.


Ordner			Datei
----------------------------------------------------------
MultiWin		Multiwin.exe
			Multiwin.gfw

In vier Fenstern werden durch Timer-Abfrage verschiedene
Berechnung gleichzeitig dargestellt (Multitasking).


Ordner			Datei
----------------------------------------------------------
MdiDemo			Mdidemo.exe
			Mdidemo.gfw
			Mdidemo.res

Grundaufbau von einem Parent-Fenster mit beliebig
vielen Childwindows, aber ohne Verwendung der
GFA-Basic-Befehle PARENTW #0, CHILDW #1.
In der Resourcen-Datei steht das Men�.
Die Programme "Edit" und "Bitmap1" sind nach diesem
Schema programmiert worden.


Ordner			Datei
----------------------------------------------------------
ToolBar			Toolbar.exe
			Toolbar.gfw
			Toolbar.res

Beispielprogramm zum Erstellen von einer ToolBar und
StutusBar mittels neuer Funktionen, die in der COMMCTRL.DLL
zu finden sind.
�ber Men�s k�nnen alle wichtigen Messages und Stile der
Bars dargestellt werden.
Unter Windows 95 l�uft dieses Programm am besten.

Die folgenden Programme beinhalten alles, was zuvor
neu programmiert und ausprobiert wurde.
Alle Programme haben eine ToolBar und eine StatusBar.
Sie laufen desweiteren mit einer DLL zusammen, in der
Befehle zur Druckereinstellung, zum Steuern von Karteikarten,
zur Steuerung von Spin-Buttons und zum Bearbeiten von Bitmaps
eingebunden sind.

Ordner			Datei
----------------------------------------------------------
Bitmap			Bitmap1.exe
			Bitmap1.gfw
			Bitmap1.res
			Bmpdll.dll
			Bitmapdl.gfw
			Bitmapdl.res

Gute Programme zur Darstellung und Bearbeitung von Bitmaps
gibt es schon genug. Aber durch Selbstprogrammierung k�nnen
Fragen �ber das Bitmap beantwortet werden.
Wie lade, speichere, bearbeite ich ein Bitmap und wie sind
die Strukturen aufgebaut.


Ordner			Datei
----------------------------------------------------------
Editor			Edit.exe
			Edit.gfw
			Edit.res
			Edit.ini
			Editdll.dll
			Editdll.gfw
			Editdll.res

Dieses Programm ist entstanden, um die Frage zu be-
antworten, wie man den Inhalt mehrerer Childwindows
eigenst�ndig, also getrennt voneinander, bearbeiten kann.

Antwort: eine Childwindow-Datenstruktur mu� her.


Wichtig: In den beiden letzten Programmen k�nnen sich noch
	 kleine Fehler eingeschlichen haben. Diese Beispielprogramme
	 sollen nur aufzeigen, wie und was man alles mit dem
	 hervorragendem GFA-Basic f�r Windows anstellen kann.
	 F�r einige Programme ben�tigt man einen Resourcen-Editor.


				Hildesheim, den 20.09.1996
 
 
 
