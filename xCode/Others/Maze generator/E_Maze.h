/*****************************************************************************
 * ==> E_Maze ---------------------------------------------------------------*
 * ***************************************************************************
 * Description : Maze generator                                              *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include <vector>

/**
* Maze generator
@author Jean-Milost Reymond
*/
class E_Maze
{
    public:
        /**
        * Maze cell
        *@author Jean-Milost Reymond
        */
        struct ICell
        {
            /**
            * Maze cell type
            *@author Jean-Milost Reymond
            */
            enum IE_Type
            {
                IE_Empty, // empty cell (passage)
                IE_Wall,  // wall
                IE_Door,  // door
                IE_Key,   // this cell contains key
                IE_Start, // start point
                IE_End,   // end point
            };

            IE_Type m_Type;
        };

        /**
        * Constructor
        *@param width - width of the maze
        *@param height - height of the maze
        */
        E_Maze(unsigned width, unsigned height);

        /**
        * Get maze width (in cells)
        *@returns maze width (in cells)
        */
        unsigned GetWidth();

        /**
        * Get maze height (in cells)
        *@returns maze height (in cells)
        */
        unsigned GetHeight();

        /**
        * Get cell at the given location
        *@param x - x location
        *@param y - y location
        *@returns cell at the given location
        */
        ICell Get(unsigned x, unsigned y);

        /**
        * Get cell at the given index
        *@param index - cell index
        *@returns cell at the given index
        */
        ICell Get(unsigned index);

        /**
        * Set cell at the given location
        *@param x - x location
        *@param y - y location
        *@param cell - cell to set
        */
        void Set(unsigned x, unsigned y, const ICell& cell);

        /**
        * Set cell at the given index
        *@param index - index to set
        *@param cell - cell to set
        */
        void Set(unsigned index, const ICell& cell);

    private:
        /**
        * Structure to represent a coordinate
        *@author Jean-Milost Reymond
        */
        struct ICoords
        {
            unsigned m_X;
            unsigned m_Y;
        };

        std::vector< std::vector<ICell> > m_Maze;

        /**
        * Create special cell of given type
        *@param type - type of cell to create
        *@throws exception
        */
        void CreateSpecialCell(const ICell::IE_Type& type);

        /**
        * Split blank space to create a wall
        *@param min - min corner of the space to split
        *@param ma - max corner of the space to split
        *@param deep - current recursive deep
        *@param maxDeep - maximum recursive deep
        */
        void Split(const ICoords& min, const ICoords& max, unsigned deep = 0, unsigned maxDeep = -1);
};
