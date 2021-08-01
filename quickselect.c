/*
 *  This Quickselect routine is based on the algorithm described in
 *  "Numerical recipes in C", Second Edition,
 *  Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
 *  This code by Nicolas Devillard - 1998. Public domain.
 */


typedef double elem_type ;

#define ELEM_SWAP(a,b) { register elem_type t=(a);(a)=(b);(b)=t; }

elem_type quick_select(elem_type arr[], int n) 
{
	elem_type * cpy = (elem_type *) malloc ( sizeof(elem_type) * n );
	memcpy (cpy, arr, sizeof(elem_type) * n);

	
	
    int low, high ;
    int median;
    int middle, ll, hh;

    low = 0 ; high = n-1 ; median = (low + high) / 2;
    for (;;) {
        if (high <= low) /* One element only */
            return cpy[median] ;

        if (high == low + 1) {  /* Two elements only */
            if (cpy[low] > cpy[high])
                ELEM_SWAP(cpy[low], cpy[high]) ;
            return cpy[median] ;
        }

    /* Find median of low, middle and high items; swap into position low */
    middle = (low + high) / 2;
    if (cpy[middle] > cpy[high])    ELEM_SWAP(cpy[middle], cpy[high]) ;
    if (cpy[low] > cpy[high])       ELEM_SWAP(cpy[low], cpy[high]) ;
    if (cpy[middle] > cpy[low])     ELEM_SWAP(cpy[middle], cpy[low]) ;

    /* Swap low item (now in position middle) into position (low+1) */
    ELEM_SWAP(cpy[middle], cpy[low+1]) ;

    /* Nibble from each end towards middle, swapping items when stuck */
    ll = low + 1;
    hh = high;
    for (;;) {
        do ll++; while (cpy[low] > cpy[ll]) ;
        do hh--; while (cpy[hh]  > cpy[low]) ;

        if (hh < ll)
        break;

        ELEM_SWAP(cpy[ll], cpy[hh]) ;
    }

    /* Swap middle item (in position low) back into correct position */
    ELEM_SWAP(cpy[low], cpy[hh]) ;

    /* Re-set active partition */
    if (hh <= median)
        low = ll;
        if (hh >= median)
        high = hh - 1;
    }
	
	free(cpy);
	cpy = NULL;
}

#undef ELEM_SWAP
