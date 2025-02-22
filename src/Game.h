#include <raylib.h>
#include <string>
#include <unordered_map>
#include "Board.h"

class Game {
private:
    int mWidth  = 0,
        mHeight = 0;

    const char* mTitle;

    const std::string mAssetsPath = "./assets/interface/";
    std::unordered_map<std::string, Texture2D> mInterfaceTextures;
    std::unordered_map<std::string, std::string> mInterfaceElements = {
        { "background", "background.png"},
        { "key", "key.png" },
        { "level", "level.png" },
        { "firstMascot", "mascot1.png" },
        { "secondMascot", "mascot2.png" },
        { "progressBar", "progress.png" },
        { "settings", "settings.png" },
        { "status", "status.png" } 
    };

    const Vector2 mBoardStartPos = { 30, 140 };
    Board mBoard = Board(mBoardStartPos);

    void LoadAssets();
    void DrawTextures();
public:
    Game(const int &width, const int &height, const char* title);
    ~Game();

    void Run();
};