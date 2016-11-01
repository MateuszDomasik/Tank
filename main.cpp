#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>


using namespace std;
using namespace sf;

//RenderWindow window(VideoMode(1000,800),"Okienko",Style::Fullscreen);
sf::RenderWindow window(sf::VideoMode(800, 600), "Okno!");

class Kolizja
{
public:
    vector <Vector2f> doKol;

    Kolizja()
    {
        Vector2f poz;
        poz.x=0;
        poz.y=0;
        doKol.push_back(poz);
    }

    bool kolRamka(sf::Vector2f pozycja, int co)
    {
        if((pozycja.x) < 50 && co == 1)
            return true;
        else if(((pozycja.x)) > 715 && co == 2)
            return true;
        else if((pozycja.y) > 525 && co == 3)
            return true;
        else if(pozycja.y < 63 && co == 4)
            return true;
        else return false;
    }

    void usunKolizje(int i)
    {
        cout << "tu jestem";
        doKol[0] = doKol[i];
        doKol[i]=doKol[doKol.size()-1];
        doKol[doKol.size()-1] = doKol[0];

        doKol.pop_back();
    }

    void przesunPociskKolizja(sf::Vector2f pozycja, int ktory)
    {
        for(int i = doKol.size()-1; i > 0; --i)
        {
            cout << "tu jestem";
            if(ktory == i)
            {
                doKol[i] = pozycja;
                cout << "dodane pociski do kolizji: " << pozycja.x << ", "<< pozycja.y << endl;
            }
        }
    }

    void dodajDoKolizji(sf::Vector2f pozycja)
    {
        doKol.push_back(pozycja);
        cout << "dodane pociski do kolizji: " << pozycja.x << ", "<< pozycja.y << endl;
    }

    void kolOtoczenie(sf::Vector2f pozycja)
    {
        //cout << "pozycja przeciwnik: " << pozycja.x << ", " << pozycja.y << endl;
        //cout << "..............rozmiar doKol " << doKol.size()-1<<endl;
        for(int i = doKol.size()-1; i > 0; --i)
        {
            //if(doKol[i].y > pozycja.y){
                cout << "KOLIZJA " << endl; //}
        }
    }
};

enum Ruch {UP, DOWN, LEFT, RIGHT, STOP, FIRE} ruch=STOP;
enum RuchP {UPP, DOWNP, LEFTP, RIGHTP, STOPP, FIREP} ruchP=STOPP;

class Pocisk        ////////////////////////////<<<<<   Pocisk   >>>>>
{
    float predkosc = 20.f;
    sf::Vector2f przesuniecie;
    int strona=0;
    Kolizja kol;

    sf::Sprite pocisk;
public:
    int k =0;
    bool czyUsun = false;

public:

    Pocisk(Vector2f gdzie, int Strona,Texture *tekstura)
    {
        pocisk.setTexture(*tekstura);

        strona = Strona;

        if(strona == 1)
        {
            pocisk.rotate(270.f);
            pocisk.setPosition(gdzie.x-20, gdzie.y+30);
        }
        if(strona == 2)
        {
            pocisk.rotate(90.f);
            pocisk.setPosition(gdzie.x+60, gdzie.y+10);
        }
        if(strona == 3)
        {
            pocisk.rotate(180.f);
            pocisk.setPosition(gdzie.x+25, gdzie.y+60);
        }
        if(strona == 4)
        {
            pocisk.setPosition(gdzie.x+10, gdzie.y-30);
        }
    }

    ~Pocisk()
    {
        //kol.usunKolizje(k);
    }

    void przesun(sf::Time czas, int ktory)
    {
        if(strona == 4 && kol.kolRamka(pocisk.getPosition(),4) == false)
        {
            przesuniecie.y = -10.f;
            przesuniecie.x=0.f;
            pocisk.move(przesuniecie * czas.asSeconds() * predkosc);

        }
        else if(strona == 4) czyUsun = true;
        if(strona == 3 && kol.kolRamka(pocisk.getPosition(),3) == false)
        {
            przesuniecie.y = 10.f;
            przesuniecie.x=0.f;
            pocisk.move(przesuniecie * czas.asSeconds() * predkosc);

        }
        else if(strona == 3) czyUsun = true;
        if(strona == 1 && kol.kolRamka(pocisk.getPosition(),1) == false)
        {
            przesuniecie.x = -10.f;
            przesuniecie.y=0.f;
            pocisk.move(przesuniecie * czas.asSeconds() * predkosc);

        }
        else if(strona == 1) czyUsun = true;
        if(strona == 2 && kol.kolRamka(pocisk.getPosition(),2) == false)
        {
            przesuniecie.x = 10.f;
            przesuniecie.y=0.f;
            pocisk.move(przesuniecie * czas.asSeconds() * predkosc);
        }
        else if(strona == 2) czyUsun = true;

        kol.przesunPociskKolizja(pocisk.getPosition(),ktory);
        k=ktory;
        //cout << "Przesuniecie pocisku podane do kolizji: " << przesuniecie.x << ", "<< przesuniecie.y << endl;
    }

    void rysuj(sf::RenderWindow &okno)
    {
        window.draw(pocisk);
    }
};

bool tylkoRazPrzeciwnik = false;
bool rakietyPrzeciwnik=false;


class Przeciwnik    ////////////////////////////<<<<<   Przeciwnik   >>>>>
{

public:

    const float predkosc = 5.f;
    sf::Vector2f przesuniecie;
    Kolizja kol;
    bool przelacznik = false;
    vector <Pocisk> rakiety;
    enum Kierunek {N,S,E,W} kierunek=N;
    Vector2f pozycja;
    sf::Clock zegar;
    sf::Time CzasOdAktualizacji = sf::Time::Zero;
    sf::Time KrokCzasowy = sf::seconds(1.f);


    sf::Texture tekstura1;
    sf::Texture tekstura2;
    sf::Texture tekstura3;
    sf::Texture tekstura4;
    sf::Texture tekstura5;
    sf::Sprite przeciwnik;


public:
    Przeciwnik(sf::Vector2f rozmiar)
    {
        if(!tekstura1.loadFromFile("tank1Przeciwnik.png"))
        {
            cout << "Nie zaladowano tekstury1";
        }
        if(!tekstura2.loadFromFile("tank2Przeciwnik.png"))
        {
            cout << "Nie zaladowano tekstury2";
        }
        if(!tekstura3.loadFromFile("tank3Przeciwnik.png"))
        {
            cout << "Nie zaladowano tekstury3";
        }
        if(!tekstura4.loadFromFile("tank4Przeciwnik.png"))
        {
            cout << "Nie zaladowano tekstury4";
        }
        if(!tekstura5.loadFromFile("pociskPrzeciwnik.png"))
        {
            cout << "Nie zaladowano tekstury4";
        }

        przeciwnik.setTexture(tekstura1);
        przeciwnik.setTextureRect(sf::IntRect(0, 0, 40, 40));

        rakiety.push_back(Pocisk(przeciwnik.getPosition(),1,&tekstura5));
        przeciwnik.setPosition(100.f, 100.f);
    }

    void samRuch(sf::Time czas)
    {
        przeciwnik.move(przesuniecie * czas.asSeconds() * predkosc);
    }

    void gdzieSieRuszyc(Vector2f graczPoz)
    {
        pozycja = przeciwnik.getPosition();

        if(pozycja.x >= graczPoz.x && pozycja.x <=(graczPoz.x+40))
        {
            ruchP = FIREP;
            rakietyPrzeciwnik = true;
            tylkoRazPrzeciwnik = true;
            kierunek = S;
            //cout << "Jestem tu";
        }
        /*if((graczPoz.x) >= (pozycja.x+40))
        {
            ruchP = RIGHTP;
        }*/

        else if((graczPoz.x+40) < pozycja.x)
        {
            ruchP = LEFTP;
        }
        else if((graczPoz.x) > (pozycja.x+40))
        {
            ruchP = RIGHTP;
        }
        /*
        if((graczPoz.x+40) <= pozycja.x)
        {
            return RuchP = LEFTP;
        }*/
    }

    void przesun(sf::Time czas, Vector2f graczPoz)
    {
        gdzieSieRuszyc(graczPoz);

        if(ruchP == UPP && kol.kolRamka(przeciwnik.getPosition(),4) == false)
        {
            przesuniecie.y = -10.f;
            przesuniecie.x=0.f;
            ruchP=STOPP;
            samRuch(czas);
            kierunek = N;
            przeciwnik.setTexture(tekstura2);
        }
        if(ruchP == DOWNP && kol.kolRamka(przeciwnik.getPosition(),3) == false)
        {
            przesuniecie.y = 10.f;
            przesuniecie.x=0.f;
            ruchP=STOPP;
            samRuch(czas);
            kierunek = S;
            przeciwnik.setTexture(tekstura1);
        }
        if(ruchP == LEFTP && kol.kolRamka(przeciwnik.getPosition(),1) == false)
        {
            przesuniecie.x = -10.f;
            przesuniecie.y=0.f;
            ruchP=STOPP;
            samRuch(czas);
            kierunek = W;
            przeciwnik.setTexture(tekstura4);
        }
        if(ruchP == RIGHTP && kol.kolRamka(przeciwnik.getPosition(),2) == false)
        {
            przesuniecie.x = 10.f;
            przesuniecie.y=0.f;
            ruchP=STOPP;
            samRuch(czas);
            kierunek = E;
            przeciwnik.setTexture(tekstura3);
        }
        if(ruchP == FIREP && tylkoRazPrzeciwnik == true)
        {
            sf::Time Czas = zegar.restart();
            CzasOdAktualizacji += Czas;

            if(CzasOdAktualizacji > KrokCzasowy)
            {
                CzasOdAktualizacji -= KrokCzasowy;
                przelacznik = true;
                if(kierunek == N) rakiety.push_back(Pocisk(przeciwnik.getPosition(),4,&tekstura5));
                if(kierunek == S) rakiety.push_back(Pocisk(przeciwnik.getPosition(),3,&tekstura5));
                if(kierunek == E) rakiety.push_back(Pocisk(przeciwnik.getPosition(),2,&tekstura5));
                if(kierunek == W) rakiety.push_back(Pocisk(przeciwnik.getPosition(),1,&tekstura5));
                ruchP=STOPP;

                tylkoRazPrzeciwnik = false;
            }
        }
        if(przelacznik == true)
        {
            //int rozmiar = rakiety.size()-1;
            for(int i = rakiety.size()-1; i > 0; --i)
            {
                rakiety[i].przesun(czas,i);
                if(rakiety[i].czyUsun == true)
                {
                    rakiety[0] = rakiety[i];
                    rakiety[i]=rakiety[rakiety.size()-1];
                    rakiety[rakiety.size()-1] = rakiety[0];

                    rakiety.pop_back();
                }
                else {}
            }
        }
        kol.kolOtoczenie(przeciwnik.getPosition());
    }

    void Rysuj(sf::RenderWindow &okno)
    {
        okno.draw(przeciwnik);
    }
};


bool tylkoRaz = false;

class Gracz         ////////////////////////////<<<<<   Gracz   >>>>>
{
public:

    const float predkosc = 10.f;
    sf::Vector2f przesuniecie;
    Kolizja kol;
    bool przelacznik = false;
    vector <Pocisk> rakiety;
    enum Kierunek {N,S,E,W} kierunek=N;

    sf::Texture tekstura1;
    sf::Texture tekstura2;
    sf::Texture tekstura3;
    sf::Texture tekstura4;
    sf::Texture tekstura5;
    sf::Sprite gracz;


public:
    Gracz(sf::Vector2f rozmiar)
    {
        if(!tekstura1.loadFromFile("tank1.png"))
            cout << "Nie zaladowano tekstury1";
        if(!tekstura2.loadFromFile("tank2.png"))
            cout << "Nie zaladowano tekstury2";
        if(!tekstura3.loadFromFile("tank3.png"))
            cout << "Nie zaladowano tekstury3";
        if(!tekstura4.loadFromFile("tank4.png"))
            cout << "Nie zaladowano tekstury4";
        if(!tekstura5.loadFromFile("pocisk.png"))
            cout << "Nie zaladowano tekstury4";

        gracz.setTexture(tekstura1);
        gracz.setTextureRect(sf::IntRect(0, 0, 40, 40));

        rakiety.push_back(Pocisk(gracz.getPosition(),1,&tekstura5));
        gracz.setPosition(100.f, 100.f);
    }

    void samRuch(sf::Time czas)
    {
        gracz.move(przesuniecie * czas.asSeconds() * predkosc);
    }

    void przesun(sf::Time czas)
    {
        if(ruch == UP && kol.kolRamka(gracz.getPosition(),4) == false)
        {
            przesuniecie.y = -10.f;
            przesuniecie.x=0.f;
            ruch=STOP;
            samRuch(czas);
            kierunek = N;
            gracz.setTexture(tekstura2);
        }
        if(ruch == DOWN && kol.kolRamka(gracz.getPosition(),3) == false)
        {
            przesuniecie.y = 10.f;
            przesuniecie.x=0.f;
            ruch=STOP;
            samRuch(czas);
            kierunek = S;
            gracz.setTexture(tekstura1);
        }
        if(ruch == LEFT && kol.kolRamka(gracz.getPosition(),1) == false)
        {
            przesuniecie.x = -10.f;
            przesuniecie.y=0.f;
            ruch=STOP;
            samRuch(czas);
            kierunek = W;
            gracz.setTexture(tekstura4);
        }
        if(ruch == RIGHT && kol.kolRamka(gracz.getPosition(),2) == false)
        {
            przesuniecie.x = 10.f;
            przesuniecie.y=0.f;
            ruch=STOP;
            samRuch(czas);
            kierunek = E;
            gracz.setTexture(tekstura3);
        }
        if(ruch == FIRE && tylkoRaz == true)
        {
            przelacznik = true;
            if(kierunek == N) rakiety.push_back(Pocisk(gracz.getPosition(),4,&tekstura5));
            if(kierunek == S) rakiety.push_back(Pocisk(gracz.getPosition(),3,&tekstura5));
            if(kierunek == E) rakiety.push_back(Pocisk(gracz.getPosition(),2,&tekstura5));
            if(kierunek == W) rakiety.push_back(Pocisk(gracz.getPosition(),1,&tekstura5));
            kol.dodajDoKolizji(gracz.getPosition());
            ruch=STOP;

            tylkoRaz = false;
        }
        if(przelacznik == true)
        {
            for(int i = rakiety.size()-1; i > 0; --i)
            {
                rakiety[i].przesun(czas,i);
                //kol.p
                if(rakiety[i].czyUsun == true)
                {
                    rakiety[0] = rakiety[i];
                    rakiety[i]=rakiety[rakiety.size()-1];
                    rakiety[rakiety.size()-1] = rakiety[0];

                    rakiety.pop_back();
                }
                else {}
            }
        }
    }



    void Rysuj(sf::RenderWindow &okno)
    {
//        kol.kolOtoczenie(gracz.getPosition());
        okno.draw(gracz);
    }
};

class Gra           ////////////////////////////<<<<<   Gra   >>>>>
{
public:

    Gracz gracz = Gracz(sf::Vector2f(50.f, 50.f));
    sf::Clock zegar;
    sf::Time CzasOdAktualizacji = sf::Time::Zero;
    const sf::Time KrokCzasowy = sf::seconds(1.f/60.f);
    enum StanGry {MENU, GRA, KONIEC} stanGry;
    Kolizja kol;
    Przeciwnik przeciwnik = Przeciwnik(sf::Vector2f(100.f, 100.f));

    sf::Texture tekstura;
    sf::Sprite tlo;

    Pocisk pocisk = Pocisk(sf::Vector2f(50.f, 50.f),4,&tekstura);
    Pocisk pocisk2 = Pocisk(sf::Vector2f(50.f, 50.f),4,&tekstura);

    bool test=false, czyBylWcisniecie = false;

    Gra()
    {
        if(!tekstura.loadFromFile("tlo2.png"))
        {
            cout << "Nie zaladowano tekstury4";
        }
        tlo.setTexture(tekstura);
        tlo.setTextureRect(sf::IntRect(0, 0, 800, 600));
        tlo.setPosition(0.f, 0.f);

        petlaGlowna();
    }

    void petlaGlowna()
    {
        while(window.isOpen())
        {
            sf::Time Czas = zegar.restart();
            CzasOdAktualizacji += Czas;

            while(CzasOdAktualizacji > KrokCzasowy)
            {
                CzasOdAktualizacji -= KrokCzasowy;
                sf::Event zdarzenie;
                /*const*/Vector2f po = gracz.gracz.getPosition();

                while(window.pollEvent(zdarzenie))
                {
                    if(zdarzenie.type == sf::Event::Closed )
                        window.close();

                    if( zdarzenie.type == sf::Event::KeyReleased
                            && zdarzenie.key.code == sf::Keyboard::Space
                            && czyBylWcisniecie == true)
                    {
                        ruch = FIRE;
                        tylkoRaz = true;
                        test = true;
                        czyBylWcisniecie = false;
                    }
                }

                gracz.przesun(KrokCzasowy);
// tu beda generowani pzeciwnicy
                //ruchP = DOWNP;
                przeciwnik.przesun(KrokCzasowy, po);


            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                ruch = UP;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                ruch = DOWN;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                ruch = LEFT;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                ruch = RIGHT;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                czyBylWcisniecie = true;

            window.clear();
            window.draw(tlo);
            gracz.Rysuj(window);
            przeciwnik.Rysuj(window);

            if(rakietyPrzeciwnik == true)
            {
                for(int i = przeciwnik.rakiety.size()-1; i > 0; --i)
                {
                    przeciwnik.rakiety[i].rysuj(window);
                }
                pocisk2.rysuj(window);
            }

            if(test == true)
            {
                for(int i = gracz.rakiety.size()-1; i > 0; --i)
                {
                    gracz.rakiety[i].rysuj(window);
                }
                pocisk.rysuj(window);

            }
            window.display();
//            kol.usunKolizje();
        }
    }
};

int main()
{
    Gra gra;

    return 0;
}
