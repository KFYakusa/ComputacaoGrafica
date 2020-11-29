#include "Screenshot.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>
#include <SOIL2/soil2.h>

using namespace std;

#include <dirent.h>

Screenshot::Screenshot(){

}



bool Screenshot::saveScreenshot(std::string folderPath, int screenshotCount, int width, int height){
    
   
    if(folderPath.size()<=0){
        cout << "Erro Screenshot::saveScreenshot: especifique um diretorio valido!\n";
        return false;
    }
        
    std::replace( folderPath.begin(), folderPath.end(), '\\', '/');

    if(folderPath.at(folderPath.size()-1)!='/'){
        folderPath += "/";
    }
    
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << screenshotCount;
    std::string path= folderPath + "screenshot_" + ss.str() + ".png";
    

    if( ! SOIL_save_screenshot( path.c_str(),	SOIL_SAVE_TYPE_PNG,	0, 0, width, height))
    {
        cout << "Erro Screenshot::saveScreenshot: erro ao salvar o arquivo com SOIL!\n";
        return false;
    }

    cout << "\nScreenshot: " << path << endl;

    return true;
}



bool Screenshot::saveScreenshot(std::string folderPath, int width, int height){
    
    
    //lista os arquivos do diretorio e verifica o numero do ultimo screenshot salvo!
    DIR *dir = opendir(folderPath.c_str());
   
    if (dir == NULL) {
        cout << "Erro Screenshot::saveScreenshot: especifique um diretorio valido!\n";
        return false;
    }

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
       
        string input(entry->d_name);
        
        std::string output;
        std::smatch match;
        std::regex_search(input, match, std::regex("([1-9])([0-9]*)"));
        if(match.size()>0){
            int n = std::stoi( match.str(0) );
            if(n> count)
                count = n;
        }
         
    }
    closedir(dir);
    count++; //incrementa o contador de screenshot

    
    //salva o screenshot e retorna
    return Screenshot::saveScreenshot(folderPath, count, width, height);
}