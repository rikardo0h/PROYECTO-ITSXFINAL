#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Estructura de los paquetes y tokens
struct paquete {
    char tipo;              //Tipo de paquete o token
    char contenido[13];     //Contenido del paquete 13 caracteres
    char org;               //Direccion origen
    char dest;              //Direccion destino
};

//F U N C I O N E S
void system_error(char *name);


bool validado=false;
char direccion;

using namespace std;
int main(int argc, char **argv) {
    int ch;
    char buffer[16];
    HANDLE file;
    COMMTIMEOUTS timeouts;
    DWORD read, written;
    DCB port;
    HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    char port_name[128] = "\\\\.\\COM4";
    char init[] = ""; // e.g., "ATZ" to completely reset a modem.


    char buffer2[1];
	HANDLE file2;				    
	DWORD read2, written2;				    				    
	char port_name2[128] = "\\\\.\\COM1";	
				    
    // open the comm port.
    file = CreateFile(port_name,
        GENERIC_READ | GENERIC_WRITE ,
        0, 
        NULL, 
        OPEN_EXISTING,
        0,
        NULL);

/////
    if ( INVALID_HANDLE_VALUE == file) {
        system_error("opening file");
        return 1;
    }
    // get the current DCB, and adjust a few bits to our liking.
    memset(&port, 0, sizeof(port));
    port.DCBlength = sizeof(port);
    if ( !GetCommState(file, &port))
        system_error("getting comm state");
    if (!BuildCommDCB("baud=19200 parity=n data=8 stop=1", &port))
        system_error("building comm DCB");
    if (!SetCommState(file, &port))
        system_error("adjusting port settings");

	// set short timeouts on the comm port.
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = 1;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 1;
    if (!SetCommTimeouts(file, &timeouts))
        system_error("setting port time-outs.");

    // set keyboard to raw reading.
    if (!GetConsoleMode(keyboard, &mode))
        system_error("getting keyboard mode");
    mode &= ~ ENABLE_PROCESSED_INPUT;
    if (!SetConsoleMode(keyboard, mode))
        system_error("setting keyboard mode");

    if (!EscapeCommFunction(file, CLRDTR))
        system_error("clearing DTR");
    Sleep(200);
    if (!EscapeCommFunction(file, SETDTR))
        system_error("setting DTR");

    if ( !WriteFile(file, init, sizeof(init), &written, NULL))
        system_error("writing data to port");

    if (written != sizeof(init))
        system_error("not all data written to port");
////


	DWORD bytes_escritos;
	char cara;
    bool band=false;
    int w=0;
    int i=0;
    int destino=0;
    bool propietario= false; 
	int ax=0;   
	char cBytes[16];
	paquete message;
    do {
        ////// check for data on port and display it on screen.
        
        	ReadFile( file,
                    buffer,
                    sizeof(buffer),
                    &read,
                    NULL
                    ); 
                    
          if(read)      
        {
            memcpy(&message, buffer, 16);
        	printf("%c %c\n", message.org, message.dest);
        }
        
        
        ///////        
        if ( kbhit() ) {
            ch = getch();
            switch (ch) 
         	{		 
            	case 49:
            		printf("Token validacion :) \n");            		            							
            		//direccion='a';
            		////				    
				    
				     file2 = CreateFile( port_name2,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL
                         );			    				    
            		            		
					   paquete paq;
					    //strcpy(paq.cName, "1234");

					    paq.org= 'a';
					    paq.dest= 'b';
                    
                        //ax= reservar_paquete(paq);
					
					
    				memcpy(cBytes, &paq, sizeof(paq));
    
					printf("%i",ax);
					WriteFile( file2,
	                   cBytes, //cBytes, //bytes_a_enviar,
	                   16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
	                   &written,
	                   NULL);
					
				
					
					CloseHandle(file2);
							
            		break;
            	case 50:
            		printf("Token descubrimiento \n");
            		//int direc = get_Direccion();
            		//printf("Direc %c \n",get_Direccion());
							 				 							
					

										
            		break;
            	case 51:
            		printf("Token disponibilidad \n");            		

					break;
            	default: 
            	{
                    printf("Salida \n");
                    return 1;
               		break;
            	}
			}                        
        }
    // until user hits ctrl-backspace.
    } while ( ch != 127);

    //////////////////////////////
	// close up and go home.
    CloseHandle(keyboard);
    CloseHandle(file);
    return 0;
}

//// F U N C I O N E S 

//E R R O R
void system_error(char *name) {
	char *ptr = NULL;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        0,
        GetLastError(),
        0,
        (char *)&ptr,
        1024,
        NULL);
    fprintf(stderr, "\nError %s: %s\n", name, ptr);
    LocalFree(ptr);
}