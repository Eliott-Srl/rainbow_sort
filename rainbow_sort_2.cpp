#include "image.h"
#include "utils.h"

#include <iostream>
#include <dirent.h>
#include <string>
#include <list>

int main() {
    DIR* dir;
    struct dirent* diread;

    std::string path;

    std::list<AveragedChannels> hue_list;

    std::string directory_name;
    std::cout << "Where are your files (for default folder type 'X'): ";
    std::cin >> directory_name;

    if (directory_name == "X" || directory_name == "x") {
        path = "D:/fichiers/Dev/projet cpp/rainbow_sort_2/img/";
    } else {
        path = directory_name;
    }

    char* char_path = new char[path.length() + 1];
    strcpy(char_path, path.c_str());

    if ((dir = opendir(char_path)) != NULL) {
        readdir(dir)->d_name;
        readdir(dir)->d_name;

        std::cout << "Renaming the file with their hue ..." << std::endl;

        do {
            diread = readdir(dir);
            if (diread != NULL) {
                char* full_path = (char*)malloc(FILENAME_MAX);
                ON_ERROR_EXIT(full_path == 0, "Error allocating memory");
                std::size_t length = path.copy(full_path, path.size(), 0);
                full_path[length] = '\0';
                strcat(full_path, diread->d_name);

                Image img(full_path);
                if (img.getDatas() != NULL) {
                    hue_list.push_back(AveragedChannels(&img, full_path));
                    hue_list.back().average();
                    hue_list.back().rgbtohsv();
                    
                    hue_list.back().getImage()->freeImage();
                }
            }
        } while (diread != NULL);

        hue_list.sort();

        int idx = 0;
        for (std::list<AveragedChannels>::iterator it = hue_list.begin(); it != hue_list.end(); ++it) {
            auto ac = it;
            char idx_name[FILENAME_MAX];
            std::size_t length = path.copy(idx_name, path.size(), 0);
            idx_name[length] = '\0';
            strcat(idx_name, std::to_string(idx + 1).c_str());
            strcat(idx_name, ".");
            strcat(idx_name, ((std::string)ac->getFileName()).substr(((std::string)ac->getFileName()).find_last_of(".") + 1).c_str());

            ON_ERROR_EXIT(rename(ac->getFileName(), idx_name) != 0, "Error renaming file");
            std::cout << ac->getFileName() << ": Done. " << std::endl;
            
            idx++;
        }

        closedir(dir);
    }
    else {
        ON_ERROR_EXIT(true, "Couldn't open the directory");
    }
}