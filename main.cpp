
#include <iostream>
#include <fstream>

using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");



struct NOD {
   int valoare;                     // valoarea nodului
   int inaltime;                    // inaltimea
   NOD *nodST;                  // subarborele drept
   NOD *nodDR;                 // subarborele stang
};

//INALTIMEA ARBORELUI
// aici doar imi returneaza inaltimea
int inaltimeArbore(NOD* rad)
{
    if(rad == NULL)
        return 0;
    else
        return rad->inaltime;
}
/*
//functie recurziva, facem inaltime de stanga si dreapta si o alegem pe cea mai mare
//+1 pentru cand urcam un nivel
int inaltimeArbore(NOD* rad)
{
    if(rad){
        int hST = inaltimeArbore(rad->nodST);
        int hDR = inaltimeArbore(rad->nodDR);
        int maxim;
        if (hST > hDR)
        {
            maxim = hST;
        }
        else{
            maxim =  hDR;
        }
        maxim +=1;
        return maxim;
    }
    else {
        return 0;              //daca nu avem radacina
    }
}

*/

//Rotirea spre stanga a arborelui
NOD* rotireST(NOD* rad)
{
    NOD *y = rad->nodDR;
    NOD *aux = y->nodST;

    y->nodST = rad;
    rad->nodDR = aux;

    rad->inaltime = max(inaltimeArbore(rad->nodST), inaltimeArbore(rad->nodDR)) + 1;
    y->inaltime = max(inaltimeArbore(y->nodST),inaltimeArbore(y->nodDR)) + 1;

    return y;
}
 //Rotirea spre dreapta a arborelui
NOD* rotireDR(NOD* rad)
{
    NOD *x = rad->nodST;
    NOD *aux = x->nodDR;

    x->nodDR = rad;
    rad->nodST = aux;

    rad->inaltime = max(inaltimeArbore(rad->nodST),inaltimeArbore(rad->nodDR)) + 1;
    x->inaltime = max(inaltimeArbore(x->nodST),inaltimeArbore(x->nodDR)) + 1;

    return x;
}



int balance(NOD* rad) //fct care returneaza valoarea dupa care trebuie sa echilibram AVL-ul
{
    if(rad == NULL)
        return 0;
    else
        return inaltimeArbore(rad->nodST) - inaltimeArbore(rad->nodDR);
}

//functia pentru CAUTAREA nodului in AVL
//comparam cu valoarea nodului si vedem in ce parte trebuie sa cautam

int cautareNod (NOD* rad, int x)
{
    if(rad == NULL|| rad->valoare == x) return rad->valoare;
    if(x<rad->valoare)
        return cautareNod(rad->nodST, x);
    if (x>rad->valoare)
        return cautareNod(rad->nodDR, x);
}


//functie pentru gasirea si afisarea "cel mai MARE numar Y, mai MIC sau egal cu X"
//comparam cu nodul curent si hotaram in ce parte sa cautam
int MareMic(NOD* rad, int x)
{
    if(rad == NULL)
        return -1;

    if(rad->valoare == x) //daca gasim nodul egal il returnam
        return x;
    else if(rad->valoare < x) //daca nodul e mai mic cautam in DREAPTA
    {
        int aux = MareMic(rad->nodDR, x);
        if(aux == -1)
            return rad->valoare;
        else                         //returnam valoarea cea mai mare, dar mai mica decat nodul curent
            return aux;
    }
    else if(rad->valoare > x)    //daca nodul e mai mare cautam in STANGA
        return MareMic(rad->nodST, x);
}


 //functie pentru gasirea si afisarea "cel mai MIC numar Y, mai MARE sau egal cu X"

int MicMare(NOD* rad, int x)
{
    if (rad->nodST == NULL && rad->nodDR == NULL && rad->valoare < x) //daca nodul curent e mai mic decat x si nici nu mai are subarbori in care sa cautam atunci returnam -1
        return -1;

    if((rad->valoare >= x && rad->nodST == NULL) || (rad->valoare >= x && rad->nodST->valoare < x)) //daca nocdul curent e mai mare decat x si nu mai are nimic in stanga
        return rad->valoare;                                                                               //sau ce e in stanga e mai mic decat x
                                                                                                        //inseamna ca am gasit ce cautam

    if(rad->valoare <= x) //daca nodul e mai mic decat X, cautam in partea dreaptea
        return MicMare(rad->nodDR, x);
    else                  //daca e mai mare cautam in continuare in stanga ptc nu au fost indeplinite cele de mai sus ^
        return MicMare(rad->nodST, x);
}


// functia de inserare
NOD* inserare(NOD* rad, int x)
{
    if(rad == NULL) //Daca nodul in care ne aflam este gol, cream unul nou
    {
        NOD* rad = new NOD();
        rad->valoare = x;
        rad->inaltime = 1;
        rad->nodST = NULL;
        rad->nodDR = NULL;
        return rad;
    }
    else
    {
        //Cautam unde ar trebui sa inseram nodul
        if(x < rad->valoare)       //daca e mai mic decat nodul curent inseamna ca o sa il inseram undeva in subarborele din stanga
            rad->nodST = inserare(rad->nodST, x);
        else if(x > rad->valoare)         //daca e mai mare inseamna ca o sa il inseram undeva in subarborele din dreapta
            rad->nodDR = inserare(rad->nodDR, x);
        else
            return rad; // returnam nodul daca x e egal cu el deoarece nu il putem avea de doua ori

        rad->inaltime = max(inaltimeArbore(rad->nodST), inaltimeArbore(rad->nodDR)) + 1; //actualizam inaltimea dupa ce am inserat

        //Calculam si verificam factorul de echilibru si sa refacem echilibrul arborelui dace e nevoie
        int bal = balance(rad);
                                          //avem 4 cazuri, 2 de dreapta si 2 de stanga
        if(bal > 1) //  Stanga
        {
            if(x < rad->nodST->valoare)   // stanga-stanga
                return rotireDR(rad);
            else if(x > rad->nodST->valoare)   //stanga-dreapta
            {
                rad->nodST = rotireST(rad->nodST);
                return rotireDR(rad);
            }
        }
        else if(bal < -1) //  Dreapta
        {
            if(x > rad->nodDR->valoare)   //dreaptra-dreapta
                return rotireST(rad);
            else if(x < rad->nodDR->valoare)   //dreapta-stanga
            {
                rad->nodDR = rotireDR(rad->nodDR);
                return rotireST(rad);
            }
        }
        return rad;
    }
}


int main()
{
    int numar_operatii;
    int op;
    int x,y;

    NOD* rad = NULL;

    fin >> numar_operatii;
    for(int i=1; i<=numar_operatii; i++)
    {
        fin >> op;
        if(op == 1) //Inserare element X
        {
            fin >> x;
            rad = inserare(rad,x);
        }
        else if(op == 2) //Stergere element X
        {
            fin >> x;     // nu am facut
        }

        else
            if(op == 3) //cautam elementul X
        {
            fin >> x;
            if( cautareNod(rad,x) != NULL)
               fout<< 1 << "\n";
            else
               fout<< 0<<"\n";
        }
        else if(op == 4) // cel mai mare element mai mic sau egal cu x
        {
            fin >> x;
            fout << MareMic(rad, x) << "\n";
        }
        else if(op == 5) //cel mai mic element mai mare sau egal cu X
        {
            fin >> x;
            fout << MicMare(rad, x) << "\n";
        }
        else if(op == 6) //Afisarea elementelor Z, unde X <= Z <= Y
        {
            fin >> x >> y; //nu am facut
        }

    }
    return 0;
}
