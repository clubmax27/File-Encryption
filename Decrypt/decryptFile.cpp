#include <iostream>
#include <fstream>
#include <string>
#include "base64.cpp"
#include <boost/algorithm/string/replace.hpp>

void decryptFile(std::string path)
{
    // Déclaration de variables
    std::string base64;
    std::string extension;
    int headerSize;
    std::ifstream::pos_type size;
    std::string memblock;

    // Trouver l'extention du fichier d'origine
    {
        std::ifstream file(path.c_str(), std::ios::in);
        char* tempheadblock = new char [100];
        file.seekg (0, std::ios::beg);
        file.read(tempheadblock, 100);
        std::string headblock = (std::string)tempheadblock;

        extension = headblock.substr(headblock.find("fileExt="), headblock.find("|"));
        std::string header = headblock.substr(0,headblock.find("b4d4c077f070bfc6205bca7d1acfadff") + 32);
        headerSize = header.size();
    }

    // Ouvrir le fichier en binaire
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate); //on ouvre le fichier en binaire

    // Si le fichier est ouvret
    if (file.is_open())
    {
        size = file.tellg(); // Récupère la taille en octet du fichier
        memblock = new char [size - headerSize];
        file.seekg (headerSize, std::ios::beg);
        getline(file,base64);
        memblock = base64_decode(base64);

        file.close();

        if(size == 0)
        {
            std::cout << "[ERR] Could not open " + path << std::endl;
            std::cout << std::endl;
            return;
        }
    }
    else std::cout << "[ERR] Could not open " + path << std::endl;

    // Ecriture
    extension = extension.substr(8);
    boost::replace_all(path,(".clop"),("." + extension));
    std::ofstream ecriture(path.c_str(), std::ios::out | std::ios::binary);

    // Si le fichier est ouvert
    if(ecriture.is_open())
    {
        ecriture << memblock;

        std::cout << "[INFO] Done !\n" << std::endl;
    }
    else std::cout << "Erreur d'ouverture de fichier en ecriture" << std::endl;

    remove(path.c_str());
}
