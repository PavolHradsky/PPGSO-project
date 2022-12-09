# TODO list

 - [ ] práca s kamerou
 - [ ] svetelne zdroje
 - [ ] post-processing

## Hodnotenie projektu [43b]

- [4b] <b>3D objekty a mapovanie textúr na 3D geometriu</b>
  -  (2b) Objekty ako unikátne 3D polygonálne modely
  -  (2b) Unikátne textúrovanie pomocou UV koordinátov
- [9b] <b>Scény</b>
  -  (1b) Dynamická scéna s objektami, ktoré sú vytvárané a zanikajú v čase
  -  Aspoň 2 rôzne typy objektov, ktoré vznikajú-zanikajú 
  -  (2b) Procedurálne generovaná scéna
  -  Obmedzenia a lokácia objektov definovaná (ne-)deterministickým
algoritmom 

  - (2b) Zmena scény a viacero virtuálnych oblastí
  - Aspoň 2 rôzne scény, môžu byť riešené v rámci jednej sceny napr.
vonku-vnútri 

  - (4b) Hierarchická reprezentácia scény 
  - Každá scéna bude mať logické priestorové usporiadanie (podlaha,
pozadie, obloha, strop, steny...) implementované pomocou grafu
scény
  - Využitie hierarchickej transformácie v scéne
  - Aspoň 2 levely hierarchie medzi 3-mi objektmi
  - Použitie kompozície maticových transformácii

- [5b] <b>Animované objekty</b>
  - (2b) Procedurálna animácia
  - Uzavretá metóda s parametrami
  - Logické vetvenie
  - (3b) Animácia na základe dát uložených v kľúčových snímkoch a
interpolácie
  - Kľúčový snímok je reprezentovaný samostatnou dátovou
štruktúrou, ktorá uchováva transformácie objektu (min. pozícia a
natočenie) v čase

- [5b] <b>Simulácia</b>
  - (3b) Efektívna kolízia medzi objektami
  - Dynamická odozva na kolíziu, napr. odraz
  - (2b) Simulácia aspoň s dvoma silami s použitím vektorovej algebry
  - Napr. gravitácia + vietor, časticový systém

- [5b] <b>Práca s kamerou</b>
  - (1b) Kamera s perspektívnou projekciou
  - Animovaná kamera pomocou:
  - (2b) kľúčových snímkov, kde sa kamera nachádza a kam sa pozerá
  - (2b) riadená animačnými krivkami pre plynulý pohyb kamery

- [11b] <b>Osvetlenie za pomoci viacerých svetelných zdrojov</b>
  - (3b) Použitie všetkých typov zdrojov svetla: smerové, bodové, reflektor
  - (1b) Zmena pozície zdrojov svetla a odtieňov farby osvetlenia
  - (2b) Správne kombinovať difúzne svetlo z 2 zdrojov svetla s difúznymi
materiálmi
  - (2b) Správny Phongov osvetlovací model s viacerými (aspoň 3) zdrojmi
svetla, pričom musí byť splnené:
  - Správne tlmenie svetla na základe hĺbky
  - Použité aspoň tri zložky materiálu a tri zložky svetla
  - Správne kombinovať zložky materialu a zložky svetla
  - Tiene implementované

  - Jednoduchým spôsobom (1b) alebo pomocou shadow-maps (3b)
 
- [4b] <b>Post-processing</b> 
  - Zobrazenie scény cez vlastný framebuffer, pomocou ktorého sa aplikuje
post-processing filter: základný napr. grayscale (1b) alebo konvolučný (2b)
alebo pokročilý, napr. bloom (4b)