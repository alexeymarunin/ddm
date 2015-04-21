#ifndef DDM_DEFINES_H
#define DDM_DEFINES_H

enum ddmDeltaViewMode
{
    DVM_POSITIVE_DELTA,
    DVM_NEGATIVE_DELTA
};

enum ddmNeighborType
{
    NT_ANY_NEIGHBOR     = 0,
    NT_FIRST_NEIGHBOR   = 1,
    NT_SECOND_NEIGHBOR  = 2,
    NT_FAR_NEIGHBOR     = 3
};

#endif // DDM_DEFINES_H

