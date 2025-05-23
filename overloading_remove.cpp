/*struct YouTubeChannel {
    string Name;
    int SubscribersCount;

    YouTubeChannel(string name, int subscribersCount) {
        Name = name;
        SubscribersCount = subscribersCount;
    }
    bool operator==(const YouTubeChannel&channel) const{
        return this->Name == channel.Name;
    }
};
ostream& operator<<(ostream& COUT, YouTubeChannel& ytChannel) {
    COUT << "Name: " << ytChannel.Name << endl;
    COUT << "Subscribers: " << ytChannel.SubscribersCount << endl;
    return COUT;
}
struct MyCollection {
    list<YouTubeChannel>myChannels;

    void operator+=(YouTubeChannel& channel) {
        this->myChannels.push_back(channel);
    }
    void operator-=(YouTubeChannel& channel) {
        this->myChannels.remove(channel);
    }
};
ostream& operator<<(ostream& COUT, MyCollection& myCollection) {
    for (YouTubeChannel ytChannel : myCollection.myChannels)
        COUT << ytChannel << endl;
    return COUT;
}

int main()
{
    YouTubeChannel yt1 = YouTubeChannel("CodeBeauty", 75000);
    YouTubeChannel yt2 = YouTubeChannel("My second channel", 80000);

    MyCollection myCollection;
    myCollection += yt1;
    myCollection += yt2;
    myCollection -= yt2;

    cout << myCollection;

    cin.get();
}//*/
// Created by marac on 4/5/2025.
//
#include <iostream>
using namespace std;
class Meniu {
public:
    void afiseazaMeniu() {
        cout << "\n--- MENIU PRINCIPAL ---\n";
        cout << "1. Adauga animal\n";
        cout << "2. Listeaza animale\n";
        cout << "3. Creeaza dosar adoptie\n";
        cout << "4. Iesire\n";
        cout << "Alege o optiune: ";
    }

    void executa() {
        int optiune;
        do {
            afiseazaMeniu();
            cin >> optiune;
            switch (optiune) {
                case 1:
                    adaugaAnimal();
                break;
                case 2:
                    listeazaAnimale();
                break;
                case 3:
                    creeazaDosar();
                break;
                case 4:
                    cout << "La revedere!\n";
                break;
                default:
                    cout << "Optiune invalida!\n";
            }
        } while (optiune != 4);
    }

private:
    void adaugaAnimal() {
        cout << "[Functie adauga animal] \n";
        // aici apelezi metodele din clasele de gestiune (ex: Adapost)
    }

    void listeazaAnimale() {
        cout << "[Functie listeaza animale] \n";
    }

    void creeazaDosar() {
        cout << "[Functie creeaza dosar adoptie] \n";
    }
};

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.