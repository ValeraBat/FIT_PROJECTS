#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h>  
#include <netinet/in.h> 
#include <unistd.h> 
#include <iostream>
#include <string> 
#include <vector>
using namespace std;
#define reg1 "lscpu | grep 'Model name' | awk -F ':' '{ print $2; }'"//command to display processor name
/*command for calculating and displaying the degree of processor load */
#define reg2 "cat /proc/stat |grep cpu |tail -1|awk '{print ($5*100)/($2+$3+$4+$5+$6+$7+$8+$9+$10)}'|awk '{print 100-$1 \"%\"}'"

/*This function acts as a parser, i.e. splits the string into smaller strings. 
The delimiter is given by the "delim" variable and the string is given by the "buffer" variable.*/
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

/*This function executes commands on the operating system and writes the output in the correct http form. 
The "command_str" variable is passed the command to be executed in the Linux shell. 
The "socket" variable is passed the socket to which the output should be sent.*/
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
    close(socket);
    return true;
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, input_read; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[2048] = {0}; 
    
    int PORT;
    /* Write the "PORT" as a command line argument. */
    if (argc > 1){
        PORT = atoi(argv[1]);
    }
    else {
        cerr<< "ERROR: Port nezadan\n" << endl;
        return 0;
    }
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        cerr<< "ERROR: Socet fail\n" << endl;
        return 0;
    } 
        
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        cerr<< "ERROR: Setsockopt fail\n" << endl;
        return 0; 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  sizeof(address))<0) 
    { 
        cerr<< "ERROR: Bind fail\n" << endl;
        return 0; 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        cerr<< "ERROR: Listen fail\n" << endl;
        return 0; 
    }
    while (1) //an infinite loop that allows to accept an infinite number of requests.
    { 
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) 
        { 
            cerr<< "ERROR: Accept fail\n" << endl;
            return 0; 
        } 
        
        input_read = read(new_socket, buffer, 2048);//Here we are write to variable "buffer" our input from client or "curl".
        buffer[input_read] = '\0';
        

        vector<string> ParcerArray = parcer(buffer," ");//Here i'm use funtion "parcer" which is at the top

        if(ParcerArray.size() < 1)//Checking if there is something in the ParcerArray.
        {
            cerr << "ERROR: Neplatny prikaz\n" << endl;
            return 0;
        }
        else
        {
            char out_err[] = "HTTP/1.1 400 Bad Request\r\n";//http response if error found

            /*If the first line in ParcerArray is not GET then we terminate the program due to invalid input data.*/
            if(ParcerArray[0] != "GET")
            {
                send(new_socket,out_err, strlen(out_err), 0 );
                return 0;
            }
            else
            {
                /*Here the program defines the required command to execute either "hostname" or "cpu-name" or "load".*/
                if(ParcerArray[1] == "/hostname")
                {
                    if (!server_output("hostname",new_socket))//Here i'm use funtion "server_output" which is at the top
                        return 0;
                    else
                        continue;
                }
                else if(ParcerArray[1] == "/cpu-name")
                {
                    if (!server_output(reg1,new_socket))
                            return 0;
                        else
                            continue;
                }
                else if(ParcerArray[1] == "/load")
                {
                    if (!server_output(reg2,new_socket))
                        return 0;
                    else
                        continue;
                }
                else
                {
                    /*If the command does not match any command from the list, the program sends an error.*/
                    send(new_socket,out_err, strlen(out_err), 0 );
                }
            }
        }
    }
    close(server_fd);
    return 0; 
} 
