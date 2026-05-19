# Aplicatie de gestiune a transportului public

O companie de transport public dintr-un oraș are nevoie de un sistem de gestiune a vehiculelor și rutelor de transport pe care le are de administrat. Astfel, aplicația
cuprinde o clasă vehicule care au un ID unic (generat automat), o capacitate maximă de pasageri, un număr natural de km parcurși, un număr natural care memorează numărul de km la care s-a efectuat
ultima revizie și un număr de înmatriculare. Aceste vehicule se împart în trei categorii:
* Autobuze, care au ca atribut specific tipul motorul (diesel sau electric).
* Tramvaie, care au ca atribut specific numărul de vagoane (având constrângerea de maximum 4 vagoane).
* Troleibuze, cau au ca atribut specific o baterie auxiliară care poate fi opțional.

Autobuzele vor avea un format specific pentru numărul de înmatriculare (ex. B123XYZ), spre deosebire de tramvaie și troleibuze (ex. B1234).

Compania depozitează aceste vehicule în depouri care au un ID unic (generat automat), un nume consacrat și număr natural pentru capacitatea maximă. De la depouri, vehiculele pleacă pe linii definite
printr-un indicativ. Pe fiecare linie poate fi asignat un singur tip de vehicul (autobuz/tramvai/troleibuz). O linie conține un număr variabil de stații unice, iar între stații există o distanță (număr natural) cunoscută.
În momentul în care începe ziua de lucru, dispeceratul alocă vehiculele disponibile din depouri pe liniile de transport, respectând compatibilitatea tipului de vehicul. La încheierea zilei de lucru, fiecare vehicul de pe traseu acumulează un număr de kilometri, calculat pe baza distanței tur-retur a liniei sale înmulțită cu numărul de curse zilnice, plus o distanță de acces de la depou.

Această uzură continuă dictează necesitatea trimiterii în service. Odată ce un vehicul depășește un anumit prag de kilometri parcurși de la ultima revizie, acesta este raportat ca având nevoie de revizie urgentă. Pragul de uzură este specific fiecărei categorii (valorile furnizate sunt pur ilustrative):
* Autobuzele au nevoie de revizie după 1500km dacă au motor electric și 1000km în caz contrar.
* Tramvaiele au nevoie de revizie după 2500km de rulare.
* Troleibuzele au nevoie de revizie după 1800km dacă au baterie și 2000km în caz contrar.

Evident, nu pot exista duplicate între entități, deci nu vor exista vehicule cu același număr de înmatriculare, depouri cu același nume, linii cu același indicativ sau stații cu același nume.
O clasă ```Manager``` se ocupă cu majoritatea operațiunilor din companie, cum ar fi încărcarea depourilor, flotei specifice fiecărui depou și liniilor din fișiere, având grijă de validarea datelor
(de exemplu, ține evidența numerelor de înmatriculare deja existente în depouri). 

Aplicația ilustrează următoarele funcționalități printr-un meniu interactiv:

* adăugarea de vehicule noi în depouri (preluând detaliile specifice fiecărei categorii).
* eliminarea (casarea) unui vehicul din inventarul companiei.
* afișarea inventarului curent detaliat (lista depourilor, a flotei aferente și a liniilor de transport).
* simularea începerii unei zile de lucru prin alocarea manuală a vehiculelor din depouri pe liniile de transport.
* simularea încheierii zilei de lucru, care atrage după sine actualizarea automată a kilometrajului pentru toate vehiculele aflate pe traseu.
* afișarea tuturor vehiculelor care necesită revizie urgentă (determinată în mod polimorfic în funcție de tipul și atributele fiecărui vehicul).
* efectuarea operațiunilor de mentenanță prin trimiterea unui vehicul defect în service (operațiune ce resetează contorul de kilometri pentru revizie la valoarea kilometrajului actual).
* tratarea excepțiilor (ex: format incorect pentru numerele de înmatriculare, număr de vagoane peste limita admisă, verificarea duplicatelor).

Clasa `Depou` a fost aleasă pentru a fi implementată ca o clasă șablon (template), având ca motivație faptul că aceasta este singura clasă de tip colecție care este suficient de izolată pentru a fi transformată în template fără a distruge polimorfismul sau ierarhia clasei `Vehicul`.
Din acest moment, funcțiile `friend` definite anterior (`swap` și supraîncărcarea operatorului `<<`) vor deveni la rândul lor de tip template.

Au fost abordate trei tipuri de *Design Patterns*: 
* **Factory** - prin intermediul clasei `VehiculFactory` a fost preluată responsabilitatea de a analiza datele citite din fișier de la `Manager`, vehiculele sunt create de clasa delegată, Managerul având acum unicul rol de a insera vehiculul creat către depoul corespunzător.
* **Singleton** - clasa `Manager` devine singleton, deci constructorul ei rămâne privat, iar instanțierea se realizează o singură dată prin metoda statică `getInstance()`.
* **Builder** - folosit pentru adăugarea vehiculelor prin intermediul meniului interactiv. Clasa `VehiculBuilder` permite stocarea datelor despre un vehicul, acesta fiind asamblat etapizat prin apeluri înlănțuite, construirea fiind realizată polimorfic la apelul metodei `build()`.

## Resurse extra
1. https://www.geeksforgeeks.org/cpp/stdfind_if-stdfind_if_not-in-c/ - pentru funcția ```std::find_if()```
2. https://www.w3schools.com/cpp/cpp_functions_lambda.asp - pentru funcțiile lambda
3. https://www.geeksforgeeks.org/cpp/regex-regular-expression-in-c/ - pentru validări cu expresii regulate (RegEx)
4. https://www.geeksforgeeks.org/cpp/iomanip-in-cpp/ - pentru afișări mai ordonate (ex. ```std::setfill()``` sau ```std::setw()```)

