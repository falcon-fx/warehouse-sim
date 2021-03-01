<table>
    <tr>
        <th>AS A</th>
        <td>felhasználó</td>
    </tr>
    <tr>
        <th>I WANT TO</th>
        <td>elindítani a szimulációt</td>
    </tr>
    <tr>
        <th rowspan="3">1</th>
        <th>GIVEN</th>
        <td>a Start gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>van létrehozott/betöltött kezdőállapot</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program elindítja a szimulációt</td>
    </tr>
    <tr>
        <th rowspan="3">2</th>
        <th>GIVEN</th>
        <td>a Start gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>nincsen létrehozott/betöltött kezdőállapot</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program dialógusablakban szólítja fel a felhasználót kezdőállapot létrehozására/betöltésére</td>
    </tr>
</table>

<table>
    <tr>
        <th>AS A</th>
        <td>felhasználó</td>
    </tr>
    <tr>
        <th>I WANT TO</th>
        <td>betölteni egy kezdőállapotot</td>
    </tr>
    <tr>
        <th rowspan="3">1</th>
        <th>GIVEN</th>
        <td>a Betöltés gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadjuk a mentett állapotot tartalmazó fájlt</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program betölti a kért fájlból az állapotot és visszatér a főképernyőre</td>
    </tr>
    <tr>
        <th rowspan="3">2</th>
        <th>GIVEN</th>
        <td>a Betöltés gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadunk egy érvénytelen fájlt</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program dialógusablakban jelzi a betöltés sikertelenségét, majd újra lehetőséget nyújt a fájl megadására</td>
    </tr>
</table>

<table>
    <tr>
        <th>AS A</th>
        <td>felhasználó</td>
    </tr>
    <tr>
        <th>I WANT TO</th>
        <td>elmenteni egy állapotot</td>
    </tr>
    <tr>
        <th rowspan="3">1</th>
        <th>GIVEN</th>
        <td>a Mentés gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadjuk a fájl nevét</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program elmenteni az adatokat a kért fájlba</td>
    </tr>
</table>

<table>
    <tr>
        <th>AS A</th>
        <td>felhasználó</td>
    </tr>
    <tr>
        <th>I WANT TO</th>
        <td>létrehozni egy új állapotot</td>
    </tr>
    <tr>
        <th rowspan="3">1</th>
        <th>GIVEN</th>
        <td>a Szerkesztő megnyitása gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>a szimuláció fut</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a szimuláció szünetel és megnyílik a szerkesztő</td>
    </tr>
    <tr>
        <th rowspan="3">2</th>
        <th>GIVEN</th>
        <td>a Szerkesztő megnyitása gombra kattintunk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>a szimuláció szünetel</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a szimuláció továbbra is szünetel és megnyílik a szerkesztő</td>
    </tr>
    <tr>
        <th rowspan="3">3</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új robot menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvényes mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új robot megjelenik</td>
    </tr>
    <tr>
        <th rowspan="3">4</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új robot menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvénytelen mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új robot nem jelenik meg</td>
    </tr>
    <tr>
        <th rowspan="3">3</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új polc menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvényes mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új polc megjelenik</td>
    </tr>
    <tr>
        <th rowspan="3">4</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új polc menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvénytelen mezőre kattintva</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új polc nem jelenik meg</td>
    </tr>
    <tr>
        <th rowspan="3">5</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új célállomás menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadjuk a 0...2^32-1 közötti termék számot, érvényes mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új célállomás megjelenik</td>
    </tr>
    <tr>
        <th rowspan="3">6</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új célállomás menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>negatív számot adunk meg, érvényes mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új célállomás nem jelenik meg</td>
    </tr>
    <tr>
        <th rowspan="3">7</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új célállomás menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvénytelen mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új célállomás nem jelenik meg</td>
    </tr>
    <tr>
        <th rowspan="3">8</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új töltőállomás menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvényes mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új töltőállomás megjelenik</td>
    </tr>
    <tr>
        <th rowspan="3">9</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új töltőállomás menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>érvénytelen mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>az új töltőállomás nem jelenik meg</td>
    </tr>
    <tr>
        <th rowspan="3">10</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új termék menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadjuk a 0...2^32-1 közötti termék számot, polcot tartalmazó mezőre kattintunk, amelyen nincsen még a megadott számú termék</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a kijelölt polcon megjelenik a kívánt termék</td>
    </tr>
    <tr>
        <th rowspan="3">11</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új termék menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadjuk a 0...2^32-1 közötti termék számot, polcot tartalmazó mezőre kattintunk, amelyen már megtalálható a megadott számú termék</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a kijelölt polcon nem jelenik meg újra a kívánt termék</td>
    </tr>
    <tr>
        <th rowspan="3">12</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új termék menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>negatív termék számot adunk meg</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program dialogúsablakban jelzi, hogy érvénytelen számot kapott, és új számot kér be</td>
    </tr>
    <tr>
        <th rowspan="3">13</th>
        <th>GIVEN</th>
        <td>a Szerkesztő nyitva van, az Új termék menüpontot választottuk</td>
    </tr>
    <tr>
        <th>WHEN</th>
        <td>megadjuk a 0...2^32-1 közötti termék számot, nem polcot tartalmazó mezőre kattintunk</td>
    </tr>
    <tr>
        <th>THEN</th>
        <td>a program dialogúsablakban jelzi, hogy rossz mezőt választottunk, újra lehetőséget ad a választásra</td>
    </tr>
</table>