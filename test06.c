#include "impl.h"

#define M (5)
#define N (3)

int
main()
{
    int r0[] = {0, 7, 0};
    int r1[] = {1, 0, 1};
    int r2[] = {0, 2, 2};
    int r3[] = {2, 0, 2};
    int r4[] = {0, 2, 0};
    int* H[] = {r0, r1, r2, r3, r4};
    int  n[] = {N, N, N, N, N};
    return 3 != trapRainWater(H, M, n);
}
