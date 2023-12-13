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


/**
 * Méthode itérative qui compte le nombre de déserts référencés dans la base de données.
 * @return int;
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
        // un désert supplémentaire
        nb++;
        // avancer au frère <deserts> suivant de currentDesert
        currentDesert = currentDesert->NextSiblingElement();
    }
    // currentDesert n’a plus de frère {currentDesert == nullptr}, c’est le dernier
    return nb;
}


/**
 * Méthode itérative qui compte le nombre d'entrée de type <categoryName> dans la base de données.
 * @return int;
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


/**
 * Méthode qui permet de trouver un élément <country>, identifié avec son nom.
 * @Param string countryName : fait référence au nom du pays cherché.
 * @return pointeur vers XMLElement de type <country>.
 */
XMLElement *Mondial::getCountryXmlelementFromNameRec(string countryName) const {
    if (countryName.empty()) {
        return nullptr;
    } else {
        // Accéder à <countriesCategory>, c'est un fils de l'élément mondial <racineMondial> s'il est trouvé dans le dictionnaire.
        XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
        // Premier fils (pays) de countriesCategory.
        XMLElement *currentPays = countriesCategory->FirstChildElement("country");
        // Appel de la méthode récursive.
        return getCountryXmlelementFromNameRecWorker(currentPays, countryName);
    }
}

/**
 * Méthode privé récursive qui permet de trouver un élément <country>, identifié avec son nom.
 * @Param XMLElement* currentCountryElement : fait référence au premier élément <country> par lequel le parcours sera commencé dans la liste.
 * @Param string countryName : fait référence au nom du pays cherché.
 * @return pointeur vers XMLElement de type <country>.
 */
XMLElement *
Mondial::getCountryXmlelementFromNameRecWorker(XMLElement *currentCountryElement, string countryName) const {
    // Si on arrive au bout (pays n'est pas trouvé dans la liste.)
    // nullptr permet de s'arrêter.
    if (currentCountryElement == nullptr) {
        return nullptr;
    }
    // Si le fils 'Name' du pays n'est pas égale à celle de countryName alors relancer avec le frère.
    if (currentCountryElement->FirstChildElement("name")->GetText() != countryName) {
        return getCountryXmlelementFromNameRecWorker(currentCountryElement->NextSiblingElement(), countryName);
    } else {
        // Stop la récursivité, on a trouvé le pays.
        return currentCountryElement;
    }
}


/**
 * Méthode qui permet d'afficher le car_code (attribut) de l'élément <country> recherché.
 * @param string countryName: fait référence au nom du pays cherché.
 * @return string.
 */
string Mondial::getCountryCodeFromName(string countryName) const throw(PrecondVioleeExcep) {
    // Création de la variable paysCible et appel de la méthode.
    XMLElement *paysCible = getCountryXmlelementFromNameRec(countryName);
    // Si le pays n'est pas trouvé.
    if (paysCible == nullptr) {
        throw PrecondVioleeExcep("Aucun pays donnée / trouvé");
    } else {
        // Sinon, on retourne son attribut 'car_code'.
        return paysCible->Attribute("car_code");
    }
}


/**
 * Méthode privé itérative qui permet de trouver un élément <country>, identifié avec son nom.
 * @param string countryName.
 * @return pointeur vers XMLElement de type <country>.
 */
XMLElement *Mondial::getCountryXmlelementFromNameIter(string countryName) const {

    if (countryName.empty()) {
        return nullptr;
    }
    // Accéder à la catégorie des pays pour ensuite la parcourir
    XMLElement *nameCategory = racineMondial->FirstChildElement("countriescategory");
    // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
    XMLElement *currentPays = nameCategory->FirstChildElement("country");
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

/**
 * Méthode qui permet de retourner la dernière mesure de population d'un pays identifié par son nom <countryName>.
 * @param string countryName.
 * @return int.
 */
int Mondial::getCountryPopulationFromName(string countryName) const {
    // Recherche du pays ciblé.
    XMLElement *paysCible = getCountryXmlelementFromNameIter(countryName);
    // Si le pays n'est pas trouvé
    if (paysCible == nullptr) {
        return -1;

    } else if (paysCible->FirstChildElement("population") == nullptr) { // S'il n'a pas de mesure de population
        return 0;

    } else {
        // On retourne la mesure de la population la plus récente (dernier élément des tags <population> supposée être la plus récente)
        return atoi(paysCible->LastChildElement("population")->GetText());
    }
}

/**
 * Méthode privé qui permet de trouver un élément <country>, identifié avec son car_code.
 * @param string countryCode.
 * @return pointeur vers XMLElement de type <country>.
 */
XMLElement *Mondial::getCountryXmlelementFromCode(string countryCode) const {
    // Accéder à la catégorie des pays pour ensuite la parcourir
    XMLElement *nameCategory = racineMondial->FirstChildElement("countriescategory");
    // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
    XMLElement *currentPays = nameCategory->FirstChildElement("country");
    // Parcourir tous les frères de la catégorie <currentCategoryFils>.
    // Si toutefois aucun pays n'est retenus, sortir de la boucle (On vérifie en premier le pointeur).
    while (currentPays != nullptr && currentPays->Attribute("car_code") !=
                                     countryCode) { // tant qu'on ne trouve pas à l'attribut 'car_code', 'car_code' == countryCode
        // Avancer au frère suivant de currentCategoryFils.
        currentPays = currentPays->NextSiblingElement();
    }
    // On retourne le pays recherché.
    return currentPays;
}

/**
 * Méthode qui permet d'afficher pour un pays, ses pays frontaliers ainsi que leur longueur.
 * @param string countryName.
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

/**
 * Méthode privé qui permet de trouver l'élément <river> avec son nom.
 * @param string riverName.
 * @return pointeur vers XMLElement de type <river>.
 */
XMLElement *Mondial::getRiverXmlelementFromNameIter(string riverName) const {
    if (riverName.empty()) {
        return nullptr;
    } else {
        // Accéder à la catégorie des pays pour ensuite la parcourir
        XMLElement *nameCategory = racineMondial->FirstChildElement("riverscategory");
        // Accéder au premier fils de la catégorie demandé <river>.
        XMLElement *currentRiver = nameCategory->FirstChildElement("river");
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

/**
 * Méthode qui permet d'afficher l'élément <river> avec son nom, sa longueur ainsi que le noms de tous les pays qu'il traverse.
 * @param string riverName.
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
        // Pour chaque mot dans le string, on va chercher son pays et afficher son nom
        for (string paysCode: countryVect) {
            XMLElement *currentPaysFleuve = getCountryXmlelementFromCode(paysCode);
            cout << currentPaysFleuve->FirstChildElement("name")->GetText() << endl;
        }

    }
}

/**
 * Méthode qui permet d'afficher l'élément <river> avec son nom, sa longueur ainsi que le noms de tous les pays qu'il traverse.
 * @param string riverName.
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

/**
 * Méthode qui permet d'afficher l'élément <river> avec son nom, sa longueur, le noms de tous les pays qu'il traverse
 * et pour chaque pays traversé qui possède des division administratives (<province>), le nom des divisions administratives
 * qu'il traverse.
 * @param string riverName.
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {
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
        for (string &paysCode: countryVect) {
            // On affiche son nom de pays.
            // On prend le car_code du pays en cours pour trouver son tag <country>.
            XMLElement *currentPaysFleuve = getCountryXmlelementFromCode(paysCode);
            cout << currentPaysFleuve->FirstChildElement("name")->GetText() << " où il traverse les divisions administratives suivantes :" << endl;
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
                for (string &province: provinceVect) {
                    // Afficher le nom de chaque province de ce pays.
                    cout << "\t - " << dicoProvinceName[province] << endl;
                }

            }
        }
    }
}

/**
 * Méthode qui permet d'afficher l'élément <city> avec son nom, son pays, sa division administrative, sa latitude,
 * sa longitude, son altitude et sa population.
 * @param string cityName.
 */
void Mondial::printCityInformation(string cityName) const {
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
                while (currentProvince != nullptr && !paysTrouver) {    // Pas de dico(map) ici, car nous avons besoin de parcourir une seule fois toutes les provinces pour chaque pays.
                    XMLElement *currentCity = currentProvince->FirstChildElement("city");
                    // Parcourir toutes les villes de chaques provinces
                    while (currentCity != nullptr && !paysTrouver) {
                        if (currentCity->FirstChildElement("name")->GetText() == cityName) {
                            // Le pays contenant la ville a été trouvé.
                            paysTrouver = true;
                            cout << "La Ville : " << currentCity->FirstChildElement("name")->GetText() << endl
                                 << "\t - Se trouve dans le pays : "
                                 << currentPays->FirstChildElement("name")->GetText() << endl
                                 << "\t - dans la division administrative : " << currentProvince->FirstChildElement("name")->GetText()
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
        // Si la ville n'est pas trouvée parmi les pays.
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
    // Accéder à la catégorie des îles pour ensuite la parcourir
    XMLElement *islandCategory = racineMondial->FirstChildElement("islandscategory");
    // Accéder au premier fils de la catégorie demandé <currentCategoryFils>.
    XMLElement *currentIsland = islandCategory->FirstChildElement("island");

    // Faire un dictionnaire des mers avec leurs noms :
    map<string, string> dicoAllSeas;
    // Accéder à la catégorie des mers pour ensuite la parcourir
    XMLElement *seasCategory = racineMondial->FirstChildElement("seascategory");
    // On prend le 1er élément de la série <sea>
    XMLElement *currentSea = seasCategory->FirstChildElement("sea");
    // Map de toutes les mers.
    while (currentSea != nullptr) {
        dicoAllSeas[currentSea->Attribute("id")] = currentSea->FirstChildElement("name")->GetText();
        // On passe au prochain
        currentSea = currentSea->NextSiblingElement("sea");
    }

    // Faire un dictionnaire des rivières avec leurs noms :
    map<string, string> dicoAllRives;
    // Accéder à la catégorie des mers pour ensuite la parcourir
    XMLElement *riverCategory = racineMondial->FirstChildElement("riverscategory");
    // On prend le 1er élément de la série <sea>
    XMLElement *currentRiver= riverCategory->FirstChildElement("river");
    // Map de toutes les mers.
    while (currentRiver != nullptr) {
        dicoAllRives[currentRiver->Attribute("id")] = currentRiver->FirstChildElement("name")->GetText();
        // On passe au prochain
        currentRiver = currentRiver->NextSiblingElement("river");
    }

    // Faire un dictionnaire des lacs avec leurs noms :
    map<string, string> dicoAllLakes;
    // Accéder à la catégorie des mers pour ensuite la parcourir
    XMLElement *lakeCategory = racineMondial->FirstChildElement("lakescategory");
    // On prend le 1er élément de la série <sea>
    XMLElement *currentLake= lakeCategory->FirstChildElement("lake");
    // Map de toutes les mers.
    while (currentLake != nullptr) {
        dicoAllLakes[currentLake->Attribute("id")] = currentLake->FirstChildElement("name")->GetText();
        // On passe au prochain
        currentLake = currentLake->NextSiblingElement("lake");
    }

    int loopRepetition = 0;

    // Parcourir toutes les îles
    while (currentIsland != nullptr){
        // On prend tous les pays de l'île en cours.
        string countries = currentIsland->Attribute("country");
        // list des pays séparés par les espaces l'île en cours.
        vector<string> countryVect = split(countries, ' ');

        // Afficher les informations de l'île en cours.
        cout << " L'île " << currentIsland->FirstChildElement("name")->GetText() << " : " << endl
                          << "\t- est situé en ";
        for (string countryCode : countryVect){
            // Prend chaque pays où est l'île.
            XMLElement * currentPaysOfIsland = getCountryXmlelementFromCode(countryCode);
            // S'il y en a d'autres
            if (loopRepetition >= countryVect.size() - 1){
                cout << currentPaysOfIsland->FirstChildElement("name")->GetText();
            } else {
                cout << currentPaysOfIsland->FirstChildElement("name")->GetText() << ", ";
            }
            loopRepetition += 1;
        }
        // reset de la variable
        loopRepetition = 0;
        // Retour à la ligne.
        cout << endl;
        // Si l'île est sur une rivière
        if (currentIsland->Attribute("sea") != NULL){
            // On prend toutes les mers de l'île en cours.
            string seas = currentIsland->Attribute("sea");
            // liste de mers séparées par les espaces l'île en cours.
            vector<string> seaVect = split(seas, ' ');
            for (string &sea : seaVect){
                cout << "\t\t +--> dans la mer " << dicoAllSeas[sea] << endl;
            }
        } else if (currentIsland->Attribute("river") != NULL) { // dans une rivière
            // On prend toutes les rivières de l'île en cours.
            string rivers = currentIsland->Attribute("river");
            // liste de rivières séparées par les espaces l'île en cours.
            vector<string> riverVect = split(rivers, ' ');
            for (string &river : riverVect){
                cout << "\t\t +--> dans la rivière " << dicoAllRives[river] << endl;
            }
        } else {
            // On affiche son lac ou l'île est.
            cout << "\t\t +--> dans le lac " << dicoAllRives[currentIsland->Attribute("lake")] << endl;
        }

        // Si l'île a un type.
        if (currentIsland->Attribute("type") != NULL){
            cout << "\t- Type de l'île : " << currentIsland->Attribute("type") << endl;
        }
        // Affichage Area s'il est présent.
        if (currentIsland->FirstChildElement("area") != nullptr){
            cout << "\t- Zone : " << currentIsland->FirstChildElement("area")->GetText() << endl;
        }
        cout << "\t- latitude : " << currentIsland->FirstChildElement("latitude")->GetText() << endl
             << "\t- longitude : " << currentIsland->FirstChildElement("longitude")->GetText() << endl;
        // Affichage de l'élévation s'il est présent.
        if (currentIsland->FirstChildElement("elevation") != nullptr){
            cout   << "\t- hauteur : " << currentIsland->FirstChildElement("elevation")->GetText() << endl;
        }

        // Avancer au frère suivant de currentCategoryFils.
        currentIsland = currentIsland->NextSiblingElement("island");
    }

    /*
     * Exemple de trace :
     * L'île [Nom Island, tag <insland>] :
     *          - est situé en [Pays <country> --> car_code to pays] dans la mer [<sea>]
     *              +----> Dans la province de : [<located>.Attribute(province) ---> itérer et trouver la province dans le pays.]
     *          - Area : [<area>]
     *          - latitude : [<latitude>]
     *          - longitude : [<longitude>]
     *          - elevation : [<elevation>]
     *          - île voisine : [Mettre le nom de toutes les iles avec le même country]
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