// Encryptor

#include <iostream>
#include <string>
#include "checkPath.cpp"
#include "cryptFile.cpp"
#include <windows.h>
#include <dirent.h>
#include <unistd.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

int main()
{
    // Déclaration des variables
    bool dir;
    struct dirent * fichier = NULL;
    DIR * directory(NULL);
    std::string file;

    // Recuperation du chemin du fichier a crypter
    std::string path;
    getline(std::cin,path);

    // Remplacer les "\" par des "\\"
    boost::replace_all(path,"\\","\\\\");

    // If the path is a dir
    if(is_dir(path.c_str()))
    {
        std::cout << "[INFO] Given path is a directory\n";
        dir = true;
    }
    // If the path is a file
    else if(is_file(path.c_str()))
    {
        std::cout << "[INFO] Given path is a file\n";
        dir = false;
    }
    //This shouldn't ever happend
    else
    {
        std::cout << "[ERR] THIS IS AN ERROR YOU SHOULDN'T SEE\n";
    }

    if(path[path.length() - 1] != '\\')
    {
        path += "\\\\";
    }

    if(dir)
    {
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
             std::cout << "Encrypting " << file << std::endl;
             cryptFile(path + file);
        }
        if(closedir(directory) == -1)
        {
            return -1;
        }
    }
    else
    {
        cryptFile(path);
    }
    std::cout << std::endl << "[ALERT] All file has been encrypted\n";

    system("pause");

    return 0;
}
