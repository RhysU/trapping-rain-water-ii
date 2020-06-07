#include "impl.h"

#define M (5)
#define N (3)

int
main()
{
    int r0[] = {0, 0, 0};
    int r1[] = {0, 0, 0};
    int r2[] = {0, 0, 0};
    int r3[] = {0, 0, 0};
    int r4[] = {0, 0, 0};
    int* H[] = {r0, r1, r2, r3, r4};
    int  n[] = {N, N, N, N, N};
    return 0 != trapRainWater(H, M, n);
}
