PROIECT- AGENTIE DE TURISM

DESCRIEREA PROIECTULUI:
      Proiectul reprezintă o aplicație C++ orientată pe obiecte (OOP) care simulează funcționalitatea unei agenții de vacanțe, adresată atât vizitatorilor interesați să rezerve pachete turistice, cât și organizatorilor care gestionează și adaugă oferte noi. Aplicația este structurată modular, folosind clase, ierarhii, polimorfism, funcții virtuale, STL, smart pointers, operatori suprascrişi, upcasting/downcasting și template-uri.
      Aplicația este împărțită în două secțiuni mari, în funcție de tipul utilizatorului: Vizitator sau Organizator. În funcție de rolul ales la autentificare, utilizatorul va avea acces la un meniu personalizat.

1. Meniul Vizitatorului
Vizitatorii au acces la următoarele funcționalități:

Listarea tuturor pachetelor de vacanță
Afișează toate vacanțele disponibile în agenție, indiferent de categorie. Este util pentru a vedea rapid toate opțiunile existente.

Căutarea pachetelor după anumite criterii
Permite căutarea specifică, cum ar fi după destinație, preț, perioadă sau tipul vacanței.

Afișarea doar a vacanțelor de tip City Break la Munte
Filtrează vacanțele disponibile și le afișează doar pe cele care sunt la munte.

Afișarea doar a vacanțelor de tip City Break la Plajă
Similar cu opțiunea anterioară, dar se referă la vacanțele de tip plajă.

Afișarea croazierelor disponibile
Oferă vizitatorului o listă cu toate croazierele disponibile pentru rezervare.

Rezervarea unei vacanțe
Permite alegerea unui pachet și rezervarea unui bilet. Numărul de bilete este limitat, așa că rezervările se închid când toate locurile sunt ocupate.

Vizualizarea biletelor achiziționate
Afișează toate biletele pe care utilizatorul le-a rezervat, împreună cu detaliile acestora.

Sortarea vacanțelor în funcție de preț
Oferă o listă a vacanțelor disponibile ordonată crescător după preț, astfel încât utilizatorul să poată vedea cele mai accesibile oferte.

Anularea rezervării unui loc de transport
Utilizatorul poate renunța la un loc rezervat anterior (de exemplu, într-un avion sau autocar).

Logout
Utilizatorul este delogat și revine la meniul principal.

Ieșirea din aplicație
Închide aplicația și oprește complet execuția.

2. Meniul Organizatorului
Organizatorii sunt responsabili de gestionarea conținutului aplicației și au acces la următoarele opțiuni:

Adăugarea unui nou pachet de vacanță
Permite introducerea manuală a unei vacanțe noi: destinație, dată, preț, număr de bilete, tipul vacanței etc.

Listarea tuturor vacanțelor
Afișează toate vacanțele create până în acel moment, inclusiv cele fără bilete disponibile sau deja rezervate.

Logout
Deloghează organizatorul și revine la meniul principal.

Ieșirea din aplicație
Oprește complet aplicația.

    Funcționalități speciale pentru vizitatori:
Sistem de puncte: La fiecare bilet rezervat, vizitatorul primește 50 de puncte. La atingerea a 100 de puncte, primește automat un cadou surpriză.

Selecție transport: După ce rezervă o vacanță, utilizatorul poate selecta mijlocul de transport dorit, alege locuri specifice și verifica disponibilitatea acestora.

Cazare detaliată: După rezervarea unei vacanțe, utilizatorul poate alege cazarea și poate vedea detalii despre aceasta (ex. facilități, număr de stele).

Anularea locurilor de transport: Dacă se răzgândește, utilizatorul poate elibera locurile rezervate pentru transport.

Mini-joc "Cursa agențiilor de zbor": Un joc interactiv ce simulează o competiție între agențiile de zbor pentru a afla care oferă cel mai rapid avion.

Statistici disponibile:
Aplicația include o clasă dedicată statisticilor, care oferă:

Numărul total de bilete vândute

Cele mai populare destinații

Cele mai rezervate tipuri de vacanță

Numărul total de puncte acumulate de utilizatori

### Folosiți template-ul corespunzător grupei voastre!

| Laborant  | Link template                                |
|-----------|----------------------------------------------|
| Dragoș B  | https://github.com/Ionnier/oop-template      |
| Tiberiu M | https://github.com/MaximTiberiu/oop-template |
| Marius MC | https://github.com/mcmarius/oop-template     |

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.

## Cerințe obligatorii

Nerespectarea duce la nepunctarea proiectului

  - programul va fi scris în C++
  - programul va avea un meniu interactiv (doar pentru ilustrarea funcționalității)
  - programul nu are erori de compilare
  - fară variabile globale
  - datele membre private(sau protected)
  - GitHub Actions trecute
  - commit-uri pe Git adecvate si punctuale
  - folosirea a funcționalităților limbajului fără sens
  - folosirea a funcționlităților limbajului cu scopul de a încălca "legal" o altă regulă
      - folosirea excesivă a claselor friend
      - folosirea excesviă a elementelor statice
  - lipsa separarea implementarii de definitie

## Cerințe
- [ ] definirea a minim **2-3 ieararhii de clase** care sa interactioneze in cadrul temei alese (fie prin compunere, agregare sau doar sa apeleze metodele celeilalte intr-un mod logic) (6p)
  - minim o clasa cu:
    - [ ] constructori de inițializare [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] constructor supraîncărcat [*](https://github.com/Ionnier/poo/tree/main/labs/L02#supra%C3%AEnc%C4%83rcarea-func%C8%9Biilor)
    - [ ] constructori de copiere [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] `operator=` de copiere [*](https://github.com/Ionnier/poo/tree/main/labs/L02#supra%C3%AEnc%C4%83rcarea-operatorilor)
    - [ ] destructor [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] `operator<<` pentru afișare (std::ostream) [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L123)
    - [ ] `operator>>` pentru citire (std::istream) [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L128)
    - [ ] alt operator supraîncărcat ca funcție membră [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L32)
    - [ ] alt operator supraîncărcat ca funcție non-membră [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L39) - nu neaparat ca friend
  - in derivate
      - [ ] implementarea funcționalităților alese prin [upcast](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding) și [downcast](https://github.com/Ionnier/poo/tree/main/labs/L04#smarter-downcast-dynamic-cast)
        - aceasta va fi făcută prin **2-3** metode specifice temei alese
        - funcțiile pentru citire / afișare sau destructorul nu sunt incluse deși o să trebuiască să le implementați 
      - [ ] apelarea constructorului din clasa de bază din [constructori din derivate](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-la-derivare)
      - [ ] suprascris [cc](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-de-copiere-la-derivare)/op= pentru copieri/atribuiri corecte
      - [ ] destructor [virtual](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding)
  - pentru celelalte clase se va definii doar ce e nevoie
  - minim o ierarhie mai dezvoltata (cu 2-3 clase dintr-o clasa de baza)
  - ierarhie de clasa se considera si daca exista doar o clasa de bază însă care nu moștenește dintr-o clasă din altă ierarhie
- [ ] cât mai multe `const` [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#reminder-const-everywhere)
- [ ] funcții și atribute `static` (în clase) [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#static)
  - [ ] 1+ atribute statice non-triviale 
  - [ ] 1+ funcții statice non-triviale
- [ ] excepții [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#exception-handling)
  - porniți de la `std::exception`
  - ilustrați propagarea excepțiilor
  - ilustrati upcasting-ul în blocurile catch
  - minim folosit într-un loc în care tratarea erorilor în modurile clasice este mai dificilă
- [ ] folosirea unei clase abstracte [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#clase-abstracte)
 - [ ] clase template
   - [ ] crearea unei clase template [(1p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [ ] 2 instanțieri ale acestei clase (0.5p)
 - STL [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L07#stl)
   - [ ] utilizarea a două structuri (containere) diferite (vector, list sau orice alt container care e mai mult sau mai putin un array)
   - [ ] utilizarea a unui algoritm cu funcție lambda (de exemplu, sort, transform)
 - Design Patterns [(0.75p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [ ] utilizarea a două șabloane de proiectare

### Observații

* Pot exista depunctări până la 2p pentru diferite aspecte precum:
  - memory leak-uri
  - nefolosirea destructorului virtual la nevoie
  - abuzarea de diferite concepte (toate funcțiile declarate virtual)
  - apelarea de funcții virtual în constructori

* În general, acestea sunt prezente în [CppCoreGuideline](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md), dar nu e nevoie să parcurgeți documentul, doar să scrieți codul suficient de organizat

* folderele `build/` și `install_dir/` sunt adăugate în fișierul `.gitignore` deoarece
conțin fișiere generate și nu ne ajută să le versionăm.
