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
    // Left hand side data for constraints, as computed below
    int* Ldat = calloc(4 * m * n, sizeof(int));

    // Index of positive contribution to constraints.
    ptrdiff_t* Rpos  = calloc(4 * m * n, sizeof(ptrdiff_t));

    // Unknown water height
    int* W = calloc(m * n, sizeof(int));

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

    // Constraints like...
    //     H[i][j] + w[i][j] <= H[i+1][j] + w[i+1][j]
    // ...are rearranged into...
    //     H[i][j] - H[i+1][j] <= w[i+1][j] - w[i][j]
    // ...so a single-pass on the H matrix finds left hand sides.
    //
    // During that pass, compute offsets to right hand side data.
    //
    // This processing completes all ingest of incoming data.
    {
        int off = 0;
        for (int i = 1; i < m-1; ++i) {
            for (int j = 1; j < n-1; ++j) {
                // H[i][j] - H[i+1][j] <= w[i+1][j] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i+1][j];
                Rpos[off++] = (i+1)*m + (j);

                // H[i][j] - H[i-1][j] <= w[i-1][j] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i-1][j];
                Rpos[off++] = (i-1)*m + (j);

                // H[i][j] - H[i][j+1] <= w[i][j+1] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i][j+1];
                Rpos[off++] = (i)*m + (j+1);

                // H[i][j] - H[i][j-1] <= w[i][j-1] - w[i][j]
                Ldat[off  ] = H[i][j] - H[i][j-1];
                Rpos[off++] = (i)*m + (j-1);

                // Observed the maximum height on the interior
                if (H[i][j] > hmax) hmax = H[i][j];
            }
        }

    }

    // Initialize water height to the overall maximum elevation
    for (int i = 1; i < m-1; ++i) {
        for (int j = 1; j < n-1; ++j) {
            W[i*m + j] = hmax - H[i][j];
        }
    }

    // Now, drain away water whenever constraints are not satisfied.
    // Indices i, j, k take on new meanings below versus that from above.
    for (int i = hmax; i --> 0;) {
        for (int j = 0; j < m * n; ++j) {
            for (int k = 0; k < 4; ++k) {
                if (Ldat[4*j+k] > W[Rpos[j]] - W[j]) {  // Violated?
                    W[j] = W[j] > 0 ? --W[j] : 0;       // Yes, drain.
                    break;                              // Next square.
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
