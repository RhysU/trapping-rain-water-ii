#include "impl.h"

#define M (3)
#define N (3)

int
main()
{
    int r0[] = {9, 0, 0};
    int r1[] = {1, 0, 1};
    int r2[] = {0, 1, 0};
    int* H[] = {r0, r1, r2};
    int  n[] = {N, N, N};
    return 0 != trapRainWater(H, M, n);
}
