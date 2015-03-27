//ARCHIVO FINALL
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
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



//F U N C I O N E S
void system_error(char *name);
void respuesta(struct paquete message);
bool propietario(char destino);
void reenvio_paquete(struct paquete paq);
void limpiar (char *cadena);

void token_validacion(struct paquete *paq, char direccion);
void token_descubrimiento(struct paquete *paq, char direccion);
void token_publicacion(struct paquete *paq, int cantidad);
void token_limpieza(struct paquete *paq, char direccion);
void ack_disponible(struct paquete *paq, char destino);
void token_disponibilidad(struct paquete *paq, char destino);

//void token_inicio(struct paquete *paq,int cantidad);
void dividir_texto(char cCadLarga[],char destino);
void token_inicio(struct paquete *paq,char destino, char cantidad);
void paquete_texto(struct paquete *paq,char destino, char parte[]);


//Variables del nodo
bool validado=false;
char direccion;
int nodos=0;
bool ping=false;

char recibido [117];
int partes=0;

//Variables de puertos
char port_name[128] = "\\\\.\\COM4";    // Puerto de  L E C T U R A
char port_name2[128] = "\\\\.\\COM3";     // Puerto de E S C R I T U R A


/// VARIABLES DE ESCRITURA
char cBytes[16];
DCB port2;
 COMMTIMEOUTS timeouts2;
    char buffer2[1];
    HANDLE file2;
    DWORD read2, written2;    
///

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
    
    
    

    
    
    //////////
    file2 = CreateFile( port_name2,
                                       GENERIC_READ | GENERIC_WRITE,
                                       0,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL
                                       );
    
					
				    memset(&port2, 0, sizeof(port2));
				    port2.DCBlength = sizeof(port2);                
				        
									    
					if ( !GetCommState(file2, &port2))
					        system_error("getting comm state");
					    if (!BuildCommDCB("baud=9600 parity=n data=8 stop=1", &port2))
					        system_error("building comm DCB");
					    if (!SetCommState(file2, &port2))
					        system_error("adjusting port settings");
					    
					    // set short timeouts on the comm port.
					    timeouts2.ReadIntervalTimeout = 1;
					    timeouts2.ReadTotalTimeoutMultiplier = 1;
					    timeouts2.ReadTotalTimeoutConstant = 1;
					    timeouts2.WriteTotalTimeoutMultiplier = 1;
					    timeouts2.WriteTotalTimeoutConstant = 1;
					    
				    if (!SetCommTimeouts(file2, &timeouts))
        system_error("setting port time-outs.");
    
    // set keyboard to raw reading.
    if (!GetConsoleMode(keyboard, &mode))
        system_error("getting keyboard mode");
    mode &= ~ ENABLE_PROCESSED_INPUT;
    if (!SetConsoleMode(keyboard, mode))
        system_error("setting keyboard mode");
    
    if (!EscapeCommFunction(file2, CLRDTR))
        system_error("clearing DTR");
    Sleep(200);
    if (!EscapeCommFunction(file2, SETDTR))
        system_error("setting DTR");
    
    				    
				    
				    
    /////////
    
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
    if (!BuildCommDCB("baud=9600 parity=n data=8 stop=1", &port))
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
	
	paquete message;
    int m;
    int corte=0;
    int partes_enviar=0;
    int contador=0;
    char to;
    bool entre;
    int aux,ay;
    
    /////////////////////////
    
    /////// M E N U
    	system("cls");    	
		printf(" M E N U    G E N E R A L \n");
	    	printf("1.- Token de validacion \n");
	        printf("2.- Token de descubrimiento \n");
	        printf("3.- Consulta de tabla \n");
	        printf("4.- Limpieza \n");
	        printf("5.- Disponibilidad \n");        
	        printf("6.- Envio de texto \n");            	
    	//////// M E N U
    
    /// C I C L O   P R I N C I P A L
    printf("Preciona cualquier cosa para entrar al menu \n");
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
            printf("Preciona para continuar \n");
        }
                
    	///////
        
        if ( kbhit() ) {
        	getch();
        	//menuu
        	
        	system("cls");			    	    	    	
			printf(" M E N U    G E N E R A L \n");
	    	printf("1.- Token de validacion \n");
	        printf("2.- Token de descubrimiento \n");
	        printf("3.- Consulta de tabla \n");
	        printf("4.- Limpieza \n");
	        printf("5.- Disponibilidad \n");        
	        printf("6.- Envio de texto \n");              
	        
	        printf("Indica la opcion: \n");
	        
	        //menuu
            ch = getch();
            switch (ch)
         	{
            	case 49:
            		printf("Token validacion :) \n");
                    
                    
				    
				    
				    
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
					
                    
					
                    
                    direccion = 'a';
                    nodos = 0;
                    validado = false;
                    
            		break;
            	case 50:
            		printf("Token descubrimiento \n");
            		
            	
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
                    
                case 52:
                    printf("Limpieza \n");
                    
                    

                    memset(&port2, 0, sizeof(port2));
				    port2.DCBlength = sizeof(port2);                
				        

                    //Creaci—n de token de validaci—n
                    
                    paquete paq3;
                    direccion='x';
                    token_limpieza(&paq3 ,'x');
                    memcpy(cBytes, &paq3, sizeof(paq3));
                    
                    
                    //Envia el paquete
                    
                    WriteFile( file2,
                              cBytes, //cBytes, //bytes_a_enviar,
                              16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
                              &written,
                              NULL);

					break;
                    
                case 53:
                    
                    printf("Token disponibilidad \n");
                    printf("Resultado del ping anterior: %i \n",ping);            		
                    ping=false;
                    printf("Para: ");
                    //to = getchar();
                    //scanf("%c", &to);
                    
                    while (((to = getchar()) != '\n' && to != EOF)&&(ch=to));
                    printf("Para: %c ",ch);
                    

					//Creaci—n de token de validaci—n
                                        
                    
                    paquete paq4;
                    //to = getchar();
                    token_disponibilidad(&paq4 ,ch);
                    printf("Bytes  %i",sizeof(paq4));
                    memcpy(cBytes, &paq4, sizeof(paq4));
                    //Envia el paquete
                    WriteFile( file2,
                              cBytes, //cBytes, //bytes_a_enviar,
                              16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
                              &written,
                              NULL);

                    break;
                    
                case 54:
                	// Escritura y envio de texto MAXIMO 13 BYTES
                	printf("Envio de texto \n");
                	printf("Max 200 caracteres \n");
                	  
					  char cadena [117];
					  char paq_parte [13];
					  
					  limpiar(cadena);
					
					  printf ("Introduzca una cadena: ");
					  fgets (cadena, 117, stdin);
					  printf ("La cadena leida es: %s \n", cadena);
					  					  					  					  					  
					  
					  
                    printf("Para: ");                    
                    while (((to = getchar()) != '\n' && to != EOF)&&(ch=to));
                    printf("Para: %c \n",ch);                                      
                    dividir_texto(cadena,ch);

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
    
    
    memcpy(cBytes, &paq, sizeof(paq));            
    WriteFile( file2,
              cBytes, //cBytes, //bytes_a_enviar,
              16,//tam_img, //sizeof(cBytes), //(bytes_a_enviar),
              &written2,
              NULL);                
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

//Limpieza

void token_disponibilidad(struct paquete *paq, char destino){
    
    paq->tipo= '4';
    paq->org= direccion;
    paq->dest= destino;
}

//ACK Disponible
void ack_disponible(struct paquete *paq, char destino){
    
    paq->tipo= '5';
    paq->org= direccion;
    paq->dest= destino;
    strcpy(paq->contenido, "1");
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
            
        case '4':
            printf("Token disponibilidad");             
            /*
            if(!propietario(message.org)){
            	if(propietario(message.dest)){
            		printf("Preguntan por mi");            		
            		paquete disponible;
                	ack_disponible(&disponible,message.org);
                	reenvio_paquete(disponible);
				}else{
					reenvio_paquete(message);
				}
			}
            */
            if (propietario(message.dest)) {
                printf("Propio y lo encontre ");
                /// ACK de host disponible
                
                paquete disponible;
                ack_disponible(&disponible,message.org);
                reenvio_paquete(disponible);
                
                ///
            } else {
                if (!propietario(message.org)) {
                    printf("Desconocio");
                    reenvio_paquete(message);
                }
                
            }
            
            break;
            
        case '5':
            printf("ACK disponible recibido");
            
            if (propietario( message.dest)) {
                
                if (message.contenido[0]==49) {
                    printf("Host encontrado");
                    ping=true;
                }else{
                    printf("No encontrado");
                    ping=false;
                }
                
            }else{
                reenvio_paquete(message);
            }
            
            break;

            
            
        case '8':
            printf("Token de limpieza");
            if (!propietario(message.dest)) {
                printf("No es para mi");
                direccion='\0';
                reenvio_paquete(message);
            }else{
                printf("Es para mi");
                //                direccion='\0';
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
		
		//Recibir texto			                        		
        case 'a':
        		if (!propietario( message.dest)) {
                	reenvio_paquete(message);
            	}else{
	            	printf("Inicio de lectura \n");
	        		printf("%i",message.contenido[0]);
	        		partes = message.contenido[0];
	        		printf("Resultado %i \n",partes);	
				}        		
        break;
        
        //Recibir paquete del texto
        case 'b':
				/*
        		if (!propietario( message.dest)) {
                	reenvio_paquete(message);
            	}else{
	            	printf("Parte a recibir \n");
	            	partes = partes-1;
	            	printf("Faltantes=  %i \n",partes);		        		
					printf("Paquete:  %s \n ",message.contenido);	
					if(partes==0){
						printf("Terminado \n");	
					}        		
				}  */
				
				if (propietario(message.dest)) {					                
					printf("Propio y lo encontre ");
                	printf("Parte a recibir \n");
	            	partes = partes-1;
	            	printf("Faltantes=  %i \n",partes);		        		
					printf("Paquete:  %s \n ",message.contenido);	
					if(partes==0){
						printf("Terminado \n");	
					}        		
	            } else {
	                if (!propietario(message.org)) {
	                    printf("Desconocio");
	                    reenvio_paquete(message);
	                }	                
	            }			      		
        break;
                                    
            
        default:
            printf("No identificado");
            break;
    }
}

//E N V I O   D E    T E X T O

//limpiar cadena
void limpiar (char *cadena)
{
  char *p;
  p = strchr (cadena, '\n');
  if (p)
    *p = '\0';
}

//Token de inicio
//void token_publicacion(struct paquete *paq, char cantidad){
void token_inicio(struct paquete *paq,char destino, char cantidad){    
    paq->tipo= 'a';
    paq->org= direccion;
    paq->dest= destino;
    strcpy(paq->contenido, &cantidad);
}

void paquete_texto(struct paquete *paq,char destino, char parte[]){    
    paq->tipo= 'b';
    paq->org= direccion;
    paq->dest= destino;
    strcpy(paq->contenido, parte);
}

void dividir_texto(char cCadLarga[],char destino){
	
	///////////////
	
	//Ccantidad d paquetes
	int contador=0;
	int corte=0;					  	
	bool entre=false;
	for(int aux=0; cCadLarga[aux]!='\n';aux++,corte++){					  						  	
	 	   if(corte<13){					  
	  	   		entre=true;
		   }
		 if(corte==13){
		 	contador++;						
			corte=0;
			aux--;							 		
			entre=false;
		} 
	}
	if(entre){
		contador++;
	}
					  	
		
	printf("Son :  %i  partes \n",contador);				
	///////////////		
	
	///Envio de paquete	    
	paquete paq5;                        
		

	//char cCadLarga[117] = "123456789ABCDEF"; //Declaracion y asignaciÃ³n del arreglo cCadLarga
	//Apuntador a arreglos de caracteres inicializado con la referencia a cCadLarga
	char * pCLarga = cCadLarga;
/* Mostramos el contenido de cCadLarga y el contenido del puntero pCLarga con el 
	simbolo * para apuntar al cotenido de la memoria */
	printf("Contenido de cCadena :%s\n Contenido del Apuntador :%c\n", cCadLarga, *pCLarga);

/* Mostramos la direccion de memoria de cCadLarga y la direccion de memoria 
	del puntero pCLarga con el simbolo * para apuntar al cotenido de la memoria */
	printf("Referencia a cCadena :%i\n Referencia del Apuntador :%i\n", &cCadLarga, &pCLarga);
	

	char cTemporal[13]; //Cremos cadena temporal
	/* Mostrar los tamaÃ±os de cCadLarga y cTemporal*/
	printf("Tamanio de cCadLarga :%i\n Tamanio de cTemporal :%i\n", strlen(cCadLarga), strlen(cTemporal));
	//Creamos for que va avanzando de 3 en 3  hasta llegar al tamaÃ±o de cCadLarga
	
    
	///Envio de cuentas partes
		token_inicio(&paq5 ,destino, contador);
	    memcpy(cBytes, &paq5, sizeof(paq5));                    
	    WriteFile( file2, cBytes, 16,&written2,NULL);
		
		
	
	for(int i=0; i<strlen(cCadLarga); i+=13)
	{				

		strncpy(cTemporal, pCLarga,13); //Copiamos del apuntador a la variable cTemporal 3 bytes (caracteres).
		cTemporal[sizeof(cTemporal)-1]='\0';						
		pCLarga += 13; // Aumentamos el apuntador en 3 posiciones.
		printf("%s :: %i bytes \n", cTemporal,sizeof(cTemporal));	//Imprimimos lo copiado a cTemporal
		
		//for(int y=0;y<13;y++){
		//	printf("copiado: ");
		//	printf("%c",cTemporal[y]);
		//}
		
		paquete_texto(&paq5 ,destino, cTemporal);
		memcpy(cBytes, &paq5, sizeof(paq5));                    
		WriteFile( file2, cBytes, 16,&written2,NULL);
						
	}
					
	
}
