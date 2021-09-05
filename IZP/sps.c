#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct
{
    char* MassivZnaku;
}Bunka;
typedef struct
{
    Bunka* MassivBunek;
}Line;
typedef struct
{
    int PocetRadku;
    int PocetSloupcu;
    Line* MassivLine;
}Tabulka;

int PocetBunek(char* massiv_bunek,char delitel1 ,char delitel2)
{
    int pocet_znaku=strlen(massiv_bunek);
    int kolvo_bunek=0;
    if(delitel1==delitel2)
    {
        for(int i=0;i<pocet_znaku+1;i++)
        {
            if(massiv_bunek[i]==delitel1)
            {
                kolvo_bunek++;
            }
        }
        if (kolvo_bunek!=0)
        {
            return(kolvo_bunek);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        for (int i=0;i<pocet_znaku+1;i++)
        {
            if(massiv_bunek[i]==delitel1 || massiv_bunek[i]==delitel2)
            {
                kolvo_bunek++;
            }
        }
        if (kolvo_bunek!=0)
        {
            return(kolvo_bunek);
        }
        else
        {
            return 0;
        }
    }
}

int pocet_symbolu_v_bunce(char* massiv,char delitel,int index)
{
    int pocet=0;
    for(int i=index;massiv[i]!=delitel;i++)
    {
        pocet++;
    }
    return(pocet);
}

void DeleteKonec(char* massiv)
{
    int a=strlen(massiv);
    for(int i=a-1;i>0;i--)
    {
        if (!(massiv[i]>=' ' && massiv[i]<='~'))
        {
            massiv[i]='\0';
        }
    }
}
//'[1,_];irow;[1,1];set Jmeno;[1,2];set Prijmeni;[1,3];set Plat'
void RozdelovacNaBunky(char* retezec_prikazu,char* massiv[],int pocet12,char* delitel)
{
    massiv[0]=strtok(retezec_prikazu,delitel);
    for (int i=1;i<pocet12;i++)
    {
        massiv[i]=strtok(NULL,delitel);
    }
}

int PocetSymblouZeVstupu(char *file)
{
    FILE *mf;
    char sym;
    int pocet=0;
    mf = fopen (file,"r");
    if (mf == NULL) 
    {
        printf ("chyba vstupu\n"); 
        return -1;
    }
    //Čtení (po bajtech) dat ze souboru v nekonečném cyklusu
    while (1)
    {
        //Čtení jednoho bajtu ze souboru
        sym = fgetc (mf);
        pocet++;

        //Zkontrolujte konec souboru nebo chybu čtení
        if (sym == EOF)
        {
            // Zkontrolujeme, co se přesně stalo: soubor skončil nebo se jedná o chybu čtení
            if ( feof (mf) != 0)
            {  
                break;
            }
            else
            {
            //Pokud během čtení dojde k chybě, zobrazte chybovou zprávu a ukončete nekonečny cyklus
                printf ("\nChyba při čtení ze souboru\n");
                break;
            }
        }
    }
    if ( fclose (mf) == EOF) printf ("chyba vystupu\n");
    return(pocet);
}

void ZaplneniRetezcu(char *file,char* massiv)
{
    FILE *mf;
    mf = fopen (file,"r");
    int i=0;
    while(1)
    {
        massiv[i] = fgetc(mf);
        if(massiv[i]==EOF)
        {
            if( feof(mf) != 0)
            {
                break;
            }
            else
            {
                printf ("\nChyba při čtení ze souboru\n");
                break;
            }
        }
        i++;
    }
    if ( fclose (mf) == EOF) printf ("chyba vystupu\n");
}

void OddeleniPrvnimOddelovacem(char* massiv,char* DELIM,int pocets)
{
    int poc_znk_DELIM=strlen(DELIM);
    for (int i=0;i<pocets;i++)
    {
        for (int j=0;j<poc_znk_DELIM;j++)
        {
            if (massiv[i]==DELIM[j])
            {
                massiv[i]=DELIM[0];
            }
        }
    }
}



//Funkce pro upravu tabulki
void irow(Tabulka Table,int CisloRadku)
{
    Tabulka Table2;
    Table2.PocetRadku=Table.PocetRadku-CisloRadku+1;
    Table2.PocetSloupcu=Table.PocetSloupcu;
    Table2.MassivLine=malloc(sizeof(Line)*Table2.PocetRadku);
    int Ii=1;
    for(int i=CisloRadku+1;i<Table2.PocetRadku;i++)
    {
        Table2.MassivLine[Ii].MassivBunek=malloc(sizeof(Bunka)*Table2.PocetSloupcu);
        Table2.MassivLine[Ii]=Table.MassivLine[i-1];
        Ii++;
    }
    Table2.MassivLine[0].MassivBunek=malloc(sizeof(Bunka)*Table2.PocetSloupcu);     
    for(int i=0;i<Table2.PocetSloupcu;i++)
    {
        Table2.MassivLine[0].MassivBunek[i].MassivZnaku="\0";
    }
    Ii=0;
    for(int i=CisloRadku;i<Table.PocetRadku;i++)
    {
        Table.MassivLine[i]=Table2.MassivLine[Ii];
        Ii++;
    }
}
void arow(Tabulka Table,int CisloRadku)
{
    irow(Table,CisloRadku+1);
}
void drow(Tabulka Table,int CisloRadku)
{
    Tabulka Table2;
    Table2.PocetRadku=Table.PocetRadku-1;
    Table2.PocetSloupcu=Table.PocetSloupcu;
    Table2.MassivLine=malloc(sizeof(Line)*Table2.PocetRadku);
    int Ii=CisloRadku;
    for(int i=CisloRadku+1;i<Table.PocetRadku;i++)
    {
        Table2.MassivLine[Ii].MassivBunek=malloc(sizeof(Bunka)*Table.PocetSloupcu);
        Table2.MassivLine[Ii].MassivBunek=Table.MassivLine[i].MassivBunek;
        Ii++;
    }
    if (CisloRadku!=0)
    {
        for(int i=CisloRadku-1;i>=0;i--)
        {
            Table2.MassivLine[i].MassivBunek=malloc(sizeof(Bunka)*Table.PocetSloupcu);
            Table2.MassivLine[i].MassivBunek=Table.MassivLine[i].MassivBunek;
        }
    }
    for(int i=0;i<Table2.PocetRadku;i++)
    {
        Table.MassivLine[i].MassivBunek=Table2.MassivLine[i].MassivBunek;
    }
}


void icol(Tabulka Table,int CisloSloupcu)
{
    Tabulka Table2;
    Table2.PocetRadku=Table.PocetRadku;
    Table2.PocetSloupcu=Table.PocetSloupcu;
    Table2.MassivLine=malloc(sizeof(Line)*Table2.PocetRadku);
    int Ji;
    for(int i=0;i<Table.PocetRadku;i++)
    {   
        Table2.MassivLine[i].MassivBunek=malloc(sizeof(Bunka)*Table2.PocetSloupcu);
        Ji=0;
        for(int j=CisloSloupcu+1;j<Table2.PocetSloupcu;j++)
        {
            Table2.MassivLine[i].MassivBunek[Ji].MassivZnaku=Table.MassivLine[i].MassivBunek[j-1].MassivZnaku;
            Ji++;
        }
        Table2.MassivLine[i].MassivBunek[CisloSloupcu].MassivZnaku="";
    }
    for(int i=0;i<Table.PocetRadku;i++)
    {
        Ji=0;
        for(int j=CisloSloupcu+1;j<Table.PocetSloupcu;j++)
        {
            Table.MassivLine[i].MassivBunek[j]=Table2.MassivLine[i].MassivBunek[Ji];
            Ji++;
        }
    }
}

void acol(Tabulka Table,int CisloSloupcu)
{
    icol(Table,CisloSloupcu-1);
}

void dcol(Tabulka Table,int CisloSloupcu)
{
    Tabulka Table2;
    Table2.PocetRadku=Table.PocetRadku;
    Table2.PocetSloupcu=Table.PocetSloupcu-1;
    Table2.MassivLine=malloc(sizeof(Line)*Table2.PocetRadku);
    int Ji;
    for(int i=0;i<Table.PocetRadku;i++)
    {   
        Table2.MassivLine[i].MassivBunek=malloc(sizeof(Bunka)*Table2.PocetSloupcu);
        Ji=0;
        for(int j=CisloSloupcu;j<Table.PocetSloupcu;j++)
        {
            Table2.MassivLine[i].MassivBunek[Ji].MassivZnaku=Table.MassivLine[i].MassivBunek[j+1].MassivZnaku;
            Ji++;
        }
    }
    for(int i=0;i<Table.PocetRadku;i++)
    {
        Ji=0;
        for(int j=CisloSloupcu;j<Table.PocetSloupcu;j++)
        {
            Table.MassivLine[i].MassivBunek[j]=Table2.MassivLine[i].MassivBunek[Ji];
            Ji++;
        }
    }
}

void setSTR(Tabulka Table,int CisloRadku ,int CisloSloupcu ,char* Retezec)
{
    Table.MassivLine[CisloRadku].MassivBunek[CisloSloupcu].MassivZnaku=Retezec;
}

void clear(Tabulka Table,int CisloRadku ,int CisloSloupcu)
{
    Table.MassivLine[CisloRadku].MassivBunek[CisloSloupcu].MassivZnaku="\0";
}
void swap(Tabulka Table,int CisloRadku,int CisloSloupcu1,int CisloRadku2,int CisloSloupcu2)
{
    Bunka Buffer1;
    Bunka Buffer2;
    Buffer1=Table.MassivLine[CisloRadku].MassivBunek[CisloSloupcu1];
    Buffer2=Table.MassivLine[CisloRadku2].MassivBunek[CisloSloupcu2];
    Table.MassivLine[CisloRadku2].MassivBunek[CisloSloupcu2]=Buffer1;
    Table.MassivLine[CisloRadku].MassivBunek[CisloSloupcu1]=Buffer2;
}


int main(int argc,char *argv[])//argc ,argv argumenty ze prikazove radky 
{
    char* DELIM=" ";
    
    FILE *mf;
    mf = fopen (argv[4],"r+");

    if (argc>1)// 1. argument vzdy ./sheet
    {
        if (strcmp(argv[1],"-d")==0)
        {
            DELIM=argv[2];
            int a = strlen(DELIM);
            for(int i=0;i<a;i++)
            {             
                for(int j=i+1;j<a;j++)
                {               
                    if (DELIM[i]==DELIM[j])
                    {
                        fprintf(stderr,"Chyba vstupu : opakovajuci znak v retezce DELIM\n");
                    }
                }
            }
        }    
    }
    //Cteni failu
    int Pocet=PocetSymblouZeVstupu(argv[4]);

    if (Pocet == -1)
    {
        return 0;
    }

    char massiv[Pocet];

    ZaplneniRetezcu(argv[4],massiv);
    //Konec cteni

    //Pocet zakladnich udaju
    OddeleniPrvnimOddelovacem(massiv,DELIM,Pocet);
    //Pocet Radku
    int PocetRadku=PocetBunek(massiv,'\n','\0');
    if (PocetRadku==0)
    {
        fprintf(stderr,"Chyba : tabulka neobsahuje zadny radok");
        return 0;
    }

    char* MassivRadku[PocetRadku];
    //Zaplneni Radku
    RozdelovacNaBunky(massiv,MassivRadku,PocetRadku,"\n");
    DeleteKonec(MassivRadku[PocetRadku-1]);
    //Pocet Bunek
    int PocetBunekVRadku=PocetBunek(MassivRadku[0],DELIM[0],'\0');
    int PocetBunekVTabulke=(PocetBunekVRadku*PocetRadku);
    char* MassivBunek[PocetBunekVTabulke];

    char Delim[2]={DELIM[0],'\0'};

    //Zaplneni Bunek
    int IZnaku=0;
    for(int IRadku=0;IRadku<PocetRadku;IRadku++)
    {
        MassivBunek[IZnaku]=strtok(MassivRadku[IRadku],Delim);
        //printf("MassivBunek je %s\n",MassivBunek[IZnaku]);
        for(int i=IZnaku+1;i<(PocetBunekVRadku+IZnaku+1);i++)
        {
            MassivBunek[i]=strtok(NULL,Delim);
            //printf("MassivBunek je %s\n",MassivBunek[i]);
        }
        IZnaku=IZnaku+PocetBunekVRadku+1;
    }
    for(int IIndex=0 ; IIndex<PocetBunekVTabulke ; IIndex++)
    {
        if (MassivBunek[IIndex]==NULL)
        {
            for(int iindex=IIndex;iindex<=PocetBunekVTabulke;iindex++)
            {
                MassivBunek[iindex]=MassivBunek[iindex+1];
            }
        }
    }
    //Scitani prikazu
    if(argv[3][0]!='[')
    {
        fprintf(stderr,"Chyba vstupu : prikazy nezadany nebo zadanzy spatne");
    }
    int kolvo_bunek_prikradky=PocetBunek(argv[3],';','\0');
    if (kolvo_bunek_prikradky==0)
    {
        fprintf(stderr,"Chyba kolvo");
        return 0;
    }
    char *Prikazy[kolvo_bunek_prikradky];
    RozdelovacNaBunky(argv[3],Prikazy,kolvo_bunek_prikradky,";");
    
    
    
    
    //Zaplneni datove struktury tabulka.Pro lepsi komunikace z tabulkou.
    Tabulka Main;
    Main.PocetRadku=PocetRadku;
    Main.PocetSloupcu=PocetBunekVRadku;
    int i2=0;
    Main.MassivLine=malloc(sizeof(Line)*Main.PocetRadku);
    if (NULL == Main.MassivLine)
        {
            fprintf(stderr,"Chyba pameti");
            return 0;
        }
    for (int IRadku=0;IRadku<Main.PocetRadku;IRadku++)
    {
        Main.MassivLine[IRadku].MassivBunek=malloc(Main.PocetSloupcu*sizeof(Bunka));
        if (NULL == Main.MassivLine[IRadku].MassivBunek)
        {
            fprintf(stderr,"Chyba pameti");
            return 0;
        }
        for(int ISloupcu=0;ISloupcu<Main.PocetSloupcu;ISloupcu++)
        {
            Main.MassivLine[IRadku].MassivBunek[ISloupcu].MassivZnaku=MassivBunek[i2];
            //printf("massiv[%d][%d] je %s\n",IRadku,ISloupcu,Main.MassivLine[IRadku].MassivBunek[ISloupcu].MassivZnaku);
            i2++;
        }        
    }



    //konec zaplneni
    int R1=0;
    int C1=0;
    char Rc[3];
    char Cc[3];
    for (int i=0;i<kolvo_bunek_prikradky;i++)
    {
        if((Prikazy[i][0]=='[') && ((Prikazy[i][1]>='0' && Prikazy[i][1]<='9') || (Prikazy[i][1]=='_')))
        {
            int PocetZnaku=strlen(Prikazy[i]);
            if (PocetZnaku==5)
            {
                int Ii=0;
                int znak1;
                for(int j = 1 ;Prikazy[i][j]!=',';j++)
                {
                    Rc[Ii]=Prikazy[i][j];
                    Ii++;
                    znak1=j;
                }
                Rc[Ii+1]='\0';

                Ii=0;
                for(int j = znak1+2 ;Prikazy[i][j]!=']';j++)
                {
                    Cc[Ii]=Prikazy[i][j];
                    Ii++;
                }
                Cc[Ii+1]='\0';
                if(!(strcmp(Rc,"_")==0 || strcmp(Cc,"_")==0))
                {
                    R1=atoi(Rc);
                    C1=atoi(Cc);
                    if(R1<=0 || C1<=0)
                    {
                        fprintf(stderr,"Chyba vstupu: cislo radku nebo sloupcu nemuze byt mensi nez 1\n");
                        return 0;
                    }
                }
            }
        }

        if(strcmp(Prikazy[i],"irow")==0)
        {
            Main.PocetRadku=Main.PocetRadku+1;
            Main.MassivLine=realloc(Main.MassivLine,sizeof(Line)*(Main.PocetRadku));
            if(NULL==Main.MassivLine)
            {
                fprintf(stderr,"Chyba funkce: 1");
                return 0;
            }
            //Sprintf("R1 je %d",R1);
            irow(Main,R1-1);
        }
        if(strcmp(Prikazy[i],"arow")==0)
        {
            Main.PocetRadku=Main.PocetRadku+1;
            Main.MassivLine=realloc(Main.MassivLine,sizeof(Line)*(Main.PocetRadku));
            if(NULL==Main.MassivLine)
            {
                fprintf(stderr,"Chyba funkce: 2");
                return 0;
            }
            arow(Main,R1-1);
        }
        if(strcmp(Prikazy[i],"drow")==0)
        {
            drow(Main,R1-1);
            Main.PocetRadku=Main.PocetRadku-1;
            Main.MassivLine=realloc(Main.MassivLine,sizeof(Line)*(Main.PocetRadku));
            if(NULL==Main.MassivLine)
            {
                fprintf(stderr,"Chyba funkce: 3");
                return 0;
            }
        }
        if(strcmp(Prikazy[i],"icol")==0)
        {
            Main.PocetSloupcu=Main.PocetSloupcu+1;
            for(int i=0;i<Main.PocetRadku;i++)
            {
                Main.MassivLine[i].MassivBunek=realloc(Main.MassivLine[i].MassivBunek,sizeof(Bunka)*Main.PocetSloupcu);
                if(NULL==Main.MassivLine)
                {
                    fprintf(stderr,"Chyba funkce: 4");
                    return 0;
                }   
            }
            icol(Main,C1-1);
        }
        if(strcmp(Prikazy[i],"acol")==0)
        {
            Main.PocetSloupcu=Main.PocetSloupcu+1;
            for(int j=0;j<Main.PocetRadku;j++)
            {
                Main.MassivLine[j].MassivBunek=realloc(Main.MassivLine[j].MassivBunek,sizeof(Bunka)*Main.PocetSloupcu);   
                if(NULL==Main.MassivLine)
                {
                    fprintf(stderr,"Chyba funkce: 5");
                    return 0;
                }
            }
            acol(Main,C1-1);
        }
        if(strcmp(Prikazy[i],"dcol")==0)
        {
            Main.PocetSloupcu=Main.PocetSloupcu-1;
            for(int j=0;j<Main.PocetRadku;j++)
            {
                Main.MassivLine[j].MassivBunek=realloc(Main.MassivLine[j].MassivBunek,sizeof(Bunka)*Main.PocetSloupcu);
                if(NULL==Main.MassivLine)
                {
                    fprintf(stderr,"Chyba funkce: 6");
                    return 0;
                }
            }
            dcol(Main,C1-1);
        }
        if(!(strstr(Prikazy[i],"set")==NULL))
        {
            char *retezec[2];
            RozdelovacNaBunky(Prikazy[i],retezec,2," ");
            setSTR(Main,R1-1,C1-1,retezec[1]);
        }
        if(!(strstr(Prikazy[i],"clear")==NULL))
        {
            clear(Main,R1-1,C1-1);
        }
        if(!(strstr(Prikazy[i],"swap")==NULL))
        {
            char RR[3];
            char CC[3];
            int znak;
            int Ii=0;
            for(int j = 6 ;Prikazy[i][j]!=',';j++)
            {
                RR[Ii]=Prikazy[i][j];
                Ii++;
                znak=j;
            }
            RR[Ii+1]='\0';

            Ii=0;
            for(int j = znak+2 ;Prikazy[i][j]!=']';j++)
            {
                CC[Ii]=Prikazy[i][j];
                Ii++;
            }
            CC[Ii+1]='\0';

            int S=atoi(RR);
            int Ss=atoi(CC);
            swap(Main,R1-1,C1-1,S-1,Ss-1);
        }
    }

    
    //vyvod do prik radky

    /*
    for(int i=0;i<Main.PocetRadku;i++)
    {
        for(int j=0;j<Main.PocetSloupcu;j++)
        {
            printf("%s",Main.MassivLine[i].MassivBunek[j].MassivZnaku);
            if(j!=Main.PocetSloupcu-1)
            {
            printf("%c",DELIM[0]);
            }
        }
        if(i!=Main.PocetRadku-1)
        {
        printf("\n");
        }
    }
    */
    fclose(mf);
    mf=fopen(argv[4],"w");
    fclose(mf);
    mf=fopen(argv[4],"r+");

    //Vyvod Tabulky do failu.
    for(int i=0;i<Main.PocetRadku;i++)
    {
        for(int j=0;j<Main.PocetSloupcu;j++)
        {
            fprintf(mf,"%s",Main.MassivLine[i].MassivBunek[j].MassivZnaku);
            if(j!=Main.PocetSloupcu-1)
            {
            fprintf(mf,"%c",DELIM[0]);
            }
        }
        if(i!=Main.PocetRadku-1)
        {
        fprintf(mf,"\n");
        }
    }

    free(Main.MassivLine);
    for(int o=0;o<Main.PocetRadku;o++)
    {
        free(Main.MassivLine[o].MassivBunek);
    }

    fclose(mf);
    return 0;    
}