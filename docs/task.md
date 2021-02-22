# Feladat

A félév során egy szimulációs programot kell megvalósítani 3 fős csapatban. A szimulációs programnak egy raktárban dolgozó szállító robotok útvonalát kell megterveznie és a tervet végrehajtania.

Egy raktár tipikus alaprajza:

A raktár négyzethálóba van szervezve. Vannak benne akkumulátoros robotok (R<sub>i</sub>), célállomások (S<sub>i</sub>), dokkolónak nevezett töltőállomások (D<sub>i</sub>), és pod-nak nevezett állványok (P). A robotok akkumulátorának van egy maximális töltöttségi állapota, ami egy egész szám. Az állványokon termékek vannak, amit a fenti ábrán az állványra írt számok jelölnek. Egy állványon minden termékszámból legfeljebb egy szerepelhet. A termékszámok a célállomás számok közül kerülnek ki. A robotok feladata, hogy a termékeket az azonos számú célállomásokhoz vigyék (1-es termék az S<sub>1</sub> célállomáshoz, 2-es az S<sub>2</sub>-höz, stb.).

Egy robot úgy tud egy terméket elvinni, hogy az állvány alá áll, megemeli az állványt, majd utána az állvánnyal együtt mozog. Egy adott számú termék akkor kerül a célállomásra, ha a robot ráállt az állvánnyal azonos számú célállomásra, majd a terméket leadja, és ezzel a termék eltűnik az állványról. A termék(ek) célállomásra szállítása után a robotoknak az állványt vissza kell vinniük az eredeti helyükre.

A robotnak be kell tartania a robotmozgás szabályait:

- A robotok az állványok alatt el tudnak menni.
- Egy négyzetben sohasem lehet egyszerre két robot.
- Egy négyzetben sohasem lehet egyszerre két állvány.
- A robotok állvánnyal nem állhatnak töltőállomásra.
- Mindig gondoskodni kell arról, hogy a robot az akkumulátorának lemerülése előtt el tudjon jutni egy töltőállomásra.

A szimuláció lépésekben történik. Egy központi vezérlő jelöli ki, hogy melyik robot melyik terméket vigye el a célállomásra. Tervezéstől függően, a robotok vagy önállóan dolgoznak, vagy egy központi vezérlő mondja meg, hogy melyik lépésben milyen műveletet végezzenek. A központi vezérlő és a robotok is az egész raktár állapotát ismerik. Minden lépésben minden robot végezhet egy műveletet, ami (a termék leadás műveletét leszámítva) eggyel csökkenti az akkumulátor töltöttségi állapotát. A lehetséges műveletek:

- kilencven fokos fordulat jobbra vagy balra,
- átlépés a menetirány szerinti él-szomszédos négyzetbe (végrehajtható, ha induláskor a cél négyzetben nincs se robot, se fal, és betartja a robotmozgás szabályait)
- állvány megemelése (végrehajtható, ha az állvánnyal egy mezőben áll, és innentől együtt mozognak),
- állvány letevése (innentől az állvány nem mozdul, csak a robot mozoghat),
- termék leadása (sikeres, ha megfelelő célállomáson áll, ez a művelet nem csökkenti az akkumulátor töltöttségét),
- akkumulátor töltésének elindítása (végrehajtható, ha töltőállomáson áll, és akkor utána még öt lépésig a robot nem mozdulhat a töltőállomásról, ami alatt teljesen feltöltődik az akkumulátora).

A szimuláció indításához meg kell adni:

- a raktár elrendezését (méretek, állványok, célállomások, dokkolók helye)
- robotok száma és helye, maximális töltöttség (tegyük fel, hogy kezdetben minden robot teljesen feltöltött)
- kiszállítandó termékek helye (melyik állványon vannak)
- (esetleg egyéb konfigurációs adatok, amik segíthetik a megoldást)

Az állítható sebességű szimuláció során szeretnénk látni a raktár alaprajzát, a robotok mozgását, a robotok telítettségi állapotát, a termékeket, az állványokat és mozgásukat.

A szimuláció végén egy napló fájlban meg akarjuk kapni:

- hány lépésig tartott a teljes feladat végrehajtása,
- minden egyes robotra, hogy összesen mennyi energiát fogyasztott,
- összesen mennyi energia kellett a feladat végrehajtásához.

Minden számonkérés maximum 30 pontig lesz pontozva (4 * 30 = 120 pont). Mindegyik beszámolónál legalább 10 pontot el kell érni. Ezenkívül az alábbiakra további maximum 30 pontot lehet kapni. Alapvetően a végső jegy az összes pontszám huszonötöd részének az egészrése. Az elégséges feltétele az alap verzió.

## Részfeladatok

### Alap (elégségeshez kell):
Szövegfájlban lehet megadni a szimuláció indításához szükséges adatokat, a robotok minimális szinten tudják kezelni a konfliktusokat, a robotok elszállítják a termékeket a célállomásokhoz, statisztika készül.

### Input adatok szerkesztése (5 pont):
Egy külön szerkesztővel lehet létrehozni a szimuláció indításához szükséges adatokat. A szerkesztett adatok elmenthetők és betölthetők.

### Kifinomult szerkesztő (5 pont):
A raktár szerkesztő lehetővé teszi, hogy kijelöljünk polcokat és együtt áthelyezzük őket. Kijelölt polcokon egyszerre ugyanazokat a termékeket helyezhetjük el.

### Termékek robotokhoz rendelése kifinomult (5 pont):
Optimalizálás történik.

### Online megrendelések (5 pont):
Kiszállítandó termékeket a szimuláció futása közben interaktív módon is ki lehet jelölni a polcokon. Esetleg az input fájlban van megadva, hogy mikor jelenjenek meg új megrendelések.

### Útvonal keresés (5 pont):
Kifinomultabb algoritmust használ, esetleg a konfliktusok elkerülésére is figyel.

### Hálózati megvalósítás (15 pont):
Legyen lehetőség arra, hogy a szimuláció sebességének állítását, megállítását (és ha van, akkor az online megrendeléseket) egy kliens számítógépen lehessen állítani. A szimuláció egy másik gépen fut.