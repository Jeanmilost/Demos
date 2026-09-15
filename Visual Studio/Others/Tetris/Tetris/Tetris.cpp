/****************************************************************************
 * ==> Tetris --------------------------------------------------------------*
 ****************************************************************************
 * Description: A small Tetris game with back end game, and front end using *
 *              the Windows Console. Wrote in the context of a course on    *
 *              Cisco Academy                                               *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Tetris                                                     *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
 * permit persons to whom the Software is furnished to do so, subject to    *
 * the following conditions:                                                *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
 ****************************************************************************/

// std
#include <conio.h>
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <chrono>
#include <random>
#include <thread>
#include <exception>

// windows SDK
#include <windows.h>

/**
* Tetris back end namespace
*@author Jean-Milost Reymond
*/
namespace Tetris
{
    /**
    * Playfield table, columns (width): 10 blocks wide, lines (Height): 20 blocks high
    */
    typedef std::array<std::array<std::size_t, 10>, 20> ITable;

    /**
    * Tetris game exception
    *@author Jean-Milost Reymond
    */
    class Exception : public std::exception
    {
        public:
            /**
            * Constructor
            *@param message - exception message
            */
            Exception(const std::string& message) :
                m_Message(message)
            {}

            /**
            * Gets the exception message
            *@return the exception message
            */
            const char* what() const noexcept override
            {
                return m_Message.c_str();
            }

        private:
            std::string m_Message;
    };

    /**
    * Randomize functions used in the game
    *@author Jean-Milost Reymond
    */
    class Random
    {
        public:
            /**
            * Get random int value between a range
            *@param min - min range the random value may reach
            *@param max - max range the random value may reach
            */
            static int GetInt(int min, int max)
            {
                std::uniform_int_distribution<int> dist(min, max);
                return dist(GetEngine());
            }

        private:
            /**
            * A properly seeded engine, created once and reused
            *@return seeded engine
            */
            static std::mt19937& GetEngine()
            {
                static std::mt19937 engine(std::random_device{}());
                return engine;
            }
    };

    /**
    * A generic Tetris piece
    *@author Jean-Milost Reymond
    */
    class Piece
    {
        public:
            /**
            * Piece rotation enumeration
            */
            enum class IERotation
            {
                R0 = 0,
                R90,
                R180,
                R270
            };

            /**
            * Piece data
            */
            struct IData
            {
                // table containing the piece shape, depends on piece
                typedef std::vector<std::vector<std::size_t>> IShape;

                IShape      m_Shape;
                std::size_t m_Width  = 0;
                std::size_t m_Height = 0;

                /**
                * Constructor
                *@param w - shape width
                *@param h - shape height
                */
                IData(std::size_t w, std::size_t h) :
                    m_Width(w),
                    m_Height(h)
                {
                    // initialize the shape rows
                    m_Shape.resize(m_Height);

                    // initialize the shape cells
                    for (std::size_t j = 0; j < m_Height; ++j)
                    {
                        m_Shape[j].resize(m_Width);

                        for (std::size_t i = 0; i < m_Width; ++i)
                            m_Shape[j][i] = 0;
                    }
                }
            };

            /**
            * Constructor
            *@param w - shape width
            *@param h - shape height
            *@param playfieldW - playfield width
            */
            Piece(std::size_t w, std::size_t h, std::size_t playfieldW) :
                m_Data(IData(w, h)),
                m_X((playfieldW / 2) - (w / 2))
            {}

            virtual ~Piece()
            {}

            /**
            * Global + operator for adding int value to an enumerated rotation value
            *@param rotation - rotation enumerated value on which the int value should be applied
            *@param value - value to apply to rotation
            *@note This operator may be accessed from outside the class due to its friendliness
            */
            friend IERotation operator + (IERotation rotation, int value)
            {
                constexpr int count = 4;

                // here modulo is applied twice to handle negative values. The first modulo brings all values into the range -3 to
                // 3. Adding count afterward shifts that range to 1 to 7, guaranteeing a positive value. The second modulo then
                // folds that back down into 0 to 3, matching the enum range
                return (IERotation)((((int)rotation + value) % count + count) % count);
            }

            /**
            * Global - operator for adding int value to an enumerated rotation value
            *@param rotation - rotation enumerated value on which the int value should be applied
            *@param value - value to apply to rotation
            *@note This operator may be accessed from outside the class due to its friendliness
            */
            friend IERotation operator - (IERotation rotation, int value)
            {
                return rotation + (-value);
            }

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const = 0;

            /**
            * Rotates the piece to the left
            *@return true if piece can rotate, otherwise false
            */
            virtual bool RotateLeft(const ITable& table)
            {
                auto rotation = m_Rotation - 1;

                // check if next rotation is valid
                if (!CanRotate(table, rotation))
                    return false;

                m_Rotation = rotation;

                return true;
            }

            /**
            * Rotates the piece to the right
            *@return true if piece can rotate, otherwise false
            */
            virtual bool RotateRight(const ITable& table)
            {
                auto rotation = m_Rotation + 1;

                // check if next rotation is valid
                if (!CanRotate(table, rotation))
                    return false;

                m_Rotation = rotation;

                return true;
            }

            /**
            * Moves the piece
            *@param table[in, out] - playfield table
            *@param x - x position
            *@param y - y position
            *@return true if piece can move, otherwise false
            */
            virtual bool Move(const ITable& table, std::size_t x, std::size_t y)
            {
                // check if the move is valid
                if (!CanMove(table, x, y))
                    return false;

                // apply the move to the piece
                m_X = x;
                m_Y = y;

                return true;
            }

            /**
            * Moves the piece to the left
            *@param table[in, out] - playfield table
            *@return true if piece can move, otherwise false
            */
            virtual bool MoveLeft(const ITable& table)
            {
                return Move(table, m_X - 1, m_Y);
            }

            /**
            * Moves the piece to the right
            *@param table[in, out] - playfield table
            *@return true if piece can move, otherwise false
            */
            virtual bool MoveRight(const ITable& table)
            {
                return Move(table, m_X + 1, m_Y);
            }

            /**
            * Moves the piece to the down
            *@param table[in, out] - playfield table
            *@return true if piece can move, otherwise false
            */
            virtual bool MoveDown(const ITable& table)
            {
                return Move(table, m_X, m_Y + 1);
            }

            /**
            * Gets x position
            *@return x position
            */
            virtual std::size_t GetX() const
            {
                return m_X;
            }

            /**
            * Gets y position
            *@return y position
            */
            virtual std::size_t GetY() const
            {
                return m_Y;
            }

            /**
            * Gets rotation
            *@return rotation
            */
            virtual IERotation GetRotation() const
            {
                return m_Rotation;
            }

            /**
            * Gets piece data
            *@param rotation - rotation to apply to piece
            *@return piece data
            */
            IData GetData(IERotation rotation) const
            {
                switch (rotation)
                {
                    case IERotation::R90:
                    {
                        // create a copy of the data and swap width and height
                        IData data(m_Data.m_Height, m_Data.m_Width);

                        // copy the data content and rotate them by 90 degrees
                        for (std::size_t j = 0; j < data.m_Height; ++j)
                            for (std::size_t i = 0; i < data.m_Width; ++i)
                                data.m_Shape[j][i] = m_Data.m_Shape[m_Data.m_Height - i - 1][j];

                        return data;
                    }

                    case IERotation::R180:
                    {
                        // create a copy of the data
                        IData data(m_Data.m_Width, m_Data.m_Height);

                        // copy the data content and rotate them by 180 degrees
                        for (std::size_t j = 0; j < data.m_Height; ++j)
                            for (std::size_t i = 0; i < data.m_Width; ++i)
                                data.m_Shape[j][i] = m_Data.m_Shape[m_Data.m_Height - j - 1][m_Data.m_Width - i - 1];

                        return data;
                    }

                    case IERotation::R270:
                    {
                        // create a copy of the data and swap width and height
                        IData data(m_Data.m_Height, m_Data.m_Width);

                        // copy the data content and rotate them by 270 degrees
                        for (std::size_t j = 0; j < data.m_Height; ++j)
                            for (std::size_t i = 0; i < data.m_Width; ++i)
                                data.m_Shape[j][i] = m_Data.m_Shape[i][m_Data.m_Width - j - 1];

                        return data;
                    }

                    default:
                        return m_Data;
                }
            }

            /**
            * Fills the piece in the playfield
            *@param[in, out] table - playfield table, filled playfield on function ends
            */
            virtual void Fill(ITable& table) const
            {
                // get current rotated piece
                const IData data = GetData(m_Rotation);

                // fills the piece in the playfield table
                for (std::size_t j = 0; j < data.m_Height; ++j)
                    for (std::size_t i = 0; i < data.m_Width; ++i)
                        // ignore empty part of the piece
                        if (data.m_Shape[j][i])
                            table[m_Y + j][m_X + i] = data.m_Shape[j][i];
            }

        protected:
            IData       m_Data;
            IERotation  m_Rotation = IERotation::R0;
            std::size_t m_X        = 0;
            std::size_t m_Y        = 0;

            /**
            * Checks if a piece can rotate
            *@param table - playfield table
            *@param rotation - rotation to check
            *@return true if piece can rotate, otherwise false
            */
            virtual bool CanRotate(const ITable& table, IERotation rotation) const
            {
                // get rotated piece
                IData data = GetData(rotation);

                // is rotated piece out of bounds?
                if (m_Y > table.size() - data.m_Height || m_X > table[m_Y].size() - data.m_Width)
                    return false;

                // check if space occupied for new piece rotation is free
                return CheckSpace(table, data, m_X, m_Y);
            }

            /**
            * Checks if a piece can move to the position
            *@param table - playfield table
            *@param x - x position
            *@param y - y position
            *@return true if piece can move, otherwise false
            */
            virtual bool CanMove(const ITable& table, std::size_t x, std::size_t y) const
            {
                // get piece
                IData data = GetData(m_Rotation);

                // is moved piece out of bounds?
                if (y > table.size() - data.m_Height || x > table[y].size() - data.m_Width)
                    return false;

                // check if space occupied for new piece position is free
                return CheckSpace(table, data, x, y);
            }

            /**
            * Check if space occupied by a piece is free
            *@param table - playfield table
            *@param data - piece data to test
            *@param x - x position
            *@param y - y position
            *@return true if space is free, otherwise false
            */
            virtual bool CheckSpace(const ITable& table, const IData& data, std::size_t x, std::size_t y) const
            {
                // check if space for new piece is free
                for (std::size_t j = 0; j < data.m_Height; ++j)
                    for (std::size_t i = 0; i < data.m_Width; ++i)
                        if (data.m_Shape[j][i] && table[y + j][x + i])
                            return false;

                return true;
            }
    };

    /**
    * I-Piece (Straight / Hero) : A straight, 4x1 block line used to score a "Tetris" by clearing four lines at once
    *@author Jean-Milost Reymond
    */
    class I_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            I_Piece(std::size_t playfieldW) :
                Piece(4, 1, playfieldW)
            {
                // fill the piece data
                for (std::size_t j = 0; j < m_Data.m_Height; ++j)
                {
                    m_Data.m_Shape[j].resize(m_Data.m_Width);

                    for (std::size_t i = 0; i < m_Data.m_Width; ++i)
                        m_Data.m_Shape[j][i] = GetIndex();
                }
            }

            virtual ~I_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 1;
            }

            /**
            * Rotates the piece to the left
            *@return true if piece can rotate, otherwise false
            */
            virtual bool RotateLeft(const ITable& table) override
            {
                const bool canRotate = Piece::RotateLeft(table);

                // shift the position on rotating in order to move the rotation origin at the center of the piece
                if (canRotate)
                    switch (m_Rotation)
                    {
                        case Piece::IERotation::R90:
                        case Piece::IERotation::R270:
                            m_X += 2;
                            break;

                        default:
                            m_X -= 2;
                            break;
                    }

                return canRotate;
            }

            /**
            * Rotates the piece to the right
            *@return true if piece can rotate, otherwise false
            */
            virtual bool RotateRight(const ITable& table) override
            {
                const bool canRotate = Piece::RotateRight(table);

                // shift the position on rotating in order to move the rotation origin at the center of the piece
                if (canRotate)
                    switch (m_Rotation)
                    {
                        case Piece::IERotation::R90:
                        case Piece::IERotation::R270:
                            m_X += 2;
                            break;

                        default:
                            m_X -= 2;
                            break;
                    }

                return canRotate;
            }

        protected:
            /**
            * Checks if a piece can rotate
            *@param table - playfield table
            *@param rotation - rotation to check
            *@return true if piece can rotate, otherwise false
            */
            virtual bool CanRotate(const ITable& table, IERotation rotation) const override
            {
                // get rotated piece
                IData       data = GetData(rotation);
                std::size_t x    = m_X;

                // shift the position on rotating in order to move the rotation origin at the center of the piece
                switch (rotation)
                {
                    case Piece::IERotation::R90:
                    case Piece::IERotation::R270:
                        x += 2;
                        break;

                    default:
                        if (x < 2)
                            return false;

                        x -= 2;
                        break;
                }

                // is rotated piece out of bounds?
                if (m_Y > table.size() - data.m_Height || x > table[m_Y].size() - data.m_Width)
                    return false;

                // check if space occupied for new piece rotation is free
                return CheckSpace(table, data, x, m_Y);
            }
    };

    /**
    * J-Piece (Blue Ricky) : Three blocks long with one square sticking out at the bottom - left corner
    *@author Jean-Milost Reymond
    */
    class J_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            J_Piece(std::size_t playfieldW) :
                Piece(2, 3, playfieldW)
            {
                // fill the piece data
                m_Data.m_Shape[0][1] = GetIndex();
                m_Data.m_Shape[1][1] = GetIndex();
                m_Data.m_Shape[2][1] = GetIndex();
                m_Data.m_Shape[2][0] = GetIndex();
            }

            virtual ~J_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 2;
            }
    };

    /**
    * L-Piece (Orange Ricky) : Three blocks long with one square sticking out at the bottom - right corner
    *@author Jean-Milost Reymond
    */
    class L_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            L_Piece(std::size_t playfieldW) :
                Piece(2, 3, playfieldW)
            {
                // fill the piece data
                m_Data.m_Shape[0][0] = GetIndex();
                m_Data.m_Shape[1][0] = GetIndex();
                m_Data.m_Shape[2][0] = GetIndex();
                m_Data.m_Shape[2][1] = GetIndex();
            }

            virtual ~L_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 3;
            }
    };

    /**
    * O-Piece (Square / Smashboy) : A 2x2 square block that does not rotate into a different position
    *@author Jean-Milost Reymond
    */
    class O_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            O_Piece(std::size_t playfieldW) :
                Piece(2, 2, playfieldW)
            {
                // fill the piece data
                for (std::size_t j = 0; j < m_Data.m_Height; ++j)
                {
                    m_Data.m_Shape[j].resize(m_Data.m_Width);

                    for (std::size_t i = 0; i < m_Data.m_Width; ++i)
                        m_Data.m_Shape[j][i] = GetIndex();
                }
            }

            virtual ~O_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 4;
            }
    };

    /**
    * S-Piece (Rhode Island Z): A zigzag shape that steps up to the right
    *@author Jean-Milost Reymond
    */
    class S_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            S_Piece(std::size_t playfieldW) :
                Piece(2, 3, playfieldW)
            {
                // fill the piece data
                m_Data.m_Shape[0][1] = GetIndex();
                m_Data.m_Shape[1][1] = GetIndex();
                m_Data.m_Shape[1][0] = GetIndex();
                m_Data.m_Shape[2][0] = GetIndex();
            }

            virtual ~S_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 5;
            }
    };

    /**
    * T-Piece (Teewee) : Three blocks in a row with one centered underneath, famous for making "T-Spins"
    *@author Jean-Milost Reymond
    */
    class T_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            T_Piece(std::size_t playfieldW) :
                Piece(3, 2, playfieldW)
            {
                // fill the piece data
                m_Data.m_Shape[0][0] = GetIndex();
                m_Data.m_Shape[0][1] = GetIndex();
                m_Data.m_Shape[0][2] = GetIndex();
                m_Data.m_Shape[1][1] = GetIndex();
            }

            virtual ~T_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 6;
            }
    };

    /**
    * Z-Piece (Cleveland Z) : A mirrored zigzag shape that steps up to the left
    *@author Jean-Milost Reymond
    */
    class Z_Piece : public Piece
    {
        public:
            /**
            * Constructor
            *@param playfieldW - playfield width
            */
            Z_Piece(std::size_t playfieldW) :
                Piece(2, 3, playfieldW)
            {
                // fill the piece data
                m_Data.m_Shape[0][0] = GetIndex();
                m_Data.m_Shape[1][0] = GetIndex();
                m_Data.m_Shape[1][1] = GetIndex();
                m_Data.m_Shape[2][1] = GetIndex();
            }

            virtual ~Z_Piece()
            {}

            /**
            * Gets the piece index
            *@return the piece index
            */
            virtual std::size_t GetIndex() const override
            {
                return 7;
            }
    };

    /**
    * Tetris game main logic
    *@author Jean-Milost Reymond
    */
    class Game
    {
        public:
            Game()
            {
                m_Piece.reset(GeneratePiece());
                m_NextPiece.reset(GeneratePiece());
            }

            virtual ~Game()
            {}

            /**
            * Generates a new piece
            *@return newly generated piece
            */
            Piece* GeneratePiece()
            {
                std::unique_ptr<Piece> pPiece;

                // generate a new piece randomly
                switch (Random::GetInt(0, 6))
                {
                    case 0:  pPiece = std::make_unique<I_Piece>(m_Table[0].size()); break;
                    case 1:  pPiece = std::make_unique<J_Piece>(m_Table[0].size()); break;
                    case 2:  pPiece = std::make_unique<L_Piece>(m_Table[0].size()); break;
                    case 3:  pPiece = std::make_unique<O_Piece>(m_Table[0].size()); break;
                    case 4:  pPiece = std::make_unique<S_Piece>(m_Table[0].size()); break;
                    case 5:  pPiece = std::make_unique<T_Piece>(m_Table[0].size()); break;
                    case 6:  pPiece = std::make_unique<Z_Piece>(m_Table[0].size()); break;
                    default: throw Exception("Piece index out of range");
                }

                return pPiece.release();
            }

            /**
            * Gets the current line count
            *@return the current line count
            */
            virtual inline std::size_t GetLines() const
            {
                return m_Lines;
            }

            /**
            * Gets the current score
            *@return the current score
            */
            virtual inline std::size_t GetScore() const
            {
                return m_Score;
            }

            /**
            * Gets the current level
            *@return the current level
            */
            virtual inline std::size_t GetLevel() const
            {
                return m_Level;
            }

            /**
            * Gets if the game is over
            *@return true if the game is over, otherwise false
            */
            virtual inline bool IsGameOver() const
            {
                return m_GameOver;
            }

            /**
            * Resets the game
            */
            virtual void Reset()
            {
                m_Piece.reset(GeneratePiece());
                m_NextPiece.reset(GeneratePiece());

                m_Table    = {};
                m_Lines    = 0;
                m_Level    = 1;
                m_Score    = 0;
                m_GameOver = false;
            }

            /**
            * Moves the piece to the left
            *@return true if piece can move, otherwise false
            */
            virtual bool MoveLeft()
            {
                return m_Piece->MoveLeft(m_Table);
            }

            /**
            * Moves the piece to the right
            *@return true if piece can move, otherwise false
            */
            virtual bool MoveRight()
            {
                return m_Piece->MoveRight(m_Table);
            }

            /**
            * Moves the piece to the down
            *@return true if piece can move, otherwise false
            */
            virtual bool MoveDown()
            {
                // play the piece move down sound
                PlaySound(IESoundContext::MoveDown);

                const bool canMove = m_Piece->MoveDown(m_Table);

                // can no longer move while piece remained on the start position?
                if (!canMove && !m_Piece->GetY())
                    m_GameOver = true;

                return canMove;
            }

            /**
            * Rotates the piece to the left
            *@return true if piece can rotate, otherwise false
            */
            virtual bool RotateLeft()
            {
                return m_Piece->RotateLeft(m_Table);
            }

            /**
            * Rotates the piece to the right
            *@return true if piece can rotate, otherwise false
            */
            virtual bool RotateRight()
            {
                return m_Piece->RotateRight(m_Table);
            }

            /**
            * Handle the piece lock down state
            */
            virtual void HandlePieceLockdown()
            {
                // fill the final piece position in the playfield table
                if (m_Piece)
                    m_Piece->Fill(m_Table);

                std::size_t index = m_Table.size();
                std::size_t lines = 0;

                // check the playfield table to check complete lines
                while (index > 0)
                {
                    --index;

                    bool foundEmptyCell = false;

                    // check if the line contains at least an empty cell
                    for (std::size_t x = 0; x < m_Table[index].size(); ++x)
                        if (!m_Table[index][x])
                        {
                            foundEmptyCell = true;
                            break;
                        }

                    // no empty cell found?
                    if (!foundEmptyCell)
                    {
                        // count the line
                        ++lines;

                        // move all upper lines one line lower
                        for (std::size_t dst = index; dst > 0; --dst)
                            // copy the previous line to the current one
                            for (std::size_t x = 0; x < m_Table[dst].size(); ++x)
                                m_Table[dst][x] = m_Table[dst - 1][x];

                        // clear the first line
                        for (std::size_t x = 0; x < m_Table[0].size(); ++x)
                            m_Table[0][x] = 0;

                        // re-test this same row, since new content just dropped into it
                        ++index;
                    }
                }

                // lines were completed?
                if (lines)
                {
                    m_Lines += lines;

                    // play the line completed sound
                    PlaySound(IESoundContext::LineCompleted);

                    // keep current level
                    const std::size_t level = m_Level;

                    // get level
                    if (m_Lines > 89)
                        m_Level = 9;
                    else
                    if (m_Lines > 79)
                        m_Level = 8;
                    else
                    if (m_Lines > 69)
                        m_Level = 7;
                    else
                    if (m_Lines > 59)
                        m_Level = 6;
                    else
                    if (m_Lines > 49)
                        m_Level = 5;
                    else
                    if (m_Lines > 39)
                        m_Level = 4;
                    else
                    if (m_Lines > 29)
                        m_Level = 3;
                    else
                    if (m_Lines > 19)
                        m_Level = 2;
                    else
                        m_Level = 1;

                    // level changed?
                    const bool levelChanged = (m_Level != level);

                    // calculate the score multiplier: since level 9 => 10, from level 5 to 9 => 6, otherwise 1
                    const std::size_t scoreMultiplier = (m_Level >= 9) ? 10 : ((m_Level >= 5) ? 6 : 1);

                    // count the score
                    switch (lines)
                    {
                        case 0:                                     break;
                        case 1:  m_Score += 40   * scoreMultiplier; break;
                        case 2:  m_Score += 100  * scoreMultiplier; break;
                        case 3:  m_Score += 300  * scoreMultiplier; break;
                        default: m_Score += 1200 * scoreMultiplier; break;
                    }

                    // score changed, draw it
                    DrawScore();

                    // level changed?
                    if (levelChanged)
                    {
                        // draw new level
                        DrawLevel();

                        // play the level up sound
                        PlaySound(IESoundContext::LevelUp);
                    }
                }

                // generate a new piece
                m_Piece.reset(m_NextPiece.release());
                m_NextPiece.reset(GeneratePiece());

                // draw the next piece
                DrawNextPiece();
            }

            /**
            * Draws the next piece
            */
            virtual void DrawNextPiece() const = 0;

            /**
            * Draws the playfield border
            */
            virtual void DrawBorder() const = 0;

            /**
            * Draws the level
            */
            virtual void DrawLevel() const = 0;

            /**
            * Draws the score
            */
            virtual void DrawScore() const = 0;

            /**
            * Draws the playfield content
            */
            virtual void Draw() const = 0;

        protected:
            /**
            * Sound contexts
            */
            enum class IESoundContext
            {
                MoveDown = 0,
                LineCompleted,
                LevelUp
            };

            ITable                 m_Table    = {};
            std::unique_ptr<Piece> m_Piece;
            std::unique_ptr<Piece> m_NextPiece;
            std::size_t            m_Lines    = 0;
            std::size_t            m_Level    = 1;
            std::size_t            m_Score    = 0;
            bool                   m_GameOver = false;

            /**
            * Emits a sound depending on context
            */
            virtual void PlaySound(IESoundContext context) const = 0;
    };
}

/**
* Windows console helper
*@author Jean-Milost Reymond
*/
class ConsoleHelper
{
    public:
        static WORD m_DefBgColor;

        /**
        * Hides the console cursor
        *@param hConsole - console handle
        */
        static void HideCursor(HANDLE hConsole)
        {
            if (!hConsole)
                return;

            CONSOLE_CURSOR_INFO info;
            ::GetConsoleCursorInfo(hConsole, &info);

            info.bVisible = FALSE;
            ::SetConsoleCursorInfo(hConsole, &info);
        }

        /**
        * Gets the current console text attributes
        *@param hConsole - console handle
        *@returns the current attribute word, or 0 on failure
        */
        static WORD GetCurrentAttributes(HANDLE hConsole)
        {
            if (!hConsole)
                return 0;

            CONSOLE_SCREEN_BUFFER_INFO csbi;

            if (!::GetConsoleScreenBufferInfo(hConsole, &csbi))
                return 0;

            return csbi.wAttributes;
        }

        /**
        * Goes to the position at X and Y
        *@param hConsole - console handle
        *@param x - x position in chars
        *@param y - y position in chars
        */
        static void GotoXY(HANDLE hConsole, short x, short y)
        {
            if (!hConsole)
                return;

            COORD pos{ x, y };

            ::SetConsoleCursorPosition(hConsole, pos);
        }

        /**
        * Resets the piece color to default console value
        *@param hConsole - console handle
        */
        static void ResetColor(HANDLE hConsole)
        {
            ::SetConsoleTextAttribute(hConsole, ConsoleHelper::m_DefBgColor);
        }
};
//---------------------------------------------------------------------------
WORD ConsoleHelper::m_DefBgColor = 0;
//---------------------------------------------------------------------------

/**
* Tetris game taking place in a Windows console
*@author Jean-Milost Reymond
*/
class Game_Console : public Tetris::Game
{
    public:
        /**
        * Constructor
        *@param hConsole - console handle
        *@param x - playfield x location
        *@param y - playfield y location
        */
        Game_Console(HANDLE hConsole, std::size_t x, std::size_t y) :
            Game(),
            m_hConsole(hConsole),
            m_X(x),
            m_Y(y)
        {
            // populate the color dictionary
            PopulateColorDict();

            // draw the game border once for all
            DrawBorder();

            // draw the score
            DrawScore();

            // draw the level
            DrawLevel();
        }

        virtual ~Game_Console()
        {}

        /**
        * Draws the next piece
        */
        virtual void DrawNextPiece() const override
        {
            Tetris::Piece::IData data = m_NextPiece->GetData(m_NextPiece->GetRotation());

            const short startX = (short)(m_Table[0].size() + 10);
            const short startY = 4;

            // clear the previous next piece
            for (std::size_t j = 0; j < 4; ++j)
                for (std::size_t i = 0; i < 4; ++i)
                {
                    ::SetConsoleTextAttribute(m_hConsole, const_cast<IColorDict&>(m_ColorDict)[0]);
                    ConsoleHelper::GotoXY(m_hConsole, (short)(startX + m_X + i), (short)(startY + m_Y + j));
                    std::cout << " ";
                    ConsoleHelper::ResetColor(m_hConsole);
                }

            // draw the next piece
            for (std::size_t j = 0; j < data.m_Height; ++j)
                for (std::size_t i = 0; i < data.m_Width; ++i)
                {
                    ::SetConsoleTextAttribute(m_hConsole, const_cast<IColorDict&>(m_ColorDict)[data.m_Shape[j][i]]);
                    ConsoleHelper::GotoXY(m_hConsole, (short)(startX + m_X + i), (short)(startY + m_Y + j));
                    std::cout << " ";
                    ConsoleHelper::ResetColor(m_hConsole);
                }
        }

        /**
        * Draws the playfield border
        */
        virtual void DrawBorder() const override
        {
            // no table?
            if (!m_Table.size())
                return;

            // iterate through playfield height
            for (std::size_t row = 0; row < m_Table.size(); ++row)
            {
                // select border color in console
                ::SetConsoleTextAttribute(m_hConsole, m_BorderColor);

                // draw left edge
                ConsoleHelper::GotoXY(m_hConsole, (short)m_X, (short)(row + m_Y));
                std::cout << " ";

                // draw right edge
                ConsoleHelper::GotoXY(m_hConsole, (short)(m_X + m_Table[row].size() + 1), (short)(row + m_Y));
                std::cout << " ";

                // revert the color to default in console
                ConsoleHelper::ResetColor(m_hConsole);
            }

            // select border color in console
            ::SetConsoleTextAttribute(m_hConsole, m_BorderColor);

            // iterate through playfield width
            for (std::size_t cell = 0; cell < m_Table[0].size() + 2; ++cell)
            {
                // draw bottom edge
                ConsoleHelper::GotoXY(m_hConsole, (short)(cell + m_X), (short)(m_Table.size() + m_Y));
                std::cout << " ";
            }

            // revert the background color to default in console
            ConsoleHelper::ResetColor(m_hConsole);

            // set the foreground color to white
            ::SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            // draw 'Next' text
            ConsoleHelper::GotoXY(m_hConsole, (short)(m_X + m_Table[0].size() + 10), (short)(m_Y + 2));
            std::cout << "Next";

            // draw 'Level' text
            ConsoleHelper::GotoXY(m_hConsole, (short)(m_X + m_Table[0].size() + 10), (short)(m_Y + 8));
            std::cout << "Level";

            // draw 'Score' text
            ConsoleHelper::GotoXY(m_hConsole, (short)(m_X + m_Table[0].size() + 10), (short)(m_Y + 14));
            std::cout << "Score";
        }

        /**
        * Draws the level
        */
        virtual void DrawLevel() const override
        {
            // set the foreground color to white
            ::SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            // draw score
            ConsoleHelper::GotoXY(m_hConsole, (short)(m_X + m_Table[0].size() + 10), (short)(m_Y + 9));
            std::cout << m_Level;
        }

        /**
        * Draws the score
        */
        virtual void DrawScore() const override
        {
            // set the foreground color to white
            ::SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            // draw score
            ConsoleHelper::GotoXY(m_hConsole, (short)(m_X + m_Table[0].size() + 10), (short)(m_Y + 15));
            std::cout << m_Score;
        }

        /**
        * Draws the playfield content
        */
        virtual void Draw() const override
        {
            // no table?
            if (!m_Table.size())
                return;

            // make a modifiable copy of the table
            Tetris::ITable table = m_Table;

            // fill the piece in the temporary playfield table
            if (m_Piece)
                m_Piece->Fill(table);

            // iterate through the playfield table and draw it cell by cell
            for (std::size_t row = 0; row < table.size(); ++row)
                for (std::size_t col = 0; col < table[row].size(); ++col)
                {
                    ::SetConsoleTextAttribute(m_hConsole, const_cast<IColorDict&>(m_ColorDict)[table[row][col]]);
                    ConsoleHelper::GotoXY(m_hConsole, (short)(col + m_X + 1), (short)(row + m_Y));
                    std::cout << " ";
                    ConsoleHelper::ResetColor(m_hConsole);
                }
        }

    private:
        typedef std::map<std::size_t, WORD> IColorDict;

        const WORD  m_BorderColor = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        HANDLE      m_hConsole    = nullptr;
        std::size_t m_X           = 0;
        std::size_t m_Y           = 0;
        IColorDict  m_ColorDict;

        /**
        * Emits a sound depending on context
        */
        virtual void PlaySound(IESoundContext context) const override
        {
            // "short" duration in ms
            const int shortMs = 200;

            switch (context)
            {
                case IESoundContext::MoveDown:
                    // do nothing
                    return;

                case IESoundContext::LineCompleted:
                {
                    // run sound in a thread to avoid the console to slow down while sound is played
                    std::thread soundThread(
                        []()
                        {
                            // do (C6)
                            ::Beep(1047, shortMs);
                        });

                    // run the thread independently
                    soundThread.detach();
                    return;
                }

                case IESoundContext::LevelUp:
                {
                    // run sound in a thread to avoid the console to slow down while sound is played
                    std::thread soundThread(
                        []()
                        {
                            // mi (E5)
                            ::Beep(659, shortMs);

                            // mi (E6)
                            ::Beep(1319, shortMs);
                        });

                    // run the thread independently
                    soundThread.detach();
                    return;
                }

                default:
                    return;

            }
        }

        /**
        * Populates the color dictionary
        */
        void PopulateColorDict()
        {
            m_ColorDict[0] = ConsoleHelper::m_DefBgColor;                              // background color
            m_ColorDict[1] = BACKGROUND_GREEN | BACKGROUND_BLUE;                       // dark cyan
            m_ColorDict[2] = BACKGROUND_BLUE;                                          // dark blue
            m_ColorDict[3] = BACKGROUND_INTENSITY | BACKGROUND_RED;                    // bright red
            m_ColorDict[4] = BACKGROUND_RED;                                           // dark red
            m_ColorDict[5] = BACKGROUND_GREEN;                                         // dark green
            m_ColorDict[6] = BACKGROUND_RED | BACKGROUND_BLUE;                         // dark magenta
            m_ColorDict[7] = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN; // bright yellow
        }
};

//---------------------------------------------------------------------------
// Main entry point
//---------------------------------------------------------------------------
int main()
{
    // get the console handle
    HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

    // initialize the console
    ConsoleHelper::HideCursor(hConsole);
    const WORD defaultAttrs = ConsoleHelper::GetCurrentAttributes(hConsole);

    Game_Console game(hConsole, 2, 2);

    // draw the next piece
    game.DrawNextPiece();

    std::chrono::time_point lastDropTime = std::chrono::steady_clock::now();
    auto                    dropInterval = std::chrono::milliseconds(500);
    bool                    quit         = false;

    while (!quit)
    {
        // set to default speed
        dropInterval = std::chrono::milliseconds(500);

        // increase the speed regarding the level
        switch (game.GetLevel())
        {
            case 0:
            case 1:                                                 break;
            case 2:  dropInterval = std::chrono::milliseconds(450); break;
            case 3:  dropInterval = std::chrono::milliseconds(400); break;
            case 4:  dropInterval = std::chrono::milliseconds(350); break;
            case 5:  dropInterval = std::chrono::milliseconds(300); break;
            case 6:  dropInterval = std::chrono::milliseconds(250); break;
            case 7:  dropInterval = std::chrono::milliseconds(200); break;
            case 8:  dropInterval = std::chrono::milliseconds(150); break;
            default: dropInterval = std::chrono::milliseconds(100); break;
        }

        // is game over?
        if (game.IsGameOver())
        {
            // set the foreground color to white
            ::SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            // draw the game over message
            ConsoleHelper::GotoXY(hConsole, 3, 12);
            std::cout << "Game over!";

            // draw the replay message
            ConsoleHelper::GotoXY(hConsole, 3, 13);
            std::cout << "Replay y/n";

            ConsoleHelper::GotoXY(hConsole, 8, 14);

            // get user input
            char c;
            std::cin >> c;

            // only restart if yes, to avoid the user to enters long line which would break the display
            if (c == 'y' || c == 'Y')
            {
                game.Reset();
                continue;
            }

            break;
        }

        bool hardDrop = false;

        // listen for a pressed key
        if (_kbhit())
        {
            // get last pressed key
            int ch = _getch();

            // arrow keys arrive as a two-byte sequence: 0xE0 (or 0x00) then the code
            if (ch == 0 || ch == 0xE0)
            {
                int arrow = _getch();

                switch (arrow)
                {
                    case 72: game.RotateLeft();                           break; // up arrow
                    case 75: game.MoveLeft();                             break; // left arrow
                    case 77: game.MoveRight();                            break; // right arrow
                    case 80: dropInterval = std::chrono::milliseconds(0); break; // down arrow
                }
            }
            else
                switch (ch)
                {
                    case ' ': hardDrop = true;    break;
                    case 'z':
                    case 'Z': game.RotateRight(); break;
                    case 27:  quit = true;        break; // escape
                }
        }

        // do hard drop?
        if (hardDrop)
        {
            // move the piece until down
            while (game.MoveDown())
            {}

            // handle lock down
            game.HandlePieceLockdown();

            continue;
        }

        // get now time
        const auto now = std::chrono::steady_clock::now();

        // wait for next interval
        if (now - lastDropTime >= dropInterval)
        {
            lastDropTime = now;

            // move piece down
            if (!game.MoveDown())
                game.HandlePieceLockdown();
        }

        // draw the game
        game.Draw();

        ::Sleep(16);
    }

    return 0;
}
