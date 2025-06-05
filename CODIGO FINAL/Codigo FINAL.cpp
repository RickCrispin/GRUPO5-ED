#include <iostream>
using namespace std;

struct Proceso { //Estructura que define un proceso
    int id; //Identificador único del proceso
    char nombre[30]; //Nombre del proceso
    int prioridad; //Nivel de prioridad (menor número = mayor prioridad)
    int bloqueMemoria; //Bloque de memoria asignado
    Proceso* sig; //Puntero al siguiente proceso en la lista
};

Proceso* cabeza = NULL; //Inicio de la lista enlazada de procesos

struct NodoCola { //Nodo para la cola de planificación
    Proceso* refProceso; //Referencia al proceso correspondiente
    NodoCola* sig; //Puntero al siguiente nodo de la cola
};

NodoCola* frente = NULL; //Inicio de la cola
NodoCola* final = NULL; //Fin de la cola

struct NodoPila { //Nodo para la pila de bloques de memoria
    int idProceso; //ID del proceso que usa el bloque
    int bloqueMemoria; //Número del bloque de memoria
    NodoPila* sig; //Puntero al siguiente nodo en la pila
};

NodoPila* cima = NULL; //Cima de la pila

void asignarMemoria(int id, int bloque) { //Agrega un bloque a la pila de memoria
    NodoPila* nuevo = new NodoPila; //Reserva memoria para el nuevo nodo
    nuevo->idProceso = id; //Asigna ID del proceso
    nuevo->bloqueMemoria = bloque; //Asigna número del bloque
    nuevo->sig = cima; //Apunta al nodo anterior en la pila
    cima = nuevo; //Actualiza la cima
}

void liberarMemoria(int id) { //Elimina un bloque de la pila según el ID del proceso
    NodoPila* actual = cima; //Puntero auxiliar para recorrer la pila
    NodoPila* anterior = NULL; //Puntero para mantener el nodo anterior
    while (actual != NULL && actual->idProceso != id) { //Busca el nodo con el ID
        anterior = actual;
        actual = actual->sig;
    }
    if (actual != NULL) { //Si se encuentra el nodo
        if (anterior == NULL) {
            cima = actual->sig; //Si es el primero, actualiza la cima
        } else {
            anterior->sig = actual->sig; //Sino, enlaza el anterior con el siguiente
        }
        delete actual; //Libera la memoria del nodo
    }
}

void encolarPorPrioridad(Proceso* p) { //Inserta un proceso en la cola según prioridad
    NodoCola* nuevo = new NodoCola; //Crea un nuevo nodo para la cola
    nuevo->refProceso = p; //Asocia el proceso
    nuevo->sig = NULL; //Inicializa el siguiente como NULL
    if (frente == NULL || p->prioridad < frente->refProceso->prioridad) { //Si es la primera posición
        nuevo->sig = frente; //Apunta al anterior frente
        frente = nuevo; //Nuevo nodo es el nuevo frente
        if (final == NULL) final = nuevo; //Si la cola estaba vacía, final también apunta
        return;
    }
    NodoCola* actual = frente; //Recorre la cola para insertar por prioridad
    NodoCola* anterior = NULL;
    while (actual != NULL && actual->refProceso->prioridad <= p->prioridad) {
        anterior = actual;
        actual = actual->sig;
    }
    nuevo->sig = actual; //Inserta el nuevo nodo en su posición
    anterior->sig = nuevo;
    if (nuevo->sig == NULL) final = nuevo; //Si es el último, actualiza final
}

void eliminarDeCola(int id) { //Elimina un proceso de la cola por ID
    NodoCola* actual = frente; //Puntero auxiliar
    NodoCola* anterior = NULL; //Nodo anterior para enlazar
    while (actual != NULL && actual->refProceso->id != id) { //Busca el proceso por ID
        anterior = actual;
        actual = actual->sig;
    }
    if (actual != NULL) { //Si se encuentra
        if (anterior == NULL) {
            frente = actual->sig; //Es el primero
        } else {
            anterior->sig = actual->sig; //Enlaza el anterior con el siguiente
        }
        if (actual == final) final = anterior; //Si era el último, actualiza final
        delete actual; //Libera el nodo
    }
}

void insertarProceso() { //Permite crear y registrar un nuevo proceso
    int bloqueTemp, idTemp;
    bool bloqueDuplicado, idDuplicado;
    do {
        idDuplicado = false;
        cout<<"Ingrese ID del proceso: ";
        cin>>idTemp;
        Proceso* aux = cabeza;
        while (aux != NULL) {
            if (aux->id == idTemp) {
                cout<<"Ese ID ya esta en uso. Intente con otro."<<endl;
                idDuplicado = true;
                break;
            }
            aux = aux->sig;
        }
    } while (idDuplicado);
    Proceso* nuevo = new Proceso; //Crea nuevo proceso
    nuevo->id = idTemp; //Asigna ID
    cout<<"Ingrese nombre del proceso: ";
    cin.ignore();
    cin.getline(nuevo->nombre, 30); //Lee nombre del proceso
    cout<<"Ingrese prioridad: ";
    cin>>nuevo->prioridad;
    do {
        bloqueDuplicado = false;
        cout<<"Ingrese numero de bloque de memoria a asignar: ";
        cin>>bloqueTemp;
        NodoPila* aux = cima;
        while (aux != NULL) {
            if (aux->bloqueMemoria == bloqueTemp) {
                cout<<"Ese bloque ya esta asignado. Intente con otro."<<endl;
                bloqueDuplicado = true;
                break;
            }
            aux = aux->sig;
        }
    } while (bloqueDuplicado);
    nuevo->bloqueMemoria = bloqueTemp; //Asigna bloque de memoria
    nuevo->sig = NULL; //Inicializa puntero siguiente
    if (cabeza == NULL) {
        cabeza = nuevo; //Si lista vacía, nuevo es la cabeza
    } else {
        Proceso* aux = cabeza;
        while (aux->sig != NULL) {
            aux = aux->sig;
        }
        aux->sig = nuevo; //Agrega al final
    }
    encolarPorPrioridad(nuevo); //Encola por prioridad
    asignarMemoria(nuevo->id, nuevo->bloqueMemoria); //Asigna memoria en pila
    cout<<"Proceso creado exitosamente."<<endl;
}

void eliminarProceso() { //Elimina un proceso por ID
    int id;
    cout<<"Ingrese ID del proceso a eliminar: ";
    cin>>id;
    Proceso* actual = cabeza;
    Proceso* anterior = NULL;
    while (actual != NULL && actual->id != id) {
        anterior = actual;
        actual = actual->sig;
    }
    if (actual == NULL) {
        cout<<"Proceso no encontrado."<<endl;
    } else {
        if (anterior == NULL) {
            cabeza = actual->sig;
        } else {
            anterior->sig = actual->sig;
        }
        eliminarDeCola(id); //Quita de la cola
        liberarMemoria(id); //Libera bloque de memoria
        delete actual; //Elimina nodo
        cout<<"Proceso eliminado correctamente."<<endl;
    }
}

void listarProcesos() { //Muestra todos los procesos registrados
    Proceso* aux = cabeza;
    if (aux == NULL) {
        cout<<"No hay procesos registrados."<<endl;
        return;
    }
    cout<<"=== LISTA DE PROCESOS ==="<<endl;
    while (aux != NULL) {
        cout<<"ID: "<<aux->id<<", Nombre: "<<aux->nombre<<", Prioridad: "<<aux->prioridad<<", Memoria: "<<aux->bloqueMemoria<<endl;
        aux = aux->sig;
    }
}

void modificarPrioridad() { //Permite cambiar la prioridad de un proceso
    int id;
    cout<<"Ingrese ID del proceso a modificar: ";
    cin>>id;
    Proceso* aux = cabeza;
    while (aux != NULL && aux->id != id) {
        aux = aux->sig;
    }
    if (aux == NULL) {
        cout<<"Proceso no encontrado."<<endl;
    } else {
        cout<<"Prioridad actual: "<<aux->prioridad<<endl;
        eliminarDeCola(id); //Elimina de la cola actual
        cout<<"Ingrese nueva prioridad: ";
        cin>>aux->prioridad;
        encolarPorPrioridad(aux); //Reinserta en la cola
        cout<<"Prioridad modificada correctamente."<<endl;
    }
}

void desencolarProceso() { //Ejecuta y elimina el primer proceso en la cola
    if (frente == NULL) {
        cout<<"No hay procesos en cola."<<endl;
    } else {
        NodoCola* temp = frente;
        frente = frente->sig;
        cout<<"Ejecutando proceso: "<<temp->refProceso->nombre<<", ID: "<<temp->refProceso->id<<endl;
        if (frente == NULL) final = NULL;
        delete temp;
    }
}

void mostrarCola() { //Muestra los procesos en cola según prioridad
    NodoCola* aux = frente;
    if (aux == NULL) {
        cout<<"Cola vacia."<<endl;
        return;
    }
    cout<<"=== COLA DE PROCESOS ===\n";
    while (aux != NULL) {
        cout<<"ID: "<<aux->refProceso->id<<", Nombre: "<<aux->refProceso->nombre<<", Prioridad: "<<aux->refProceso->prioridad<<endl;
        aux = aux->sig;
    }
}

void verMemoria() { //Muestra los bloques de memoria asignados
    NodoPila* aux = cima;
    if (aux == NULL) {
        cout<<"Pila de memoria vacia."<<endl;
        return;
    }
    cout<<"=== ESTADO DE MEMORIA ===\n";
    while (aux != NULL) {
        cout<<"ID Proceso: "<<aux->idProceso<<", Bloque: "<<aux->bloqueMemoria<<endl;
        aux = aux->sig;
    }
}

void GestorDeProcesos() { //Submenú para gestionar procesos
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

void PlanificadorCPU() { //Submenú para ejecutar y ver cola de procesos
    int op;
    do {
        cout<<"\n=== PLANIFICADOR DE CPU ===\n";
        cout<<"1. Ejecutar siguiente\n";
        cout<<"2. Ver cola\n";
        cout<<"3. Volver al menu\n";
        cout<<"Seleccione una opcion: ";
        cin>>op;
        if(op<1 || op>3){
            cout<<"Opcion invalida. Intente nuevamente."<<endl;
        }
        switch (op) {
            case 1: 
				desencolarProceso(); 
				break;
            case 2: 
				mostrarCola(); 
				break;
            case 3: 
				break;
        }
    } while (op != 3);
}

void GestorDeMemoria() { //Submenú para gestionar la memoria
    int op;
    do {
        cout<<"\n=== GESTOR DE MEMORIA ===\n";
        cout<<"1. Ver estado de memoria\n";
        cout<<"2. Volver al menu\n";
        cout<<"Seleccione una opcion: ";
        cin>>op;
        if(op<1 || op>2){
            cout<<"Opcion invalida. Intente nuevamente."<<endl;
        }
        switch (op) {
            case 1: 
				verMemoria(); 
				break;
            case 2: 
				break;
        }
    } while (op != 2);
}

int main() { //Menú principal del sistema
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



