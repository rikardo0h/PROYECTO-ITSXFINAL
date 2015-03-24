#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Estructura de los paquetes y tokens  TAMA„O 16 BYTES
struct paquete {
    char tipo;              //Tipo de paquete o token
    char contenido[13];     //Contenido del paquete 13 caracteres
    char org;               //Direccion origen
    char dest;              //Direccion destino
};

struct direcciones {
    char tipo;              //Tipo de paquete o token
    int  cant;              //Cantidad
    char org;               //Direccion origen
    char dest;              //Direccion destino
};



//F U N C I O N E S
void system_error(char *name);
void respuesta(struct paquete message);
bool propietario(char destino);
void reenvio_paquete(struct paquete paq);


void token_validacion(struct paquete *paq, char direccion);
void token_descubrimiento(struct paquete *paq, char direccion);
void token_publicacion(struct paquete *paq, int cantidad);
void token_limpieza(struct paquete *paq, char direccion);



//Variables del nodo
bool validado=false;
char direccion;
int nodos=0;

//Variables de puertos
char port_name[128] = "\\\\.\\COM2";    // Puerto de  L E C T U R A
char port_name2[128] = "\\\\.\\COM3";     // Puerto de E S C R I T U R A


using namespace std;
int main(int argc, char **argv) {
    
    /////// D E C L A R A C I O N E S     I N I C I A L E S
    
    int ch;
    char buffer[16];
    HANDLE file;
    COMMTIMEOUTS timeouts;
    DWORD read, written;
    DCB port;
    HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    char init[] = "";
    
    
    char buffer2[1];
    HANDLE file2;
    DWORD read2, written2;
    
    
    /////////////////////////
    
    /////////// A B R I R   P U E R T O    DE    L E C T U R A
    
    file = CreateFile(port_name,
                      GENERIC_READ | GENERIC_WRITE ,
                      0,
                      NULL,
                      OPEN_EXISTING,
                      0,
                      NULL);
    
    
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
    
    /////////////////////////
    
    
    //// V A R I A B L E S    A U X I L I A R E S
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
    int m;
    
    /////////////////////////
    
    /// C I C L O   P R I N C I P A L
    do {
        
        //Lectura de paquetes
        ReadFile( file, buffer, sizeof(buffer), &read, NULL );
        
        if(read)      //Si recibio Bytes
        {
            memcpy(&message, buffer, 16);   //    Recibo el paquete
            //Muestra paquete tipo - contenido - origen - destino
        	printf("%c %s %c %c \n", message.tipo , message.contenido ,message.org, message.dest);
            
            //Dar respuesta al paquete
            respuesta(message);
            
        }
        
        
        ///////
        if ( kbhit() ) {
            ch = getch();
            switch (ch)
         	{
            	case 49:
            		printf("Token validacion :) \n");
                    
                    file2 = CreateFile( port_name2,
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL
                                       );
                    
                    //Creaci—n de token de validaci—n
                    
                    paquete paq;
                    direccion = 'a';
                    token_validacion(&paq ,'a');
                    memcpy(cBytes, &paq, sizeof(paq));
                    
                    
                    //Envia el paquete
                    
                    WriteFile( file2,
                              cBytes, //cBytes, //bytes_a_enviar,
                              16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
                              &written,
                              NULL);
					
                    
					
                    CloseHandle(file2);  //Cierra la escritura
                    direccion = 'a';
                    nodos = 0;
                    validado = false;
                    
            		break;
            	case 50:
            		printf("Token descubrimiento \n");
            		
                    
                    file2 = CreateFile( port_name2,
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL
                                       );
                    
                    //Creaci—n de token de validaci—n
                    
                    paquete paq2;
                    token_descubrimiento(&paq2 ,'a');
                    memcpy(cBytes, &paq2, sizeof(paq2));
                    
                    
                    //Envia el paquete
                    
                    WriteFile( file2,
                              cBytes, //cBytes, //bytes_a_enviar,
                              16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
                              &written,
                              NULL);
					
                    
					
                    CloseHandle(file2);  //Cierra la escritura
					
                    
                    
            		break;
            	case 51:
                    printf("Consultar tabla \n");
                    printf("Cantidad de Nodos totales en la red:  %i \n", nodos );
                    
                    m=0;
                    while ( m < nodos) {
                        printf("Equipo %c ", 97+m);
                        if (direccion == 97+m) {
                            printf("<<<<<  \n ");
                        }else
                            printf("\n");
                        m=m+1;
                    }

                    
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

//Creaci—n de token de validaci—n 1

void token_validacion(struct paquete *paq, char direccion){
    
    paq->tipo= '1';
    paq->org= direccion;
    paq->dest= direccion;
    
    
}

// Propietario del token

bool propietario(char destino){
    if (destino == direccion) {
        return true;
    }
    return false;
}

// Funci—n de envio de paquete independiente
void reenvio_paquete(struct paquete paq){
    
    DWORD read2, written2;
    char cBytes[16];
    HANDLE file2;
    file2 = CreateFile( port_name2,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL
                       );
    
    //Creaci—n de token de validaci—n
    
    memcpy(cBytes, &paq, sizeof(paq));
    
    
    //Envia el paquete
    
    WriteFile( file2,
              cBytes, //cBytes, //bytes_a_enviar,
              16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
              &written2,
              NULL);
    
    
    CloseHandle(file2);  //Cierra la escritura
    
}



//Creaci—n de ACK de red validada

void ack_validado (struct paquete *paq){
    
    paq->tipo = '9';
    paq->org  = direccion;
    paq->dest = direccion;
    strcpy(paq->contenido, "1");
    
}

//Creaci—n de token de descubrimiento 2

void token_descubrimiento(struct paquete *paq, char direccion){
    
    paq->tipo= '2';
    paq->org= direccion;
    paq->dest= direccion;
    strcpy(paq->contenido, &direccion);
    
}

//Creaci—n de token de publicacion 3  7 bytes

void token_publicacion(struct paquete *paq, char cantidad){
    
    paq->tipo= '3';
    paq->org= direccion;
    paq->dest= direccion;
    strcpy(paq->contenido, &cantidad);
    
    
}

//Limpieza
void token_limpieza(struct paquete *paq, char direccion){
    
    paq->tipo= '8';
    paq->org= direccion;
    paq->dest= direccion;
}

// Dar respuesta al paquete de lectura
void respuesta(struct paquete message){
    printf("------------ %c --------------- \n", direccion);
    printf("En la funcion: %c %s %c %c \n", message.tipo , message.contenido ,message.org, message.dest);
    
    //Depende del tipo de paquete / token
    switch (message.tipo) {
        case '1': //Token de validaci—n
            printf("Validacion");
            
            if (propietario(message.dest)) {
                printf("Propio y la red es valida ");
                validado = true;
                /// ACK de red Validado
                
                paquete paq;
                ack_validado(&paq);
                reenvio_paquete(paq);
                
                
                ///
            } else {
                //Se pasa al siguiente host el mensaje
                printf("Desconocio");
                strcpy(message.contenido, "reen");
                reenvio_paquete(message);
                direccion='\0';
                
            }
            break;
        case '2':
            printf("Descubrimiento");
            printf("En el paquete %c",message.contenido[0]);
            // A S I G N A   N O M B R E   AL  N O D O
            if (!propietario( message.dest)) {
                direccion = message.contenido[0]+1;
                printf("Direccion asignada %c",direccion);
                strcpy(message.contenido, &direccion);
                reenvio_paquete(message);
            }else{
                printf("Nodos %i", message.contenido[0]- 96 );
                nodos = message.contenido[0]- 96;
                //Token de publicaci—n
                
                paquete tabla;
                token_publicacion(&tabla,message.contenido[0]);
                reenvio_paquete(tabla);
                
                //token de publicacion
                
            }
            
            break;
            
        case '3':
            printf("Token de publicaci—n");
            if (!propietario( message.dest)) {
                nodos = message.contenido[0]- 96;
                printf("Nodos %i", nodos);
                reenvio_paquete(message);
            }else{
                printf("Token de publicaci—n terminado");
                printf("Nodos %i", nodos);
            }
            
            
            
            break;
            
        case '8':
            printf("Token de limpieza");
            if (!propietario(message.dest)) {
                direccion='\0';
                reenvio_paquete(message);
            }else{
                direccion='\0';
            }
            
            break;

            
        case '9':
            printf("ACK recibido");
            if (message.contenido[0]==49) {
                printf("Red correcta");
                validado=true;
            }else
                printf("No correcta");
            
            if (!propietario( message.dest)) {
                reenvio_paquete(message);
            }
            
            break;
            
            
        default:
            printf("No identificado");
            break;
    }
}