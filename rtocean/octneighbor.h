#ifndef OCTNEIGHBOR_H
#define OCTNEIGHBOR_H

////////////////////////////////////////////////////////////////
// INCLUDE FILES
////////////////////////////////////////////////////////////////

// Own includes
#include "definitions.h"
#include "dllist.h"

////////////////////////////////////////////////////////////////
// PREDECLARATIONS
////////////////////////////////////////////////////////////////

class octneighbor;
class octcell;

////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////

typedef  dllist<octneighbor>   nlist ;
typedef  dllnode<octneighbor>  nlnode;

////////////////////////////////////////////////////////////////
// CLASS DEFINTION
////////////////////////////////////////////////////////////////

class octneighbor
{
public:
    octneighbor();

public:

    /***********
     * General *
     ***********/

    /* Neighbor */
    octcell* n; /* Pointer to the neighbor cell */
    nlnode*  cnle; // The neighbor's corresponding neighbor list entry

    /* Direction of the octneighbor */
    uint dim;
    bool pos_dir;

    /*****************
     * Navier-Stokes *
     *****************/

    /* Water flow */
    pftype vel; /* Velocity of the water in the positive direction (dist if pos_dir = true, -dist otherwise) */

    /* Distance between cells */
    pfvec  dist;
    pftype dist_abs;

    /* Surface */
    pftype cf_area; /* Cell face area */

public:
    /* Public methods */
    void set(octcell* neighbor_cell, nlnode* corresponding_neighbor_list_entry, uint dimension, bool positive_direction, pftype water_velocity_in_the_positive_direction, pfvec distance, pftype distance_absolute_value, pftype cell_face_area);

    /* Navier-Stokes */
    bool should_calculate_new_velocity();
    void quick_update_velocity(octcell* cell1, octcell* cell2);

private:
    /*************************
     * Disabled constructors *
     *************************/
    octneighbor(octneighbor&); // Copy constructor prevented from all use
};

inline
bool octneighbor::should_calculate_new_velocity()
{
    return pos_dir;
}



#endif // OCTNEIGHBOR_H
