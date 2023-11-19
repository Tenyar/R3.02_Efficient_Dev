#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>

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
void minMaxIter(const vector<T> v, T &min, T &max, int &nbComp) {
    // {v.size()≥ 1} => {min = plus petite valeur de v,
    //                   max = plus grande valeur de v,
    //                   nbcomp = nombre de comparaisons impliquant au moins un élément de v}

    if (v.size() > 1){
        // On prend 2 variables différentes pour éfféctuer les comparaisons
        min = v[0];
        max = v[0];

        for (int i = 0; i < v.size(); ++i) {
            // Pour le minimum
            if (v[i] < min){
                min = v[i];
            }
            // Pour le maximum
            else if(v[i] > max){
                max = v[i];
            }
            nbComp+= 2;
        }
        // On soustrait 2 pour ne pas prenre en compte la comparaison au premier index des premiers éléments
        nbComp-=2;
    }
    else{
        min = v[0];
        max = v[0];
    }
}

template<class T>
void minMaxRecWorker(const vector<T>& v, int inf, int sup, T &minV, T &maxV, int &nbComp) {
    // Vérifie si ce n'est pas un calcul "triviale" / instantané.
    if (inf == sup) {
        minV = v[inf];
        maxV = v[inf];
    } else if (sup - inf == 1) { // Si il reste 2 éléments inf et sup choisir le min et max des 2.
        minV = min(v[inf], v[sup]);
        maxV = max(v[inf], v[sup]);
        nbComp++;
    } else {
        // Séparation du vecteur en 2
        int mid = (inf + sup) / 2;
        // Variables pour les parties du vecteur
        T minLeft, maxLeft, minRight, maxRight;
        // Appel sur la partie gauche du vecteur
        minMaxRecWorker(v, inf, mid, minLeft, maxLeft, nbComp);
        // Appel sur la partie droite du vecteur
        minMaxRecWorker(v, mid + 1, sup, minRight, maxRight, nbComp);

        // Fusion des résultats pour les 2 parties de vecteurs.
        minV = min(minLeft, minRight);
        maxV = max(maxLeft, maxRight);
        nbComp += 2; // For the comparison of min and max of two parts
    }
}


template<class T>
void minMaxRec(const vector<T>& v, int inf, int sup, T &min, T &max, int &nbComp) {
    // {v.size()≥ 1} => {min = plus petite valeur de v sur l'intervalle [inf..sup],
    //                   max = plus grande valeur de v sur l'intervalle [inf..sup],
    //                   nbcomp = nombre de comparaisons impliquant au moins un élément de v}

    if(v.size() == 1){
        min = v[0];
        max = v[0];
    }
    else{
        minMaxRecWorker(v, inf, sup, min, max, nbComp); // Division pour régner
    }

}


void testMinMaxIter() {
    cout << "***************************" << endl;
    cout << "*        min max iter     *" << endl;
    cout << "***************************" << endl;

    int min;
    int max;
    int nbComp = 0;

    vector<int> v = {10};
    minMaxIter(v, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> " << " min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v = {10, 3, 6, 8, 67, 2, 9, 1};
    minMaxIter(v, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> " << " min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v.clear();
    srand((unsigned)time(NULL));
    for (int i =0; i < 128; i++){
        int b = rand() % 128 + 1;
        v.push_back(b);
    }
    minMaxIter(v, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> " << " min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;
}

void testMinMaxDR() {
    cout << "***************************" << endl;
    cout << "*         min max DR      *" << endl;
    cout << "***************************" << endl;

    int min;
    int max;
    int inf;
    int sup;
    int nbComp = 0;

    vector<int> v = {10};
    inf = 0;
    sup = v.size()-1;
    minMaxRec(v, inf, sup, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() <<  " -> min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v = {10, 3, 6, 8, 67, 2, 9, 1};
    inf = 0;
    sup = v.size()-1;
    minMaxRec(v, inf, sup, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() <<  " -> min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v.clear();
    srand((unsigned)time(NULL));
    for (int i =0; i < 128; i++){
        int b = rand() % 128 + 1;
        v.push_back(b);
    }
    inf = 0;
    sup = v.size()-1;
    minMaxRec(v, inf, sup, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;
}

int main(int argc, char **argv) {

    testMinMaxIter();
    testMinMaxDR();

    return 0;
}
