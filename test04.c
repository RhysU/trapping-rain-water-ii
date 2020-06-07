#include "impl.h"

#define M (3)
#define N (3)

int
main()
{
    int r0[] = {0, 3, 0};
    int r1[] = {2, 0, 2};
    int r2[] = {0, 2, 0};
    int* H[] = {r0, r1, r2};
    int  n[] = {N, N, N};
    return 2 != trapRainWater(H, M, n);
}
