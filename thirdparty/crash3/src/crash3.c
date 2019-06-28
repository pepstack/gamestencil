/*!
 * \file
 *    crash3.c
 *
 * \since
 *    Sat, Feb 07, 2015  7:39:24 AM
 *
 * \copyright
 *
 *    Copyright (c) 2015, cheungmine
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see LICENSE.LGPL).
 * This option is discussed in more detail in
 *     "CRASH3.html".
 *
 * ----------------------------------------------------------------------------
 * PERMISSION IS HEREBY GRANTED, FREE OF CHARGE, TO ANY PERSON OBTAINING
 * A COPY OF THIS SOFTWARE AND ASSOCIATED DOCUMENTATION FILES (THE "SOFTWARE"),
 * TO DEAL IN THE SOFTWARE WITHOUT RESTRICTION, INCLUDING WITHOUT LIMITATION
 * THE RIGHTS TO USE, COPY, MODIFY, MERGE, PUBLISH, DISTRIBUTE, SUBLICENSE,
 * AND/OR SELL COPIES OF THE SOFTWARE, AND TO PERMIT PERSONS TO WHOM THE
 * SOFTWARE IS FURNISHED TO DO SO, SUBJECT TO THE FOLLOWING CONDITIONS:
 *
 * THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED
 * IN ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "crash3.h"
#include "isaac_rand.h"

#include <stdlib.h>
#include <memory.h>

#define FIXED_CRASH3  (3)

#define CRA3_CELL_ISVALID(row,col)  ((row) >= 0 && (row) < hcra3->_ROWS && (col) >= 0 && (col) < hcra3->_COLS)
#define CRA3_CELL_INDEX(row,col)  ((row) * (hcra3->_COLS) + (col))
#define CRA3_CELL_COLOR(row,col)  (hcra3->colors_[CRA3_CELL_INDEX(row,col)])
#define CRA3_CELL_COLORNAME(row,col)  ((char) (hcra3->_COLORNAMES[ (hcra3->colors_[CRA3_CELL_INDEX(row,col)]) ]))
#define CRA3_CELL_RESULT(row,col)  (hcra3->results_[CRA3_CELL_INDEX(row,col)])

#define CRA3_ERASEDCOL_ENTS()  (hcra3->erasedColEnts[0]._entry)
#define CRA3_ERASEDROW_ENTS()  (hcra3->erasedRowEnts[0]._entry)
#define CRA3_COLLAPSED_ENTS()  (hcra3->collapsedEnts[0]._entry)
#define CRA3_CRASHED_ENTS()  (hcra3->crashedEnts[0]._entry)

#define CRA3_RAND_COLOR(type)  ((type) (isaac_randint(&hcra3->ctx_, CRASH3_START_COLOR, hcra3->_NUMCLRS)))

#define CRA3_COLORNAMES_SIZE  8

static int CRA3_COLORS4[] = {'O', 'R', 'G', 'B', 'Y', 0};
static int CRA3_COLORS5[] = {'O', 'R', 'G', 'B', 'Y', 'P', 0};
static int CRA3_COLORS6[] = {'O', 'R', 'G', 'B', 'Y', 'P', 'C', 0};


typedef struct crash3_colors_t
{
    /* const members */
    int _ROWS;
    int _COLS;
    int _CELLS;
    int _NUMCLRS;
    int _COLORNAMES[CRA3_COLORNAMES_SIZE];

    int colors_[CRASH3_MAX_ROWS*CRASH3_MAX_COLS];
    int results_[CRASH3_MAX_ROWS*CRASH3_MAX_COLS];

    int temps_[CRASH3_MAX_ROWS*CRASH3_MAX_COLS];

    randctx ctx_;

    CRA3Entry  erasedColEnts[CRASH3_MAX_ROWS*CRASH3_MAX_COLS + 1];
    CRA3Entry  erasedRowEnts[CRASH3_MAX_ROWS*CRASH3_MAX_COLS + 1];

    CRA3Entry  collapsedEnts[CRASH3_MAX_ROWS*CRASH3_MAX_COLS + 1];
    CRA3Entry  crashedEnts[CRASH3_MAX_ROWS*CRASH3_MAX_COLS + 1];
} * CRASH3_HANDLE;


static void CRA3ClearResults(CRASH3_HANDLE hcra3)
{
    int at = 0;
    int * p = & hcra3->results_[0];
    while (at++ < CRASH3_MAX_ROWS*CRASH3_MAX_COLS) {
        *p++ = 0;
    }
}
   

static CRASH3_BOOL CRA3CellIsNeighbor(const CRA3Cell * cell, const CRA3Cell * cell2)
{
    if (cell == cell2) {
        return CRASH3_FALSE;
    } else {
        int d = 0;
    
        if (cell->row == cell2->row) {
            d = cell->col - cell2->col;
        } else if (cell->col == cell2->col) {
            d = cell->row - cell2->row;
        }

        return ((d == 1 || d == -1)? CRASH3_TRUE : CRASH3_FALSE);
    }
}


static CRASH3_BOOL CRA3IsCrash3Colors(int at, const int * clrs, int num)
{
    int clr = clrs[at];
    int crashs = 1;

    int i = at;
    while (i-- > 0) {
        if (clrs[i] != clr) {
            break;
        }
        ++crashs;
    }
    if (crashs >= FIXED_CRASH3) {
        return CRASH3_TRUE;
    }

    i = at;
    while (++i < num) {
        if (clrs[i] != clr) {
            break;
        }
        ++crashs;
    }

    return ((crashs >= FIXED_CRASH3) ? CRASH3_TRUE : CRASH3_FALSE);
}


static void CRA3ResultAdd(CRASH3_HANDLE hcra3, int row, int col, int val)
{
    int *p = & hcra3->results_[CRA3_CELL_INDEX(row,col)];
    *p = val;
}


static void CRA3ResultSub(CRASH3_HANDLE hcra3, int row, int col)
{
    int *p = & hcra3->results_[CRA3_CELL_INDEX(row,col)];
    if (*p > 0) {
        (*p)--;
    }
}


static int CRA3CellGetDims(CRASH3_HANDLE hcra3, int row, int col)
{
    int dims = 0;

    if (CRA3_CELL_ISVALID(row, col)) {
        int color = CRA3_CELL_COLOR( row, col);

        if (CRA3_CELL_RESULT(row, col)) {
            if (CRA3_CELL_ISVALID(row, col-1) &&
                CRA3_CELL_COLOR(row, col-1) == color &&
                CRA3_CELL_RESULT(row, col-1)) {
                ++dims;
            } else if (CRA3_CELL_ISVALID(row, col+1) &&
                CRA3_CELL_COLOR(row, col+1) == color &&
                CRA3_CELL_RESULT(row, col+1)) {
                ++dims;
            }

            if (dims) {
                if (CRA3_CELL_ISVALID(row-1, col) &&
                    CRA3_CELL_COLOR(row-1, col) == color &&
                    CRA3_CELL_RESULT(row-1, col)) {
                    ++dims;
                } else if (CRA3_CELL_ISVALID(row+1, col) &&
                    CRA3_CELL_COLOR(row+1, col) == color &&
                    CRA3_CELL_RESULT(row+1, col)) {
                    ++dims;
                }
            }
        }

        return dims;
    }

    return dims;
}


static void CRA3MarkEraseCell(CRASH3_HANDLE hcra3, int row, int col)
{
    if (CRA3_CELL_ISVALID(row, col)) {
        int a, b, c;

        b = CRA3_CELL_COLOR(row, col);

        if (b != CRASH3_INVALID_COLOR) {
            /* mark with row */
            if (CRA3_CELL_ISVALID(row-1, col)) {
                a = CRA3_CELL_COLOR(row-1, col);

                if (a == b) {
                    if (CRA3_CELL_ISVALID(row+1, col)) {
                        c = CRA3_CELL_COLOR(row+1, col);

                        if (b == c) {
                            CRA3ResultAdd(hcra3, row, col, 1);
                            CRA3ResultAdd(hcra3, row-1, col, 1);
                            CRA3ResultAdd(hcra3, row+1, col, 1);
                        }
                    }
                }
            }

            /* mark with col */
            if (CRA3_CELL_ISVALID(row, col-1)) {
                a = CRA3_CELL_COLOR(row, col-1);

                if (a == b) {
                    if (CRA3_CELL_ISVALID(row, col+1)) {
                        c =CRA3_CELL_COLOR(row, col+1);

                        if (b == c) {
                            CRA3ResultAdd(hcra3, row, col, 1);
                            CRA3ResultAdd(hcra3, row, col-1, 1);
                            CRA3ResultAdd(hcra3, row, col+1, 1);
                        }
                    }
                }
            }
        }
    }
}


static void CRA3CollapseCol(CRASH3_HANDLE hcra3, int col, int * row_colors)
{
    int *p = & row_colors[hcra3->_ROWS];

    /* q-->p */
    while (p-- > row_colors) {
        if (*p == CRASH3_INVALID_COLOR) {
            int * q = p;

            while (q-- > row_colors) {
                if (*q != CRASH3_INVALID_COLOR) {
                    /* swap p with q */
                    int at = ++hcra3->collapsedEnts[0]._entry;
                    CRA3Entry * entry = & hcra3->collapsedEnts[at];

                    entry->color = (char) *q;
                    entry->col = (char) col;
                    entry->row = (char) (q - row_colors);   // from row
                    entry->cells = (char) (p - row_colors); // to row

                    hcra3->colors_[CRA3_CELL_INDEX(entry->row, entry->col)] = CRASH3_INVALID_COLOR;
                    hcra3->colors_[CRA3_CELL_INDEX(entry->cells, entry->col)] = entry->color;

                    *p = *q;
                    *q = 0;
                    break;
                }
            }

            if (q == row_colors) {
                break;
            }
        }
    }
}

/**********************************************************************
 *                                                                    *
 *                        CRASH3 PUBLIC API                           *
 *                                                                    *
 *********************************************************************/

CRA3COLORS_HANDLE CRA3ColorsCreate (int rows, int cols, int maxcolors)
{
    CRASH3_HANDLE p = 0;

    if (rows > CRASH3_MAX_ROWS || rows < CRASH3_MIN_ROWS) {
        return 0;
    }
    if (cols > CRASH3_MAX_COLS || cols < CRASH3_MIN_COLS) {
        return 0;
    }

    if (maxcolors == CRASH3_MAX_COLORS_4) {
        p = (CRASH3_HANDLE) calloc(1, sizeof(*p));
        p->_NUMCLRS = CRASH3_MAX_COLORS_4;
        memcpy(p->_COLORNAMES, CRA3_COLORS4, sizeof(CRA3_COLORS4));
    } else if (maxcolors == CRASH3_MAX_COLORS_5) {
        p = (CRASH3_HANDLE) calloc(1, sizeof(*p));
        p->_NUMCLRS = CRASH3_MAX_COLORS_5;
        memcpy(p->_COLORNAMES, CRA3_COLORS5, sizeof(CRA3_COLORS5));
    } else if (maxcolors == CRASH3_MAX_COLORS_6) {
        p = (CRASH3_HANDLE) calloc(1, sizeof(*p));
        p->_NUMCLRS = CRASH3_MAX_COLORS_6;
        memcpy(p->_COLORNAMES, CRA3_COLORS6, sizeof(CRA3_COLORS6));
    } else {
        return 0;
    }

    p->_ROWS = rows;
    p->_COLS = cols;
    p->_CELLS = rows*cols;

    randctx_init(&p->ctx_, 1);

    return p;
}


void CRA3ColorsDestroy (CRA3COLORS_HANDLE hcra3)
{
    free(hcra3);
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

void CRA3ColorsPrintLog (CRA3COLORS_HANDLE hcra3,
    const char * file,
    const char * state,
    const char * func)
{
    FILE * fp = fopen(file, "a+");
    if (fp) {
        int row, col;

        if (func) {
            fprintf(fp, "\n>>>> %s (%s) {", state, func);
        } else {
            fprintf(fp, "\n>>>> %s {", state);
        }

        fprintf(fp, "\n---- COLORS ----\n");
        for (row = 0; row < hcra3->_ROWS; ++row) {
            for (col = 0; col < hcra3->_COLS; ++col) {
                fprintf(fp, " %c", CRA3_CELL_COLORNAME(row,col));
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n---- RESULTS ----\n");
        for (int row = 0; row < hcra3->_ROWS; ++row) {
            for (int col = 0; col < hcra3->_COLS; ++col) {
                fprintf(fp, " %d", CRA3_CELL_RESULT(row,col));
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "}\n");
        fclose(fp);
    }
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif


int CRA3ColorsRows (CRA3COLORS_HANDLE hcra3)
{
    return hcra3->_ROWS;
}


int CRA3ColorsCols (CRA3COLORS_HANDLE hcra3)
{
    return hcra3->_COLS;
}


int CRA3GetMaxColors (CRA3COLORS_HANDLE hcra3)
{
    return hcra3->_NUMCLRS;
}


int CRA3GetInvalidColors (CRA3COLORS_HANDLE hcra3)
{
    int count = 0;
    int * p = hcra3->colors_;

    while (p - hcra3->colors_ < hcra3->_CELLS) {
        if (! *p++) {
            ++count;
        }
    }

    return count;
}


int CRA3ColorsGetCell (CRA3COLORS_HANDLE hcra3,
    int row, int col, CRA3Cell *cell)
{
    if (row < 0 || row >= hcra3->_ROWS ||
        col < 0 || col >= hcra3->_COLS) {
        return CRASH3_ERROR;
    } else {
        int index = CRA3_CELL_INDEX(row,col);
        if (cell) {
            cell->color = hcra3->colors_[index];
            cell->flag = hcra3->results_[index];        
            cell->row = row;
            cell->col = col;
        }
        return index;
    }
}


CRASH3_RESULT CRA3ColorsSetCellColor (CRA3COLORS_HANDLE hcra3, const CRA3Cell *cell)
{
    if (cell->row < 0 || cell->row >= hcra3->_ROWS ||
        cell->col < 0 || cell->col >= hcra3->_COLS) {
        return CRASH3_ERROR;
    } else {
        hcra3->colors_[ CRA3_CELL_INDEX(cell->row, cell->col) ] = cell->color;
        return CRASH3_SUCCESS;
    }
}

void CRA3ColorsShuffle (CRA3COLORS_HANDLE hcra3)
{
    static int fixed_colors[] = {
         3, 1, 2, 1, 2, 4, 1,
         1, 2, 2, 3, 4, 2, 3,
         4, 2, 4, 2, 3, 4, 4,
         1, 4, 4, 3, 3, 4, 4,
         1, 4, 1, 4, 4, 2, 1
    };

    do {
        int n = 0;
        int * p = hcra3->colors_;
        int * q = hcra3->results_;

        while (n++ < hcra3->_CELLS) {
            *q++ = 0;

            /* fixed colors for testing:
                *p++ = fixed_colors[n-1];
            */

            *p++ = CRA3_RAND_COLOR(int);
        }
    } while (CRA3ColorsMarkErase(hcra3));
}


CRASH3_BOOL CRA3ColorsHasResults (CRA3COLORS_HANDLE hcra3)
{
    const int * p = hcra3->results_;
    while (p - hcra3->results_ < hcra3->_CELLS) {
        if (*p++) {
            return CRASH3_TRUE;
        }
    }
    return CRASH3_FALSE;
}


CRASH3_BOOL CRA3ColorsMarkErase (CRA3COLORS_HANDLE hcra3)
{
    int row, col;

    CRA3ClearResults(hcra3);
    
    for (row = 0; row < hcra3->_ROWS; ++row) {
        for (col = 0; col < hcra3->_COLS; ++col) {
            CRA3MarkEraseCell(hcra3, row, col);
        }
    }

    for (row = 0; row < hcra3->_ROWS; ++row) {
        for (col = 0; col < hcra3->_COLS; ++col) {
            if (CRA3CellGetDims(hcra3, row, col) == 2) {
                CRA3ResultAdd(hcra3, row, col, 2);
            }
        }
    }

    return CRA3ColorsHasResults(hcra3);
}


CRASH3_BOOL CRA3ColorsCanSwap (CRA3COLORS_HANDLE hcra3,
    const CRA3Cell *cell, const CRA3Cell *cell2)
{
    if (! CRA3CellIsNeighbor(cell, cell2)) {
        CRASH3_FALSE;
    }

    if (cell->color == cell2->color) {
        return CRASH3_FALSE;
    } else {
        int clr = cell->color;
        int clr2 = cell2->color;
        int row, col;

        if (cell->row == cell2->row) {
            // swap two cols(c1,c2) in the same row(r0) horiztally
            int *r0 = & hcra3->temps_[0];
            int *c1 = r0 + CRASH3_MAX_COLS;
            int *c2 = c1 + CRASH3_MAX_ROWS;

            // copy row
            col = 0;
            while (col < hcra3->_COLS) {
                r0[col] = CRA3_CELL_COLOR(cell->row, col);
                ++col;
            }

            // swap cols
            r0[cell->col] = clr2;
            r0[cell2->col] = clr;

            if (CRA3IsCrash3Colors(cell->col, r0, hcra3->_COLS)) {
                return CRASH3_TRUE;
            }
            if (CRA3IsCrash3Colors(cell2->col, r0, hcra3->_COLS)) {
                return CRASH3_TRUE;
            }

            // copy cols
            row = 0;
            while (row < hcra3->_ROWS) {
                c1[row] = CRA3_CELL_COLOR(row, cell->col);
                c2[row] = CRA3_CELL_COLOR(row, cell2->col);
                ++row;
            }

            // swap rows
            c1[cell->row] = clr2;
            c2[cell->row] = clr;

            if (CRA3IsCrash3Colors(cell->row, c1, hcra3->_ROWS)) {
                return CRASH3_TRUE;
            }
            if (CRA3IsCrash3Colors(cell->row, c2, hcra3->_ROWS)) {
                return CRASH3_TRUE;
            }
        } else if (cell->col == cell2->col) {
            // swap two rows(r1,r2) in the same col(c0) vertically
            int *c0 = & hcra3->temps_[0];
            int *r1 = c0 + CRASH3_MAX_ROWS;
            int *r2 = r1 + CRASH3_MAX_COLS;

            // copy col
            row = 0;
            while (row < hcra3->_ROWS) {
                c0[row] = CRA3_CELL_COLOR(row, cell->col);
                ++row;
            }
            c0[cell->row] = clr2;
            c0[cell2->row] = clr;

            if (CRA3IsCrash3Colors(cell->row, c0, hcra3->_ROWS)) {
                return CRASH3_TRUE;
            }
            if (CRA3IsCrash3Colors(cell2->row, c0, hcra3->_ROWS)) {
                return CRASH3_TRUE;
            }

            // copy rows
            col = 0;
            while (col < hcra3->_COLS) {
                r1[col] = CRA3_CELL_COLOR(cell->row, col);
                r2[col] = CRA3_CELL_COLOR(cell2->row, col);
                ++col;
            }

            // swap cols
            r1[cell->col] = clr2;
            r2[cell->col] = clr;

            if (CRA3IsCrash3Colors(cell->col, r1, hcra3->_COLS)) {
                return CRASH3_TRUE;
            }
            if (CRA3IsCrash3Colors(cell->col, r2, hcra3->_COLS)) {
                return CRASH3_TRUE;
            }
        }
        
        return CRASH3_FALSE;
    }
}


CRASH3_BOOL CRA3ColorsSwap (CRA3COLORS_HANDLE hcra3,
    int row, int col,
    int row2, int col2)
{
     if (CRA3_CELL_ISVALID(row, col) &&
         CRA3_CELL_ISVALID(row2, col2)) {
        int *p = & hcra3->colors_[CRA3_CELL_INDEX(row, col)];
        int *q = & hcra3->colors_[CRA3_CELL_INDEX(row2, col2)];

        if (p == q || *p == *q) {
            return CRASH3_FALSE;
        } else {
            int t = *p;
            *p = *q;
            *q = t;
            return CRASH3_TRUE;
        }
    } else {
        return CRASH3_FALSE;
    }
}


int CRA3GetResults (CRA3COLORS_HANDLE hcra3)
{
    int nResults = 0;
    int * p = hcra3->results_;

    while (p - hcra3->results_ < hcra3->_CELLS) {
        if (*p++) {
            ++nResults;
        }
    }

    return nResults;
}


int CRA3ColorsErase (CRA3COLORS_HANDLE hcra3)
{
    CRA3Entry *entry;

    int row, col, row2, col2, color;

    int colEntry = 1;
    int rowEntry = 1;

    int erasedResults = CRA3GetResults(hcra3);

    CRA3_ERASEDCOL_ENTS() = 0;
    CRA3_ERASEDROW_ENTS() = 0;

    for (col = 0; col < hcra3->_COLS; ++col) {
        for (row = 0; row < hcra3->_ROWS; ++row) {
            /* check cols */
            if (CRA3_CELL_RESULT(row, col) > 0) {
                color = CRA3_CELL_COLOR(row, col);

                entry = & hcra3->erasedColEnts[colEntry];
                entry->cells = 1;

                /* check to right cols... */
                col2 = col;
                while (++col2 < hcra3->_COLS) {
                    if (color == CRA3_CELL_COLOR(row, col2) &&
                        CRA3_CELL_RESULT(row, col2) > 0) {
                        CRA3ResultSub(hcra3, row, col2);
                        ++entry->cells;
                    } else {
                        break;
                    }
                }

                if (entry->cells > 2) {
                    /* horizontal colors found */
                    CRA3ResultSub(hcra3, row, col);

                    entry->color = (char) color;
                    entry->row = (char) row;
                    entry->col = (char) col;

                    CRA3_ERASEDCOL_ENTS() = colEntry++;
                }
            }

            /* check rows */
            if (CRA3_CELL_RESULT(row, col) > 0) {
                color = CRA3_CELL_COLOR(row, col);

                entry = & hcra3->erasedRowEnts[rowEntry];
                entry->cells = 1;

                /* check to down rows... */
                row2 = row;
                while (++row2 < hcra3->_ROWS) {
                    if (color == CRA3_CELL_COLOR(row2, col) &&
                        CRA3_CELL_RESULT(row2, col) > 0) {
                        CRA3ResultSub(hcra3, row2, col);
                        ++entry->cells;
                    } else {
                        break;
                    }
                }

                if (entry->cells > 2) {
                    // vertical colors found
                    CRA3ResultSub(hcra3, row, col);

                    entry->color = (char) color;
                    entry->row = (char) row;
                    entry->col = (char) col;

                    CRA3_ERASEDROW_ENTS() = rowEntry++;
                }
            }
        }
    }

    return erasedResults;
}


int CRA3ColorsCollapse (CRA3COLORS_HANDLE hcra3)
{
    int row, col;

    CRA3_COLLAPSED_ENTS() = 0;

    for (col = 0; col < hcra3->_COLS; ++col) {
        for (row = 0; row < hcra3->_ROWS; ++row) {
            hcra3->temps_[row] = CRA3_CELL_COLOR(row, col);
        }
        CRA3CollapseCol(hcra3, col, hcra3->temps_);
    }

    return CRA3_COLLAPSED_ENTS();
}


/**
 * crash colors
 */
int CRA3ColorsCrash (CRA3COLORS_HANDLE hcra3)
{
    int row, col, at, *pclr;
    CRA3Entry * entry;

    CRA3_CRASHED_ENTS() = 0;

    for (col = 0; col < hcra3->_COLS; ++col) {
        for (row = hcra3->_ROWS - 1; row >= 0 ; --row) {
            pclr = & hcra3->colors_[CRA3_CELL_INDEX(row, col)];

            if (* pclr == CRASH3_INVALID_COLOR) {
                entry = & hcra3->crashedEnts[0];

                at = ++entry->_entry;
                entry = & hcra3->crashedEnts[at];

                entry->row = (char) row;
                entry->col = (char) col;
                entry->color = CRA3_RAND_COLOR(char);

                * pclr = (int) entry->color;
            }
        }
    }

    return CRA3_CRASHED_ENTS();
}


int CRA3GetErasedRows (CRA3COLORS_HANDLE hcra3)
{
    return CRA3_ERASEDROW_ENTS();
}


CRASH3_ENTRY CRA3GetErasedRowEntry (CRA3COLORS_HANDLE hcra3, int at)
{
    if (at >= 0 && at < CRA3_ERASEDROW_ENTS()) {    
        return & hcra3->erasedRowEnts[at+1];
    } else {
        return 0;
    }
}


int CRA3GetErasedCols (CRA3COLORS_HANDLE hcra3)
{
    return CRA3_ERASEDCOL_ENTS();
}


CRASH3_ENTRY CRA3GetErasedColEntry (CRA3COLORS_HANDLE hcra3, int at)
{
    if (at >= 0 && at < CRA3_ERASEDCOL_ENTS()) {    
        return & hcra3->erasedColEnts[at+1];
    } else {
        return 0;
    }
}


int CRA3GetCollapsedCols (CRA3COLORS_HANDLE hcra3)
{
    return CRA3_COLLAPSED_ENTS();
}


CRASH3_ENTRY CRA3GetCollapsedEntry (CRA3COLORS_HANDLE hcra3, int at)
{
    if (at >= 0 && at < CRA3_COLLAPSED_ENTS()) {
        return & hcra3->collapsedEnts[at+1];
    } else {
        return 0;
    }
}


int CRA3GetCrashedCols (CRA3COLORS_HANDLE hcra3)
{
    return CRA3_CRASHED_ENTS();
}


CRASH3_ENTRY CRA3GetCrashedEntry (CRA3COLORS_HANDLE hcra3, int at)
{
    if (at >= 0 && at < CRA3_CRASHED_ENTS()) {
        return & hcra3->crashedEnts[at+1];
    } else {
        return 0;
    }
}


void CRA3ClearErased (CRA3COLORS_HANDLE hcra3)
{
    CRA3_ERASEDROW_ENTS() = 0;
    CRA3_ERASEDCOL_ENTS() = 0;
}


void CRA3ClearCollapsed (CRA3COLORS_HANDLE hcra3)
{
    CRA3_COLLAPSED_ENTS() = 0;
}


void CRA3ClearCrashed (CRA3COLORS_HANDLE hcra3)
{
    CRA3_CRASHED_ENTS() = 0;
}
