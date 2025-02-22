#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(const Vector2 &start) : mStart(start)
{
    std::srand(std::time({}));

    FillBoard();
}

void Board::LoadAssets()
{
    for (auto pair : mFruitsElements)
    {
        mFruitsTextures[pair.first] = LoadTexture((mAssetsPath + pair.second).c_str());
    }

    mFruitsElements.clear();
}

void Board::FillBoard()
{
    mBoard.reserve(mRows);

    for (int i = 0; i < mRows; i++)
    {
        std::vector<Cell> t;
        t.reserve(mCols);
        FillRow(t);

        mBoard.push_back(t);
    }
}

Board::Cell Board::GenerateCell()
{
    Cell tCell = {
        .type = static_cast<FruitsType>(1 + std::rand() % 3)
    };

    switch (tCell.type)
    {
        case FruitsType::Avocado:
            tCell.texture = std::rand() % 2 ? "avocadoDark" : "avocado";
            break;
        case FruitsType::Lemon:
            tCell.texture = "lemon";
            break;
        case FruitsType::Strawberry:
            tCell.texture = std::rand() % 2 ? "strawberryDark" : "strawberry";
            break;
        default:
            break;
    }

    return tCell;
}

void Board::FillRow(std::vector<Cell> &row)
{
    for (int j = 0; j < mCols; j++)
    {
        row.emplace_back(GenerateCell());
    }
}

Board::~Board()
{
    for (auto pair : mFruitsTextures)
    {
        UnloadTexture(pair.second);
    }
}

void Board::DrawBoard()
{
    for (int i = 0; i < mRows; i++)
    {
        for (int j = 0; j < mCols; j++)
        {
            float posX = mStart.x + j * (mCellSize + mGap);
            float posY = mStart.y + i * (mCellSize + mGap);
            
            // Анимация перемещения
            if (mIsDragging && i == mSelectedCell.x && j == mSelectedCell.y)
            {
                Vector2 mousePos = GetMousePosition();
                // Вычисление координат для отрисовки текстуры под курсором в центре
                posX += (mousePos.x - posX) * mSwapAnimationProgress - mCellSize / 2;
                posY += (mousePos.y - posY) * mSwapAnimationProgress - mCellSize / 2;
            }
            
            DrawTexturePro(
                mFruitsTextures[mBoard[i][j].texture],
                {
                    0, 0,
                    static_cast<float>(mFruitsTextures[mBoard[i][j].texture].width),
                    static_cast<float>(mFruitsTextures[mBoard[i][j].texture].height)
                },
                {
                    posX, posY,
                    mCellSize, mCellSize
                },
                { 0, 0 },
                0,
                WHITE
            );
        }
    }
}

void Board::HandleInput()
{
    Vector2 mousePos = GetMousePosition();
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Проверяем клик внутри доски
        if (mousePos.x >= mStart.x && mousePos.y >= mStart.y &&
            mousePos.x <= mStart.x + (mCols * (mCellSize + mGap)) &&
            mousePos.y <= mStart.y + (mRows * (mCellSize + mGap)))
        {
            
            // Вычисляем индексы ячейки
            int col = (mousePos.x - mStart.x) / (mCellSize + mGap);
            int row = (mousePos.y - mStart.y) / (mCellSize + mGap);
            
            if (!mIsDragging)
            {
                mSelectedCell = {
                    static_cast<float>(row),
                    static_cast<float>(col)
                };
                mIsDragging = true;
            }
            else
            {
                mTargetCell = {
                    static_cast<float>(row),
                    static_cast<float>(col)
                };
                
                // Проверяем соседние ячейки
                if (std::abs(mSelectedCell.x - mTargetCell.x) + 
                    std::abs(mSelectedCell.y - mTargetCell.y) == 1)
                {
                    
                    SwapCells(mSelectedCell.x, mSelectedCell.y, 
                             mTargetCell.x, mTargetCell.y);
                    
                    if (!CheckMatches(false))
                    {
                        // Если совпадений нет - возвращаем обратно
                        SwapCells(mSelectedCell.x, mSelectedCell.y,
                                 mTargetCell.x, mTargetCell.y);
                    }
                }
                
                mIsDragging = false;
            }
        }
    }
}

bool Board::CheckMatches(bool destroy)
{
    bool found = false;
    
    // Сбрасываем флаги совпадений
    for (auto& row : mBoard)
    {
        for (auto& cell : row)
        {
            cell.matched = false;
        }
    }

    // Горизонтальные совпадения
    for (int i = 0; i < mRows; i++)
    {
        for (int j = 0; j < mCols - 2; j++)
        {
            if (mBoard[i][j].type != FruitsType::Empty &&
                mBoard[i][j].type == mBoard[i][j+1].type && 
                mBoard[i][j].type == mBoard[i][j+2].type)
            {
                
                mBoard[i][j].matched = true;
                mBoard[i][j+1].matched = true;
                mBoard[i][j+2].matched = true;
                found = true;
            }
        }
    }

    // Вертикальные совпадения
    for (int j = 0; j < mCols; j++)
    {
        for (int i = 0; i < mRows - 2; i++)
        {
            if (mBoard[i][j].type != FruitsType::Empty &&
                mBoard[i][j].type == mBoard[i+1][j].type && 
                mBoard[i][j].type == mBoard[i+2][j].type)
            {
                
                mBoard[i][j].matched = true;
                mBoard[i+1][j].matched = true;
                mBoard[i+2][j].matched = true;
                found = true;
            }
        }
    }

    if (found && destroy)
    {
        DestroyMatches();
    }
    
    return found;
}

void Board::Update()
{
    if (CheckMatches(true))
    {
        ShiftCellsDown();
    }
}

void Board::ShiftCellsDown()
{
    for (int j = 0; j < mCols; j++)
    {
        int emptyRow = mRows - 1;
        
        // Идем снизу вверх
        for (int i = mRows - 1; i >= 0; i--) {
            if (mBoard[i][j].type != FruitsType::Empty) {
                // Сдвигаем вниз
                if (i != emptyRow)
                {
                    std::swap(mBoard[i][j], mBoard[emptyRow][j]);
                }
                emptyRow--;
            }
        }
        
        // Заполняем пустые места новыми фруктами
        for (int i = emptyRow; i >= 0; i--)
        {
            mBoard[i][j] = GenerateCell();
        }
    }
}

void Board::SwapCells(int row1, int col1, int row2, int col2)
{
    // Проверка валидности
    if (row1 < 0 || row1 >= mRows || col1 < 0 || col1 >= mCols ||
        row2 < 0 || row2 >= mRows || col2 < 0 || col2 >= mCols) {
        return;
    }

    std::swap(mBoard[row1][col1], mBoard[row2][col2]);
}

void Board::DestroyMatches()
{
    for (int i = 0; i < mRows; i++)
    {
        for (int j = 0; j < mCols; j++)
        {
            if (mBoard[i][j].matched)
            {
                mBoard[i][j].type = FruitsType::Empty;
                mBoard[i][j].texture = "empty";
            }
        }
    }
}