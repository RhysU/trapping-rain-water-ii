#include "impl.h"

#define M (2)
#define N (2)

int
main()
{
    int r1[] = {0, 0};
    int r2[] = {0, 0};
    int* H[] = {r1, r2};
    int  n[] = {N, N};
    return 0 != trapRainWater(H, M, n);
}
