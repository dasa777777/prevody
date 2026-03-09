/*Riešte problém na výpočet súčtu členov postupnosti čísel. Počet členov postupnosti nie je dopredu známy, končí sa stlačením tlačidla Q/q. Daný súčet
 podľa voľby používateľa potom môže program previesť buď do:
-	Binárnej sústavy
-	Osmičkovej sústavy
-	Šestnástkovej sústavy*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define KAPACITA 20



//funkcia na kontrolu zadavaneho vstupu,ktora kontroluje, ci sa jedna o cislo
int jeCislica(char c);

//funkcia na spracovanie zaporneho cislo-kedze zapisujeme vstupy ako char-string
int spracujZnamienko(char *s, int *index);

//funkcia na prevod zo stringu na cislo
int spracujCislo(char *s, int start, double *cislo);

//funkcia na prevod zo stringu na cislo/double, vratane znamienka
int stringNaFloat(char *s, double *vysledok);

//funkcia na sucet zadanych vstupov
double sucetPolia(double *pole, int pocet);

//funkcia na zaokruhlenie vysledku
double zaokruhli(double cislo, int desatinneMiesta);

// funkcia na prevod vysledku do binarnej sustavy
void doBinarnej(double cislo, int presnost);

//funkcia na prevod vysledku do osmickovej sustavy
void doOsmickovej(double cislo, int presnost);

//funkcia na prevod vysledku do sestnastkovej sustavy
void doHexadec(double cislo, int presnost);




int main(void)
{
    double *pole = malloc(KAPACITA * sizeof(double));
    if (!pole)
    {
        printf("Chyba alokacie pamate!\n");
        return 1;
    }

    int kapacita = KAPACITA, index = 0;
    char vstup[50];//premenna na ukladanie zadanych vstupov
    double cislo;

    printf("Vitaj v programe na vypocet suctu zadavanych cisel a ich prevod do vybranej ciselnej sustavy.\n");
    printf("Pri zapise desatinneho cisla pouzi na oddelenie desatinnej casti bodku. Nezapisuj cisla dlhsie ako 15 cifier, program nebude pracovat spravne.\n");
    printf("Program zaokruhluje vysledny sucet na osem desatinnych miest.\n");
    printf("Zadaj cislo (q/Q pre ukoncenie): ");
    fgets(vstup, sizeof(vstup), stdin);
    vstup[strcspn(vstup, "\n")] = '\0';

    //v pripade, ak zada iba q alebo Q tak program ihned skonci a uvolni alokovanu pamat
    if (vstup[0] == 'q' || vstup[0] == 'Q')
    {
        printf("Nezadal si ziadne cisla, program skoncil.\n");
        free(pole);
        return 0;
    }



    while (!(vstup[0] == 'q' || vstup[0] == 'Q'))
    {
        if (stringNaFloat(vstup, &cislo))
        {
            // dynamická kontrola kapacity - ak nepostacuje, pole sa zvacsi, zdvojnasobime kapacitu a alokujeme novu pamat
            if (index >= kapacita)
            {
                kapacita *= 2;
                double *temp = realloc(pole, kapacita * sizeof(double));
                if (!temp)
                {
                    printf("Chyba alokacie pamate!\n");
                    free(pole);
                    return 1;
                }
                pole = temp;
            }
            pole[index++] = cislo;
        }


        else
        {
            printf("Chybne zadane cislo, opakuj.\n");
        }

        printf("Zadaj cislo (q/Q pre ukoncenie): ");
        fgets(vstup, sizeof(vstup), stdin);
        vstup[strcspn(vstup, "\n")] = '\0';
    }

    printf("Zadavanie ukoncene.\nZadane cisla:\n");
    for (int i = 0; i < index; i++)
    {
        printf("Cislo %d: %.8lf\n", i + 1, pole[i]);
    }

    double vyslednySucet = sucetPolia(pole, index);

// zaokrúhlenie na 8 desatinných miest
    vyslednySucet = zaokruhli(vyslednySucet, 8);

    printf("Sucet v desiatkovej sustave (zaokruhleny): %.8lf\n", vyslednySucet);

    char volba = 0;//premenna na vyber moznosti prevodu
    // opakovany vyber moznosti prevodu a vypocet, ukoncenie zadanim nuly
    do
    {
        printf("\nVyber si, do akej sustavy chces previest vysledok, zadaj iba jedno cislo (0 ak chces ukoncit program):\n");
        printf("1 - Binarna\n");
        printf("2 - Osmickova\n");
        printf("3 - Sestnastkova\n");
        printf("Tvoja volba: ");

        char riadok[10];
fgets(riadok, sizeof(riadok), stdin);
volba = riadok[0];


        switch (volba)
        {
        case '1':
            printf("Binarne: ");
            doBinarnej(vyslednySucet, 8);
            printf("\n");
            break;
        case '2':
            printf("Osmickove: ");
            doOsmickovej(vyslednySucet, 8);
            printf("\n");
            break;
        case '3':
            printf("Hexadecimalne: ");
            doHexadec(vyslednySucet, 8);
            printf("\n");
            break;
        case '0':
            printf("Koniec programu.\n");
            break;
        default:
            printf("Neplatna volba, skus znova.\n");
        }
    }
    while (volba != '0');

    free(pole);
    return 0;
}




//funkcia na kontrolu zadavaneho vstupu,ktora kontroluje, ci sa jedna o cislo
int jeCislica(char c)
{
    return (c >= '0' && c <= '9');
}

//funkcia na spracovanie zaporneho cislo-kedze zapisujeme vstupy ako char-string
int spracujZnamienko(char *s, int *index)
{
    if (s[0] == '-')
    {
        *index = 1;
        return 1;
    }
    *index = 0;
    return 0;
}

//funkcia na prevod zo stringu na cislo
int spracujCislo(char *s, int start, double *cislo)
{
    int i = start;
    double vysledok = 0.0, desatina = 0.1;
    int bodka = 0; //na rozlisenie celej a desatinnej casti

    for (; s[i] != '\0'; i++)
    {
        if (jeCislica(s[i]))
        {
            if (!bodka) vysledok = vysledok * 10 + (s[i] - '0');
            else
            {
                vysledok += (s[i] - '0') * desatina;
                desatina /= 10;
            }
        }
        else if (s[i] == '.')
        {
            if (bodka) return 0;
            bodka = 1;
        }
        else return 0;
    }

    *cislo = vysledok;
    return 1;
}


//funkcia na prevod zo stringu na cislo/double, vratane znamienka
int stringNaFloat(char *s, double*vysledok)
{
    int index, zaporne = spracujZnamienko(s, &index);
    if (s[index] == '\0') return 0;
    if (!spracujCislo(s, index, vysledok)) return 0;
    if (zaporne) *vysledok = -(*vysledok); //ak je cislo zaporne, vynasobi ho -1
    return 1;
}

//funkcia na sucet zadanych vstupov
double sucetPolia(double *pole, int pocet)
{
    double sucet = 0.0;
    for (int i = 0; i < pocet; i++) sucet += pole[i];
    return sucet;
}


// funkcia na zaokruhlenie vysledku na n desatinnych miest
double zaokruhli(double cislo, int desatinneMiesta)
{
    double mocnina = pow(10.0, desatinneMiesta);   // 10^n
    return round(cislo * mocnina) / mocnina;
}
// funkcia na prevod vysledku do binarnej sustavy
void doBinarnej(double cislo, int presnost)
{
    if (cislo < 0)
    {
        printf("-");
        cislo = -cislo;
    }

    int cela = (int)cislo;
    double desatina = cislo - cela;

    // 1. prevod celej casti, pred desatinnou ciarkou
    if (cela == 0) printf("0");
    else
    {
        int bin[32], index = 0;
        while (cela > 0)
        {
            bin[index++] = cela % 2;
            cela /= 2;
        }
        for (int i = index - 1; i >= 0; i--) printf("%d", bin[i]);
    }

    printf(".");

    // 2. ak je cislo desatinne, prevedie aj desatinnu cast
    for (int i = 0; i < presnost; i++)
    {
        desatina *= 2;
        if (desatina >= 1.0)
        {
            printf("1");
            desatina -= 1.0;
        }
        else printf("0");
    }
}

//funkcia na prevod vysledku do osmickovej sustavy
void doOsmickovej(double cislo, int presnost)
{
    if (cislo < 0)
    {
        printf("-");
        cislo = -cislo;
    }
    int cela = (int)cislo;
    double desatina = cislo - cela;
    char octal[64];
    int index = 0;
    if (cela == 0) octal[index++] = '0';
    else
    {
        while (cela > 0)
        {
            octal[index++] = '0' + (cela % 8);
            cela /= 8;
        }
    }
    for (int i = index - 1; i >= 0; i--) putchar(octal[i]);
    putchar('.');
    for (int i = 0; i < presnost; i++)
    {
        desatina *= 8;
        int digit = (int)desatina;
        putchar('0' + digit);
        desatina -= digit;
    }
}

//funkcia na prepocet do sestnastkovej sustavy
void doHexadec(double cislo, int presnost)
{
    if (cislo < 0) {
        printf("-");
        cislo = -cislo;
    }

    int cela = (int)cislo;
    double desatina = cislo - cela;

    char hexal[64];
    int index = 0;
    const char *hex = "0123456789ABCDEF";

    if (cela == 0) hexal[index++] = '0';
    else {
        while (cela > 0) {
            hexal[index++] = hex[cela % 16];
            cela /= 16;
        }
    }

    for (int i = index - 1; i >= 0; i--)
        putchar(hexal[i]);

    putchar('.');

    for (int i = 0; i < presnost; i++) {
        desatina *= 16;
        int digit = (int)desatina;
        putchar(hex[digit]);
        desatina -= digit;
    }
}





