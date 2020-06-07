#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

// A non-ragged array implementation solving:
//
//  maximize sum(W[i][j])
//  subject to
//     H[i][j] + w[i][j] <= H[i+1][j] + w[i+1][j]
//     H[i][j] + w[i][j] <= H[i-1][j] + w[i-1][j]
//     H[i][j] + w[i][j] <= H[i][j+1] + w[i][j+1]
//     H[i][j] + w[i][j] <= H[i][j-1] + w[i][j-1]
//               w[i][j] >= 0
//  \forall i \in 1 .. (m-1)
//  \forall j \in 1 .. (n-1)
//
//  where
//
//    H[i][j] represents the terrain elevation (i.e. height)
//    W[i][j] represents the water atop the terrain
//
//  No claims of optimal anything on the solution.
static
int impl(int** H, int m, int n)
{
    // Constraints like...
    //     H[i][j] + w[i][j] <= H[i+1][j] + w[i+1][j]
    // ...are rearranged into...
    //     H[i][j] - H[i+1][j] <= w[i+1][j] - w[i][j]
    //     H[i][j] - H[i-1][j] <= w[i-1][j] - w[i][j]
    //     H[i][j] - H[i][j+1] <= w[i][j+1] - w[i][j]
    //     H[i][j] - H[i][j-1] <= w[i][j-1] - w[i][j]
    // ...so a single-pass on the H matrix finds left hand sides.
    //
    // During that pass, compute offsets to right hand side data.
    //
    // Afterwards, compute an upper bound on the possible water W.
    int* Ldat = calloc(4 * m * n, sizeof(int));
    ptrdiff_t* Rpos  = calloc(4 * m * n, sizeof(ptrdiff_t));
    ptrdiff_t* Rneg  = calloc(4 * m * n, sizeof(ptrdiff_t));
    int* W = calloc(m * n, sizeof(int));
    {
        int hmax = 0;
        int off = 0;
        for (int i = 1; i < m-1; ++i) {
            for (int j = 1; j < n-1; ++j) {
                // H[i][j] - H[i+1][j] <= w[i+1][j] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i+1][j];
                Rpos[off  ] = (i+1)*m + (j);
                Rneg[off++] = i*m + j;

                // H[i][j] - H[i-1][j] <= w[i-1][j] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i-1][j];
                Rpos[off  ] = (i-1)*m + (j);
                Rneg[off++] = i*m + j;

                // H[i][j] - H[i][j+1] <= w[i][j+1] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i][j+1];
                Rpos[off  ] = (i)*m + (j+1);
                Rneg[off++] = i*m + j;

                // H[i][j] - H[i][j-1] <= w[i][j-1] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i][j-1];
                Rpos[off  ] = (i)*m + (j-1);
                Rneg[off++] = i*m + j;

                // Observed the maximum height
                if (H[i][j] > hmax) hmax = H[i][j];
            }
        }

        // The maximum water in any location is no more than maximum height
        for (int i = 1; i < m-1; ++i) {
            for (int j = 1; j < n-1; ++j) {
                W[i*m + j] = hmax - H[i][j];
            }
        }
    }

    return 0;
}

// Possibly ragged array implementation delegates to non-ragged variant.
int trapRainWater(int** H, int m, int* n)
{
    assert(m >= 0);            // Non-positive rows nonsensical
    if (m < 2) return 0;       // Too few rows is trivial

    assert(n[0] >= 0);         // Non-positive cols nonsensical
    if (n[0] < 2) return 0;    // Too few cols is trivial

    for (int i = 1; i < m; ++i) {
        assert(n[0] == n[i]);  // Confirm array isn't ragged
    }

    return impl(H, m, n[0]);
}
