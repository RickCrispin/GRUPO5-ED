// Commit:Implementación completa con cola y prioridad
// Integrante: Olarte Tuya Shadi Rashel Allegra 

#include <iostream>

using namespace std;

struct Proceso {
	int id;
	char nombre[30];
	int prioridad;
	Proceso* sig;
};

Proceso* cabeza = NULL;


// Variables globales para cola de CPU (por prioridad)
Proceso* frenteCola = NULL;
Proceso* finalCola = NULL;

// Variables globales para pila de memoria
Proceso* topePila = NULL;

void insertarProceso() {
   Proceso* nuevo = new Proceso;
   
   cout<<"Ingrese ID del proceso: ";
   cin>>nuevo->id;
   
   cout<<"Ingrese nombre del proceso: ";
   cin.ignore();
   cin.getline(nuevo->nombre, 30);
   cout<<"Ingrese prioridad: ";
   cin>>nuevo->prioridad;
   nuevo->sig = NULL;
   
   if (cabeza == NULL){
   		cabeza = nuevo;
   }
   else{
   		Proceso* aux = cabeza;
   		while (aux->sig != NULL){
   			aux = aux->sig;
		   }
		   aux->sig = nuevo;
	}
	
	cout<<"Proceso creado exitosamente."<<endl;
}

void eliminarProceso() {
    int id;
    cout << "Ingrese ID del proceso a eliminar: ";
    cin >> id;

    Proceso*actual=cabeza;//Puntero para recorrer la lista
    Proceso*anterior=NULL; //Puntero para guardar el espacio anterior

    //Buscar el proceso con el ID ingresado
    while (actual != NULL && actual->id != id) {
        anterior=actual;//Avanzar el puntero anterior
        actual=actual->sig;//Avanzar al siguiente esapacio
    }

    //Si no se encontr? el proceso
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
	
   
}

void modificarPrioridad() {
	 int id, nuevaPrioridad;
    cout << "Ingrese ID del proceso: ";
    cin >> id;
    
    // Buscar el proceso por ID
    Proceso* actual = cabeza;
    while (actual != NULL && actual->id != id) {
        actual = actual->sig;
    }
    
    if (actual == NULL) {
        cout << "Proceso no encontrado." << endl;
        return;
    }
    
    cout << "Prioridad actual: " << actual->prioridad << endl;
    cout << "Ingrese nueva prioridad : ";
    cin >> nuevaPrioridad;
    
    actual->prioridad = nuevaPrioridad;
    cout << "Prioridad modificada exitosamente." << endl;

   
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
	 int id;
    cout << "Ingrese ID del proceso a encolar: ";
    cin >> id;
    
    // Buscar el proceso en la lista principal
    Proceso* buscar = cabeza;
    while (buscar != NULL && buscar->id != id) {
        buscar = buscar->sig;
    }
    
    if (buscar == NULL) {
        cout << "Proceso no encontrado en la lista." << endl;
        return;
    
}
    // Crear una copia del proceso para la cola
    Proceso* nuevo = new Proceso;
    nuevo->id = buscar->id;
    strcpy(nuevo->nombre, buscar->nombre);  // Copiar el nombre del proceso
    nuevo->prioridad = buscar->prioridad;
    nuevo->sig = NULL;
    
    // Insertar en cola ordenada por prioridad (1 = menor número = mayor prioridad)
    if (frenteCola == NULL) {
        // Cola vacía - primer elemento
        frenteCola = finalCola = nuevo;
    } else if (nuevo->prioridad < frenteCola->prioridad) {
        // Insertar al frente (menor número = mayor prioridad)
        nuevo->sig = frenteCola;
        frenteCola = nuevo;
    } else {
        // Buscar posición correcta manteniendo orden ascendente por prioridad
        Proceso* actual = frenteCola;
        while (actual->sig != NULL && actual->sig->prioridad < nuevo->prioridad) {
            actual = actual->sig;
        }
        nuevo->sig = actual->sig;
        actual->sig = nuevo;
        
        // Actualizar final si se insertó al final
        if (nuevo->sig == NULL) {
            finalCola = nuevo;
        }
    }
    
    cout << "Proceso encolado exitosamente." << endl;
}

void desencolarProceso() {
	if (frenteCola == NULL) {
        cout << "Cola vacía. No hay procesos para ejecutar." << endl;
        return;
    }
    
    Proceso* ejecutar = frenteCola;
    cout << "Ejecutando proceso:" << endl;
    cout << "ID: " << ejecutar->id << endl;
    cout << "Nombre: " << ejecutar->nombre << endl;
    cout << "Prioridad: " << ejecutar->prioridad << endl;
    
    // Remover de la cola (desencolado)
    frenteCola = frenteCola->sig;
    if (frenteCola == NULL) {
        finalCola = NULL; // Cola quedó vacía
    }
    
    delete ejecutar;
    cout << "Proceso ejecutado y removido de la cola." << endl;
}

   
}

void mostrarCola() {
   
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
