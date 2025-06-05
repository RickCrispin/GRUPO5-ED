#include <iostream>
using namespace std;

struct Proceso {
    int id; // ID del proceso
    char nombre[30]; // Nombre del proceso
    int prioridad; // Nivel de prioridad
    int bloqueMemoria; // Bloque de memoria asignado
    Proceso* sig; // Apunta al siguiente proceso
};

Proceso* cabeza = NULL; // Inicio de la lista de procesos

struct NodoCola {
    Proceso* refProceso; // Referencia al proceso
    NodoCola* sig; // Apunta al siguiente nodo en la cola
};

NodoCola* frente = NULL; // Inicio de la cola
NodoCola* final = NULL; // Final de la cola

struct NodoPila {
    int idProceso; // ID del proceso asociado al bloque
    int bloqueMemoria; // Bloque de memoria reservado
    NodoPila* sig; // Apunta al siguiente nodo en la pila
};

NodoPila* cima = NULL; // Cima de la pila de memoria

void asignarMemoria(int id, int bloque) {
    NodoPila* nuevo = new NodoPila; // Crea nuevo nodo
    nuevo->idProceso = id; // Asigna ID
    nuevo->bloqueMemoria = bloque; // Asigna bloque
    nuevo->sig = cima; // Apunta al nodo anterior
    cima = nuevo; // Actualiza cima
}

void liberarMemoria(int id) {
    NodoPila* actual = cima; NodoPila* anterior = NULL; // Punteros auxiliares
    while (actual != NULL && actual->idProceso != id) { anterior = actual; actual = actual->sig; } // Buscar nodo
    if (actual != NULL) {
        if (anterior == NULL) { cima = actual->sig; } // Es la cima
        else { anterior->sig = actual->sig; } // Nodo intermedio o final
        delete actual; // Libera memoria
    }
}

void encolarPorPrioridad(Proceso* p) {
    NodoCola* nuevo = new NodoCola; nuevo->refProceso = p; nuevo->sig = NULL; // Nodo nuevo
    if (frente == NULL || p->prioridad < frente->refProceso->prioridad) {
        nuevo->sig = frente; frente = nuevo; if (final == NULL) final = nuevo; return; // Insertar al frente
    }
    NodoCola* actual = frente; NodoCola* anterior = NULL;
    while (actual != NULL && actual->refProceso->prioridad <= p->prioridad) {
        anterior = actual; actual = actual->sig; // Avanzar
    }
    nuevo->sig = actual; anterior->sig = nuevo; // Insertar en medio
    if (nuevo->sig == NULL) final = nuevo; // Actualiza final si es necesario
}

void eliminarDeCola(int id) {
    NodoCola* actual = frente; NodoCola* anterior = NULL;
    while (actual != NULL && actual->refProceso->id != id) { anterior = actual; actual = actual->sig; }
    if (actual != NULL) {
        if (anterior == NULL) frente = actual->sig; else anterior->sig = actual->sig; // Reajustar enlaces
        if (actual == final) final = anterior; // Actualiza final
        delete actual; // Libera memoria
    }
}

void insertarProceso() {
    Proceso* nuevo = new Proceso;
    cout<<"Ingrese ID del proceso: "; cin>>nuevo->id;
    cout<<"Ingrese nombre del proceso: "; cin.ignore(); cin.getline(nuevo->nombre, 30);
    cout<<"Ingrese prioridad: "; cin>>nuevo->prioridad;
    cout<<"Ingrese numero de bloque de memoria a asignar: "; cin>>nuevo->bloqueMemoria;
    nuevo->sig = NULL;
    if (cabeza == NULL) cabeza = nuevo;
    else { Proceso* aux = cabeza; while (aux->sig != NULL) aux = aux->sig; aux->sig = nuevo; }
    encolarPorPrioridad(nuevo); asignarMemoria(nuevo->id, nuevo->bloqueMemoria);
    cout<<"Proceso creado exitosamente."<<endl;
}

void eliminarProceso() {
    int id; cout<<"Ingrese ID del proceso a eliminar: "; cin>>id;
    Proceso* actual = cabeza; Proceso* anterior = NULL;
    while (actual != NULL && actual->id != id) { anterior = actual; actual = actual->sig; }
    if (actual == NULL) cout<<"Proceso no encontrado."<<endl;
    else {
        if (anterior == NULL) cabeza = actual->sig; else anterior->sig = actual->sig;
        eliminarDeCola(id); liberarMemoria(id); delete actual;
        cout<<"Proceso eliminado correctamente."<<endl;
    }
}

void listarProcesos() {
    Proceso* aux = cabeza;
    if (aux == NULL) { cout<<"No hay procesos registrados."<<endl; return; }
    cout<<"=== LISTA DE PROCESOS ==="<<endl;
    while (aux != NULL) {
        cout<<"ID: "<<aux->id<<", Nombre: "<<aux->nombre<<", Prioridad: "<<aux->prioridad<<", Memoria: "<<aux->bloqueMemoria<<endl;
        aux = aux->sig;
    }
}

void modificarPrioridad() {
    int id; cout<<"Ingrese ID del proceso a modificar: "; cin>>id;
    Proceso* aux = cabeza;
    while (aux != NULL && aux->id != id) aux = aux->sig;
    if (aux == NULL) cout<<"Proceso no encontrado."<<endl;
    else {
        cout<<"Prioridad actual: "<<aux->prioridad<<endl;
        eliminarDeCola(id);
        cout<<"Ingrese nueva prioridad: "; cin>>aux->prioridad;
        encolarPorPrioridad(aux);
        cout<<"Prioridad modificada correctamente."<<endl;
    }
}

void GestorDeProcesos() {
    int op;
    do {
        cout<<"\n=== GESTOR DE PROCESOS ===\n";
        cout<<"1. Crear nuevo proceso\n2. Eliminar proceso\n3. Listar procesos\n4. Cambiar prioridad\n5. Volver al menu\nSeleccione una opcion: ";
        cin>>op;
        if(op<1 || op>5) cout<<"Opcion invalida. Intente nuevamente."<<endl;
        switch (op) {
            case 1: insertarProceso(); break;
            case 2: eliminarProceso(); break;
            case 3: listarProcesos(); break;
            case 4: modificarPrioridad(); break;
        }
    } while (op != 5);
}

void desencolarProceso() {
    if (frente == NULL) cout<<"No hay procesos en cola."<<endl;
    else {
        NodoCola* temp = frente;
        frente = frente->sig;
        cout<<"Ejecutando proceso: "<<temp->refProceso->nombre<<", ID: "<<temp->refProceso->id<<endl;
        if (frente == NULL) final = NULL;
        delete temp;
    }
}

void mostrarCola() {
    NodoCola* aux = frente;
    if (aux == NULL) { cout<<"Cola vacia."<<endl; return; }
    cout<<"=== COLA DE PROCESOS ===\n";
    while (aux != NULL) {
        cout<<"ID: "<<aux->refProceso->id<<", Nombre: "<<aux->refProceso->nombre<<", Prioridad: "<<aux->refProceso->prioridad<<endl;
        aux = aux->sig;
    }
}

void PlanificadorCPU() {
    int op;
    do {
        cout<<"\n=== PLANIFICADOR DE CPU ===\n";
        cout<<"1. Ejecutar siguiente\n2. Ver cola\n3. Volver al menu\nSeleccione una opcion: ";
        cin>>op;
        if(op<1 || op>3) cout<<"Opcion invalida. Intente nuevamente."<<endl;
        switch (op) {
            case 1: desencolarProceso(); break;
            case 2: mostrarCola(); break;
        }
    } while (op != 3);
}

void verMemoria() {
    NodoPila* aux = cima;
    if (aux == NULL) { cout<<"Pila de memoria vacia."<<endl; return; }
    cout<<"=== ESTADO DE MEMORIA ===\n";
    while (aux != NULL) {
        cout<<"ID Proceso: "<<aux->idProceso<<", Bloque: "<<aux->bloqueMemoria<<endl;
        aux = aux->sig;
    }
}

void GestorDeMemoria() {
    int op;
    do {
        cout<<"\n=== GESTOR DE MEMORIA ===\n";
        cout<<"1. Ver estado de memoria\n2. Volver al menu\nSeleccione una opcion: ";
        cin>>op;
        if(op<1 || op>2) cout<<"Opcion invalida. Intente nuevamente."<<endl;
        switch (op) {
            case 1: verMemoria(); break;
        }
    } while (op != 2);
}

int main() {
    int op;
    do {
        cout<<"===== MENU PRINCIPAL =====\n";
        cout<<"1. Gestor de Procesos\n2. Planificador de CPU\n3. Gestor de Memoria\n4. Salir\nSeleccione una opcion: ";
        cin>>op;
        if(op<1 || op>4) cout<<"Opcion invalida. Intente nuevamente."<<endl;
        switch (op) {
            case 1: GestorDeProcesos(); break;
            case 2: PlanificadorCPU(); break;
            case 3: GestorDeMemoria(); break;
            case 4: cout<<"Saliendo del sistema..."<<endl; break;
        }
        if (op != 4) {
            cout<<"\nPresione ENTER para continuar...";
            cin.ignore(); cin.get(); system("cls");
        }
    } while (op != 4);
    cout<<"Fin del programa."<<endl;
    return 0;
}

