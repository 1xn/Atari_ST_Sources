REM FILENAME = BILDO2.BAT FUNCTION = BUILD & FORMAT 3.5" DISKETTE
C:
C:\DSKFMT
CD SCRNDRV
COPY @OEM2.PRN A:
CD ..
REN A:INSTO2.BAT A:INSTALL.BAT
REM TRANSFER COMPLETED; ENTER THE APPLE KEY AND NUMERIC ENTER KEY TO
REM EJECT DISKETTE;
REM LABEL DISKETTE WITH A GREEN COLORED DISKETTE PAPER LABEL;
REM LABEL DISKETTE WITH A PRINTED LABEL FOR OEM , DISKETTE # 2
