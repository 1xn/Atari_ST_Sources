DEFINITION MODULE StrConv;

(*
 * Umwandlungsfunktionen zw. Zahlen und Strings
 *)

FROM SYSTEM IMPORT LONGWORD, WORD;

FROM Strings IMPORT String;


PROCEDURE StrToLCard ( REF str: ARRAY OF CHAR; VAR pos: CARDINAL;
                       VAR valid : BOOLEAN ): LONGCARD;

PROCEDURE StrToCard  ( REF str: ARRAY OF CHAR; VAR pos: CARDINAL;
                       VAR valid: BOOLEAN ): CARDINAL;

PROCEDURE StrToLInt  ( REF str: ARRAY OF CHAR; VAR pos: CARDINAL;
                       VAR valid: BOOLEAN ): LONGINT;

PROCEDURE StrToInt   ( REF str: ARRAY OF CHAR; VAR pos: CARDINAL;
                       VAR valid: BOOLEAN ): INTEGER;

  (*
     Wenn 'valid' nach dem Aufruf FALSE ist, bedeutet das, da� der
     geforderte Wert nicht ermittelt werden konnte.

     'pos' ist vor dem Aufruf auf die Stringposition zu setzen, ab der
     die Zahl gelesen werden soll. Nach dem Aufruf liefert 'pos' die
     Position im String, hinter der die umgewandelte Zahl endete.
     Endet der String nach der Zahl, ist 'pos' = Length(str).
     In der Regel ist 'pos' also vor dem Aufruf auf Null zu setzen!

     F�r die vier StrTo...-Funktionen gilt:

     - f�hrende Blanks werden ignoriert, innerhalb der Zahl d�rfen
       dann keine Leerzeichen mehr vorkommen.
     - ein eventuell vorangestelltes '+' wird ignoriert.
     - Pr�fixe steuern die Interpretation der folgenden Zeichen:
       '$' f�r Hexadezimal, '%' f�r Bin�r, kein Pr�fix bedeutet
       dezimal.
     - Bei StrToInt bzw. StrToLInt mu� das '-' zuerst stehen, erst
       dann darf der Pr�fix folgen
       
     Beispiele: StrToCard ('  123 12') = 123
                StrToLInt ('-$80') = -128L
  *)

PROCEDURE StrToLNum (REF str   : ARRAY OF CHAR;
                         base  : CARDINAL; (* [2..36] *)
                     VAR pos   : CARDINAL;
                     VAR valid : BOOLEAN ): LONGCARD;

PROCEDURE StrToNum (REF str   : ARRAY OF CHAR;
                        base  : CARDINAL; (* [2..36] *)
                    VAR pos   : CARDINAL;
                    VAR valid : BOOLEAN ): CARDINAL;


PROCEDURE StrToReal ( REF str: ARRAY OF CHAR; VAR pos: CARDINAL;
                      VAR valid: BOOLEAN ): LONGREAL;

  (*
    StrToReal liefert den Wert einer reellen Zahl. Dabei darf entweder der
    Nachkommateil (einschlie�lich '.') oder der Vorkommateil fehlen. Ebenfalls
    braucht kein Exponent angegeben werden. Ein eventueller Skalierungsfaktor
    wird durch ein vorangestelltes 'E' erkannt und darf im Bereich von -1232
    bis +1232 liegen.

    Es gilt:
    
    - f�hrende Blanks werden ignoriert, innerhalb der Zahl d�rfen
      dann keine Leerzeichen mehr vorkommen.
    - ein eventuell vorangestelltes '+' wird ignoriert.
    - 'ThousandsChar' werden �berlesen.
    
    z.B. : StrToReal ('123E12') = 1.23 E14
           StrToReal ('.012E-23') = 1.2 E-25
  *)



(* ======================================================================= *
 * Es folgen Funktionen zur Umwandlung von Zahlen in Strings. Dabei gilt   *
 * allgemein:                                                              *
 * - 'spc' bestimmt die Mindestl�nge des resultierenden Strings 'str'.     *
 * ======================================================================= *)


PROCEDURE CardToStr ( v: LONGCARD; spc: CARDINAL): String;
PROCEDURE IntToStr  ( v: LONGINT;  spc: CARDINAL): String;

  (*
    Die ...ToStr-Funktionen wandeln eine Zahl in einen String um. Dabei steht
    ein eventuelles Minuszeichen direkt vor der ersten Ziffer. Falls die Zahl
    weniger Stellen (inklusive Vorzeichen) als in 'spc' angegeben haben sollte,
    so werden entsprechend viele Leerzeichen vorangestellt.
    
    z.B. : CardToStr (123,10) = '       123'
           IntToStr (-2345,2) = '-2345'
  *)

PROCEDURE LHexToStr ( v: LONGWORD; spc: CARDINAL): String;
PROCEDURE HexToStr  ( v: WORD;     spc: CARDINAL): String;
PROCEDURE LBinToStr ( v: LONGWORD; spc: CARDINAL): String;
PROCEDURE BinToStr  ( v: WORD;     spc: CARDINAL): String;

  (*
    Die Hex- und Bin-Funktionen erzeugen, wie der Name sagt, Strings in
    hexadezimaler bzw. bin�rer Repr�sentation. Es werden nur positive Zahlen
    erzeugt. Zu Beachten ist, da� den Zahlen dabei ein '$' bzw. '%' vorange-
    stellt wird und dieses Zeichen auch mit zur Mindestl�ngenangabe 'spc' z�hlt.
    Ggf. ist das vorangestellte Zeichen mit der Funktion 'Strings.Delete' zu
    entfernen. F�hrende Nullen werden nicht unterdr�ckt.
    
    z.B.: HexToStr (128,4)  = '$080'
          HexToStr (128,0)  = '$80'
          LHexToStr (-1L,0) = '$FFFFFFFF' (8 Ziffern)
          BinToStr (100,9)  = '%01100100'
          LBinToStr (-1L,1) = '%11111111111111111111111111111111' (32 Ziffern)
  *)

PROCEDURE LNumToStr (    lnum : LONGWORD;
                         base : CARDINAL; (* [2..36] *)
                         space: CARDINAL;
                         fill : CHAR    ): String;

PROCEDURE NumToStr (    num  : WORD;
                        base : CARDINAL; (* [2..36] *)
                        space: CARDINAL;
                        fill : CHAR    ): String;
  (*
   * 'fill' bestimmt das Zeichen, mit dem ggf. vorn aufgef�llt wird.
   *)

PROCEDURE FloatToStr ( v: LONGREAL; spc, n:CARDINAL): String;

  (*
    Durch diese Funktion wird die Zahl 'v' in wissenschaftlicher Notation dar-
    gestellt. Es wird eine Vorkommastelle und 'n' Nachkommastellen, gefolgt von
    'E', dem Vorzeichen des Skalierungsfaktors und dem vierstelligen Wert.
    Ggf. wird der String links mit Leerzeichen bis zur L�nge 'spc' aufgef�llt.
    
    z.B. : FloatToStr (0.0123,12,1) = '   1.2E-0002'
           FloatToStr (9.9,0,10) = '9.9000000000E+0000'
  *)

PROCEDURE FixToStr ( v: LONGREAL; spc, n:CARDINAL): String;

  (*
    Diese Funktion stellt 'v' in Festpunktnotation dar, wobei entsprechend viele
    Vorkommastellen, jedoch nur genau 'n' Nachkommastellen gebracht werden.
    Wenn der String k�rzer als 'spc' sein sollte, so werden entsprechend viele
    Leerzeichen vorangestellt.
    
    z.B. : FixToStr (0.0123,12,1) = '         0.0'
           FixToStr (9.9,0,10) = '9.9000000000'
  *)

PROCEDURE RealToStr( v: LONGREAL; spc, n: CARDINAL): String;

  (*
    Diese Prozedur verwendet je nach Lage des Wertes 'v' innerhalb der Werte
    von FixToFloatMin und FixToFloatMax (aus 'MOSConfig') das Festkomma- oder
    Flie�komma-Format zur Darstellung von 'v'.
  *)
  
PROCEDURE EngToStr ( v: LONGREAL; spc, n:CARDINAL): String;

  (*
    Durch diese Funktion wird die Zahl 'v' in Ingenieur-Notation dargestellt.
    Es werden bis zu drei Vorkommastellen und 'n' Nachkommastellen, gefolgt
    von 'E', dem Vorzeichen des Skalierungsfaktors und dem vierstelligen Wert,
    der immer ein Vielfaches von Drei ist. Ggf. wird der String links mit Leer-
    zeichen bis zur L�nge 'spc' aufgef�llt.
    
    z.B. : EngToStr (0.0123,12,1) = '  12.3E-0003'
           EngToStr (9.9,0,10) = '9.9000000000E+0000'
  *)

END StrConv.
