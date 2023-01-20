// Stan Robert-Ionut  - 164

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUME_MAX 30

enum client
{
    lista_clienti = 1,
    detalii_client = 2,
    adaugare_client = 3,
    modificare_nume = 4,
    modificare_balanta = 5,
    stergere_client = 6,
    transfer = 7,
    imprumut = 8,
    achitare_rate = 9,
    extras_de_cont = 10
};

typedef struct Client
{
    char nume[NUME_MAX];
    unsigned imprumut_activ : 1 ;
    float balanta;
    float balanta2;
    float imprumut;
    unsigned int iban;

} Client;

typedef struct Banca
{
    double balanta_banca;
    float comision;

} Banca;

void createClients(Client **c,int i)
{
    char n[NUME_MAX];
    int j;
    *c = malloc(i*sizeof(Client));
    FILE *f = fopen ("nume_balanta.txt","r");
    for(j=0; j<i&&fgets(n, sizeof(n), f); j++)
    {
        sscanf(n,"%29s %f", (*c)[j].nume, &(*c)[j].balanta);
        (*c)[j].imprumut_activ=0;
        (*c)[j].balanta2=(*c)[j].balanta;
    }

    fclose(f);

}

void getClientList(Client* c, int k)
{
    int i;
    for(i=0; i<k; i++)
        if(strcmp(c[i].nume,"NULL"))
            printf("%d - %s %.2f \n", i+1, c[i].nume,c[i].balanta);

}

int getClientListLoans(Client* c, int k)
{
    int i,d=0;
    for(i=0; i<k; i++)
        if(strcmp(c[i+1].nume,"NULL")&&c[i].imprumut_activ==1)
            d=1;

    if(d==1)
    {
        printf("Lista clienti cu imprumut activ:\n");
        for(i=0; i<k; i++)
        {
            if(strcmp(c[i+1].nume,"NULL")&&c[i].imprumut_activ==1)
                printf("\n%d - %s %.2f\n", i+1, c[i].nume,c[i].balanta);
        }
    }
    else
        printf("Nu exista clienti cu imprumuturi active.\n");

    return d;
}

void getClientInfo(Client *c, unsigned i )
{
    unsigned j,i2;
    int k=-1;
    char n[NUME_MAX];
    i2=0;
    while(i2<i)
    {
        if(strcmp(c[i2].nume,"NULL"))
            printf("%s\n", c[i2].nume);
        i2++;
    }


    printf("\nAlege numele clientului: ");
    scanf("%29s", n);
    for (j=0; j<i; j++)
        if(!strcmp(c[j].nume,n))
        {
            k=j;
            printf("\n%s are suma de %.2f  |  Cod IBAN: %u  |  ", c[k].nume,c[k].balanta,c[k].iban);
            if(c[k].imprumut_activ==0)
                printf(" (Imprumut activ: NU)\n");
            else if(c[k].imprumut_activ==1)
                printf(" (Imprumut activ: DA)\n");
        }

    if(k==-1)
        printf("\nClientul nu exista! Pentru a incerca din nou introduceti cifra 2\n");
}

void setClientName(Client* c, int i)
{
    char n[NUME_MAX];
    int k,r;
    printf("\nIntroduceti numarul clientului: ");
    scanf("%d", &k);
    while(k>i||k<=0)
    {
        printf("\nNu exista niciun client cu acest numar. Doriti sa incercati din nou? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
        scanf("%d", &r);
        while(r!=0&&r!=1)
        {
            printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca sa incercati din nou / cifra 0 daca nu: ");
            scanf("%d", &r);
        }
        if(r==1)
        {
            printf("\nIntroduceti numarul clientului: ");
            scanf("%d", &k);
        }
        else
            return;
    }
    printf("\nIntorduceti noul nume de forma Nume_Prenume: ");
    scanf("%29s",n);
    strcpy(c[k-1].nume, n);
    printf("\nNume modificat cu succes!\n");
}

void setClientBalance(Client *c, int i)
{
    float b;
    int k,r;
    printf("\nIntroduceti numarul clientului: ");
    scanf("%d", &k);
    while(k>i||k<=0)
    {
        printf("\nNu exista niciun client cu acest numar. Doriti sa incercati din nou? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
        scanf("%d", &r);
        while(r!=0&&r!=1)
        {
            printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca sa incercati din nou / cifra 0 daca nu: ");
            scanf("%d", &r);
        }
        if(r==1)
        {
            printf("\nIntroduceti numarul clientului: ");
            scanf("%d", &k);
        }
        else
            return;
    }
    printf("\nIntorduceti noua balanta: ");
    scanf("%f",&b);
    c[k-1].balanta=b;
    printf("\nBalanta modificata cu succes!\n");
}

void deleteClient(Client **c, int *i)
{
    int k,index=-1;
    char n[NUME_MAX];

    printf("\nIntroduceti numele clientului: ");
    scanf("%29s", n);

    for(k=0; k<*i; k++)
        if(strcmp((*c)[k].nume, n)==0)
        {
            index=k;
        }
    if(index==-1)
    {
        printf("\nClientul nu exista! Pentru a incerca din nou introduceti cifra 2\n");
    }
    else
    {
        for( int j=0; j<(*i); j++)
        {
            if(strcmp((*c)[j].nume, n) == 0)
            {
                memset((*c)[j].nume,0,sizeof((*c)[j].nume));
                for (k = index ; k < *i - 1; k++)
                {
                    (*c)[k] = (*c)[k + 1];
                }
                (*i)--;
                *c = realloc(*c, (*i) * sizeof(Client));
                break;
            }
        }
        printf("\nClient sters cu succes!\n");
    }
}


void Transfer(Banca b, Client *c, int i)
{
    int k,j,r;
    float s;
    puts("________________________________\n");
    printf("Alege numarul clientului care trasnfera banii: ");
    scanf("%d", &k);
    while(k>i||k<=0)
    {
        printf("\nNu exista niciun client cu acest numar. Doriti sa incercati din nou? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
        scanf("%d", &r);
        while(r!=0&&r!=1)
        {
            printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca sa incercati din nou / cifra 0 daca nu: ");
            scanf("%d", &r);
        }
        if(r==1)
        {
            printf("\nIntroduceti numarul clientului: ");
            scanf("%d", &k);
        }
        else
            return;
    }
    printf("\nAlege numarul clientului care primeste banii: ");
    scanf("%d", &j);
    while(j>i||j<=0||j==k)
    {
        if(j==k)
        {
            printf("\nNu se pot trasnfera bani de la un client spre acelasi client. Doriti sa incercati din nou? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
            scanf("%d", &r);
            while(r!=0&&r!=1)
            {
                printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca sa incercati din nou / cifra 0 daca nu: ");
                scanf("%d", &r);
            }
            if(r==1)
            {
                printf("\nIntroduceti numarul clientului: ");
                scanf("%d", &j);
            }
            else
                return;
        }
        else

            printf("\nNu exista niciun client cu acest numar. Doriti sa incercati din nou? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
        scanf("%d", &r);
        while(r!=0&&r!=1)
        {
            printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca sa incercati din nou / cifra 0 daca nu: ");
            scanf("%d", &r);
        }
        if(r==1)
        {
            printf("\nIntroduceti numarul clientului: ");
            scanf("%d", &j);
        }
        else
            return;
    }

    printf("\nAlege suma: ");
    scanf("%f", &s);
    b.comision = 0.03 * s;
    while(s>(c[i-1].balanta-b.comision))
    {
        printf("\nClientul %s nu are suficienti bani pentru acest trasnfer! Introduceti o suma mai mica: ", c[i-1].nume);
        scanf("%f", &s);
        b.comision = 0.03 * s;
    }
    c[i-1].balanta = c[i-1].balanta - s - b.comision;
    c[j-1].balanta += s;

    printf("\nComision suportat de catre clientul %s: %.2f\n",c[i-1].nume, b.comision);
    printf("\nTrasnfer efectuat cu succes!\n");
}

void addClient(Client **c, unsigned int i)
{

    char n[NUME_MAX];
    float b;

    FILE *f = fopen ("nume_balanta.txt","a+");
    *c =realloc(*c, (i+1)*sizeof(Client));
    printf("\nIntrodu numele de forma Nume_Prenume: ");
    scanf("%29s", n);
    fprintf(f,"\n%s ",n);
    strcpy((*c)[i].nume, n);
    printf("\nIntrodu balanta: ");
    scanf("%f", &b);
    fprintf(f,"%.2f",b);
    (*c)[i].balanta=b;
    printf("\nClient adaugat cu succes!\n");

    fclose(f);


}

void loan (Banca *b, Client *c, int j)
{
    int i;
    float s;
    printf("\nSuma detinuta de banca inainte de imprumut: %.2lf\n", b->balanta_banca);
    printf("\nAlege numarul clientului care face imprumutul: ");
    scanf("%d", &i);
    if(i>j||i<=0)
    {
        printf("\nClientul nu exista! Pentru a incerca din nou tastati cifra 8.\n");
        return;
    }
    if(c[i-1].imprumut_activ==1)
        printf("\nClientul are deja un imprumut activ! Pentru un nou imprumut, trebuie achitate ratele.\n ");
    else
    {
        printf("\nAlege valoarea imprumutului: ");
        scanf("%f", &s);
        b->balanta_banca -= s;
        c[i-1].balanta += s;
        c[i-1].imprumut_activ = 1;
        c[i-1].imprumut = s;
        printf("\nSuma detinuta de banca dupa imprumut: %.2lf\n", b->balanta_banca);
        printf("\nImprumut realizat cu succes!\n");
    }

}

void rate(Client* c, int i)
{
    if(getClientListLoans(c,i)==0)
        return;
    else
    {
        float s;
        int n,k,r;
        float dobanda;
        printf("\nIntroduceti numarul clientului: ");
        scanf("%d", &k);
        dobanda=c[k-1].imprumut*0.13*0.08;  // Dobanda=sold de rambursat * rata dobanzii (13%) * nr de zile/360 (30/360)
        s=dobanda+c[k-1].imprumut;
        printf("\nRatele se pot achita pe 12 / 24 / 60 luni. Introduceti numarul de luni in care doriti sa achitati ratele: ");
        scanf("%d",&n);
        while(n!=12&&n!=24&&n!=60)
        {
            printf("\nNu se pot achita ratele in numarul de luni alese de dumeneavoastra! Alegeti intre 12 / 24 / 60 luni: ");
            scanf("%d", &n);
        }
        printf("\nSuma de platit %.2f timp de %d luni\n", s/n, n);
        if(c[k-1].balanta<s)
        {
            printf("\nNu sunt suficienti bani pentru a se plati ratele.\n");
            return;
        }
        else
            printf("\nDoriti sa achitati acum ratele? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
        scanf("%d", &r);
        while(r!=0&&r!=1)
        {
            printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca doriti achitarea ratelor / cifra 0 daca nu: ");
            scanf("%d", &r);
        }
        if(r==1)
        {
            c[k-1].balanta -= s;
            c[k-1].imprumut_activ = 0;
            printf("\nRate achitate cu succes!\n");
        }
        else return;
    }

}

void extras(Client *c, int i)
{
    int i2,r,j;

    i2=0;
    while(i2<i)
    {
        if(strcmp(c[i2].nume,""))
            printf("%d - %s\n", i2+1, c[i2].nume);
        i2++;
    }

    printf("\nIntroduceti numarul clientului: ");
    scanf("%d", &j);
    while(j>i)
    {
        printf("\nNu exista niciun client cu acest numar. Doriti sa incercati din nou? Introduceti cifra 1 daca da / cifra 0 daca nu: ");
        scanf("%d", &r);
        while(r!=0&&r!=1)
        {
            printf("\nAti introdus o valoare gresita! Introduceti cifra 1 daca sa incercati din nou / cifra 0 daca nu: ");
            scanf("%d", &r);
        }
        if(r==1)
        {
            printf("\nIntroduceti numarul clientului: ");
            scanf("%d", &j);
        }
        else
            return;

    }

    time_t current_time=time(NULL);
    struct tm*tm =localtime(&current_time);
   char buffer[20];
strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm);
printf("\n\t\t\t\tExtras de cont generat in data : %s\n", buffer);
    printf("\nCod IBAN: %u\n", c[j-1].iban);
    printf("\nProduse in valuta RON\n");
    printf("\nTitular: %s\n", c[j-1].nume);
    if(c[j-1].balanta2-c[j-1].balanta>0)
        printf("\nDiferenta fata de soldul initial: %.2f\n", -c[j-1].balanta2+c[j-1].balanta);
    else
        printf("\nDiferenta fata de soldul initial: %.2f\n", c[j-1].balanta2-c[j-1].balanta);
    printf("\nSold disponibil: %.2f\n", c[j-1].balanta);

}

void IBAN_Gen(Client *c, unsigned int i)
{
    srand(time(NULL));

    unsigned int j=0;

    while(j<i)
    {
        c[j].iban=(rand()%(10000000000-1000000000+1))+1000000000;
        j++;

    }
}

void IBAN_Gen1pers(Client *c, unsigned int i)
{
    srand(time(NULL));
    c[i-1].iban=(rand()%(10000000000-1000000000+1))+1000000000;

}

void comenziClienti(void)
{
    puts("________________________________\n");
    printf("1 - Lista clienti\n");
    printf("2 - Detalii despre un client\n");
    printf("3 - Adaugare client\n");
    printf("4 - Modificare nume client\n");
    printf("5 - Modificare balanta client\n");
    printf("6 - Stergere client\n");
    printf("7 - Transfer\n");
    printf("8 - Imprumut de la banca\n");
    printf("9 - Achitare rate\n");
    printf("10 - Generare extras de cont\n\n");
}


int main()
{
    Banca b;
    int i=5;
    Client *c = NULL;

    b.balanta_banca=1E9;

    createClients(&c,i);
    IBAN_Gen(c,i);
    comenziClienti();

    int choice;
    scanf("%d", &choice);

    while (choice)
    {

        switch (choice)
        {
        case lista_clienti:
            puts("________________________________\n");
            getClientList(c,i);
            comenziClienti();
            break;
        case detalii_client:
            puts("________________________________\n");
            getClientInfo(c,i);
            comenziClienti();
            break;
        case adaugare_client:
            puts("________________________________\n");
            addClient(&c,i);
            i++;
            IBAN_Gen1pers(c,i);
            comenziClienti();
            break;
        case modificare_nume:
            puts("________________________________\n");
            getClientList(c,i);
            setClientName(c,i);
            comenziClienti();
            break;
        case modificare_balanta:
            puts("________________________________\n");
            getClientList(c,i);
            setClientBalance(c,i);
            comenziClienti();
            break;
        case stergere_client:
            puts("________________________________\n");
            getClientList(c,i);
            deleteClient(&c,&i);
            comenziClienti();
            break;
        case transfer:
            puts("________________________________\n");
            getClientList(c,i);
            Transfer(b,c,i);
            comenziClienti();
            break;
        case imprumut:
            puts("________________________________\n");
            getClientList(c,i);
            loan(&b,c,i);
            comenziClienti();
            break;
        case achitare_rate:
            puts("________________________________\n");
            rate(c,i);
            comenziClienti();
            break;
        case extras_de_cont:
            puts("________________________________\n");
            extras(c,i);
            comenziClienti();
            break;
        default:
            break;
        }
        scanf("%d", &choice);
    }
    free(c);
    return 0;
}
