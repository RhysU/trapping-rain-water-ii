#include "impl.h"

#define M (3)
#define N (3)

int
main()
{
    int r0[] = {0, 1, 9};
    int r1[] = {1, 0, 1};
    int r2[] = {9, 1, 0};
    int* H[] = {r0, r1, r2};
    int  n[] = {N, N, N};
    return 1 != trapRainWater(H, M, n);
}
