/*!\file voronoi.h 
 *
 *\brief Diagramme de voronoi par cercles grossissants.
 *
 * \author Farès BELHADJ, amsi@up8.edu
 * \date June 03 2020
 */
#include <GL4D/gl4duw_SDL2.h>
#ifndef VORONOI_H_INCLUDED
#  define VORONOI_H_INCLUDED

#  ifdef __cpluplus
extern "C" {
#  endif

  extern void vdInit(int nb_sites, SDL_Surface* bg);
  extern void vdFree(void);
  extern void vdDraw(void);
  extern void vdMove(void);

#  ifdef __cpluplus
}
#  endif

#endif
