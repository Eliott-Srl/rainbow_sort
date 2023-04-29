//Imports of my others functions
#include "image.hpp"
#include "utils.hpp"

//Imports 
#include <iostream>
#include <dirent.h>
#include <string>
#include <list>

//cross-platform way to get the current path
#include <filesystem>
namespace fs = std::filesystem;

int main() {
    //string that stores the user input absolute path
    std::string directory_name;
    std::cout << "Where are your files (for the default folder type 'X'): ";
    std::cin >> directory_name;

    std::string path; //string that stores the final path, user input or default
    
    if (directory_name == "X" || directory_name == "x") {
        //actually getting the current path
        std::string current_path = fs::current_path().string();
        std::replace(current_path.begin(), current_path.end(), '\\', '/'); // replace all '\' to '/'
        path = current_path + "/img/";
    } else {
        path = directory_name;
    }

    std::cout << path << std::endl;

    //transformation from string to char* of the final path
    char* char_path = new char[path.length() + 1];
    strcpy(char_path, path.c_str());
     
    DIR* dir; //final path compatible with the file reader

    struct dirent* diread; //file reader

    if ((dir = opendir(char_path)) != NULL) { //verification if the path is valid or not
        readdir(dir)->d_name; //skip the "." directory
        readdir(dir)->d_name; //skip the ".." directory

        std::list<AveragedChannels> hue_list; //this will store the image averaged to access to them for renaming them
         
        do {
            diread = readdir(dir); //I store in diread the actual image

            if (diread != NULL) {
                char* full_path = (char*)malloc(FILENAME_MAX); //memory allocation for the char* because i'll need it later
                ON_ERROR_EXIT(full_path == 0, "Error allocating memory"); //Little verification bc idk
                std::size_t length = path.copy(full_path, path.size(), 0); //Creation of the path with the name and the extension of the file
                full_path[length] = '\0'; //this is for finishing the buffer
                strcat(full_path, diread->d_name);
                 
                Image img(full_path); //creation of an object Image
                if (img.getDatas() != NULL) {                 
                    hue_list.push_back(AveragedChannels(&img, full_path)); //Add to the list an object AveragedChannels that inherit Image
                    hue_list.back().average(); //average the last image i've added
                    hue_list.back().rgbtohsv(); //calculate the hsv color space
                                        
                    hue_list.back().getImage()->freeImage(); //free the original image bc I don't need it anymore
                }
            }
        } while (diread != NULL);

        std::cout << "Renaming the file with their hue ..." << std::endl;
         
        hue_list.sort(); //the hue list is sorted by hue beacause in the AveragedChannels class i've overloaded the operator "<"
        
        int idx = 0; //this counter will be the final name of the file
        for (std::list<AveragedChannels>::iterator it = hue_list.begin(); it != hue_list.end(); ++it) {
            //creation of the final name of the file, no need for a memory allocation because the file is instantly renamed
            char idx_name[FILENAME_MAX];
            std::size_t length = path.copy(idx_name, path.size(), 0);
            idx_name[length] = '\0';
            strcat(idx_name, std::to_string(idx + 1).c_str());
            strcat(idx_name, ".");
            //my way to get the extension ¯\_(ツ)_/¯
            strcat(idx_name, ((std::string)it->getFileName()).substr(((std::string)it->getFileName()).find_last_of(".") + 1).c_str());

            //rename the file with the new name and a little verification because why not
            ON_ERROR_EXIT(rename(it->getFileName(), idx_name) != 0, "Error renaming file");
            std::cout << it->getFileName() << ": Done. " << std::endl;
                        
            free(it->getFileName()); //freeing the memory allocation of every char* path

            idx++;
        }
        
        closedir(dir); //close the directory, important !
    }
    else {
        ON_ERROR_EXIT(true, "Couldn't open the directory");
    }
}