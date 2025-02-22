#include "Game.h"

Game::Game(const int &width, const int &height, const char* title)
{
    this->mWidth  = width;
    this->mHeight = height;
    this->mTitle  = title;
}

Game::~Game()
{
    CloseWindow();

    for (auto pair : mInterfaceTextures)
    {
        UnloadTexture(pair.second);
    }
}

// Загрузка всех ассетов
void Game::LoadAssets()
{
    mBoard.LoadAssets();
    for (auto pair : mInterfaceElements)
    {
        mInterfaceTextures[pair.first] = LoadTexture((mAssetsPath + pair.second).c_str());
    }

    mInterfaceElements.clear();
}

void Game::DrawTextures()
{
    const float optionSize = 50.0f;

    DrawTexture(mInterfaceTextures["background"], 0, 0, WHITE);

    // Заглушки интерфейса
    DrawTexture(
        mInterfaceTextures["status"],
        30, 0,
        WHITE
    );

    DrawTexture(
        mInterfaceTextures["progressBar"],
        50, 30,
        WHITE
    );

    DrawTexture(
        mInterfaceTextures["level"],
        mWidth / 4, 75,
        WHITE
    );

    DrawTexture(
        mInterfaceTextures["firstMascot"],
        mWidth / 4 - 40, 54,
        WHITE
    );

    DrawTexture(
        mInterfaceTextures["secondMascot"],
        mInterfaceTextures["level"].width + 50,
        65,
        WHITE
    );

    Vector2 settingsPos = { 
        10, 
        static_cast<float>(mHeight - mInterfaceTextures["settings"].height - 3)
    };
    
    DrawTexturePro(
        mInterfaceTextures["settings"],
        { 
            0, 0, 
            static_cast<float>(mInterfaceTextures["settings"].width),
            static_cast<float>(mInterfaceTextures["settings"].height)
        },
        {
            settingsPos.x, settingsPos.y, 
            optionSize, optionSize
        },
        { 0, 0 },
        0,
        WHITE
    );
    
    Vector2 keyPos = {
        static_cast<float>(mWidth - mInterfaceTextures["key"].width - 3),
        static_cast<float>(mHeight - mInterfaceTextures["key"].height)
    };
        
    DrawTexturePro(
        mInterfaceTextures["key"],
        {
            0, 0, 
            static_cast<float>(mInterfaceTextures["key"].width),
            static_cast<float>(mInterfaceTextures["key"].height)
        },
        {
            keyPos.x, keyPos.y,
            optionSize, optionSize
        },
        { 0, 0 },
        0,
        WHITE
    );
}

// Основной цикл игры
void Game::Run()
{
    InitWindow(mWidth, mHeight, mTitle);
    SetTargetFPS(60);

    LoadAssets();

    while (!WindowShouldClose())
    {
        // Обновляем ячейки в зависимости от ввода
        mBoard.HandleInput();
        mBoard.Update();
        
        // Отрисовка всех элементов
        BeginDrawing();

        ClearBackground(BLACK);
        
        DrawTextures();
        mBoard.DrawBoard();

        EndDrawing();
    }
}