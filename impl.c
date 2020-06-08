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
//  Implementation is gross, gross, gross.
//  There are smart linear programming algorithms for this stuff.
static
int
impl(int** H, int m, int n)
{
    // Observe the maximum height on the perimeter excluding corners
    int hmax = 0;
    for (int i = 1; i < m-1; ++i) {
        if (H[i][0  ] > hmax) hmax = H[i][0  ];
        if (H[i][n-1] > hmax) hmax = H[i][n-1];
    }
    for (int j = 1; j < n-1; ++j) {
        if (H[0  ][j] > hmax) hmax = H[0  ][j];
        if (H[m-1][j] > hmax) hmax = H[m-1][j];
    }

    // Left hand side data for constraints, as computed below.
    int* Ldat = calloc(4 * m * n, sizeof(int));

    // Index of positive contribution to constraints, as computed below.
    ptrdiff_t* Rpos  = calloc(4 * m * n, sizeof(ptrdiff_t));

    // Constraints like...
    //     H[i][j] + w[i][j] <= H[i+1][j] + w[i+1][j]
    // ...are rearranged into...
    //     H[i][j] - H[i+1][j] <= w[i+1][j] - w[i][j]
    // ...so a single-pass on the H matrix finds left hand sides.
    //
    // During that pass, compute offsets to right hand side data.
    //
    // This processing completes all ingest of incoming data.
    for (int i = 1; i < m-1; ++i) {
        for (int j = 1; j < n-1; ++j) {
            // Observe the maximum height on the interior
            if (H[i][j] > hmax) hmax = H[i][j];

            // Compute offset to the first of 4 constraints for (i, j)
            int off = 4*(i*m + j);

            // H[i][j] - H[i+1][j] <= w[i+1][j] - w[i][j]
            Ldat[off+0] = H[i][j] - H[i+1][j];
            Rpos[off+0] = (i+1)*m + (j);

            // H[i][j] - H[i-1][j] <= w[i-1][j] - w[i][j]
            Ldat[off+1] = H[i][j] - H[i-1][j];
            Rpos[off+1] = (i-1)*m + (j);

            // H[i][j] - H[i][j+1] <= w[i][j+1] - w[i][j]
            Ldat[off+2] = H[i][j] - H[i][j+1];
            Rpos[off+2] = (i)*m + (j+1);

            // H[i][j] - H[i][j-1] <= w[i][j-1] - w[i][j]
            Ldat[off+3] = H[i][j] - H[i][j-1];
            Rpos[off+3] = (i)*m + (j-1);
        }
    }

    // Initialize water height to the overall maximum elevation.
    // That is, assume some deluge instantly fills the interior.
    int* W = calloc(m * n, sizeof(int));
    for (int i = 1; i < m-1; ++i) {
        for (int j = 1; j < n-1; ++j) {
            W[i*m + j] = hmax - H[i][j];
        }
    }

    // Now, drain away water from interior when constraints not satisfied.
    for (int h = hmax; h --> 0;) {
        for (int i = 1; i < m-1; ++i) {
            for (int j = 1; j < n-1; ++j) {
                const int off = i*m + j;
                if (W[off]) {                                      // Water?
                    for (int k = 4 * off; k < 4 * off + 4; ++k) {
                        if (Ldat[k] > W[Rpos[k]] - W[off]) {       // Runs?
                            --W[off];                              // Drain!
                            break;                                 // Next.
                        }
                    }
                }
            }
        }
    }

    // How much water remains?
    int sum = 0;
    for (int j = 0; j < m * n; ++j) sum += W[j];

    // Clean up
    free(W);
    free(Rpos);
    free(Ldat);

    return sum;
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
