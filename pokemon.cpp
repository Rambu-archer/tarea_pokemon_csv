#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Pokemon {
    int numero;
    string nombre;
    string tipo1;
    string tipo2;
    int total;
    int hp;
    int ataque;
    int defensa;
    int ataqueEsp;
    int defensaEsp;
    int velocidad;
    int generacion;
    bool legendario;
};

string limpiar(string texto) {
    size_t inicio = texto.find_first_not_of(" \r\n\t");
    if (inicio == string::npos) return "";

    size_t fin = texto.find_last_not_of(" \r\n\t");
    return texto.substr(inicio, fin - inicio + 1);
}

string minusculas(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });
    return texto;
}

bool leerCSV(string archivo, vector<Pokemon>& pokemones) {
    ifstream entrada(archivo);

    if (!entrada.is_open()) {
        cout << "Error: no se pudo abrir el archivo CSV.\n";
        return false;
    }

    string linea;
    getline(entrada, linea); // Omitir encabezado

    while (getline(entrada, linea)) {
        stringstream ss(linea);
        string campo;
        vector<string> datos;

        while (getline(ss, campo, ',')) {
            datos.push_back(limpiar(campo));
        }

        if (datos.size() < 13) continue;

        try {
            Pokemon p;

            p.numero = stoi(datos[0]);
            p.nombre = datos[1];
            p.tipo1 = datos[2];
            p.tipo2 = datos[3];
            p.total = stoi(datos[4]);
            p.hp = stoi(datos[5]);
            p.ataque = stoi(datos[6]);
            p.defensa = stoi(datos[7]);
            p.ataqueEsp = stoi(datos[8]);
            p.defensaEsp = stoi(datos[9]);
            p.velocidad = stoi(datos[10]);
            p.generacion = stoi(datos[11]);
            p.legendario = (datos[12] == "True" ||
                            datos[12] == "true");

            pokemones.push_back(p);
        } catch (...) {
            cout << "Se omitio una fila con datos invalidos.\n";
        }
    }

    entrada.close();
    return true;
}

void mostrarPokemon(const Pokemon& p) {
    cout << "\n----------------------------------\n";
    cout << "Numero Pokedex: " << p.numero << '\n';
    cout << "Nombre: " << p.nombre << '\n';
    cout << "Tipo 1: " << p.tipo1 << '\n';
    cout << "Tipo 2: "
         << (p.tipo2.empty() ? "Ninguno" : p.tipo2) << '\n';
    cout << "Total: " << p.total << '\n';
    cout << "HP: " << p.hp << '\n';
    cout << "Ataque: " << p.ataque << '\n';
    cout << "Defensa: " << p.defensa << '\n';
    cout << "Ataque especial: " << p.ataqueEsp << '\n';
    cout << "Defensa especial: " << p.defensaEsp << '\n';
    cout << "Velocidad: " << p.velocidad << '\n';
    cout << "Generacion: " << p.generacion << '\n';
    cout << "Legendario: "
         << (p.legendario ? "Si" : "No") << '\n';
    cout << "----------------------------------\n";
}

void buscarNumero(const vector<Pokemon>& pokemones) {
    int numero;
    cout << "Introduce el numero de Pokedex: ";

    if (!(cin >> numero)) {
        cout << "Debes introducir un numero valido.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    for (const Pokemon& p : pokemones) {
        if (p.numero == numero) {
            mostrarPokemon(p);
            return;
        }
    }

    cout << "No se encontro ese numero de Pokedex.\n";
}

void buscarNombre(const vector<Pokemon>& pokemones) {
    string nombre;
    cout << "Introduce el nombre del Pokemon: ";
    getline(cin >> ws, nombre);

    string buscado = minusculas(nombre);
    bool encontrado = false;

    for (const Pokemon& p : pokemones) {
        if (minusculas(p.nombre).find(buscado) != string::npos) {
            mostrarPokemon(p);
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron coincidencias.\n";
    }
}

void buscarTipo(const vector<Pokemon>& pokemones) {
    string tipo;
    cout << "Introduce el tipo (por ejemplo, Fire, Water, Grass): ";
    getline(cin >> ws, tipo);

    string buscado = minusculas(tipo);
    bool encontrado = false;

    for (const Pokemon& p : pokemones) {
        if (minusculas(p.tipo1) == buscado ||
            minusculas(p.tipo2) == buscado) {
            cout << p.numero << " - " << p.nombre
                 << " | Tipo: " << p.tipo1;

            if (!p.tipo2.empty()) {
                cout << " / " << p.tipo2;
            }

            cout << " | Total: " << p.total << '\n';
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron Pokemon de ese tipo.\n";
    }
}

void mostrarEstadisticas(vector<Pokemon> pokemones) {
    if (pokemones.empty()) {
        cout << "No hay datos disponibles.\n";
        return;
    }

    sort(pokemones.begin(), pokemones.end(),
        [](const Pokemon& a, const Pokemon& b) {
            return a.total > b.total;
        });

    cout << "\n=== TOP 3 POKEMON MAS FUERTES ===\n";

    int cantidad = min(3, static_cast<int>(pokemones.size()));

    for (int i = 0; i < cantidad; i++) {
        cout << i + 1 << ". " << pokemones[i].nombre
             << " - Total: " << pokemones[i].total << '\n';
    }

    cout << "\n=== TOP 3 POKEMON MAS DEBILES ===\n";

    for (int i = 0; i < cantidad; i++) {
        int indice = static_cast<int>(pokemones.size()) - 1 - i;

        cout << i + 1 << ". " << pokemones[indice].nombre
             << " - Total: " << pokemones[indice].total << '\n';
    }
}

void menu() {
    cout << "\n========== MENU POKEMON ==========\n";
    cout << "1. Buscar por numero de Pokedex\n";
    cout << "2. Buscar Pokemon por nombre\n";
    cout << "3. Ver Pokemon de un tipo\n";
    cout << "4. Consultar informacion por nombre\n";
    cout << "5. Ver top 3 mas fuertes y mas debiles\n";
    cout << "0. Salir\n";
    cout << "Selecciona una opcion: ";
}

int main() {
    vector<Pokemon> pokemones;

    if (!leerCSV("pokemon_150_registros.csv", pokemones)) {
        return 1;
    }

    cout << "Se cargaron " << pokemones.size()
         << " Pokemon correctamente.\n";

    int opcion;

    do {
        menu();

        if (!(cin >> opcion)) {
            cout << "Introduce una opcion numerica.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                buscarNumero(pokemones);
                break;

            case 2:
                buscarNombre(pokemones);
                break;

            case 3:
                buscarTipo(pokemones);
                break;

            case 4:
                buscarNombre(pokemones);
                break;

            case 5:
                mostrarEstadisticas(pokemones);
                break;

            case 0:
                cout << "Programa finalizado.\n";
                break;

            default:
                cout << "Opcion no valida. Intenta nuevamente.\n";
        }

    } while (opcion != 0);

    return 0;
}
