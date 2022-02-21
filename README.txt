Program využívá principy steganografie, kdy se jeden Byte šifrovaného obrázku zakóduje na 
8 Bytů jiného obrázku (zde Weber) na 'Least significant bit' a tím nedojde k (vážnému) poškození 
obrázku, do kterého je zpráva kódována.

Příklad: 
1B z tajného obrázku: R    82(10)      52(16)      1010010(2)

8B z obrázku, do kterého šifrujeme:
10001001(2) -> 0 -> 10001000(2)
11011011(2) -> 1 -> 11011011(2)
01010101(2) -> 0 -> 01010100(2)
11111001(2) -> 0 -> 11111000(2)
11011010(2) -> 1 -> 11011011(2)
00101101(2) -> 0 -> 00101100(2)
11010110(2) -> 1 -> 11010111(2)
01110001(2) -> 0 -> 01110000(2)

Analýza zakryptování větších obrázků:
Všechny obrázky nejprve šifruji na LSB v obrázku, do kterého šifruji, pokud mi dojdou volné Byty a já se 
dostanu nakonec obrázku, do kterého šifruji, přesouvám se znovu na začátek a dále šifruji na druhý bit "zprava", 
takto stejně pokračuji pro všechny obrázky, dokud nejsou celé zašifrovány. Největší ze vzorových obrázku je
šifrován na 4 bity.

Všechny 'tajné' obrázky jsou nejprve šifrovány do obrázku Weber na LSB z každého Bytu, pokud dojde k využití 
všech volných Bytů, pak se program přesune opět na začátek obrázku Weber a pokračuje v šifrování na druhý nejméně
významný bit 'zprava'. Takto stejně se pokračuje pro všechny obrázky, které cchceme utajit. Největší z obrázků
v daném příkladu je šifrován na čtyři bity.

Překlad: 
gcc main.c -o main.exe -Wall -Wextra -pedantic

Použití programu:
1) pro zašifrování obrázku: .\main.exe weber.bmp dwarf_small.bmp

argv[1] = obrázek, do kterého šifrujeme, argv[2] = obrázek, který chceme zašifrovat.
Výstupem je soubor "merged_image.bmp", ve kterém je zašifrovaný tajný obrázek

2) pro rozšifrování obrázku: .\main.exe merged_image.bmp

argv[1] = obrázek, ve kterém je zašifrovaný tajný obrázek. Výstupem jsou dva
soubory: "obr1_separated.bmp", který obsahuje tajný obrázek, a "obr2_separated.bmp", který obsahuje obrázek,
do kterého se šifrovalo. Jeho bity, které se použily pro šifrování tajného obrázku, nejsou zpětně dohledatelné.
