/*****************************************************************************
 * ==> QR_GameInterface -----------------------------------------------------*
 *****************************************************************************
 * Description : Game interface, draws game on screen                        *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_GameInterface.h"
// std
#include <cstdio>
#include <cwchar>
#include <memory>
#include <conio.h>

//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------
// ASCII full char code
const char g_FullChar = 0xDB;
//------------------------------------------------------------------------------
// QR_GameInterface
//------------------------------------------------------------------------------
QR_GameInterface::QR_GameInterface()
{
    m_hWinConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
    m_Quit        = false;

    // reserve space for next piece cells
    m_NextPieceCells.reserve(15);

    // create cells that will contain next piece (it's a 3 * 5 cell array)
    for (unsigned i = 0; i < 15; ++i)
    {
        std::auto_ptr<QR_Cell> pCell(new QR_Cell());
        m_NextPieceCells.push_back(pCell.release());
    }}
//------------------------------------------------------------------------------
QR_GameInterface::~QR_GameInterface()
{
    // get next piece cell count
    const unsigned cellCount = m_NextPieceCells.size();

    // iterate through cells
    for (unsigned i = 0; i < cellCount; ++i)
        // delete cell
        delete m_NextPieceCells[i];

    // clear cell list
    m_NextPieceCells.clear();
}
//------------------------------------------------------------------------------
void QR_GameInterface::InitConsole()
{
    ::SetConsoleTitleW(L"Tetris");

    ::CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize   = 1;
    cursorInfo.bVisible = false;

    // configure Windows console
    ::SetConsoleCursorInfo(m_hWinConsole, &cursorInfo);
    ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightWhite);

    // set console cursor color to white
    SetCursorColor(QR_Cell::IE_LightWhite);
}
//------------------------------------------------------------------------------
void QR_GameInterface::DrawGame(QR_GameEngine* pGameEngine)
{
    // change console text color
    ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightWhite);

    // get play field width and height
    const unsigned playfieldWidth  = pGameEngine->GetPlayfield()->GetWidth();
    const unsigned playfieldHeight = pGameEngine->GetPlayfield()->GetHeight();

    // draw play field
    for (unsigned y = 0; y < playfieldHeight; ++y)
    {
        // draw play field left border
        std::printf("X");

        // draw current line
        for (unsigned x = 0; x < playfieldWidth; ++x)
        {
            QR_Cell* pCell;

            // get cell from play field
            if (!pGameEngine->GetPlayfield()->GetCell(x, y, pCell))
                continue;

            // is cell empty?
            if (pCell->GetStatus() != QR_Cell::IE_Empty)
            {
                // draw cell
                SetCursorColor(pCell->GetColor());
                std::cout << g_FullChar;
            }
            else
                // insert blank space
                std::printf(" ");

            // reset console text color to default value
            SetCursorColor(QR_Cell::IE_LightWhite);
        }

        // draw play field right border and go to next line
        std::printf("X\r\n");
    }

    // draw play field bottom after game is completely drawn
    printf ( "XXXXXXXXXXXXXXXXX" );

    // get next piece that will be played
    QR_Piece* pNextPiece = pGameEngine->GetNextPiece();

    // calculate info panel x position
    const unsigned infoPosX = pGameEngine->GetPlayfield()->GetWidth() + 7;

    // found it?
    if (pNextPiece)
    {
        // iterate through next piece cells
        for ( int y = 0; y < 3; y++ )
            for ( int x = 0; x < 5; x++ )
                // clear next piece cell
                m_NextPieceCells[(y * 5) + x]->Clear();

        // get play field width and height
        const unsigned nextPieceWidth  = pNextPiece->GetWidth();
        const unsigned nextPieceHeight = pNextPiece->GetHeight();

        // iterate through next piece cells to get
        for (unsigned y = 0; y < nextPieceHeight; ++y)
            for (unsigned x = 0; x < nextPieceWidth; ++x)
            {
                QR_Cell* pCell;

                // get next piece cell
                pNextPiece->GetCell(x, y, pCell);

                // is piece cell filled?
                if (pCell->GetStatus() != QR_Cell::IE_Empty)
                    // copy it
                    *m_NextPieceCells[(y * 5) + x] = *pCell;
            }

        // show next piece title
        ShowMessage(L"Next piece", infoPosX, 3, false);

        // show next piece name
        ShowMessage(pNextPiece->GetName(), infoPosX, 4);

        // draw blank line to erase previous name end, if needed
        std::printf("          ");

        ::COORD cursorPos;

        // draw next piece
        for ( int y = 0; y < 3; y++ )
            for ( int x = 0; x < 5; x++ )
            {
                // move console cursor to next draw position
                cursorPos.X = (SHORT)infoPosX + x;
                cursorPos.Y = (SHORT)y + 6;
                ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);

                // is next piece cell filled?
                if (m_NextPieceCells[(y * 5) + x]->GetStatus() != QR_Cell::IE_Empty)
                {
                    // draw it
                    SetCursorColor(m_NextPieceCells[(y * 5) + x]->GetColor());
                    std::cout << g_FullChar;
                }
                else
                    // draw blank space instead
                    std::printf(" ");

                // reset console text color to default
                SetCursorColor(QR_Cell::IE_LightWhite);
            }
    }

    ::COORD cursorPos;

    // draw score
    cursorPos.X = (SHORT)infoPosX;
    cursorPos.Y = (SHORT)10;
    ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);
    std::wprintf(L"Points : %d", pGameEngine->GetPlayfield()->GetScore());

    // draw level
    cursorPos.X = (SHORT)infoPosX;
    cursorPos.Y = (SHORT)11;
    ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);
    std::wprintf(L"Level  : %d", pGameEngine->GetLevel());

    // replace cursor on the console left top corner
    cursorPos.X = (SHORT)0;
    cursorPos.Y = (SHORT)0;
    ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);
}
//------------------------------------------------------------------------------
void QR_GameInterface::DrawGameOver(QR_GameEngine* pGameEngine)
{
    // calculate info panel x position
    const unsigned infoPosX = pGameEngine->GetPlayfield()->GetWidth() + 7;

    // Show game over message
    ShowMessage(L"GAME OVER",infoPosX, 13);
    ShowMessage(L"Press Enter to restart", infoPosX, 14);

    int c = 0x0;

    // wait until enter key is pressed
    while (c != VK_RETURN)
    {
        c = getch();

        // check if esc key is pressed, quit game if yes
        if (c == VK_ESCAPE)
        {
            m_Quit = true;
            return;
        }
    }

    ::COORD cursorPos;

    // set cursor on the console left top edge
    cursorPos.X = (SHORT)0;
    cursorPos.Y = (SHORT)0;
    ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);

    // get play field width and height
    const unsigned playfieldWidth  = pGameEngine->GetPlayfield()->GetWidth();
    const unsigned playfieldHeight = pGameEngine->GetPlayfield()->GetHeight();

    // clear console completely
    for (unsigned y = 0; y < playfieldHeight; ++y)
    {
        for (unsigned x = 0; x < playfieldWidth + 30; ++x)
            std::printf(" ");

        std::printf("\r\n");
    }

    // set cursor on the console left top edge
    cursorPos.X = (SHORT)0;
    cursorPos.Y = (SHORT)0;
    ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);
}
//------------------------------------------------------------------------------
void QR_GameInterface::ShowMessage(const std::wstring& message, int x, int y,
        bool doResetCursor)
{
    ::COORD cursorPos;

    // change cursor location
    cursorPos.X = (SHORT)x;
    cursorPos.Y = (SHORT)y;
    ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);

    // show message
    std::wprintf(message.c_str());
    // do reset cursor?
    if (doResetCursor)
    {
        // reset cursor on console left top corner
        cursorPos.X = (SHORT)0;
        cursorPos.Y = (SHORT)0;
        ::SetConsoleCursorPosition(m_hWinConsole, cursorPos);
    }
}
//------------------------------------------------------------------------------
void QR_GameInterface::CheckKeyPressed(QR_GameEngine* pEngine)
{
    // get current key
    int c = getch();

    // is extended key?
    if (!c || c == 224)
    {
        // read current key again (to obtain correct code)
        c = getch();

        // search for key code
        switch (c)
        {
            // left arrow key
            case 75:
            {
                pEngine->MoveLeft();

                break;
            }

            // right arrow key
            case 77:
            {
                pEngine->MoveRight();

                break;
            }

            // down arrow key
            case 80:
            {
                pEngine->BypassTimer();

                break;
            }
        }
    }
    else
    {
        // on escape, quit game
        if (c == VK_ESCAPE)
        {
            m_Quit = true;

            return;
        }

        // is y key pressed?
        if (c == 'y')
             pEngine->RotateCounterclockwise();

        // is x key pressed?
        if (c == 'x')
             pEngine->RotateClockwise();

        // is p key presed?
        if (c == 'p')
        {
            // show pause message
            ShowMessage(L"PAUSE", (pEngine->GetPlayfield()->GetWidth() >> 1) - 1,
                    (pEngine->GetPlayfield()->GetHeight() >> 1) - 1, true);

            // wait until p key is pressed again
            do
            {
                c = getch();

                // if esc key is pressed, quit game
                if (c == VK_ESCAPE)
                {
                    m_Quit = true;

                    return;
                }
            }
            while (c != 'p');
        }
    }
}
//------------------------------------------------------------------------------
bool QR_GameInterface::DoQuit()
{
    return m_Quit;
}
//------------------------------------------------------------------------------
void QR_GameInterface::SetCursorColor(QR_Cell::IEColor color)
{
    // Select color
    switch (color)
    {
        case QR_Cell::IE_Red:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Red);
            return;

        case QR_Cell::IE_Green:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Green);
            return;

        case QR_Cell::IE_Blue:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Blue);
            return;

        case QR_Cell::IE_Yellow:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Yellow);
            return;

        case QR_Cell::IE_Violet:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Violet);
            return;

        case QR_Cell::IE_Marine:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Marine);
            return;

        case QR_Cell::IE_White:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_White);
            return;

        case QR_Cell::IE_Gray:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_Gray);
            return;

        case QR_Cell::IE_LightRed:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightRed);
            return;

        case QR_Cell::IE_LightGreen:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightGreen);
            return;

        case QR_Cell::IE_LightBlue:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightBlue);
            return;

        case QR_Cell::IE_LightYellow:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightYellow);
            return;

        case QR_Cell::IE_LightViolet:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightViolet);
            return;

        case QR_Cell::IE_LightMarine:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightMarine);
            return;

        case QR_Cell::IE_LightWhite:
            ::SetConsoleTextAttribute(m_hWinConsole, M_Console_Color_LightWhite);
            return;

        default:
            throw ("Unknown color");
    }
}
//------------------------------------------------------------------------------

