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

struct dar_msg {
      char cName[10];
      int iVida;
    };

//F U N C I O N E S
int reservar_paquete(dar_msg paq);
void system_error(char *name);
void crear_fichero();
char get_Direccion();
bool sin_tareas();
bool Validar_origen(char origen);
void respuesta(char cad[]);
void reenvio(char cad[]);
bool Validar_Org(char origen);
void reenvio_direccional(char cad[],char direcciones[],int aux);
	
void token_validacion(char cad[]);
void ack_validacion();
void token_descubrimiento(char cad[]);
	
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
    char port_name[128] = "\\\\.\\COM2";
    char init[] = ""; // e.g., "ATZ" to completely reset a modem.


    char buffer2[1];
	HANDLE file2;				    
	DWORD read2, written2;				    				    
	char port_name2[128] = "\\\\.\\COM3";	
				    
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
    	
    	ReadFile( file,
                    buffer,
                    sizeof(buffer),
                    &read,
                    NULL
                    ); 
                    
          if(read)      
        {
            memcpy(&message, buffer, 16);
        	printf("%s %i\n", message.org, message.dest);
        }
        
        // check for data on port and display it on screen.
       // ReadFile(file, buffer, sizeof(buffer), &read, NULL);
          
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
					    //paq.iVida=1;
					    paq.org= 'a';
					    paq.dest= 'b';
    
				//	ax = reservar_paquete(paq);
					
					
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
					            		            				
					file2 = CreateFile(port_name2,
				         GENERIC_WRITE ,
				        0, 
				        NULL, 
				        OPEN_EXISTING,
				        0,
				        NULL);				    				    
																																												
					ch='*';					
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch='2';					
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch=',';					
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch='{';					
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch=direccion;						//Direccion A
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch='}';					
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch=',';					
					WriteFile(file2, &ch, 1, &written2, NULL);				
					ch=direccion;					//Origen
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 				
					ch=',';					
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch=direccion;					//Direccion A +1 = B
					WriteFile(file2, &ch, 1, &written2, NULL);
					ch='*';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					
					CloseHandle(file2);
										
            		break;
            	case 51:
            		printf("Token disponibilidad \n");            		

					file2 = CreateFile(port_name2,
				         GENERIC_WRITE ,
				        0, 
				        NULL, 
				        OPEN_EXISTING,
				        0,
				        NULL);				    				    
									            		
            		
            		ch='*';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch='3';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch=',';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch='{';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch='}';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch=',';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch=get_Direccion();					//Origen
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch=',';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            							
					ch = getch();
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
					ch='*';					
					WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
            		
            		CloseHandle(file2);
            		break;
            	default: 
            	{
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

// Para guardar direcci€n de inicio
void crear_fichero()
{
    FILE *archivo;/*El manejador de archivo*/
    archivo=fopen("direccion.txt", "w"); //Abrimos en modo escritura    
    fprintf(archivo, "a");/*Escribimos en el archivo*/
    fclose(archivo);/*Cerramos el archivo*/
    direccion='a';
}

char get_Direccion()
{
    FILE *fichero; /* Variable que permite abrir y cerrar un fichero */
    char dir[1];
    fichero = fopen("direccion.txt","r"); /* Abre el archivo miarchivo.txt para lectura de texto */
    char local;    
    fgets(dir,30,fichero);
    local = dir[0];    
    fclose(fichero);    
    return local;
}

/// Archivo vacio
bool sin_tareas(){
	FILE *rv;
	rv = fopen( "bloque.txt", "r" );
	fseek( rv, 0, SEEK_END );
	if (ftell( rv ) == 0 )
	{
		return true;
	}else{
		return false;
	}
}


//Respuesta a eventos
void respuesta(char cad[]){
	char cara=cad[0];
	//T O K E N   DE  V A L I D A C I O N	
	if(cara=='1'){
		printf("\n >>>>>>>Token de validacion<<<<<<< \n");
		token_validacion(cad);
	}	
	if(cara=='2'){
		printf("\n >>>>>>>Token de descubrimiento<<<<<<< \n");		
		///
		token_descubrimiento(cad);
		///
	}	
	if(cara=='3'){
		printf("\n >>>>>>>Token de disponibilidad<<<<<<< \n");
	}
	if(cara=='4'){
		printf("\n >>>>>>>Token de ack de validaci€n<<<<<<< \n");
		validado=true;
	}
}

void token_validacion(char cad[]){
	bool propietario=false;
	printf("\n >>>>>>>Token de validacion<<<<<<< \n");
	printf("\n Origen %c ",cad[5]);												
	printf("\n Destino %c ",cad[7]);												
	propietario = Validar_Org(cad[7]);		
	
	if(propietario){
		//printf("Es para mi %d <<<", propietario );
		printf(" LA RED ESTA CORRECTA");
		validado=true;		
		//Token de aviso de validaci€n correcto
		ack_validacion();
		//Token
	}else{
		printf("No es para mi %d <<<", propietario );
		////
		validado=false;
		reenvio(cad);				
		///
	}
}
//Token de ACK de validaci€n
void ack_validacion(){
	
	printf("Token ack validado \n");            		
	char buffer2[1];
	HANDLE file2;				    
	DWORD read2, written2;				    				    
	char port_name2[128] = "\\\\.\\COM5";	
	
	file2 = CreateFile(port_name2,
	        GENERIC_WRITE ,
	        0, 
	        NULL, 
	        OPEN_EXISTING,
	        0,
	        NULL);				    				    									            		           		
	char ch;
	ch='*';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch='4';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch=',';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch='{';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch='}';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch=',';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch='a';					//Origen
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch=',';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            							
	ch ='a';
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
	ch='*';					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            		
            		
    CloseHandle(file2);
}


//Reenvio de paquetes
void reenvio(char cad[]){
	
	char buffer2[1];
	HANDLE file2;				    
	DWORD read2, written2;				    				    
	char port_name2[128] = "\\\\.\\COM5";	
	
	file2 = CreateFile(port_name2,
	        GENERIC_WRITE ,
	        0, 
	        NULL, 
	        OPEN_EXISTING,
	        0,
	        NULL);		
					    				    									            	            		
    int x= sizeof cad / sizeof *cad;
    int y=0;
    char ch='*';
    WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            			
    while(y<x){
    	WriteFile(file2, &cad[y], 1, &written2, NULL);					 				 									            			
    	y++;
	}					
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            			
	CloseHandle(file2);
}

//funciones 
bool Validar_origen(char origen){
    FILE *fichero; /* Variable que permite abrir y cerrar un fichero */
    //char dir; /* Variable que guardara cada palabra */
    fichero = fopen("direccion.txt","r"); /* Abre el archivo miarchivo.txt para lectura de texto */        
    //fgets(dir,30,fichero);                
    char dir = fgetc(fichero); /* Variable que guardara cada palabra */
    printf("\n %c <> %c \n",origen,dir);
    printf("hola");
    
    if( dir == origen){
       	printf("SON iguales");
       	fclose(fichero);
       	return true;
	}            		
    fclose(fichero);
    return false;
}
//validar si es mio
bool Validar_Org(char origen){    
    char dir = direccion;    
    if( dir == origen){
       	printf("SON iguales");       	
       	return true;
	}            		    
    return false;
}


//Token de descubrimiento
void token_descubrimiento(char cad[]){
	//ASIGNACIîN Falta el d presentaci€n
	bool propietario=false;
	
	printf("\n >>>>>>>Token de descubrimiento<<<<<<< \n");
	//printf("\n Origen %c ",cad[5]);												
	//printf("\n Destino %c ",cad[7]);												
	//propietario = Validar_origen(cad[7]);		
	int x= sizeof cad / sizeof *cad;
    int y=0,w=0;
    char direcciones[25];
    bool adentro = false;
    while(y<x){
    	if(cad[y]=='{'){
    		adentro = true;    		
    		//printf("> %c < ",cad[y]);
    	}else{
    		if((adentro)&&(cad[y]!=',')&&(cad[y]!='}')){
    			direcciones[w]=cad[y];
    			//printf("> %i c ",direcciones[w]);
    			w++;
			}
			if(cad[y]=='}'){
    			adentro = false;    		    			
    		}
		}
		y++;				    			    		
	}
	y=0;
	x=sizeof cad / sizeof *cad;
	int aux=0;
	while(y<x){
		if((direcciones[y]>=97)&&(direcciones[y]<=122)){
			printf("- %c - ",direcciones[y]);	
			aux++;
		}		
		y++;
	}	
	char nueva = direcciones[aux-1]+1;
	//Nueva direccion 	
	
	printf("- Direccion actual %c - ",nueva);	
	reenvio_direccional(cad,direcciones,aux);
	//////
	
}

//Reenvio con nueva direccion
void reenvio_direccional(char cad[],char direcciones[],int aux){
	
	char buffer2[1];
	HANDLE file2;				    
	DWORD read2, written2;				    				    
	char port_name2[128] = "\\\\.\\COM5";	
	
	file2 = CreateFile(port_name2,
	        GENERIC_WRITE ,
	        0, 
	        NULL, 
	        OPEN_EXISTING,
	        0,
	        NULL);		
	
	int xy =sizeof cad / sizeof *cad;				    				    									            	            		    
    int y=0;
    /////// E R R O R
    char ch='*';
    WriteFile(file2, &ch, 1, &written2, NULL);						 				 									            			
    ch='0';
    WriteFile(file2, &ch, 1, &written2, NULL);						 				 									            			
    while(y<xy){
    	if(cad[y]=='{'){
    		WriteFile(file2, &cad[y], 1, &written2, NULL);					 				 									            			
    		int h=0;
    		while(h<aux){
    			WriteFile(file2, &direcciones[h], 1, &written2, NULL);	
				ch=',';				 				 									            			
    			WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            			
    			h++;
			}
		}    	
    	y++;
	}	
	ch='*';				
	WriteFile(file2, &ch, 1, &written2, NULL);					 				 									            			
	
	printf("- Enviado con la nueva dir- ");	
	
	CloseHandle(file2);
}



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

//

//Verifica el tamaÒo del paquete y asigna memoria
int reservar_paquete(dar_msg paq){
    int cantidad =sizeof(paq);
    char cBytes[cantidad];
    printf("%i\n", sizeof(paq));
    memcpy(cBytes, &paq, sizeof(paq));
    return cantidad;
}
