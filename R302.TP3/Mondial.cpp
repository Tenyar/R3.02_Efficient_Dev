/* 
 * File:   Mondial.cpp
 * Author: hb
 * 
 * Created on 22 novembre 2018, 16:05
 */

#include "Mondial.h"


#include <iostream>     // pour cout
#include <iomanip>      // pour setw()
#include <sstream>
#include <iterator>
#include <string.h>

Mondial::Mondial(const char *filename) {
    // Chargement du fichier XML en mémoire
    imageMondial.LoadFile(filename);
    // Initialisation de l'attribut racineMondial avec la racine (élément <mondial>)
    racineMondial = imageMondial.FirstChildElement();
}

void Mondial::Print() {
    imageMondial.Print();
}

/*
 * FOURNIE
 */
int Mondial::getNbAirports() const {
    // initialisation du nombre d’aéroports
    int nb = 0;
    // accéder à <airportscategory>, c’est un fils de l'élément <racineMondial>
    XMLElement *airportsCategory = racineMondial->FirstChildElement("airportscategory");
    // parcours complet des fils de <airportscategory> en les comptants
    // 1) accéder au premier fils <airport> de <airportscategory>
    XMLElement *currentAirport = airportsCategory->FirstChildElement();
    // 2) parcourir tous les <airport> qui sont des frères
    while (currentAirport != nullptr) {
        // un aéroport supplémentaire
        nb = nb + 1;
        // avancer au frère <airport> suivant de currentAirport
        currentAirport = currentAirport->NextSiblingElement();
    }
    // currentAirport n’a plus de frère {currentAirport == nullptr}, c’est le dernier
    return nb;
}

/*
 * FOURNIE
 */
void Mondial::printCountriesCode() const {
    int rank = 1; // rang du pays
    string carcodeValue; // valeur de l'attribut "car_cod" du pays courant
    // accéder à <countriescategory>, c’est un fils de l'élément <racineMondial>)
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // parcours complet des fils de <countriescategory> en affichant le rang et le code
    // 1) accéder au premier fils <country> de <countriescategory>
    XMLElement *currentCountry = countriesCategory->FirstChildElement();
    // 2) parcourir tous les <country> qui sont des frères
    while (currentCountry != nullptr) {
        // traiter le pays courant
        //      1) récupérer la valeur de l’attribut "car_code"
        carcodeValue = currentCountry->Attribute("car_code");
        //      2) faire l’affichage
        cout << setw(5) << rank << " : " << carcodeValue << endl;
        // avancer au frère <country> suivant de currentCountry
        currentCountry = currentCountry->NextSiblingElement();
        // mettre à jour le rang
        rank = rank + 1;
    }
    // currentCountry n’a pas de frère {currentCountry == nullptr}, c’est fini
}


/*
 * A COMPLETER
 */
int Mondial::getNbDeserts() const {
    // initialisation du nombre de déserts
    int nb = 0;
    // accéder à <desertcategory>, c’est un fils de l'élément <racineMondial>
    XMLElement *desertCategory = racineMondial->FirstChildElement("desertscategory");
    // parcours complet des fils de <desertscategory> en les comptants
    // 1) accéder au premier fils <desert> de <desertscategory>
    XMLElement *currentDesert = desertCategory->FirstChildElement();
    // 2) parcourir tous les <deserts> qui sont des frères
    while (currentDesert != nullptr) {
        // un déserts supplémentaire
        nb++;
        // avancer au frère <deserts> suivant de currentDesert
        currentDesert = currentDesert->NextSiblingElement();
    }
    // currentDesert n’a plus de frère {currentDesert == nullptr}, c’est le dernier
    return nb;
}


/*
 * A COMPLETER
 */
int Mondial::getNbElemCat(const string categoryName) {
    // Init du nombre d'entrée de type "categoryName".
    int nb = 0;

    // Accéder à <nameCategory>, c'est un fils de l'élément mondial => <racineMondial> si il est trouvé dans le dictionnaire.
    // Utilisation du dictionnaire "decode_category" pour utiliser le nom d'une des catégories passé en paramètre.
    // puis "c_str()" pour convertir string en char *.
    XMLElement *nameCategory = racineMondial->FirstChildElement(decod_category[categoryName].c_str());
    // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
    XMLElement *currentCategoryFils = nameCategory->FirstChildElement();
    // Parcourir tous les frères de la catégorie <currentCategoryFils>.
    while (currentCategoryFils != nullptr) {
        // On compte chaque frère supplémentaire.
        nb++;
        // Avancer au frère suivant de currentCategoryFils.
        currentCategoryFils = currentCategoryFils->NextSiblingElement();
    }
    // On retourne le nombre de fils de categoryName.
    return nb;
}


/*
 * A COMPLETER
 */
XMLElement *Mondial::getCountryXmlelementFromNameRec(string countryName) const {
    /*
     * A COMPLETER
     */
    // supprimer à partir d'ici après complétion
    if (countryName.empty()) {
        return nullptr;
    } else {
        // Accéder à <countriescategory>, c'est un fils de l'élément mondial => <racineMondial> si il est trouvé dans le dictionnaire.
        XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
        // Premier fils (pays) de countriesCategory.
        XMLElement *currentPays = countriesCategory->FirstChildElement();
        return getCountryXmlelementFromNameRecWorker(currentPays, countryName);
    }
}

/*
 * A COMPLETER
 */
XMLElement *
Mondial::getCountryXmlelementFromNameRecWorker(XMLElement *currentCountryElement, string countryName) const {
    // Si on arrive au bout (pays n'est pas trouvé dans la liste.)
    // nullptr permet de s'arreter
    if (currentCountryElement == nullptr) {
        return nullptr;
    }
    // supprimer à partir d'ici après complétion
    // Si le fils 'Name' du pays n'est pas égale à celle de countryName alors relancer avec le frère.
    if (currentCountryElement->FirstChildElement("name")->GetText() != countryName) {
        return getCountryXmlelementFromNameRecWorker(currentCountryElement->NextSiblingElement(), countryName);
    } else {
        // Stop la récursivité, on a trouvé le pays.
        return currentCountryElement;
    }
}


/*
 * A COMPLETER
 */
string Mondial::getCountryCodeFromName(string countryName) const throw(PrecondVioleeExcep) {
    // Création de la variable paysCible et appel de la méthode.
    XMLElement *paysCible = getCountryXmlelementFromNameRec(countryName);
    // Si le pays n'est pas trouvé.
    if (paysCible == nullptr) {
        throw PrecondVioleeExcep("Aucun pays donnée / trouvé");
    } else {
        // Sinon on retourne son attribut 'car_code'.
        return paysCible->Attribute("car_code");
    }
}

/*
 * A COMPLETER
 */
/**
 * élément <country> d'un pays identifié par son nom countryName
 * @param countryName
 * @return pointeur sur l'élément <country> dont la valeur du fils <name> est égal à countryName, nullprt sinon
 */
XMLElement *Mondial::getCountryXmlelementFromNameIter(string countryName) const {

    if (countryName.empty()) {
        return nullptr;
    }
    // Accéder à la catégorie des pays pour ensuite la parcourir
    XMLElement *nameCategory = racineMondial->FirstChildElement("countriescategory");
    // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
    XMLElement *currentPays = nameCategory->FirstChildElement();
    // Parcourir tous les frères de la catégorie <currentCategoryFils>.
    // Si toutefois aucun pays n'est retenus, sortir de la boucle (On vérifie en premier le pointeur).
    while (currentPays != nullptr && currentPays->FirstChildElement("name")->GetText() !=
                                     countryName) { // tq on ne trouve pas à l'attribut 'name', 'name' == countryName
        // Avancer au frère suivant de currentCategoryFils.
        currentPays = currentPays->NextSiblingElement();
    }
    // On retourne le pays recherché.
    return currentPays;
}

/*
 * A COMPLETER
 */
int Mondial::getCountryPopulationFromName(string countryName) const {
    // Recherche du pays ciblé.
    XMLElement *paysCible = getCountryXmlelementFromNameIter(countryName);

    if (paysCible == nullptr) {
        return -1;

    } else if (paysCible->FirstChildElement("population") == nullptr) {
        return 0;

    } else {
        // TODO : a faire
        // L'année qu'on prendra pour prendre la mesure de la population.
        /*const char * current_population;
        XMLElement * currentTag = paysCible->FirstChildElement("population");
        // S'il y a au moins 1 tag <population>
        if (currentTag != nullptr){
            int datePlusRecente = 0;
            // Boucle sur tous les tags "<population>", du pays "<country>"
            // et vérifie la mesure qui est la plus actuelle (plus récente en date).
            while(currentTag->NextSiblingElement("population") != nullptr){ // Tq le prochain tag est <population> et peut être comparé.
                // Initialisation des dates a comparé.
                XMLElement * nextPopulationYear = (currentTag->NextSiblingElement("population"));
                // Si la population mesurée est d'une date plus récente par rapport à la mesure suivante, on garde cette date.
                if (currentTag->Attribute("year") > nextPopulationYear->Attribute("year")){
                    datePlusRecente = atoi(currentTag->FirstChildElement("population")->GetText());
                } else { // Sinon on affecte la mesure de la population la plus récente
                    datePlusRecente = atoi(nextPopulationYear->GetText());
                }
                // On passe au prochain tag <population>
                currentTag = currentTag->NextSiblingElement("population");
            }
            // On retourne la mesure de la population la plus récente
            return datePlusRecente;
        }   else    {
            return 0; // Pas de tag population
        }*/
        // On retourne la mesure de la population la plus récente (dernier élément des tags <population> supposée être la plus récente)
        return atoi(paysCible->LastChildElement("population")->GetText());
    }
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getCountryXmlelementFromCode(string countryCode) const {
    // Accéder à la catégorie des pays pour ensuite la parcourir
    XMLElement *nameCategory = racineMondial->FirstChildElement("countriescategory");
    // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
    XMLElement *currentPays = nameCategory->FirstChildElement();
    // Parcourir tous les frères de la catégorie <currentCategoryFils>.
    // Si toutefois aucun pays n'est retenus, sortir de la boucle (On vérifie en premier le pointeur).
    while (currentPays != nullptr && currentPays->Attribute("car_code") !=
                                     countryCode) { // tq on ne trouve pas à l'attribut 'car_code', 'car_code' == countryCode
        // Avancer au frère suivant de currentCategoryFils.
        currentPays = currentPays->NextSiblingElement();
    }
    // On retourne le pays recherché.
    return currentPays;
}

/*
 * A COMPLETER
 */
void Mondial::printCountryBorders(string countryName) const {
    // Appel à une méthode déjà implémenter auparavant pour chercher le pays souhaité.
    XMLElement *paysCible = getCountryXmlelementFromNameIter(countryName);
    // Je cherche si ce pays existe
    if (paysCible == nullptr) {
        cout << "Ce pays n'existe pas dans les données !" << endl;
    } else if (paysCible->FirstChildElement("border") == nullptr) { // Si un pays à au moins un pays frontalier.
        cout << "Ce pays ne contient pas de pays frontalier !" << endl;
    } else {

        // Accéder au premier fils de la catégorie demandé <border> soit pays frontalier.
        XMLElement *currentBoder = paysCible->FirstChildElement("border");
        // Affichage du nom du pays ciblé.
        cout << "Pays " << paysCible->FirstChildElement("name")->GetText() << " : " << endl;
        // Parcourir tous les frères de la catégorie <border>.
        // Itéré sur chaque pays frontalier, et afficher son pays ainsi que la longueur de sa frontière.
        while (currentBoder != nullptr) {
            // On passe le "car_code" de chaque pays frontalier à la méthode
            XMLElement *paysFrontalier = getCountryXmlelementFromCode(currentBoder->Attribute("country"));
            // Afficher le nom du pays frontalier ainsi que sa longueur.
            cout << " Frontière : " << paysFrontalier->FirstChildElement("name")->GetText()
                 << " Longueur : " << currentBoder->Attribute("length") << endl;
            // Avancer au frère suivant de <border> ! pas d'autres tags disponibles.
            currentBoder = currentBoder->NextSiblingElement("border");
        }
    }
    // Ensuite un appel à la fonction qui permet de trouver le pays correspondant à un car_code
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getRiverXmlelementFromNameIter(string riverName) const {
    if (riverName.empty()) {
        return nullptr;
    } else {
        // Accéder à la catégorie des pays pour ensuite la parcourir
        XMLElement *nameCategory = racineMondial->FirstChildElement("riverscategory");
        // Accéder au premier fils de la catégorie demandé <river>.
        XMLElement *currentRiver = nameCategory->FirstChildElement();
        /*
            // Récupère le nom de la rivière/fleuve.
            string parseRiverName = currentRiver->Attribute("id");
            // Parse le nom de la rivière/fleuve (id) pour enlever "river-" et ne garder que le nom de la rivière/fleuve.
            // On enlève toujours les 6 premiers caractères
            parseRiverName = parseRiverName.erase(0,6);
        */
        // Parcourir tous les frères de la catégorie <currentCategoryFils>.
        // Si toutefois aucune rivière/pays n'est retenus, sortir de la boucle.
        while (currentRiver != nullptr && currentRiver->FirstChildElement("name")->GetText() != (riverName)) {
            // Avancer au frère suivant de currentCategoryFils.
            currentRiver = currentRiver->NextSiblingElement();
        }
        // On retourne le fleuve/rivière recherché.
        return currentRiver;
    }
}

/*
 * A COMPLETER
 */
void Mondial::printAllCountriesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
    // On prend le fleuve demandé
    XMLElement *riverCible = getRiverXmlelementFromNameIter(riverName);
    if (riverCible == nullptr) { // Si ce fleuve n'est pas trouvé
        cout << "Aucune rivière/fleuve rentrée ou aucune rivière/fleuve de ce nom dans nos données !" << endl;
    } else {
        cout << "Longueur du fleuve " << riverCible->FirstChildElement("name")->GetText()
             << " : " << riverCible->FirstChildElement("length")->GetText() << endl;
        // On prend tous les pays traversés par le fleuve
        string countries = riverCible->Attribute("country");
        // list de pays séparé par les espaces
        vector<string> countryVect = split(countries, ' ');
        // Pour chaque mots dans le string on va chercher son pays et afficher sont nom
        for (string paysCode: countryVect) {
            XMLElement *currentPaysFleuve = getCountryXmlelementFromCode(paysCode);
            cout << currentPaysFleuve->FirstChildElement("name")->GetText() << endl;
        }

    }
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesWithProvincesCrossedByRiver(string riverName) const {
    // On prend le fleuve demandé
    XMLElement *riverCible = getRiverXmlelementFromNameIter(riverName);
    if (riverCible == nullptr) { // Si ce fleuve n'est pas trouvé
        cout << "Aucune rivière/fleuve rentrée ou aucune rivière/fleuve de ce nom dans nos données !" << endl;
    } else {
        cout << "Longueur du fleuve " << riverCible->FirstChildElement("name")->GetText()
             << " : " << riverCible->FirstChildElement("length")->GetText() << endl;
        // On prend le 1er élément de la série <located>
        XMLElement *currentLocated = riverCible->FirstChildElement("located");
        // Si le fleuve ne traverse aucun pays.
        if (currentLocated == nullptr) {
            cout << "Ce fleuve ne traverse aucun pays." << endl;
        } else {
            while (currentLocated != nullptr) {
                // Cherché le pays provenant du car_code de <country> contenu dans le <located> où on est à ce moment.
                XMLElement *currentPaysLocated = getCountryXmlelementFromCode(currentLocated->Attribute("country"));
                cout << currentPaysLocated->FirstChildElement("name")->GetText() << endl;
                // On passe au prochain
                currentLocated = currentLocated->NextSiblingElement("located");
            }
        }
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
    // On prend le fleuve demandé
    XMLElement *riverCible = getRiverXmlelementFromNameIter(riverName);
    if (riverCible == nullptr) { // Si ce fleuve n'est pas trouvé
        cout << "Aucune rivière/fleuve rentrée ou aucune rivière/fleuve de ce nom dans nos données !" << endl;
    } else if (riverCible->Attribute("country") == nullptr) { // Si ce fleuve ne traverse aucun pays
        cout << "Ce fleuve ne traverse aucun pays" << endl;
    } else {
        cout << "Longueur du fleuve " << riverCible->FirstChildElement("name")->GetText()
             << " : " << riverCible->FirstChildElement("length")->GetText() << endl;

        // On prend tous les pays traversés par le fleuve
        string countries = riverCible->Attribute("country");
        // list de pays séparé par les espaces
        vector<string> countryVect = split(countries, ' ');
        // Faire un dictionnaire des paysCode avec leurs provinces :
        map<string, string> dicoPaysCodeWithProvinces;
        // On prend le 1er élément de la série <located>
        XMLElement *currentLocated = riverCible->FirstChildElement("located");
        while (currentLocated != nullptr) {
            dicoPaysCodeWithProvinces[currentLocated->Attribute("country")] = currentLocated->Attribute("province");
            // On passe au prochain
            currentLocated = currentLocated->NextSiblingElement("located");
        }

        // Pour chaque mot (car_code/code du pays) dans le string, on va chercher son pays et afficher son nom.
        for (string paysCode: countryVect) {
            // On affiche son nom de pays.
            // On prend le car_code du pays en cours pour trouver son tag <country>.
            XMLElement *currentPaysFleuve = getCountryXmlelementFromCode(paysCode);
            cout << currentPaysFleuve->FirstChildElement("name")->GetText() << endl;
            // S'il est dans le dico (il a donc des provinces).
            if (dicoPaysCodeWithProvinces.find(paysCode) != dicoPaysCodeWithProvinces.end()) {
                // list de province séparée par les espaces.
                vector<string> provinceVect = split(dicoPaysCodeWithProvinces[paysCode], ' ');
                // 2ème dictionnaire pour stocker toutes les clés = provinces, valeur = nom (de ces provinces).
                map<string, string> dicoProvinceName;
                // On itère pour stocker une fois toutes les provinces d'un pays.
                XMLElement *currentProvince = currentPaysFleuve->FirstChildElement(
                        "province");
                while (currentProvince != nullptr) {
                    // Enregistre dans la map ---> clé [id_province]; valeur [name_province].
                    dicoProvinceName[currentProvince->Attribute("id")] = currentProvince->FirstChildElement(
                            "name")->GetText();
                    // On avance de tag <province>
                    currentProvince = currentProvince->NextSiblingElement("province");
                }

                // Pour chacune des provinces prendre son nom
                for (string province: provinceVect) {
                    // Afficher le code le nom de chaque province de se pays.
                    cout << "\t - " << dicoProvinceName[province] << endl;
                }

            }
        }

        /*        for (string paysCode: countryVect) {
        // on affiche son nom de pays
        // car_code du mot en cours ----> Pays en cours
        XMLElement *currentPaysFleuve = getCountryXmlelementFromCode(paysCode);
        cout << currentPaysFleuve->FirstChildElement("name")->GetText() << endl;
        // S'il est dans le dico (il a donc des provinces).
        if (dicoPaysCodeWithProvinces.find(paysCode) != dicoPaysCodeWithProvinces.end()) {
            // list de province séparée par les espaces
            vector<string> provinceVect = split(dicoPaysCodeWithProvinces[paysCode], ' ');
                // 2ème dictionnaire pour stocker toutes les clés = provinces, valeur = nom (de ces provinces).
                map<string, string> dicoProvinceName;
            // Pour chacune des provinces prendre son nom
            for (string province: provinceVect) {
                // On commence par le premier élément <province> du pays
                XMLElement *currentProvince = currentPaysFleuve->FirstChildElement(
                        "province"); // conversion de string province --> const char *.
                while (currentProvince != nullptr) {
                    // Si la province est trouvée dans le pays
                    if (currentProvince->Attribute("id") == province) {
                        // On affiche son nom
                        cout << "\t - " << currentProvince->FirstChildElement("name")->GetText() << endl;
                    }
                    currentProvince = currentProvince->NextSiblingElement("province");
                }

            }
        }*/

    }
}

/*
 * A COMPLETER
 */
void Mondial::printCityInformation(string cityName) const {
    /*
     * A COMPLETER
     */
    if (cityName.empty()) {
        cout << "Aucun nom de ville rentrée !" << endl;
    } else {
        // Accéder à la categorie des pays.
        XMLElement *nameCategory = racineMondial->FirstChildElement("countriescategory");
        // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
        XMLElement *currentPays = nameCategory->FirstChildElement();
        // Parcourir tous les frères de la catégorie <countries>.
        // On cherche le pays avec le nom de la ville.
        bool paysTrouver = false;
        while (currentPays != nullptr && !paysTrouver) {
            // Reset du boolean pour chaque pays
            paysTrouver = false;
            // Récupération de l'élément <province>.
            XMLElement *currentProvince = currentPays->FirstChildElement("province");
            // Si le pays contient des provinces.
            if (currentProvince != nullptr) {
                // Parcourir toutes les provinces et voir dans leur tag <city> le tag <name>.
                // Pas de dico ici, car nous avons besoin de parcourir une seul fois toutes les provinces pour chaque pays.
                while (currentProvince != nullptr && !paysTrouver) {
                    XMLElement *currentCity = currentProvince->FirstChildElement("city");
                    // Parcourir toutes les villes de chaques provinces
                    while (currentCity != nullptr && !paysTrouver) {
                        if (currentCity->FirstChildElement("name")->GetText() == cityName) {
                            // Le pays contenant la ville a été trouvé.
                            paysTrouver = true;
                            cout << "La Ville : " << currentCity->FirstChildElement("name")->GetText() << endl
                                 << "\t - Se trouve dans le pays : "
                                 << currentPays->FirstChildElement("name")->GetText() << endl
                                 << "\t - dans la division administrative : " << currentProvince->Attribute("id")
                                 << endl
                                 << "\t - sa latitude est : " << currentCity->FirstChildElement("latitude")->GetText()
                                 << endl
                                 << "\t - sa longitude est : " << currentCity->FirstChildElement("longitude")->GetText()
                                 << endl
                                 << "\t - son altitude est : " << currentCity->FirstChildElement("elevation")->GetText()
                                 << endl
                                 << "\t - sa population est : "
                                 << currentProvince->LastChildElement("population")->GetText() << endl;
                        }
                        currentCity = currentCity->NextSiblingElement("city");
                    }
                    // Avancer à la province suivante.
                    currentProvince = currentProvince->NextSiblingElement("province");
                }
            } else { // Si le pays ne contient pas de province, regarder juste ses tags <city> et non <province>.
                XMLElement *currentCity = currentPays->FirstChildElement("city");
                // Parcourir toutes les villes de chaques pays
                while (currentCity != nullptr && !paysTrouver) {
                    if (currentCity->FirstChildElement("name")->GetText() == cityName) {
                        // Le pays contenant la ville a été trouvé.
                        paysTrouver = true;
                        cout << "La Ville : " << currentCity->FirstChildElement("name")->GetText() << endl
                             << "\t - Se trouve dans le pays : " << currentPays->FirstChildElement("name")->GetText()
                             << endl
                             << "\t - sa latitude est : " << currentCity->FirstChildElement("latitude")->GetText()
                             << endl
                             << "\t - sa longitude est : " << currentCity->FirstChildElement("longitude")->GetText()
                             << endl
                             << "\t - son altitude est : " << currentCity->FirstChildElement("elevation")->GetText()
                             << endl
                             << "\t - sa population est : " << currentCity->LastChildElement("population")->GetText()
                             << endl;
                    }
                    currentCity = currentCity->NextSiblingElement("city");
                }
            }
            // Avancer au pays suivant (frère du pays courant).
            currentPays = currentPays->NextSiblingElement("country");
        }
        if (currentPays == nullptr) {
            cout << "La ville " << cityName << ", n'existe pas !" << endl;
        }
    }
}

/**
 * Exemple de question additionnelle pour l'exercice 9 afficher toutes les informations disponibles
 * dans Mondial concernant toutes les îles.
 * On peut commencer par une île en particulier à partir de son nom
 */
void Mondial::printIslandsInformations() const {
    /*
     * A COMPLETER
     */
}

/*
 * Méthodes de service fournies
 */

template<typename Out>
void Mondial::split(string &s, char delim, Out result) const {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

/**
 * Méthode à utiliser pour découper les mots d'une chaîne dans un vecteur
 * Utile pour la gestion des attributs à valeurs multiples
 * @param s chaîne à découper
 * @param delim délimiteur des mots de la chaîne à découper
 * @return vecteur contenant les mots de la chaîne découpée
 */
vector<std::string> Mondial::split(string &s, char delim) const {
    vector<std::string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

Mondial::~Mondial() {
}