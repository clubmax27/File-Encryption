// Decryptor

#include <iostream>
#include <string>
#include "checkPath.cpp"
#include "decryptFile.cpp"
#include <windows.h>
#include <dirent.h>
#include <unistd.h>

#ifndef WIN32
    #include <sys/types.h>
#endif


void exploreFolder(std::string path)
{
    // Déclaration des variables
    struct dirent * fichier = NULL;
    DIR * directory(NULL);
    std::string file;
    int lastDot;

    //If there is no \\ at the end of the path, add them
    if(path[path.length() - 1] != '\\')
    {
        path += "\\\\";
    }

    directory = opendir(path.c_str());
    if(directory == NULL) // If the dirrectory don't exist
    {
        std::cout << std::endl << "[ERR] The directory could not be found\n";
        system("pause");
    }

    fichier = readdir(directory); //Skip the first base directory in every folder
    fichier = readdir(directory); //Skip the second base directory in every folder

    while((fichier = readdir(directory)) != NULL)
    {
        file = fichier->d_name;
        if(is_dir((path + file).c_str()))
        {
            path += "\\";
            exploreFolder(path + file);
        }
        else
        {
            // Savoir si il y a plusieurs points dans le path
            for(int i = 0; i < file.length(); i++)
            {
                if(file[i] == '.')
                {
                    // Mettre la position du dernier point dans lastDot
                    lastDot = i;
                }
            }

            // Recuperer l'extention de fichier
            std::string extension = file.substr(lastDot + 1);

            if(extension != "clop")
            {
                std::cout << "[ALERT] " + file + " is not a .clop file, skipping ...\n\n";
                continue;
            }

            std::cout << "[INFO] Decrypting " << path + file << "..." << std::endl;
            decryptFile(path + file);
            remove((path + file).c_str());
        }
    }

    if(closedir(directory) == -1)
    {
        return;
    }
}

int main()
{
    // Déclaration des variables
    bool dir;

    // Recuperation du chemin du fichier a crypter
    std::string path;
    getline(std::cin,path);

    // Remplacer les "\" par des "\\"
    boost::replace_all(path,"\\","\\\\");

    // If the path is a dir
    if(is_dir(path.c_str()))
    {
        std::cout << "[INFO] Given path is a directory\n";
        std::cout << "-------------------------------------\n\n";
        dir = true;
    }
    // If the path is a file
    else if(is_file(path.c_str()))
    {
        std::cout << "[INFO] Given path is a file\n";
        std::cout << "-------------------------------------\n\n";
        dir = false;
    }
    //This shouldn't ever happend
    else
    {
        std::cout << "[ERR] THIS IS AN ERROR YOU SHOULDN'T SEE\n";
        std::cout << "-------------------------------------\n\n";
    }

    if(dir)
    {
        exploreFolder(path);
    }
    else
    {
        decryptFile(path);
    }
    std::cout << "-------------------------------------\n\n";
    std::cout << "[INFO] All file has been decrypted\n\n";

    system("pause");

    return 0;
}
