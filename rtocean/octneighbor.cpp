
////////////////////////////////////////////////////////////////
// INCLUDE FILES
////////////////////////////////////////////////////////////////

// Standard includes
//#include <stdexcept>
//using std::exception;
//using std::logic_error;
//using std::out_of_range;

// Own includes
#include "octneighbor.h"

////////////////////////////////////////////////////////////////
// CONSTRUCTORS AND DESTRUCTOR
////////////////////////////////////////////////////////////////

// TODO: Remove the default constructor, this is just unnecessary
/* Default constructor */
octneighbor::octneighbor()
{
#if DEBUG
    initialize(0, 0, 0, pfvec(), 0, 0, 0, 0);
#endif
}

void octneighbor::initialize(nlnode* neighbor_list_entry, nlnode* matching_neighbor_list_entry, octcell* neighbor_cell, pfvec distance, pftype distance_absolute_value, pftype cell_face_area, uint direction, bool reverse_direction)
{
    nle      = neighbor_list_entry         ;
    mnle     = matching_neighbor_list_entry;
    n        = neighbor_cell               ;
    dist     = distance                    ;
    dist_abs = distance_absolute_value     ;
    cf_area  = cell_face_area              ;
    dir      = direction                   ;
    rev_dir  = reverse_direction           ;
}
