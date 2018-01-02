// Decryptor

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <boost/algorithm/string/replace.hpp>

int main()
{
    // D�claration de variables
    std::ifstream::pos_type size;
    char * memblock;

    // Recuperation du chemin du fichier a crypter
    std::string dir;
    getline(std::cin,dir);

    // Remplacer les "\" par des "\\"
    boost::replace_all(dir,"\\","\\\\");

    // Trouver l'extention du fichier d'origine
    std::string extension = dir.substr(dir.find("fileExt_") + 8);

    extension = extension.substr(0, extension.find("_"));

    // Ouvrir le fichier en binaire
    std::ifstream file (dir.c_str(), std::ios::in | std::ios::binary | std::ios::ate); //on ouvre le fichier en binaire

    // Si le fichier est ouvret
    if (file.is_open())
    {
        size = file.tellg(); // R�cup�re la taille en octet du fichier
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size); // Associer a meme block le ANSI du fichier
        file.close();

        std::cout << "Chargement du fichier termine. size:" << size << std::endl;
    }
    else std::cout << "Erreur d'ouverture de fichier en lecture" << std::endl;


    // Ecriture
    std::string dir_png = dir;
    boost::replace_all(dir_png,("_fileExt_" + extension + "_Encrypted.clop"),("." + extension));
    std::ofstream ecriture(dir_png.c_str(), std::ios::out | std::ios::binary);

    // Si le fichier est ouvert
    if(ecriture)
    {
        for (int i=0; i<size; ++i)
        {
            ecriture << memblock[i];// Mettre dans le fichier tout ce que contiend memeblock
        }

        std::cout << "Le fichier est ecrit" << std::endl;
    }
    else std::cout << "Erreur d'ouverture de fichier en ecriture" << std::endl;

    remove(dir.c_str());

    delete memblock;

    system("pause");

    return 0;
}
