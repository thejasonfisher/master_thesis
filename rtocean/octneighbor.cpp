
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
#include "octcell.h"
#include "physics.h"

////////////////////////////////////////////////////////////////
// CONSTRUCTORS AND DESTRUCTOR
////////////////////////////////////////////////////////////////

// TODO: Remove the default constructor, this is just unnecessary
/* Default constructor */
octneighbor::octneighbor()
{
}

void octneighbor::set(octcell* neighbor_cell, nlnode* corresponding_neighbor_list_entry, uint dimension, bool positive_direction, pftype water_volume_coefficient, pftype total_volume_coefficient, pftype water_velocity_in_the_out_direction, pfvec distance, pftype distance_absolute_value, pftype cell_face_area)
{
    n               = neighbor_cell                      ;
    cnle            = corresponding_neighbor_list_entry  ;
    dim             = dimension                          ;
    pos_dir         = positive_direction                 ;
    water_vol_coeff = water_volume_coefficient           ;
    total_vol_coeff = total_volume_coefficient           ;
    vel_out         = water_velocity_in_the_out_direction;
    dist            = distance                           ;
    dist_abs        = distance_absolute_value            ;
    cf_area         = cell_face_area                     ;
}

//inline
pftype octneighbor::get_average_cell_density()
{
    octcell* c1 = n;
    octcell* c2 = cnle->v.n;
    //return c1->get_density();
    return (c1->get_density()*c1->s + c2->get_density()*c2->s)/(c1->s + c2->s);
}

pftype octneighbor::get_associated_mass_per_unit_area()
{
    octcell* c1 = n;
    octcell* c2 = cnle->v.n;
    //return c1->get_density();
    return 0.5 * (c1->get_density()*c1->s + c2->get_density()*c2->s);
}

pftype octneighbor::get_cell_face_density()
{
    return physics::vol_coeffs_to_density(water_vol_coeff, total_vol_coeff);
}

void octneighbor::set_velocity_out(pftype velocity_out)
{
    vel_out = velocity_out;
    cnle->v.vel_out = -velocity_out;
}

void octneighbor::set_volume_coefficients(pftype water_volume_coefficient, pftype total_volume_coefficient)
{
#if  DEBUG
#if  0
    cout << endl;
    cout << "Cell wall: Old water volume coefficient: " << water_vol_coeff << endl;
    cout << "Cell wall: Old total volume coefficient: " << total_vol_coeff << endl;
    cout << "Cell wall: Additional water volume coefficient: " << water_volume_coefficient-water_vol_coeff << endl;
    cout << "Cell wall: Additional total volume coefficient: " << total_volume_coefficient-total_vol_coeff << endl;
    cout << "Cell wall: New water volume coefficient: " << water_volume_coefficient << endl;
    cout << "Cell wall: New total volume coefficient: " << total_volume_coefficient << endl;
#endif
    if (IS_NAN(water_volume_coefficient)) {
        throw logic_error("Trying to set a NaN water_volume_coefficient in cell wall");
    }
    if (IS_NAN(total_volume_coefficient)) {
        throw logic_error("Trying to set a NaN total_volume_coefficient in cell wall");
    }
    if (water_volume_coefficient < 0) {
        throw logic_error("Trying to set a negative water_volume_coefficient in cell wall");
    }
    if (total_volume_coefficient < 0) {
        throw logic_error("Trying to set a negative total_volume_coefficient in cell wall");
    }
    if (water_volume_coefficient > total_volume_coefficient) {
        throw logic_error("Trying to set a higher water_volume_coefficient than total_volume_coefficient in cell wall");
    }
#endif
    water_vol_coeff = water_volume_coefficient;
    total_vol_coeff = total_volume_coefficient;
    cnle->v.water_vol_coeff = water_volume_coefficient;
    cnle->v.total_vol_coeff = total_volume_coefficient;
}

void octneighbor::update_velocity(octcell* cell1, octcell* cell2, pftype dt)
{
    /* No advection term implemented */
    pftype double_distance = cell1->s + cell2->s;
#if  !NO_ATMOSPHERE
    //pftype average_density = (cell1->total_density*cell1->s + cell2->total_density*cell2->s) / distance;
    pftype average_density = get_average_cell_density();
#endif
    //average_total_density = MIN(average_total_density, P_WATER_DENSITY); // Prevent nasty circulation behaviours in the water
    //TODO: Prevevt circulation behaviour even in the air
    pftype distance = 0.5 * double_distance;
#if  NO_ATMOSPHERE
    vel_out += ((cell1->p - cell2->p) / (distance * NORMAL_WATER_DENSITY) - dist[VERTICAL_DIMENSION]/dist_abs * P_G) * dt;
#else
    if (!average_density) {
        // Nothing to accelerate
        return;
    }
    //vel_out += ((cell1->p - cell2->p) / (distance * average_density) - dist[VERTICAL_DIMENSION]/dist_abs * P_G * (average_density > NORMAL_WATER_DENSITY ? NORMAL_WATER_DENSITY/average_density : 1)) * dt;
    vel_out += ((cell1->p - cell2->p) / (distance * average_density) - dist[VERTICAL_DIMENSION]/dist_abs * P_G) * dt;
#endif
    cnle->v.vel_out = -vel_out;
}
