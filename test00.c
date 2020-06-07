#include "impl.h"

#define M (2)
#define N (2)

int
main()
{
    int r0[] = {0, 0};
    int r1[] = {0, 0};
    int* H[] = {r0, r1};
    int  n[] = {N, N};
    return 0 != trapRainWater(H, M, n);
}
