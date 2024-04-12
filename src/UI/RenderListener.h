#pragma once
#include <unordered_map>
#include <string>
#include "Utilities/Vector2.h"
#include "Utilities/Vector4.h"

// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderTargetListener.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {
class MainLoop;
class WindowManager;

// TEMPORAL
class Image;
class ProgressBar;
class Slider;
class DropBox;

/*
* @brief Clase que se encarga de renderizar cuando Ogre termine de renderizar en el viewport
*/
class RenderListener : public Ogre::RenderTargetListener {
private:
    MainLoop* mainLoop;             // Referencia al MainLoop
    WindowManager* windowManager;   // Referencia al WindowManager

    // TEMPORAL
    std::unordered_map<std::string, Image*> images;
    uint32_t testid;
    std::unordered_map<std::string, ProgressBar*> progressbars;
    std::unordered_map<std::string, Slider*> sliders;
    std::unordered_map<std::string, DropBox*> dropboxes;
    std::string items[2];
    int item_current;

public:
    /*
    * @brief Inicializa el puntero al Game
    */
    RenderListener(MainLoop* mainLoop);

    /*
    * @brief Pone a nullptr el puntero al Game
    */
    ~RenderListener();

    /*
    * @brief Se llama cuando Ogre termine de renderizar en el viewport al que hemos asignado el RenderListener como listener
    */
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override;

    // TEMPORAL
    Image* createImage(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy);
    ProgressBar* createProgressBar(float initprogress, Vector4 progesscolor, std::string backtext, Vector2 siz,
                                   Vector2 pos);
    Slider* createSlider(std::string tittle, bool verticalMode, float initvalue, float maxValue, float minValue,
                         Vector2 siz, Vector2 pos);
    DropBox* createDropBox(std::string tittle, std::vector<std::string> content, int initialselect, Vector2 siz,
                           Vector2 pos);
};
}
