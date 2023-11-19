#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

template<class T>
void afficheVecteur(const vector<T> v) {
    // {v.size() > 0} => {pretty print de v}
    cout << "[" << v[0];
    int i = 1;
    while (i < v.size()) {
        cout << ", " << v[i];
        i++;
    }
    cout << "]" << endl;
}

template<class T>
void swap(vector<T> &v, const int first, const int second) {
    // {v.size() >0} => {permutation des valeurs de v aux indices first et second}

    // Sauvegarde une des valeurs
    T indexDeux = v[second];
    // 2 -> 1
    v[second] = v[first];
    // 1 -> 2[s]
    v[first] = indexDeux;
}

template<class T>
T partage(vector<T> &v, const int inf, const int sup) {
// {v.size()≥ 1} => {résultat = valeur du médian tel que défini dans l’étape 1)
//                   et mise en place du plus petit et du plus grand dans v telles que définies dans l’étape 2)}
    int midle = round((inf+sup) / 2);

    int pivot = v[inf];
    int leftwall = inf;

    for (int i = inf + 1; i <= sup; i++){
        if (v[i] < pivot){
            swap(v, v[i], v[leftwall]);
            leftwall += 1;
        }
    }
    swap(v,pivot, v[leftwall]);
    return leftwall;

}

template<class T>
void insertionSort(vector<T> &a) {


}

template<class T>
void triR302Worker(vector<T> &v, const int inf, const int sup) {

    // Trouver le pivot qui serait le plus proche de la médianne du vecteur entier.
    if (inf < sup){
        int pivot_location = partage(v, inf, sup);
    }

}

template<class T>
void triR302(vector<T> &v, const int inf, const int sup) {

    triR302Worker(v, inf, sup);
}

void testTriR302() {
    cout << "***************************" << endl;
    cout << "*          triR302        *" << endl;
    cout << "***************************" << endl;
    vector<int> v = {10, 3, 6, 8, 67, 2, 9, 1};
    cout << "vecteur initial : ";
    afficheVecteur(v);
    triR302(v, 0, (int) v.size() - 1);
    cout << "vecteur trié : ";
    afficheVecteur(v);

    v.clear();
    srand((unsigned) time(NULL));
    for (int i = 0; i < 128; i++) {
        int b = rand() % 128 + 1;
        v.push_back(b);
    }
    cout << "vecteur initial : ";
    afficheVecteur(v);
    triR302(v, 0, (int) v.size() - 1);
    cout << "vecteur trié : ";
    afficheVecteur(v);
}

int main(int argc, char **argv) {
    testTriR302();
    return 0;
}