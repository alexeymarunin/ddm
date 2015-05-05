#ifndef DDM_DEFINES_H
#define DDM_DEFINES_H

enum ddmDeltaViewMode
{
    DVM_INITIAL_STATE = -1,
    DVM_POSITIVE_DELTA = 0,
    DVM_NEGATIVE_DELTA = 1
};

enum ddmNeighborType
{
    NT_ANY_NEIGHBOR     = 0,
    NT_FIRST_NEIGHBOR   = 1,
    NT_SECOND_NEIGHBOR  = 2,
    NT_FAR_NEIGHBOR     = 3
};

#endif // DDM_DEFINES_H

