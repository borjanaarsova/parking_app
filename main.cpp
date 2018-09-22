/* OVOJ PROGRAM PRESMETUVA CENA NA PARKING USLUGA
Cenata za 1 cas e smestena vo konstantata CENA_CAS
Cenata za doplata za sekoi naredni 10 minuti po prviot cas e smestena vo konstantata CENA_DOPLATA
Brojot na pretplateni korisnici e smesten vo konstantata PRETPLATENI_MESTA
Brojot na ostanati parking mesta e smesten vo konstantata PARKING_MESTA
ZABELESKA: PROGRAMOT GO ZEMA SISTEMSKOTO VREME PRI VLEZ I IZLEZ NA VOZILATA!
ZABELESKA1: PROGRAMOT JA KORISTI DATOTEKATA data.txt KOJA MORA DA BIDE VO IST DIREKTORIUM KAKO IZVRSNATA VERZIJA NA PROGRAMOT!
PODATOCITE ZA PRETPLATENITE KORISNICI SE IZMENUVAAT DIREKTNO VO DATOTEKATA.
*/

#include <iostream> //standardna biblioteka vo C++
#include <string> //biblioteka za operacii so stringovi
#include <fstream> //biblioteka za operacii so datoteki
#include <ctime> //biblioteka za sistemsko vreme
#include <cstdlib> //biblioteka za konvertiranje i generiranje

using namespace std;

struct PodatociZaVozilo{ //struktura podatocizavozilo
    char ime[15];
    char prezime[15];
    char registerski_broj[15]; //ST-001-AA, SR-0001-AA
    int vreme_vlez; //HHMM
    int data_vlez; //GGMMDD
};

int i,j,slobodni_mesta; //globalni promenlivi, nadolu se konstanti
const int PARKING_MESTA=7;//mesta za nepretplateni korisnici
const int PRETPLATENI_MESTA=3;//pretplateni mesta
const int VKUPNO_MESTA=PARKING_MESTA+PRETPLATENI_MESTA;//vkupno mesta
const int CENA_CAS=40; //cena na parking za eden cas
const int CENA_DOPLATA=10; //doplata za sekoi naredni 10 min po izminuvanjeto na 1 cas

void vlez_na_vozilo(struct PodatociZaVozilo &vozilo1); //najaveni void funkcii
void izlez_na_vozilo();                               //za vlez i izlez na vozila

int presmetaj_cena(int vkupno_minuti){ //ovde se presmetuva cenata koja treba
    if (vkupno_minuti<=60) //da se plati pri izlez na voziloto
        return (CENA_CAS); //ako se zadrzal <=60 minuti, plakja 40 denari
    else{
        vkupno_minuti-=60; //ako se zadrzal >60 minuti, plakja 40 denari za
        vkupno_minuti/=10; //eden cas plus 10 denari za sekoi naredni 10
        return ((vkupno_minuti*CENA_DOPLATA)+CENA_CAS); //minuti
    }
}

int main(){ //ova e glavnata funkcija main
    PodatociZaVozilo vozilo[VKUPNO_MESTA+1];
    string sostojba[VKUPNO_MESTA];
    string akcija;
    while (akcija!="3"){ //glavno meni vo programata
        cout<<"Vnesi go indeksot pred akcijata koja sakas da ja izvrsis: " << endl; 
        cout<<"1. Vlez na vozilo vo garazata" << endl; 
        cout<<"2. Izlez na vozilo od garazata" << endl; 
        cout<<"3. Izlez od programot" << endl; 
        cin>>akcija;
        system("CLS");
        while (akcija!="1" && akcija!="2" && akcija!="3")
        {
            cout<<"Vnesovte pogresna akcija!" << endl; 
            cout<<"Vnesi go indeksot pred akcijata koja sakas da ja izvrsis:" << endl; 
            cout<<"1. Vlez na vozilo vo garazata" << endl; 
            cout<<"2. Izlez na vozilo od garazata " << endl; 
            cout<<"3. Izlez od programot" << endl; ;
            cin>>akcija;
        }

        if (akcija=="1"){ //pobaruvanje za vlez na vozilo
            fstream fin; //se otvora datotekata i se oznacuvaat podatocite
            fin.open("data.txt", ios::in);
            for (i=0; i<VKUPNO_MESTA; i++)
                fin>>sostojba[i]>>vozilo[i].ime>>vozilo[i].prezime>>vozilo[i].registerski_broj>>vozilo[i].vreme_vlez>>vozilo[i].data_vlez;
            fin>>slobodni_mesta;
            fin.close();
            if (slobodni_mesta!=0){
                j=PRETPLATENI_MESTA;
                while (sostojba[j]!="slobodno") //koga ke najde nekoe slobodno mesto
                    j++;
                sostojba[j]="zafateno";
                vlez_na_vozilo(vozilo[j]); //pristapuva do vlez na voziloto
                slobodni_mesta--;
                string str1=vozilo[j].registerski_broj;
                for (i=0; i<PRETPLATENI_MESTA; i++){ //dokolku vleglo pretplateno vozilo
                    string str2=vozilo[i].registerski_broj; //ovde se smestuva na
                    if (str1==str2){  //pretplatenite pozicii, a kaj vekje smestenata
                        vozilo[i].data_vlez=vozilo[j].data_vlez; //pozicija se
                        vozilo[i].vreme_vlez=vozilo[j].vreme_vlez;  //zapisuva
                        sostojba[j]="slobodno"; //deka e slobodna pozicija
                        slobodni_mesta++;
                    }
                }
                cout<<"Podatocite bea uspesno vneseni." << endl;
            }
            else {  //dokolku vo garazata nema slobodni mesta se izvrsuva slednoto
                cout<<"Vo garazata nema slobodni mesta. Vozmozno e samo vlez na pretplateni i izlez na vozila! " << endl; 
                vlez_na_vozilo(vozilo[VKUPNO_MESTA+1]); //se vnesuva registracijata
                string str1=vozilo[VKUPNO_MESTA+1].registerski_broj; //se proveruva
                bool ok=false; //dali registracijata se odnesuva na nekoj
                for (i=0; i<PRETPLATENI_MESTA; i++){ //pretplaten avtomobil
                    string str2=vozilo[i].registerski_broj; //i ako e pretplaten
                    if (str1==str2){ //vrakja uspesen vlez, a ako ne e pretplaten
                        cout<<"Podatocite bea uspesno vneseni." << endl;  //vrakja deka ne e pretplaten
                        ok=true; //i ne moze da vleze
                        vozilo[i].data_vlez=vozilo[VKUPNO_MESTA+1].data_vlez;
                        vozilo[i].vreme_vlez=vozilo[VKUPNO_MESTA+1].vreme_vlez;
                    }
                }
                if (!ok)
                cout<<"Voziloto ne e pretplateno! Ne e ovozmozeno vnesuvanje na voziloto!" << endl; 
            }
            fstream fout; //dokolku avtomobilot vleze, toj se smestuva na
            fout.open("data.txt", ios::out); //slobodnata pozicija, a na
            for (i=0; i<VKUPNO_MESTA; i++) //ostanatite slobodni pozicii
                if (sostojba[i]!="slobodno") //se zapisuvaat sistemski
                    fout<<sostojba[i]<<" "<<vozilo[i].ime<<" "<<vozilo[i].prezime<<" "<<vozilo[i].registerski_broj<<"  "<<vozilo[i].vreme_vlez<<" "<<vozilo[i].data_vlez<<endl;
                else  //ime prezime i registracija
                    fout<<"slobodno "<<"ZZZZZ "<<"ZZZZZ "<<"ZZ-000-ZZ "<<0<<" "<<0<<endl;
            fout<<slobodni_mesta;
            fout.close();
        }
        if (akcija=="2"){
            izlez_na_vozilo();
        }
        
    }
    
cout<<"Vnesi sort za da se izvrsi sortiranje: "; //za da se sortira
string sort; //treba da se vnese tekstot sort
cin>>sort;

if(sort=="sort")
{
fstream fin; //se zemaat podatocite za avtomobilite na parkingot
fin.open("data.txt", ios::in);
for (i=0; i<VKUPNO_MESTA; i++)
fin>>sostojba[i]>>vozilo[i].ime>>vozilo[i].prezime>>vozilo[i].registerski_broj>>vozilo[i].vreme_vlez>>vozilo[i].data_vlez;
fin>>slobodni_mesta;
fin.close();

string str1,str2,str3,str4,str5,str6,str7; //pomosni promenlivi
int b1,b2;
for(b1=0;b1<VKUPNO_MESTA-1;b1++)
{
for(b2=b1+1;b2<VKUPNO_MESTA;b2++)
{
str1=vozilo[b1].prezime;
str2=vozilo[b2].prezime;
if(str1.compare(str2)>0) //proveruva preziminja
{
vozilo[VKUPNO_MESTA]=vozilo[b1]; //sortiranje
vozilo[b1]= vozilo[b2];
vozilo[b2]=vozilo[VKUPNO_MESTA];
str7=sostojba[b1];
sostojba[b1]=sostojba[b2];
sostojba[b2]=str7;
}
else
{
str3=vozilo[b1].ime;
str4=vozilo[b2].ime;
if(str1.compare(str2)==0 && str3.compare(str4)>0) //proveruva iminja
{
vozilo[VKUPNO_MESTA]=vozilo[b1]; //sortiranje
vozilo[b1]= vozilo[b2];
vozilo[b2]=vozilo[VKUPNO_MESTA];
str7=sostojba[b1];
sostojba[b1]=sostojba[b2];
sostojba[b2]=str7;
}
else
{
str5=vozilo[b1].registerski_broj;
str6=vozilo[b2].registerski_broj;
if(str1.compare(str2)==0 && str3.compare(str4)==0 && str5.compare(str6)>0) //proveruva registracii
{
vozilo[VKUPNO_MESTA]=vozilo[b1]; //sortiranje
vozilo[b1]= vozilo[b2];
vozilo[b2]=vozilo[VKUPNO_MESTA];
str7=sostojba[b1];
sostojba[b1]=sostojba[b2];
sostojba[b2]=str7;
}
}
}
}
}

fstream fout; //sortiranite podatoci se zapisuvaat vo sort.txt
fout.open("sort.txt", ios::out);
for (i=0; i<VKUPNO_MESTA; i++)
fout<<sostojba[i]<<" "<<vozilo[i].ime<<" "<<vozilo[i].prezime<<" "<<vozilo[i].registerski_broj<<" "<<vozilo[i].vreme_vlez<<" "<<vozilo[i].data_vlez<<endl;
fout.close();
}
    system ("pause");
    return 0;
}

void vlez_na_vozilo(PodatociZaVozilo &vozilo1){ //funkcija za vlez na vozilo
    cout<<"Ime na sopstvenikot: ";  //vnes na ime
    cin>>vozilo1.ime;
        if (!cin)
            cout<<"Gresen podatok";
    cout<<"Prezime na sopstvenikot: "; //vnes na prezime
    cin>>vozilo1.prezime;
        if (!cin)
            cout<<"Gresen podatok";
    cout<<"Registraciski broj na voziloto: "; //vnes na registracija
    cin>>vozilo1.registerski_broj;
        if (!cin)
            cout<<"Gresen podatok";
    time_t t = time(0);   // go zema sistemskoto vreme vo momentot
    struct tm * now = localtime( & t );
    int godina=(now->tm_year + 1900);
    int mesec=(now->tm_mon + 1);
    int den=now->tm_mday;
    int cas=now->tm_hour;
    int minuta=now->tm_min;
    vozilo1.data_vlez=(godina%2000)*10000+(mesec*100)+den; //ja pretvara datata vo format GGMMDD
    vozilo1.vreme_vlez=cas*100+minuta; //go pretvara vremeto vo format HHMM
}

void izlez_na_vozilo(){ //funkcija za izlez na vozilo
    PodatociZaVozilo vozilo1[VKUPNO_MESTA]; //deklaracija na struktura
    string sostojba[VKUPNO_MESTA];
    fstream fin;
    fin.open("data.txt", ios::in); //otvoranje na datotekata
    for (i=0; i<VKUPNO_MESTA; i++)
            fin>>sostojba[i]>>vozilo1[i].ime>>vozilo1[i].prezime>>vozilo1[i].registerski_broj>>vozilo1[i].vreme_vlez>>vozilo1[i].data_vlez;
    fin>>slobodni_mesta;
    fin.close();
    char registerski_broj[15];
    cout<<"Vnesi go registraciskiot broj na voziloto za prebaruvanje." << endl;  //se vnesuva registracija za izlez na vozilo
    cout<<"Registraciski broj: ";
    cin>>registerski_broj;
    bool ok=false;
    for (i=0; i<VKUPNO_MESTA; i++){ //ovde gi prebaruva registraciite
        string registerski_broj1=vozilo1[i].registerski_broj;
        if (registerski_broj1==registerski_broj){  //proveruva dali se isti registraciite
            ok=true;
            if (i<PRETPLATENI_MESTA) { //ovde gi prebaruva pretplatenite vozila
                cout<<"Ime: ";
                cout<<vozilo1[i].ime<<endl;
                cout<<"Prezime: ";
                cout<<vozilo1[i].prezime<<endl;
                cout<<"Registraciski broj: ";
                cout<<vozilo1[i].registerski_broj<<endl;
                cout<<"Vlez: ";
                cout<<vozilo1[i].vreme_vlez/100<<":"<<vozilo1[i].vreme_vlez%100<<" "<<vozilo1[i].data_vlez%100<<"."<<(vozilo1[i].data_vlez/100)%100<<"."<<vozilo1[i].data_vlez/10000<<endl;
                cout<<"Cena: Pretplaten! " << endl; 
                cout<<"Voziloto se naoga na parking mesto so broj: "<<i+1<<endl<<endl;
                break;
            }
            else{ //ovde gi prebaruva ne pretplatenite vozila
                time_t t = time(0);   // go zema sistemskoto vreme vo momentot
                struct tm * now = localtime( & t );
                int godina2=((now->tm_year + 1900))%2000;
                int mesec2=(now->tm_mon + 1);
                int den2=now->tm_mday;
                int cas2=now->tm_hour;
                int minuta2=now->tm_min;
                int vkupno_minuti;
                int cas1=vozilo1[i].vreme_vlez/100;
                int minuta1=vozilo1[i].vreme_vlez%100;
                int godina1=vozilo1[i].data_vlez/10000;
                int mesec1=(vozilo1[i].data_vlez/100)%100;
                int den1=vozilo1[i].data_vlez%100;
                vkupno_minuti=minuta2-minuta1;
                vkupno_minuti+=(cas2-cas1)*60;
                vkupno_minuti+=(den2-den1)*1440;
                vkupno_minuti+=(mesec2-mesec1)*43200;
                vkupno_minuti+=(godina2-godina1)*518400; //presmetuva kolku vkupno minuti voziloto se zadrzalo vo garazata
                cout<<"Ime: ";
                cout<<vozilo1[i].ime<<endl;
                cout<<"Prezime: ";
                cout<<vozilo1[i].prezime<<endl;
                cout<<"Registraciski broj: ";
                cout<<vozilo1[i].registerski_broj<<endl;
                cout<<"Vlez: ";
                cout<<vozilo1[i].vreme_vlez/100<<":"<<vozilo1[i].vreme_vlez%100<<" "<<vozilo1[i].data_vlez%100<<"."<<(vozilo1[i].data_vlez/100)%100<<"."<<vozilo1[i].data_vlez/10000<<endl;
                cout<<"Cena: "<<presmetaj_cena(vkupno_minuti)<<endl;
                cout<<"Voziloto se naoga na parking mesto so broj: "<<i+1<<endl<<endl;
                sostojba[i]="slobodno";
                ++slobodni_mesta;
                fstream fout;
                fout.open("data.txt", ios::out);
                for (i=0; i<VKUPNO_MESTA; i++)  //na praznite parking mesta zapisuva slobodno
                    if (sostojba[i]!="slobodno")
                        fout<<sostojba[i]<<" "<<vozilo1[i].ime<<" "<<vozilo1[i].prezime<<" "<<vozilo1[i].registerski_broj<<"  "<<vozilo1[i].vreme_vlez<<" "<<vozilo1[i].data_vlez<<endl;
                    else
                         fout<<"slobodno "<<"ZZZZZ "<<"ZZZZZ "<<"ZZ-0000-ZZ "<<0<<" "<<0<<endl;
                fout<<slobodni_mesta;
                fout.close();
                break;
            }
        }
    }
    if (!ok)
        cout<<"Vo garazata ne postoi vozilo so gorenavedenite detali! " << endl;  //ako ne e pronajdeno vozilo go pecati ova
}
