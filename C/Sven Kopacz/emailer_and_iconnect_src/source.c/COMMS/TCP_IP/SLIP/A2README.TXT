Hi,

hier das wichtigste zu dieser Beta:

Der Resolver merkt sich jetzt die letzte DNS-Anfrage, das wirkt 
sich nat�rlich besonders bei CAB aus, wo f�r eine Seite zig mal der
selbe Host resolved wird. D.h. die Anzeige "Resolving host" sollte in
der CAB-Status-Zeile jetzt nur noch beim ersten Aufruf einer Seite
f�r l�ngere Zeit erscheinen (je nach Laune des DNS), f�r alle weiteren 
Elemente, die f�r diese Seite vom selben Server geladen werden (also 
meistens f�nfhundert GIFs;-) sollte die Anzeige nur einen Sekundenbruchteil
dauern.

Das CAB.OVL l�uft jetzt (hoffentlich) auch auf allen Rechnern, auf denen 
nicht Macintosh drauf steht.

PPP ist jetzt drin und ich hoffe, da� ihr das alle fr�hlich testet.
Zur PPP-Einwahl geht man wie folgt vor:
SOCKETS.PRG mu� nat�rlich installiert sein (kann auch vom Desktop gestartet
werden. Oh, da f�llt mir ein, diese Version ist mit dem "Beta-Override-
Feature" ausgestattet. D.h. wenn bereits ein SOCKET.PRG installiert ist, wird
das einfach ohne Warnung �berb�gelt. Also am besten NICHT SOCKET.PRG neu
starten, wenn gerade eine Anwendung l�uft, die auf die Sockets zugreift!)

So, dann wird wie bisher SLIP.PRG gestartet (hm, das h�tte ich sinnvoller-
weise umbenennen k�nnen). Man sieht jetzt den neuen Dialog, der ist fast
wie der alte, nur Quer- statt Hochformat (wg. 640x400) und es gibt die beiden
neuen Buttons "SLIP" und "PPP". Nat�rlich klickt man da jetzt PPP an. Alles
andere ist beim alten geblieben, es k�nnen auch die alten Connection-Setups
weiterverwendet werden (mittels "Load").
Wenn man PPP verwendet, kann es sein, da� der Peer beim Einw�hlen die DNS
rausr�ckt, aber nicht alle Provider unterst�tzen das (meine Uni z.B. nicht).
Besser ist es also, den DNS selber einzutragen, andererseits h�tte ich gerne
gewu�t, ob die automatische Nachfrage �berhaupt richtig funktioniert, ich
kanns nicht testen. Wer das kann, m�ge es bitte tun. Dazu bei DNS-IP einfach
0.0.0.0 eintragen.
Das gleiche kann man bei der Default-IP machen, denn die eigene IP sollte
auf jeden Fall automatisch erfragt werden k�nnen.

So, dann wie bisher einfach auf "OK" klicken, das Terminal-Fenster erscheint
und man w�hlt sich wieder manuell ein. Beim Provider dann falls n�tig PPP
aktivieren. Wahrscheinlich wird der Host des Providers gleich mal eine PPP-
Konfig-Anfrage abschicken, das hei�t das im Terminal-Fenster ein paar wirre 
Zeichen erscheinen, sobald PPP aktiviert ist. Das macht aber nix.
Jetzt wird einfach in dem kleinen Fenster auf "Established" geklickt.

Der socket-Kernel beginnt dann mit der PPP-Konfiguration, dazu gibt es 
momentan noch keine optische Anzeige! Sonder nur zwei m�gliche Resultate:

1) Wenn alles gutgeht, erscheint nach ein paar Sekunden unter "Local IP" 
  (dort wo man f�r SLIP bisher manuell die IP eintragen mu�te) die eigene IP.
 Also diese Anzeige im Auge behalten, zuerst steht da ja einfach nur 0.0.0.0
 sofern ihr das im Setup-Dialog als Default eingetragen habt.
 Erst jetzt sollte man irgendwelche Programme wie CAB o.�., also socket-Apps
 starten!

2) Gibt es bei der PPP-Abstimmung Probleme, erscheint nach einiger Zeit 
 (maximal so 30 Sekunden) ein Fehlerfenster. Dort werden alle m�glichen 
 Ursachen aufgelistet und dahinter steht entweder YES oder No.
 Falls das passiert, m��t ihr das ganze nochmal machen, aber vorher im Setup
 den "Debug-File"-Button aktivieren. Dieses File liegt dann als DEBUG.LOG im
 gleichen Ordner wie SLIP.PRG. Das File brauche ich und au�erdem eine 
 Auflistung aller Fehler, die auf YES standen.
 Falls der einzige Fehler, der auf "YES" steht, aber der ist, da� der DNS
 nicht erfragt werden konnte, dann tragt den einfach selber vorne im Setup-
 Dialog ein. Euer Provider unterst�tzt dann eben diese Anfrage nicht.
 

So, als n�chstes wird es dann nat�rlich die Loginscripts und das alles geben.
Das wird alles �ber ein GEM-Programm abgewickelt, eine vern�nftige Status-
Anzeige w�hrend der PPP-Konfiguration wird nat�rlich drin sein etc. Aber
erstmal mu� ja der Kern stimmen. Also schaut euch �ber PPP einfach ein paar
Seiten mit CAB an oder probiert ein paar der Testprogramme aus dem ersten
Blip-Archiv oder so. Ich bin auf eure Meldungen gespannt.

Gru�, Sven