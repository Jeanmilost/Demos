/******************************************************************************
 * Cette classe représente un bloc. C'est le matériel de base pour les pièces *
 * et le terrain de jeu.                                                      *
 ******************************************************************************/

#ifndef CELL_H
#define CELL_H

/*
 * Enumérateur pour choisir la couleur des blocs
 */
enum Game_Color
{
    RED         = 1,
    GREEN       = 2,
    BLUE        = 3,
    YELLOW      = 4,
    VIOLET      = 5,
    MARINE      = 6,
    WHITE       = 7,
    GREY        = 8,
    LIGHTRED    = 9,
    LIGHTGREEN  = 10,
    LIGHTBLUE   = 11,
    LIGHTYELLOW = 12,
    LIGHTVIOLET = 13,
    LIGHTMARINE = 14,
    LIGHTWHITE  = 15
};

/*
 * Classe représentant un bloc, utilisé pour les pièces et le terrain de jeu.
 */
class Cell
{
  private:

    Game_Color p_Color;

    bool       p_IsBusy;
    bool       p_IsLocked;

  public:

    // Constructeur
    Cell();

    // Destructeur
    ~Cell();

  public:

    Game_Color GetColor();
    void SetColor( Game_Color Color );

    bool IsBusy();
    void SetBusy( bool Value );

    bool IsLocked();
    void SetLock( bool Value );
};

#endif // CELL_H
