#include "impl.h"

#define M (2)
#define N (2)

int
main()
{
    int H[M][N] = {
        {0, 0},
        {0, 0},
    };
    return 0 != trapRainWater(H, M, N);
}
