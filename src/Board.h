#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

class Board {
public:
    enum class FruitsType { Empty, Avocado, Lemon, Strawberry };
    
    struct Cell
    {
        FruitsType type = FruitsType::Empty;
        std::string texture;
        bool matched = false; // Флаг для отметки совпадений
    };

private:
    int mRows = 5,
        mCols = 4;
 
    Vector2 mStart = { 0, 0 };

    const std::string mAssetsPath = "./assets/fruits/";
    std::unordered_map<std::string, Texture2D> mFruitsTextures;

    std::unordered_map<std::string, std::string> mFruitsElements = {
        { "avocado", "avocado.png"},
        { "avocadoDark", "avocado-dark.png" },
        { "lemon", "lemon.png" },
        { "strawberry", "strawberry.png" },
        { "strawberryDark", "strawberry-dark.png" },
    };

    std::vector<std::vector<Cell>> mBoard;

    bool mIsDragging = false;
    Vector2 mSelectedCell = {-1, -1};
    Vector2 mTargetCell = {-1, -1};
    float mSwapAnimationProgress = 1.0f;

    const float mCellSize = 48.0f;
    const int mGap = 6;

    void FillBoard();
    void FillRow(std::vector<Cell> &row);
public:
    Board() = default;
    Board(const Vector2 &start);
    
    ~Board();
    
    void DrawBoard();
    void LoadAssets();

    void HandleInput();
    void Update();
    bool CheckMatches(bool destroy);
    void DestroyMatches();
    void ShiftCellsDown();
    void SwapCells(int row1, int col1, int row2, int col2);
    Cell GenerateCell();
};