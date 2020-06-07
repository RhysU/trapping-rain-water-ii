#include <assert.h>

// A non-ragged array implementation
static
int impl(int** H, int m, int n)
{
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
