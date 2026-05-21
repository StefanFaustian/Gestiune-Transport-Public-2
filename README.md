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

# **Review**

Proiectul reprezintă o aplicație software utilă pentru managementul flotelor de transport public (Sistem de Gestiune a Transportului Public). Aplicația modelează interacțiunile dintr-o companie de transport (STB SA), gestionând entități precum vehicule (Autobuze, Tramvaie, Troleibuze), depouri și linii de transport. Prin intermediul unui meniu interactiv, sistemul permite adăugarea și eliminarea vehiculelor din inventar, alocarea lor pe trasee specifice în funcție de compatibilitate, simularea uzurii kilometrice zilnice și generarea rapoartelor pentru mentenanță urgentă.

În ceea ce privește organizarea codului, există o separare clară între interfețe și implementare: clasele sunt declarate în fișiere header (_.h_), în timp ce logica și corpul metodelor se regăsesc în fișierele sursă (_.cpp_). De exemplu, fișierul _Exceptii.h_ conține toate declarațiile excepțiilor personalizate într-o structură de tip header-only, adaptată pentru clasele de erori, care au implementări minimale direct în corpul declarației, toate clasele de erori fiind derivate din _std::runtime_error_.

Ierarhia aplicației este construită în jurul clasei de bază abstracte _Vehicul_, derivându-se în _Autobuz_, _Tramvai_ și _Troleibuz_. _Vehicul_ definește o interfață comună, fiind o clasă pur abstractă datorită metodelor virtuale pure precum _clone()_, _afisare()_ sau metoda _necesitaRevizie()_. Polimorfismul la runtime este utilizat în acest context: deși managerul cunoaște doar o flotă de pointeri inteligenți la _Vehicul_ (_std::shared_ptr&lt;Vehicul&gt;_), apelul metodei _necesitaRevizie()_ va declanșa testarea criteriilor specifice fiecărei derivate (de exemplu: 2500 km pentru un tramvai, versus 1500 km pentru un autobuz electric). Alocarea vehiculelor pe linii se face într-un mod sigur: metoda _alocaVehiculPeLinie_ folosește operatorul _dynamic_cast_ pentru a filtra la runtime compatibilitatea. Astfel, sistemul aruncă o excepție imediată dacă un dispecer încearcă să asigneze un troleibuz pe o linie de tramvai.

Asigurarea flexibilității codului este dată de implementarea programării generice prin transformarea clasei _Depou_ într-un șablon (_Depou&lt;T&gt;_). Acest pas nu a fost o simplă adăugare a sintaxei _&lt;T&gt;_, ci a implicat o remodelare arhitecturală care permite reutilizarea codului. Atributul care stochează flota a fost adaptat pentru a fi dependent de parametrul generic, devenind un vector de smart pointers _std::vector&lt;std::shared_ptr<T&gt;> vehicule;_. Metodele de interogare și manipulare, precum _adaugaVehicul_ și _cautaVehicul_, funcționează acum adaptate la tipul cu care este instanțiat depoul.

Mai departe, proiectul abordează și funcțiile libere șablon (_template_), integrând prietenia (_friend_) în mediul de lucru. Supraincărcarea operatorului de afișare _operator<<_ și funcția de interschimbare _swap_ au fost declarate și implementate ca funcții șablon care acceptă un argument de tip _Depou&lt;T&gt;_. Soluția aleasă pentru evitarea erorilor de linker (_undefined reference_) este de ordin practic: a fost realizată separarea fișierelor _.h_ și _.cpp_, urmată de o instanțiere explicită (_template class Depou&lt;Vehicul&gt;;_) la finalul fișierului _Depou.cpp_. Această tehnică a fost aleasă pe baza modului în care compilatorul C++ traduce șabloanele în cod binar, menținând în acest fel un timp de compilare optim și un sistem de build stabil. Astfel, aplicația creează cu ușurință un depou prototip _Depou&lt;Vehicul&gt;_, urmând să folosească polimorfismul pentru a găzdui orice derivată (autobuz, tramvai sau troleibuz) în interiorul acestuia.

Arhitectura aplicației are ca obiectiv principal integrarea a trei modele de proiectare distincte, fiecare rezolvând o problemă specifică și respectând principiile SOLID:

- **Factory:** S-a constatat că delegarea responsabilității de creare a vehiculelor direct către clasa _Manager_ (prin instrucțiuni multiple de tip _if-else_ la citirea din fișier) încălca principiul responsabilității unice (_Single Responsibility Principle_). Prin introducerea clasei _VehiculFactory_, a fost separată operațiunea de analiză a fișierelor de procesul de instanțiere. Metoda statică _creeazaVehicul_ preia datele parsate (tip vehicul, număr de înmatriculare, capacitate, atribute specifice) și generează instantaneu un obiect (_shared pointer_) corespunzător tipului cerut (_Autobuz_, _Tramvai_ sau _Troleibuz_). Acest design respectă principiul _Open-Closed_: adăugarea unui nou tip de vehicul pe viitor necesită modificări doar în _Factory_, lăsând clasa _Manager_ neatinsă.
- **Singleton:** Din moment ce entitatea _Manager_ reprezintă dispeceratul unic al companiei de transport, sistemul trebuia să blocheze instanțierea multiplă, accidentală, a mai multor manageri care ar putea duce la entități concurente invalide. Prin ascunderea constructorului (mutat în zona _private_) și ștergerea completă a metodelor de copiere și atribuire (_Manager(const Manager&) = delete_), aplicația expune singura instanță permisă printr-o funcție statică _getInstance()_. Acest singleton este utilizat la pornirea programului în _main()_, fiind ulterior transmis ca referință către interfața vizuală (_Meniu_).
- **Builder:** Pentru a eficientiza experiența citirii interactive de la tastatură, care era inițial dezordonată și se baza pe constructori înghesuiți în aceeași clasă, aplicația apelează la o clasă utilitară _VehiculBuilder_. Spre deosebire de _Factory_ (care este _stateless_ - nu păstrează date între apeluri, obiectul e creat instant - și decizional la momentul T), _Builder-ul_ este un instrument _stateful_ - obiectul e creat pe componente, informațiile sunt memorate - colectând iterativ datele introduse de dispecer în _Meniu::trateazaAdaugareVehicul_. Prin înlănțuirea metodelor (_method chaining_ care returnează _\*this_), codul din zona meniului capătă un aspect mai lizibil și aerisit (de exemplu: _.setTip().setNr().setCapacitate()_). Scopul arhitecturii este dat de metoda finală _build()_, care analizează setul de variabile interne stocate temporar și inițiază alocarea corespunzătoare a clasei derivate (_Autobuz_/_Tramvai_/_Troleibuz_), construind astfel obiecte într-un mod polimorfic.

Gestiunea erorilor este implementată folosind ierarhii de excepții derivate din clasa standard _std::runtime_error_. Clasa de bază _EroareGenerala_ permite capturarea unificată a problemelor. În proiect se identifică clar posibilele greșeli (de exemplu: _EroareCapacitateDepou_, _EroareValidareVehicul_ sau _EroareFisier_) și le aruncă acolo unde este relevant din punctul de vedere al logicii aplicației. De menționat este utilizarea expresiilor regulate (_std::regex_) în constructorii entităților derivate, validând rapid formatele numerelor de înmatriculare specifice (de exemplu: B123XYZ pentru autobuze vs B1234 pentru tramvaie/troleibuze) și ridicând excepții specifice imediat ce o dată preluată nu trece testele. Riscul scurgerilor de memorie (_memory leaks_) este evitat cât este posibil prin utilizarea pointerilor inteligenți (_smart pointers_) _std::shared_ptr_. Flota din interiorul depourilor este eliberată automat odată ce instanțele sunt distruse la finalizarea execuției.

Utilizarea cuvântului cheie _const_ se regăsește uniform în întregul cod. Funcțiile de inspectare, afișările, dar și interogările legate de status (cum ar fi _necesitaRevizie()_) sunt marcate cu _const_. Șirurile de caractere și obiectele de mari dimensiuni sunt manipulate eficient și sigur, fiind transferate spre funcții prin referințe constante (_const std::string&_).

Majoritatea funcțiilor de clase nu expun variabilele interne prin apeluri de _getter_/_setter_ triviale, ci pun la dispoziție funcționalități cu sens la nivel înalt. Exemple concrete apar în operațiuni complexe precum _incheieZiuaDeLucru()_ în clasa _Manager_, care se ocupă intern de înmulțirea ciclică a kilometrajului (_kmTotalNou = distantaTurRetur \* CICLURI_PE_ZI + KM_ACCES_) și efectuarea automată a actualizărilor pentru toate entitățile de pe traseu, fără a implementa logica în _main_.

Deși proiectul și-a atins scopul pentru respectarea cerințelor, se pot identifica arii minore unde optimizările de performanță ar putea fi consolidate. Spre exemplu, algoritmul din funcția _Manager::gasesteVehiculGlobal_ realizează o traversare secvențială (O(N \* M)) a întregii liste de depouri și a flotelor acestora, pentru a găsi un număr de înmatriculare. Pe viitor, această complexitate ar putea fi redusă la O(1) dacă _Managerul_ ar folosi, în paralel cu lista generală _numereInmatriculate_, un _map_ global (_std::unordered_map&lt;std::string, std::shared_ptr<Vehicul&gt;>_) cu toate vehiculele înregistrate în sistem, independent de depoul de care aparțin. Totodată, adăugarea unui nou atribut specific în cadrul vreunui tip de vehicul ar aduce probleme majore la funcționarea programului, acesta fiind proiectat să funcționeze pentru cazul în care fiecare vehicul are un singur atribut specific. La nivel conceptual, pentru a trata cazul cu mai multe atribute specifice, ar putea fi introdus un vector de atribute specifice, fapt ce ar necesita modificări aduse la toate nivelurile, de la ierarhia claselor la citirea din fișier și tratarea excepțiilor. Așadar, având în vedere că scopul proiectului este aplicarea anumitor concepte de POO, a fost aleasă o variantă de implementare simplă, dar cuprinzătoare.


## Resurse extra
1. https://www.geeksforgeeks.org/cpp/stdfind_if-stdfind_if_not-in-c/ - pentru funcția ```std::find_if()```
2. https://www.w3schools.com/cpp/cpp_functions_lambda.asp - pentru funcțiile lambda
3. https://www.geeksforgeeks.org/cpp/regex-regular-expression-in-c/ - pentru validări cu expresii regulate (RegEx)
4. https://www.geeksforgeeks.org/cpp/iomanip-in-cpp/ - pentru afișări mai ordonate (ex. ```std::setfill()``` sau ```std::setw()```)

