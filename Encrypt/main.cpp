// Encryptor

#include <fstream>
#include <iostream>
#include <string>
#include "base64.cpp"
#include <windows.h>
#include <boost/algorithm/string/replace.hpp>

int main()
{
    // Déclaration de variables
    std::ifstream::pos_type size;
    std::string base64;
    char * memblock;
    int headerSize;

    // Recuperation du chemin du fichier a crypter
    std::string dir;
    getline(std::cin,dir);

    // Recuperer l'extention de fichier
    std::string extension = dir.substr(dir.find(".") + 1);

    // Mise en place du header
    std::string header;
    header = "fileExt=" + extension + "|";

    // Remplacer les "\" par des "\\"
    boost::replace_all(dir,"\\","\\\\");

    // Ecriture
    std::string dir_png = dir;
    boost::replace_all(dir_png,("." + extension),".clop");

    std::ofstream fileWrite(dir_png.c_str(),std::ios::out | std::ios::binary | std::ios::ate);
    if(fileWrite.is_open())
    {
        fileWrite.seekp(0,std::ios::beg);
        fileWrite << header;
        fileWrite << "b4d4c077f070bfc6205bca7d1acfadff";
        headerSize =  fileWrite.tellp();
    }

    // Ouvrir le fichier en binaire
    std::ifstream fileRead(dir.c_str(), std::ios::in | std::ios::binary | std::ios::ate); //on ouvre le fichier en binaire

    // Si le fichier est ouvret
    if (fileRead.is_open())
    {
        size = fileRead.tellg(); // Récupère la taille en octet du fichier
        memblock = new char [size];
        fileRead.seekg (0, std::ios::beg);
        fileRead.read (memblock, size); // Associer a meme block le ANSI du fichier

        const unsigned char* base64_memblock;
        base64_memblock = reinterpret_cast <const unsigned char*>(memblock);
        base64 = base64_encode(base64_memblock,(int)size);

        fileRead.close();

        std::cout << "Chargement du fichier termine. size:" << size << std::endl;
    }
    else std::cout << "Erreur d'ouverture de fichier en lecture" << std::endl;

    std::ofstream ecriture(dir_png.c_str(), std::ios::out | std::ios::binary | std::ios::ate);

    // Si le fichier est ouvert
    if(ecriture.is_open())
    {
        ecriture.seekp(headerSize, std::ios::beg);
        ecriture << base64;

        std::cout << "Le fichier est ecrit" << std::endl;
    }
    else std::cout << "Erreur d'ouverture de fichier en ecriture" << std::endl;

    delete memblock;

    remove(dir.c_str());

    system("pause");

    return 0;
}
