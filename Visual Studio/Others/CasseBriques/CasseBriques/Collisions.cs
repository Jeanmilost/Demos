using System;
using System.Drawing;

namespace CasseBriques
{
    /// <summary>
    /// Classe pour tester les collisions
    /// </summary>
    class Collisions
    {
        /// <summary>
        /// Teste si une collision a eu lieu
        /// </summary>
        /// <param name="Object1">Rectangle autour du 1er objet à tester</param>
        /// <param name="Object2">Rectangle autour du 2ème objet à tester</param>
        /// <returns>True si il y a collision, autrement false</returns>
        static public bool IsCollided( Rectangle Object1, Rectangle Object2 )
        {
            // on calcule un rectangle à l'intersection des deux rectangles
            Rectangle Result = Rectangle.Intersect( Object1, Object2 );

            // si le rectangle a une dimension autre que 0, on est en collision
            if ( Result.Width > 0 || Result.Height > 0 )
                return true;
            else
                return false;
        }

        /// <summary>
        /// Calcule quel côté de la brique la balle a touché 
        /// </summary>
        /// <param name="balle">L'objet balle</param>
        /// <param name="brique">L'objet brique</param>
        static public void Edge( Balle balle, Briques brique )
        {
            // on calcule la distance la plus courte du côté gauche,
            // et on s'assure qu'elle sera toujours plus grande que 0
            int left = brique.X - ( balle.X + balle.Diametre );
            left = ( left < 0 ) ? -left : left;

            // on calcule la distance la plus courte du côté haut,
            // et on s'assure qu'elle sera toujours plus grande que 0
            int top = brique.Y - ( balle.Y + balle.Diametre );
            top = ( top < 0 ) ? -top : top;

            // on calcule la distance la plus courte du côté droite,
            // et on s'assure qu'elle sera toujours plus grande que 0
            int right = ( brique.X + brique.Width ) - balle.X;
            right = ( right < 0 ) ? -right : right;

            // on calcule la distance la plus courte du côté bas,
            // et on s'assure qu'elle sera toujours plus grande que 0
            int bottom = ( brique.Y + brique.Height ) - balle.Y;
            bottom = ( bottom < 0 ) ? -bottom : bottom;

            // on teste de quel côté (gauche ou droite) la balle est le plus
            // proche de la brique
            if ( left < top && left < right && left < bottom )
                balle.OffsetX = -balle.Speed;
            else if ( right < left && right < top && right < bottom )
                balle.OffsetX = balle.Speed;

            // on teste de quel côté (haut ou bas) la balle est le plus
            // proche de la brique
            if ( top < left && top < right && top < bottom )
                balle.OffsetY = -balle.Speed;
            else if ( bottom < left && bottom < right && bottom < top )
                balle.OffsetY = balle.Speed;
        }
    }
}
