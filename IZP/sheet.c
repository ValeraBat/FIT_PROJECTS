#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define line_lenght 10242

/*v mem kodu jsou promenne: a je pocet znaku v retezce line , b je pocet znaku v retezce DELIM,
cisla R,N,M,C jsou promnenne ktere zavadi uzivatel,massiv to je retezec ze vstupniho failu , delitel to je retezec DELIM
Take bych se chtel omluvit za to ze neudelal jsem selekce radku , bohuzel nepodarilo mi to.
Jsem nikdy ne pracoval z ceckem ,proto myslim nepodarilo mi to.Takze omlouvam se.*/

void insert_row(char* massiv,char* delitel,int count,int R)//irow R - vloží řádek tabulky před řádek R > 0 (insert-row).
{   
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    char m;
    for(int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                m=delitel[j];
            }
        }
    }
    if (count==R)//count tady pocet radku
    {
        printf("%c\n",m);
    }
}

void append_row(char* massiv,char* delitel)
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    for(int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if (massiv[i]==delitel[j] && massiv[i+1]=='\0' && massiv[i+1]!='\n')
            {
                massiv[i+1]='\n';
                massiv[i+2]=delitel[j];
                massiv[i+3]='\0';
            }
        }
    }
}


void delete_row(char* massiv,int count,int R)//odstraní řádek číslo R > 0 (delete-row).
{
    int a = strlen(massiv)+1;
    if (count==R)//count tady pocet radku
    {
        for (int i=0;i<a;i++)
        {
            massiv[i]='\0';
        }
    }
}

void delete_rows(char* massiv,int count,int N,int M)//odstraní řádky N až M (N <= M). V případě N=M se příkaz chová stejně jako drow N.
{
    int a = strlen(massiv)+1;
    if (count==N && count==M)//count tady pocet radku
    {
        for (int i=0;i<a;i++)
        {
            massiv[i]='\0';
        }
    }
    for (int i=N;i<=M;i++)
    {
        if (count == i)//count tady pocet radku
        {
            for (int j=0;j<a;j++)
            {
                massiv[j]='\0';
            }
        }
    }
    
}

void insert_col(char* massiv , char* delitel , int C)//icol C - vloží prázdný sloupec před sloupec daný číslem C.
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    int count=0;//count tady pocet sloupcu
    if(C==1)
    {  
        char m;//m tady znak delitelu
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }         
        for (int l=a-1;l>=0;l--)
        massiv[l+1] = massiv[l];
        massiv[0]=m;      
    }
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    char m;
                    m=massiv[i+1];
                    massiv[i+1]=delitel[j];
                    for(int l=i;l<a;l++)
                    {
                        char o=m;
                        m=massiv[l+2];
                        massiv[l+2]=o;
                    }
                }
            }
        }
    }         
}

void append_col(char* massiv,char* delitel)//přidá prázdný sloupec za poslední sloupec.
{
    int a = strlen(massiv)+1;
    char m = massiv[a-2];//m tady znak ktery stoji pred znakem '\n'
    massiv[a-2]=delitel[0];
    massiv[a-1]=m;
    massiv[a]='\0';
}

void delite_col(char* massiv, char* delitel, int C)//odstraní sloupec číslo C.
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    int count=0;//count tady pocet sloupcu
    if (C==1)
    {
        char m;//m tady znak delitelu
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
         int pocet=0;
        for (int i=0;massiv[i]!=m;i++)
            {
                pocet++;
            }
        for (int i=0;i<a;i++)
        {
            massiv[i]=massiv[i+pocet+1];
        }
    }

    for (int i=0;i<a;i++)
    {   
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    int pocet=0;
                    for (int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        pocet++;
                    }
                    for (int k=i+1;k<a;k++)
                    {
                        massiv[k]=massiv[k+pocet+1];
                    }
                }
            }
        }
    }
}

void delite_cols(char* massiv,char* delitel,int N,int M)//odstraní sloupce N až M (N <= M). V případě N=M se příkaz chová stejně jako dcol N.
{
    int a =strlen(massiv);
    int b =strlen(delitel);
    if (N==1)
    {
        char m;//m tady znak delitelu 
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
        int pocet=0;//pocet tady pocet znaku v bunce 
        for (int i=0;massiv[i]!=m;i++)
            {
                pocet++;
            }
        for (int i=0;i<a;i++)
        {
            massiv[i]=massiv[i+pocet];
        }
    }
    for(int l=N;l<=M;l++)
    {
        delite_col(massiv,delitel,N);
    }
}

void tlower(char* massiv,char* delitel,int C)//řetězec ve sloupci C bude převeden na malá písmena.
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    int count=0;//count tady pocet delitelu a v dalsich funkcich to bude stejne 
    if(C==1)
    {
        char m;//m tady znak delitelu a v dalsich funkcich to bude stejne 
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
            for(int k=0;massiv[k]!=m;k++)
            {
                if(massiv[k]>='A' && massiv[k]<='Z')
                {
                    massiv[k]=massiv[k]+32;
                }
            }
    }
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        if(massiv[k]>='A' && massiv[k]<='Z')
                        {
                            massiv[k]=massiv[k]+32;
                        }
                    }
                }
            }
        }
    }
}

void tupper(char* massiv,char* delitel,int C)//řetězec ve sloupce C bude převeden na velká písmena.
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    int count=0;
    if(C==1)
    {
        char m;
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
            for(int k=0;massiv[k]!=m;k++)
            {
                if(massiv[k]>='a' && massiv[k]<='z')
                {
                    massiv[k]=massiv[k]-32;
                }
            }
    }
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        if(massiv[k]>='a' && massiv[k]<='z')
                        {
                            massiv[k]=massiv[k]-32;
                        }
                    }
                }
            }
        }
    }
}
void integer(char* massiv,char* delitel,int C)//odstraní desetinnou část čísla ve sloupci C.
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    int count=0;
    if(C==1)
    {
        char m;
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
        int p=1;//p tady pocet cisel v bunce po '.' a v dalsi funkce to stejne 
        int p1=0;//p1 tady pocet cisel pred teckou v bunce a v dalsi funcke to stejne 
        while(massiv[p1]!='.')
        {
            p1++;
        }
        for (int i=0;massiv[i]!=m;i++)
        {
              if(massiv[i]=='.' && massiv[i+1]>='0' && massiv[i+1]<='9' && massiv[i-1]>='0' && massiv[i-1]<='9')
              {
                for(int k=i+1;massiv[k]>='0' && massiv[k]<='9';k++)
                {
                    p++;
                }
              }
        }
        for(int l=p1;l<a;l++)
        {
            massiv[l]=massiv[l+p];
        } 
    }
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    int p1=0;
                    int p=1;
                    for (int index=i+1;massiv[index]!='.';index++)
                    {
                        p1++;
                    }
                    for (int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        if((massiv[k]=='.' || massiv[k]==',') && (massiv[k+1]>='0' && massiv[k+1]<='9' && massiv[k-1]>='0' && massiv[k-1]<='9'))
                        {
                            for(int k1=k+1;massiv[k1]>='0' && massiv[k1]<='9';k1++)
                            {
                                p++;
                            }
                        }
                    }
                    for(int l=p1+i+1;l<a;l++)
                    {
                        massiv[l]=massiv[l+p];
                    }
                }
            }
        }
    }    
}

void rund(char* massiv,char* delitel,int C)//ve sloupci C zaokrouhlí číslo na celé číslo.
{
    int a =strlen(massiv)+1;
    int b =strlen(delitel);
    int count=0;
    if(C==1)
    {
        char m;
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
        int p=1;
        int p1=0;
        while(massiv[p1]!='.')
        {
            p1++;
        }
        for (int i=0;massiv[i]!=m;i++)
        {
            if(massiv[i]=='.' && massiv[i+1]>='0' && massiv[i+1]<='9' && massiv[i-1]>='0' && massiv[i-1]<='9')
            {
                for(int k=i+1;massiv[k]>='0' && massiv[k]<='9';k++)
                {
                    p++;
                }
                for(int k1=p1+p;k1>p1+1;k1--)
                {
                    if(massiv[k1]>='5')
                    {
                        massiv[k1-1]=massiv[k1-1]+1;
                    }
                }
                if (massiv[i+1]>='5')
                {
                    massiv[i-1]=massiv[i-1]+1;
                }
            }
        }
        for(int l=p1;l<a;l++)
        {
            massiv[l]=massiv[l+p];
        }
    }
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    int p1=0;
                    int p=1;
                    for (int index=i+1;massiv[index]!='.';index++)
                    {
                        p1++;
                    }
                    for (int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        if((massiv[k]=='.' || massiv[k]==',') && (massiv[k+1]>='0' && massiv[k+1]<='9' && massiv[k-1]>='0' && massiv[k-1]<='9'))
                        {
                            for(int k1=k+1;massiv[k1]>='0' && massiv[k1]<='9';k1++)
                            {
                                p++;
                            }
                            for(int k2=p1+p+k-2;k2>p1+k+1;k2--)
                            {
                                if(massiv[k2]>='5')
                                {
                                    massiv[k2-1]=massiv[k2-1]+1;
                                }
                            }
                            if (massiv[k+1]>='5')
                            {
                                massiv[k-1]=massiv[k-1]+1;
                            }
                        }
                    }
                    for(int l=p1+i+1;l<a;l++)
                    {
                        massiv[l]=massiv[l+p];
                    }
                }
            }
        }
    }
}

void cset(char* massiv,char *delitel ,int C, char* STR) //cset C STR - do buňky ve sloupci C bude nastaven řetězec STR.
{
    int a = strlen(massiv);
    int b = strlen(delitel);
    int c = strlen(STR);//c tady delka retezcu STR
    int count=0;
    if(C==1)
    {
        char m;
        for (int i=0;i<a;i++)
        {
            for (int j=0;j<b;j++)
            {
                if(massiv[i]==delitel[j])
                {
                    m=delitel[j];
                }
            }
        }
        int p=0;//p tady pocet znaku v bunce 
        for(int l=0;massiv[l]!=m;l++)
            {
                p++;
            }
        for (int l=a-1;l>=0;l--)
        massiv[l+c] = massiv[l+p];
        for(int i=0;i<c;i++)
        {
            massiv[i]=STR[i];
        }
    }
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==C-1)
                {
                    int p=0;
                    for(int l=i+1;(massiv[l]!=delitel[j]) && !(massiv[l]=='\0' || massiv[l]=='\n');l++)
                    {
                        p++;
                    }
                    for (int l=a-1;l>=i+1;l--)
                    {
                    massiv[l+c] = massiv[l+p];
                    }
                    int k1=0;
                    for(int k=i+1;k<c+i+1;k++)
                    {
                        massiv[k]=STR[k1];
                        k1++;
                    }
                }
            }
        }
    }
}

void copy(char* massiv,char* delitel,int N,int M)//přepíše obsah buněk ve sloupci M hodnotami ze sloupce N
{
    int a = strlen(massiv);
    int b = strlen(delitel);
    int count=0;
    int delka_slova=0;//delka_slova tady pocet znaku v bunce a v dalsich funkcich to bude stejne 

    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==N)
                {
                    if(count==1)
                    {
                        for(int k=i-1;k>=0;k--)
                        {
                            delka_slova++;
                        }
                    }
                    else
                    {
                        for(int k=i-1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k--)
                        {
                            delka_slova++;
                        }
                    }
                }
            }
        }
    }
    char copy[delka_slova];
    count=0;
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if(count==1)
                {
                    int p=i-1;
                    for(int k=i-1;k>=0;k--)
                    {
                        copy[p]=massiv[k];
                        p--;
                    }
                }
                if (count==N-1)    
                {
                    int p=0;
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {    
                        copy[p]=massiv[k];
                        p++;
                    }
                }
            }
        }
    }
    cset(massiv,delitel,M,copy);
}

void swap(char* massiv,char* delitel,int N,int M)//zamění hodnoty buněk ve sloupcích N a M.
{
    int a = strlen(massiv);
    int b = strlen(delitel);
    int count=0;
    int delka_slova=0;

    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==N)
                {
                    if(count==1)
                    {
                        for(int k=i-1;k>=0;k--)
                        {
                            delka_slova++;
                        }
                    }
                }
                if (count==N-1)
                {
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        delka_slova++;
                    }
                }
            }
        }
    }
    char copy[delka_slova];
    count=0;
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==N)
                {
                    if(count==1)
                    {
                        int p=i-1;
                        for(int k=i-1;k>=0;k--)
                        {
                            copy[p]=massiv[k];
                            p--;
                        }
                    }
                }
                if (count==N-1)
                {
                    int p=0;
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        copy[p]=massiv[k];
                        p++;
                    }
                }
            }
        }
    }
    count=0;
    int delka_slova1=0;
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==M)
                {
                    if(count==1)
                    {
                        for(int k=i-1;k>=0;k--)
                        {
                            delka_slova1++;
                        }
                    }
                }
                if(count==M-1)
                {
                    for(int k=i-1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k--)
                    {
                        delka_slova1++;
                    }
                }
                
            }
        }
    }
    char copy1[delka_slova1];
    count=0;
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==N)
                {
                    if(count==1)
                    {
                        int p=i-1;
                        for(int k=i-1;k>=0;k--)
                        {
                            copy1[p]=massiv[k];
                            p--;
                        }
                    }
                }
                if (count==M-1)    
                {
                    int p=0;
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {    
                        copy1[p]=massiv[k];
                        p++;
                    }
                }
            }
        }
    }
    cset(massiv,delitel,M,copy);
    cset(massiv,delitel,N,copy1);
}

void move(char* massiv,char* delitel,int N,int M)//move N M - přesune sloupec N před sloupec M.
{
    int a = strlen(massiv);
    int b = strlen(delitel);
    int count=0;
    int delka_slova=0;

    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==N)
                {
                    if(count==1)
                    {
                        for(int k=i-1;k>=0;k--)
                        {
                            delka_slova++;
                        }
                    }
                }
                if (count==N-1)
                {
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        delka_slova++;
                    }
                }
            }
        }
    }
    char copy[delka_slova];
    count=0;
    for (int i=0;i<a;i++)
    {
        for (int j=0;j<b;j++)
        {
            if(massiv[i]==delitel[j])
            {
                count++;
                if (count==N)
                {
                    if(count==1)
                    {
                        int p=i-1;
                        for(int k=i-1;k>=0;k--)
                        {
                            copy[p]=massiv[k];
                            p--;
                        }
                    }
                }
                if (count==N-1)
                {
                    int p=0;
                    for(int k=i+1;(massiv[k]!=delitel[j]) && !(massiv[k]=='\0' || massiv[k]=='\n');k++)
                    {
                        copy[p]=massiv[k];
                        p++;
                    }
                }
            }
        }
    }
    if (N<M)
    {
    insert_col(massiv,delitel,M);
    cset(massiv,delitel,M,copy);
    delite_col(massiv,delitel,N);
    }
    if (N>M)
    {
        insert_col(massiv,delitel,M);
        cset(massiv,delitel,M,copy);
        delite_col(massiv,delitel,N+1);
    }
}
/* DELIM retezec delitelu , line retezec znaku ze failu */
int main(int argc,char *argv[])//argc ,argv argumenty ze prikazove radky 
{
    
    char* DELIM=" ";
    
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
        else 
        {
            fprintf(stderr,"Chyba vstupu : delimetr ne zadan");
        }       
    }

    int pocet_prikazu_spracovani_dat=0;

    if(argc>1)
    {
        for(int i=0;i<argc;i++)
        {
            if (strcmp(argv[i],"cset")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"tolower")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"toupper")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"round")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"int")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"copy")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"swap")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
            if (strcmp(argv[i],"move")==0)
            {
                pocet_prikazu_spracovani_dat++;
            }
        }
    }

    if (pocet_prikazu_spracovani_dat>1)
    {
        fprintf(stderr,"Chyba vstupu : pocet prikazu pro spracovani dat nemuze byt vic nez 1\n");
        return 0;
    }
    char line[line_lenght];
    int count=0;//count tady pocet radku
        while(fgets(line,line_lenght,stdin)!=NULL)
    {  
        int a=strlen(line)+1;
        for(int i = 0;i<a;i++)
        {
            if(line[i]=='\0')
            {
                count++;
            }
        }
        for(int i=0;i<argc;i++)
        {
            if (argc>2)
            {
                if (strcmp(argv[i],"irow")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int R=atoi(argv[i+1]);
                        if (R < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument R (cislo radku) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        insert_row(line,DELIM,count,R);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument R (cislo radku) nezadan\n");
                    }
                }
                if (strcmp(argv[i],"arow")==0)
                {
                    append_row(line,DELIM);
                }
                if (strcmp(argv[i],"drow")==0)
                {  
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int R=atoi(argv[i+1]);
                        if (R < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument R (cislo radku) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        delete_row(line,count,R);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument R(cislo radku) nezadan\n");
                    }
                }
                if (strcmp(argv[i],"drows")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {    
                        int N = atoi(argv[i+1]);
                        if (N <= 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument N (cislo radku) nemuze byt mensi nez 1\n");
                            return 0;
                        }
                        if (argv[i+2][0]>='0' && argv[i+2][0]<='9')
                        {
                            int M = atoi(argv[i+2]);
                            if (M <= 0)
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo radku) nemuze byt mensi nez 1\n");
                                return 0;
                            }
                            if(M>=N)
                            {
                            delete_rows(line,count,N,M);
                            }
                            else
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo radku) nemuze byt mensi nez N (cislo radku)\n");
                            }
                        }
                        else
                        {
                            fprintf(stderr,"Chyba vstupu : argument M (cislo radku) nezadan\n");
                            return 0;
                        }
                    }
                    else 
                    {
                        fprintf(stderr,"Chyba vstupu : argument N (cislo radku) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"icol")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        insert_col(line,DELIM,C);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                    }
                }
                if(strcmp(argv[i],"acol")==0)
                {
                    append_col(line,DELIM);
                }
                if (strcmp(argv[i],"dcol")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        delite_col(line,DELIM,C);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                    }
                }
                if (strcmp(argv[i],"dcols")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {    
                        int N = atoi(argv[i+1]);
                        if (N <= 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nemuze byt mensi nez 1\n");
                            return 0;
                        }
                        if (argv[i+2][0]>='0' && argv[i+2][0]<='9')
                        {
                            int M = atoi(argv[i+2]);
                            if (M <= 0)
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nemuze byt mensi nez 1\n");
                                return 0;
                            }
                            if(M>=N)
                            {
                            delite_cols(line,DELIM,N,M);
                            }
                            else
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nemuze byt mensi nez N (cislo sloupcu)\n");
                            }
                        }
                        else
                        {
                            fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nezadan\n");
                            return 0;
                        }
                    }
                    else 
                    {
                        fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"cset")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        char* STR = argv[i+2];
                        cset(line,DELIM,C,STR);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"tolower")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        tlower(line,DELIM,C);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"toupper")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        tupper(line,DELIM,C);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"int")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        integer(line,DELIM,C);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"round")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {
                        int C=atoi(argv[i+1]);
                        if (C < 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nemuze byt mensi nez 0\n");
                            return 0;
                        }
                        rund(line,DELIM,C);
                        }
                    else
                    {
                        fprintf(stderr,"Chyba vstupu : argument C (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"copy")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {    
                        int N = atoi(argv[i+1]);
                        if (N <= 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nemuze byt mensi nez 1\n");
                            return 0;
                        }
                        if (argv[i+2][0]>='0' && argv[i+2][0]<='9')
                        {
                            int M = atoi(argv[i+2]);
                            if (M <= 0)
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nemuze byt mensi nez 1\n");
                                return 0;
                            }
                            copy(line,DELIM,N,M);
                        }
                        else
                        {
                            fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nezadan\n");
                            return 0;
                        }
                    }
                    else 
                    {
                        fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"swap")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {    
                        int N = atoi(argv[i+1]);
                        if (N <= 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nemuze byt mensi nez 1\n");
                            return 0;
                        }
                        if (argv[i+2][0]>='0' && argv[i+2][0]<='9')
                        {
                            int M = atoi(argv[i+2]);
                            if (M <= 0)
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nemuze byt mensi nez 1\n");
                                return 0;
                            }
                            swap(line,DELIM,N,M);
                        }
                        else
                        {
                            fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nezadan\n");
                            return 0;
                        }
                    }
                    else 
                    {
                        fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
                if (strcmp(argv[i],"move")==0)
                {
                    if (argv[i+1][0]>='0' && argv[i+1][0]<='9')
                    {    
                        int N = atoi(argv[i+1]);
                        if (N <= 0)
                        {
                            fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nemuze byt mensi nez 1\n");
                            return 0;
                        }
                        if (argv[i+2][0]>='0' && argv[i+2][0]<='9')
                        {
                            int M = atoi(argv[i+2]);
                            if (M <= 0)
                            {
                                fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nemuze byt mensi nez 1\n");
                                return 0;
                            }
                            move(line,DELIM,N,M);;
                        }
                        else
                        {
                            fprintf(stderr,"Chyba vstupu : argument M (cislo sloupcu) nezadan\n");
                            return 0;
                        }
                    }
                    else 
                    {
                        fprintf(stderr,"Chyba vstupu : argument N (cislo sloupcu) nezadan\n");
                        return 0;
                    }
                }
            }
        }
        printf("%s",line);
    }
return 0;   
}