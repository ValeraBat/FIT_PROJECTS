# HTTP SERVER
#### Autor : Valerii Batykov
#
### Funkce aplikace
Moje aplikace má funkcionalitu jednoduchého http serveru. Je schopen reagovat na požadavky GET. 
Existují tři hlavní příkazy: cpu-name, hostname, load. 
- Load ukazuje míru zatížení procesoru. 
- Hostname vrací síťové jméno počítače včetně domény. 
- Cpu-name vrací jmeno processoru.

### Návrh programu

Aby server zpracovával požadavky, použil jsem systém soketů.
![](http://cdncontribute.geeksforgeeks.org/wp-content/uploads/Socket-Programming-in-C-C-.jpg)
Aby server mohl přijímat a zpracovávat příkazy, potřebuje naslouchat na konkrétním portu na IP adrese. K tomu nám pomáhají sockety.
#### Sokcety
Nejprve jsem vytvořil socket, který bude naslouchat na této IP adrese a portu, aby klient, v našem případě příkaz „curl“, mohl navázat spojení. 
```c++
int sockfd = socket(domain, type, protocol)
```
Dále si musíte tuto adresu zapamatovat a v tom pomáhá funkce bind.
```c++
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
A nakonec pomocí funkce listen uvedu svůj server do „pohotovostního režimu“, aby se klient mohl připojit.
```c++
int listen(int sockfd, int backlog);
```
Dále vytvoříme nový soket pro přenos dat mezi klientem a serverem.
```c++
int new_socket = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
Díky tomuto socketu bude můj server schopen rozpoznat HTTP požadavky a odeslat klientovi odpověď.
Například:
```sh
./hinfosvc 8000 &
curl localhost:8000/load
19,0357%
```
Kde 1. řádek je spuštění programu, tedy serveru. 2. Řádek je požadavek http z příkazu "curl". 3. Řádek je odpověď, v tomto případě je to zatížení CPU.

#### Parcer
Vzhledem k tomu, že se zabýváme http, máme vstupní a výstupní standardy, zde je příklad vstupu, který přichází od klienta (příkazy curl).
```sh
GET /hostname HTTP/1.1
User-Agent: curl/7.29.0
Host: localhost:8000
Accept: */*
```
Aby můj program pochopil, co od něj chtějí, je nutné rozdělit "http vstup" na samostatné tokeny. Moje funkce parcer mi s tím pomůže.
```c++
vector<string> parcer(char* buffer, const char *delim)
{
    char *token;
    vector<string> ParcerArray; //Array for output strings.
    token = strtok(buffer,delim);
    while (token != NULL)
    {
        ParcerArray.push_back(token);
        token = strtok(NULL,delim);
    }
    return(ParcerArray);
}
```
Z celé HTTP odpovědi potřebujeme pouze 2 příkazy z 1 řádku, takže je musíme nějak získat. Tato funkce rozdělí řetězec s touto odpovědí na podřetězce a zapíše je do dynamického pole, se kterým pak budu pracovat. V tomto případě je náš dělitel mezera a tokeny jsou:
```c++
Token : GET
Token : /hostname
Token : HTTP/1.1
```
a tak dále, dokud nedojdeme na konec.

Z těch tokenů pak vezmu jen první dva a zkontroluji, zda se pro jistotu jedná o HTTP požadavek a co po nás chtějí.

Zde zkontroluji, zda se jedná o požadavek GET.
```c++
if(ParcerArray[0] != "GET")
{
    send(new_socket,out_err, strlen(out_err), 0 );
    return 0;
}
```
Zde si ověřím, co konkrétně je potřeba zodpovědět, tedy který příkaz je potřeba inicializovat. Použití příkazu hostname jako příklad:
```c++
if(ParcerArray[1] == "/hostname")
{
    if (!server_output("hostname",new_socket))//Here i'm use funtion "server_output" which is at the top
        return 0;
    else
        continue;
}
```
#### Server_output 
Toto je moje funkce, která odešle odpověď klientovi.
```c++ 
bool server_output(string command_str,int socket)
{
    char out_ok[] = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\n";//http response if all is ok

    FILE* input_file = popen(command_str.c_str(), "r");//execute command i shell
    if(!input_file)
    {
        cerr << "ERROR: Neplatny prikaz BASH\n" << endl;
        return false;
    }

    char copy_from_file_buffer[128];
    while (fgets(copy_from_file_buffer, sizeof(copy_from_file_buffer), input_file) == NULL);//copy command output to variable

    /*This part of the code is needed to remove unnecessary spaces when initializing the "cpu-name" command */
    if (command_str == reg1)
    {
        int j = 0;
        for(; copy_from_file_buffer[j] == ' '; j++ );
        int n = strlen(copy_from_file_buffer) - j;
        for(int i = 0; i < n; i++)
            copy_from_file_buffer[i] = copy_from_file_buffer[i + j];
        copy_from_file_buffer[strlen(copy_from_file_buffer) - j + 1] = '\0';
    }

    char out_str[256];
    snprintf(out_str, sizeof out_str, "%s%s", out_ok, copy_from_file_buffer);// out_str = out_ok + copy_from_file_buffer
    send(socket, out_str , strlen(out_str), 0 );
    fclose(input_file);
    return true;
}
```
V této funkci používám funkci "popen" k „otevření“ požadovaného příkazu v linuxovém shellu. A převedu to pomocí c ++ do požadované podoby. Klientovi také posílám odpověď na nový socket.
### Priklady
```sh
./hinfosvc 8000 &
curl localhost:8000/load
19,0357%
curl localhost:8000/hostname
valerii-X751LB
curl localhost:8000/cpu-name
Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz
```
