#ifndef SCREENSHOT_H_INCLUDED
#define SCREENSHOT_H_INCLUDED

#include <string>

class Screenshot
{
public:
    Screenshot();

    //salva um screenshot na pasta "folderPath", nomeando o arquivo como screenshot_001.png, screenshot_002.png, ...
    //A numeracao dos arquivos é definida a partir da variavel "screenshotCount"
    //Arquivos previamente capturados podem ser sobrescritos/excluidos!
    static bool saveScreenshot(std::string folderPath, int screenshotCount, int width, int height);

    //salva um screenshot na pasta "folderPath", nomeando o arquivo como screenshot_001.png, screenshot_002.png, ...
    //A numeracao dos arquivos é definida a partir do último screenshot realizado
    //Arquivos previamente capturados são preservados
    static bool saveScreenshot(std::string folderPath, int width, int height);

};

#endif