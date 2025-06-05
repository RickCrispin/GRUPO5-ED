// Commit: Se agreg� las funciones desencolar y mostrar cola al m�dulo del Planificador de CPU.
// Integrante: Camarena Alvarez Leandro
#include <iostream>
using namespace std;

struct Proceso {
	int id;
	char nombre[30];
	int prioridad;
	Proceso* sig;
};

Proceso* cabeza = NULL;

struct NodoCola {
    int idProceso;
    char nombre[30];
    NodoCola* sig;
};

NodoCola* frente = NULL;
NodoCola* final = NULL;

void insertarProceso() {
   Proceso* nuevo = new Proceso; // Reserva de memoria para el nuevo proceso
   
   cout<<"Ingrese ID del proceso: ";
   cin>>nuevo->id;
   
   cout<<"Ingrese nombre del proceso: ";
   cin.ignore(); // Limpiar el b�fer de entrada
   cin.getline(nuevo->nombre, 30); // Leer nombre con espacios
   cout<<"Ingrese prioridad: ";
   cin>>nuevo->prioridad;
   nuevo->sig = NULL; // El nuevo nodo apunta a NULL porque ser� el �ltimo
   
   if (cabeza == NULL){ // Si la lista est� vac�a
   		cabeza = nuevo; // El nuevo proceso se convierte en la cabeza
   }
   else{
   		Proceso* aux = cabeza; // Puntero auxiliar para recorrer la lista
   		while (aux->sig != NULL){ // Recorrer hasta el �ltimo nodo
   			aux = aux->sig;
		   }
		   aux->sig = nuevo; // Enlazar el nuevo nodo al final
	}
	
	cout<<"Proceso creado exitosamente."<<endl;
}

void eliminarProceso() {
    int id;
    cout<<"Ingrese ID del proceso a eliminar: ";
    cin>>id;

    Proceso*actual=cabeza;//Puntero para recorrer la lista
    Proceso*anterior=NULL; //Puntero para guardar el espacio anterior

    //Buscar el proceso con el ID ingresado
    while (actual != NULL && actual->id != id) {
        anterior=actual;//Avanzar el puntero anterior
        actual=actual->sig;//Avanzar al siguiente esapacio
    }

    //Si no se encontra el proceso
    if (actual == NULL) {
        cout<<"Proceso no encontrado"<<endl;
        return;
    }

    //Si el proceso a eliminar es el primero osera la cabeza de la lista
    if (anterior==NULL){
        cabeza=actual->sig;//Mover cabeza al siguiente espacio
    }else{
        
        anterior->sig=actual->sig;//Saltar el actual (eliminado) y conectar el anterior para el siguiente
    }  
    delete actual;//Liberar memoria del lugar eliminado

    cout<<"Proceso eliminado correctamente"<<endl;
}


void listarProcesos() {
	Proceso* aux = cabeza; // Puntero auxiliar para recorrer la lista
    if (aux == NULL) { // Si la lista est� vac�a
        cout<<"No hay procesos registrados."<<endl;
        return;
    }
    cout<<"=== LISTA DE PROCESOS ==="<<endl;
    while (aux != NULL) { // Recorrer e imprimir cada proceso
        cout<<"ID: "<<aux->id<<", Nombre: "<<aux->nombre<<", Prioridad: "<<aux->prioridad<<endl;
        aux = aux->sig;
    }
}

void modificarPrioridad() {
	int id;
    cout<<"Ingrese ID del proceso a modificar: ";
    cin>>id;

    Proceso* aux = cabeza; // Puntero auxiliar para buscar el proceso
    while (aux != NULL && aux->id != id) { // Buscar por ID
        aux = aux->sig;
    }

    if (aux == NULL) { // Si no se encuentra
        cout<<"Proceso no encontrado."<<endl;
    } else {
        cout<<"Prioridad actual: "<<aux->prioridad<<endl;
        cout<<"Ingrese nueva prioridad: ";
        cin>>aux->prioridad; // Actualizar prioridad
        cout<<"Prioridad modificada correctamente."<<endl;
    }
}

void GestorDeProcesos() {
    int op;
    do {
        cout<<"\n=== GESTOR DE PROCESOS ===\n";
        cout<<"1. Crear nuevo proceso\n";
		cout<<"2. Eliminar proceso\n";
		cout<<"3. Listar procesos\n";
		cout<<"4. Cambiar prioridad\n";
		cout<<"5. Volver al menu\n";
        cout<<"Seleccione una opcion: ";
        cin>>op; 
		
		if(op<1 || op>5){
        	cout<<"Opcion invalida. Intente nuevamente."<<endl;
		}
		 
        switch (op) {
            case 1: 
				insertarProceso(); 
				break;
            case 2: 
				eliminarProceso(); 
				break;
            case 3: 
				listarProcesos(); 
				break;
            case 4: 
				modificarPrioridad(); 
				break;
            case 5: 
				break;
        }
    } while (op != 5);
}

void encolarProceso() {
	NodoCola* nuevo = new NodoCola; // Se crea un nuevo nodo din�mico para agregarlo a la cola

    cout<<"Ingrese ID del proceso: ";
    cin>>nuevo->idProceso; // Se almacena el ID del proceso en el nuevo nodo

    cout<<"Ingrese nombre del proceso: ";
    cin.ignore(); // Se limpia el b�fer para evitar errores al leer la cadena
    cin.getline(nuevo->nombre, 30); // Se lee el nombre del proceso (hasta 29 caracteres)

    nuevo->sig = NULL; // Se inicializa el puntero siguiente como NULL (ya que ser� el �ltimo nodo)

    if (frente == NULL) { // Si la cola est� vac�a
        frente = final = nuevo; // El nuevo nodo es tanto el frente como el final de la cola
    } else {
        final->sig = nuevo; // Se enlaza el nuevo nodo al final actual
        final = nuevo; // Se actualiza el final de la cola al nuevo nodo
    }

    cout<<"Proceso encolado correctamente."<<endl; // Mensaje de confirmaci�n
}

void desencolarProceso() {
    if (frente == NULL) //Verifica si la cola est� vac�a
	{
        cout<<"No hay procesos en cola"<<endl;
        return;
    }

    NodoCola*temp=frente;//Crea un puntero temporal para guardar el proceso a eliminar
    cout<<"Ejecutando y eliminando proceso:"<<temp->nombre<<"|ID:"<<temp->idProceso<<"|"<<endl;//Muestra informaci�n del proceso que ser� ejecutado (y eliminad
    frente = frente->sig;//Mueve el puntero frente al siguiente proceso
    delete temp;//Libera la memoria ocupada por el proceso eliminado

    // Si la cola qued� vac�a, actualizamos tambi�n el final
    if (frente==NULL) 
	{   
        final=NULL;//se actualiza el puntero final a NULL para mantener la integridad de la estructura
    }
    
}
	

void mostrarCola() {
    if(frente==NULL)//Verifica si la cola est� vac�a
	{
        cout<<"La cola de procesos est� vac�a."<<endl;
        return;
    }
    cout<<"===COLA DE PROCESOS==="<<endl;
    NodoCola*aux=frente;//Puntero auxiliar para recorrer la cola desde el frente
    while(aux!=NULL) {
        cout << "ID:"<<aux->idProceso<<"|Nombre:"<<aux->nombre<<endl;//Muestra los datos del proceso actual
        aux=aux->sig;//Avanza al siguiente proceso en la cola
    }
}


void PlanificadorCPU() {
    int op;
    do {
        cout<<"\n=== PLANIFICADOR DE CPU ===\n";
        cout<<"1. Encolar proceso\n";
		cout<<"2. Ejecutar siguiente\n";
		cout<<"3. Ver cola\n";
		cout<<"4. Volver al menu\n";
        cout<<"Seleccione una opcion: ";
        cin>>op;
        
        if(op<1 || op>4){
        	cout<<"Opcion invalida. Intente nuevamente."<<endl;
		}
        
        switch (op) {
            case 1:
            	encolarProceso(); 
				break;
            case 2:
				desencolarProceso(); 
				break;
            case 3:
				mostrarCola();
				break;
            case 4: 
				break;
        }
    } while (op != 4);
}

void pushMemoria() {
    
}

void popMemoria() {
   
}

void verMemoria() {
   
}

void GestorDeMemoria() {
    int op;
    do {
        cout<<"\n=== GESTOR DE MEMORIA ===\n";
        cout<<"1. Asignar memoria (push)\n";
		cout<<"2. Liberar memoria (pop)\n";
		cout<<"3. Ver estado de memoria\n";
		cout<<"4. Volver al menu\n";
        cout<<"Seleccione una opcion: ";
        cin>>op;
        
        if(op<1 || op>4){
        	cout<<"Opcion invalida. Intente nuevamente."<<endl;
		}
        
        switch (op) {
            case 1:
				pushMemoria();
				break;
            case 2:
				popMemoria();
				break;
            case 3:
				verMemoria();
				break;
            case 4:
				break;
        }
    } while (op != 4);
}

int main() {
    int op;
    do {
        cout<<"===== MENU PRINCIPAL =====\n";
        cout<<"1. Gestor de Procesos\n";
        cout<<"2. Planificador de CPU\n";
        cout<<"3. Gestor de Memoria\n";
        cout<<"4. Salir\n";
        cout<<"Seleccione una opcion: ";
        cin>>op;
        
        if(op<1 || op>4){
        	cout<<"Opcion invalida. Intente nuevamente."<<endl;
		}
        
        switch (op) {
            case 1:
				GestorDeProcesos(); 
				break;
            case 2:
				PlanificadorCPU(); 
				break;
            case 3: 
				GestorDeMemoria(); 
				break;
            case 4:
				cout<<"Saliendo del sistema..."<<endl; 
				break;
        }

        if (op != 4) {
            cout<<"\nPresione ENTER para continuar...";
            cin.ignore();
            cin.get();
            system("cls");
        }

    } while (op != 4);

    cout<<"Fin del programa."<<endl;
    return 0;
}
