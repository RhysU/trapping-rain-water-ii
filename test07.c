#include "impl.h"

#define M (5)
#define N (4)

int
main()
{
    int r0[] = {6, 6, 6, 6};
    int r1[] = {6, 5, 5, 6};
    int r2[] = {6, 5, 6, 6};
    int r3[] = {6, 5, 0, 6};
    int r4[] = {6, 6, 6, 6};
    int* H[] = {r0, r1, r2, r3, r4};
    int  n[] = {N, N, N, N, N};
    return 10 != trapRainWater(H, M, n);
}
