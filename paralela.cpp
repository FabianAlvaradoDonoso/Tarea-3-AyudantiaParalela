#include <mpi.h>    //MPI_*
#include <vector>   //vector<int>
#include <math.h>   //pow()
#include <cstdlib>  //rand()

using namespace std;

//subdivide el arreglo en partes mas pequeñas dando un inicio y fin
vector<int> crearSubArreglo(vector<int> v, int inicio, int fin) { 
     vector<int> subArreglo; //vector a devolver

     for (int i = inicio; i <= fin; i++) { //se recorre de inicio a fin
         subArreglo.push_back(v[inicio + i]); //se hace un push del valor en el vector nuevo
     }

     return subArreglo; 
}

//llena el vector con numeros "aleatorios"
void llenarArregloV(vector<int> &arreglo, int tamanio){
    for (int i = 0; i < tamanio; i++) {
        arreglo.push_back(1 + rand() % (1001 - 1)); //funcion rand() devuelve numero "aleatorio"
    }
}

//Imprimir subvectores, int i es solo para saber que subvector se esta imprimiendo
void imprimirSubVector(vector<int> v, int i){ 
    cout << "SubVector grupo" << i << ": [";
    for(int elemento : v){ //recorrido de vector tipo foreach
       cout << elemento << ", ";
    }
    cout << "]" << endl;
}

void imprimirVector(vector<int> v){
    cout << "Vector: [";
    for(int elemento : v){
        cout << elemento << ", ";
    }
    cout << "]" << endl;
}

float obtenerPromedio(vector<int> v){
    int suma = 0;
    for (int i : v)
    {
        suma += i;
    }
    return (float) suma / v.size();
}

float varianza(vector<int> v, int prom) {
    float sumaCuadrados = 0;
    for (int elemento : v) {
        sumaCuadrados += (float) pow(elemento - prom, 2);
    }
    return (float) sumaCuadrados / v.size();
}


int main(){

    vector<int> arreglo, subArreglo;
    int tamanio = 10000;
    int tamSubVectores;
    float subPromedio;

    llenarArregloV(arreglo, tamanio);
    // imprimirVector(arreglo);

    MPI_Init(NULL, NULL);
    
    int cantProcesadores = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesadores);
    
    int procesador = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procesador);
    
    MPI_Status status;



   if (procesador == 0) {
       float sumPromedio = 0;
       float subPromedioR;

        for(int i = 1; i < cantProcesadores; i++){
            MPI_Recv(&subPromedioR, 1, MPI_FLOAT, MPI_ANY_SOURCE, 7, MPI_COMM_WORLD, &status);
            cout << "Subpromedio " << i << ": " << subPromedioR << endl;
            sumPromedio += subPromedioR;
        }

        cout << "Suma Final: " << sumPromedio << endl;
        cout << "Promedio Final: " << sumPromedio/cantProcesadores << endl;
        cout << "Varianza: " << varianza(arreglo, sumPromedio/cantProcesadores) << endl;
        cout << "Desviacion Estandar: " << sqrt(varianza(arreglo, sumPromedio/cantProcesadores)) << endl;


    } else {

        tamSubVectores = arreglo.size()/(cantProcesadores-1);
        // cout << "hola" << endl;
        for (int i = 1; i < cantProcesadores; i++) {
            if(i != (cantProcesadores - 1)){ //Procesador correcto y no es el final
                int limiteIzq = (procesador - 1) * tamSubVectores;
                int limiteDer = limiteIzq + tamSubVectores - 1;

                subArreglo = crearSubArreglo(arreglo, limiteIzq, limiteDer);
                float subPromedio = obtenerPromedio(subArreglo);

                MPI_Send(&subPromedio, 1, MPI_FLOAT, 0, 7, MPI_COMM_WORLD);

            } else if(i == (cantProcesadores - 1)){ //Procesador correcto y final
                int limiteIzq = (procesador - 1) * tamSubVectores;
                int limiteDer = arreglo.size() - 1;

                subArreglo = crearSubArreglo(arreglo, limiteIzq, limiteDer);
                float subPromedio = obtenerPromedio(subArreglo);

                MPI_Send(&subPromedio, 1, MPI_FLOAT, 0, 7, MPI_COMM_WORLD);
            }

        }
    }

    MPI_Finalize();
    return 0;

}
