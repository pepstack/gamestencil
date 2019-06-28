/*!
 * \file
 *    crash3.h
 *
 * \brief
 *    CRASH3 public header defines the interface that the
 *    CRASH3 library presents to client programs.
 *
 * \author
 *    cheungmine
 * 
 * \since
 *    Sat, Feb 07, 2015  7:39:24 AM
 *
 * \date
 *    ${date}
 *
 * \version
 *    ${version}
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
 *
 * ----------------------------------------------------------------------------
 *
 * \note
 *
 * This header file defines the interface that the CRASH3 library presents
 * to client programs.  If a C-function, structure, datatype, or constant
 * definition does not appear in this file, then it is not a published API of
 * CRASH3, is subject to change without notice, and should not be referenced
 * by programs that use CRASH3.
 *
 * Some of the definitions that are in this file are marked as "experimental".
 * Experimental interfaces are normally new features recently added to CRASH3.
 * We do not anticipate changes to experimental interfaces but reserve the
 * right to make minor changes if experience from use "in the wild" suggest
 * such changes are prudent.
 *
 * The official C-language API documentation for CRASH3 is derived from
 * comments in this file.  This file is the authoritative source on how
 * CRASH3 interfaces are suppose to operate.
 *
 * The name of this file under configuration management is "crash3.h.in".
 * The makefile makes minor changes to this file (such as inserting the
 * version number) and changes its name to "crash3.h"
 * as part of the build process.
 */

#ifndef CRASH3_H_INCLUDED
#define CRASH3_H_INCLUDED

/*
 * Make sure we can call this stuff from C++.
 */
#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Add the ability to override "extern"
 */
#ifndef CRASH3_EXTERN
# define CRASH3_EXTERN extern
#endif

#ifndef CRASH3_API
# define CRASH3_API
#endif

/*
 * These no-op macros are used in front of interfaces to mark those
 * interfaces as either deprecated or experimental.  New applications
 * should not use deprecated interfaces - they are support for backwards
 * compatibility only.  Application writers should be aware that
 * experimental interfaces are subject to change in point releases.
 *
 * These macros used to resolve to various kinds of compiler magic that
 * would generate warning messages when they were used.  But that
 * compiler magic ended up generating such a flurry of bug reports
 * that we have taken it all out and gone back to using simple
 * noop macros.
 */
#define CRASH3_DEPRECATED
#define CRASH3_EXPERIMENTAL

/*
 * Ensure these symbols were not defined by some previous header file.
 */
#ifdef CRASH3_VERSION
# undef CRASH3_VERSION
#endif

#ifdef CRASH3_VERSION_NUMBER
# undef CRASH3_VERSION_NUMBER
#endif

#define CRASH3_VERSION        "${version}"
#define CRASH3_VERSION_NUMBER  ${version_number}

/* place your code below */
#define CRASH3_BOOL int
#define CRASH3_TRUE  1
#define CRASH3_FALSE 0

#define CRASH3_RESULT int
#define CRASH3_SUCCESS 0
#define CRASH3_ERROR  (-1)

#define CRASH3_MAX_ROWS  8
#define CRASH3_MAX_COLS  8
#define CRASH3_MIN_ROWS  5
#define CRASH3_MIN_COLS  6

#define CRASH3_INVALID_COLOR  0
#define CRASH3_START_COLOR    1

#define CRASH3_MAX_COLORS_4   4
#define CRASH3_MAX_COLORS_5   5
#define CRASH3_MAX_COLORS_6   6

/*
*        col->
*      +-------+-------+-------+-------+-------+-------+-------+
*  row |00     |01     |02     |03     |04     |05     |06     |
*   |  |       |       |       |       |       |       |       |
*   v  |       |       |       |       |       |       |       |
*      +-------+-------+-------+-------+-------+-------+-------+
*      |10     |11     |12     |13     |14     |15     |16     |
*      |       |       |       |       |       |       |       |
*      |       |       |       |       |       |       |       |
*      +-------+-------+-------+-------+-------+-------+-------+
*      |20     |21     |22     |23     |24     |25     |26     |
*      |       |(xp,yp)|       |       |       |       |       | height
*      |       | .     |       |       |       |       |       |
*      +-------+-------+-------+-------+-------+-------+-------+
*      |30     |31     |32     |33     |34     |35     |36     |
*      |       |       |       |       |       |       |       |
*      |       |       |       |       |       |       |       |
*      +-------+-------+-------+-------+-------+-------+-------+
*  y   |40     |41     |42     |43     |44     |45     |46     |
*  ^   |       |       |       |       |       |       |       |
*  |   |       |       |       |       |       |       |       |
*  |   o-------+-------+-------+-------+-------+-------+-------+
*  |  origin                     width
*  +------------>x
* (0,0)
*/

typedef struct crash3_colors_t  * CRA3COLORS_HANDLE;

typedef struct CRA3Cell
{
    int row;
    int col;
    int color;
    int flag;
} CRA3Cell;


typedef struct CRA3Entry
{
    union {
        struct {
            char color;
            char row;
            char col;
            char cells;
        };
        int _entry;
    };
} CRA3Entry, * CRASH3_ENTRY;


typedef enum CRA3Color4
{
    CLR4_INVALID    = CRASH3_INVALID_COLOR,
    CLR4_RED        = CRASH3_START_COLOR,
    CLR4_GREEN,
    CLR4_BLUE,
    CLR4_YELLOW,
    CLR4_END
} CRA3CLR4;


typedef enum CRA3Color5
{
    CLR5_INVALID    = CRASH3_INVALID_COLOR,
    CLR5_RED        = CRASH3_START_COLOR,
    CLR5_GREEN,
    CLR5_BLUE,
    CLR5_YELLOW,
    CLR5_PURPLE,
    CLR5_END
} CRA3CLR5;


typedef enum CRA3Color6
{
    CLR6_INVALID    = CRASH3_INVALID_COLOR,
    CLR6_RED        = CRASH3_START_COLOR,
    CLR6_GREEN,
    CLR6_BLUE,
    CLR6_YELLOW,
    CLR6_PURPLE,
    CLR6_CYAN,
    CLR6_END
} CRA3CLR6;


CRASH3_API CRASH3_EXTERN
CRA3COLORS_HANDLE CRA3ColorsCreate (
    int rows,
    int cols,
    int maxcolors);


CRASH3_API CRASH3_EXTERN
void CRA3ColorsDestroy (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
void CRA3ColorsPrintLog (
    CRA3COLORS_HANDLE hcra3,
    const char * file,
    const char * state,
    const char * func);


CRASH3_API CRASH3_EXTERN
int CRA3ColorsRows (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3ColorsCols (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3GetMaxColors (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3GetInvalidColors (
    CRA3COLORS_HANDLE hcra3);


CRASH3_BOOL CRA3ColorsHasResults(
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3ColorsGetCell (
    CRA3COLORS_HANDLE hcra3,
    int row,
    int col,
    CRA3Cell *cell);


CRASH3_API CRASH3_EXTERN
CRASH3_RESULT CRA3ColorsSetCellColor (
    CRA3COLORS_HANDLE hcra3,
    const CRA3Cell *cell);


CRASH3_API CRASH3_EXTERN
void CRA3ColorsShuffle (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
CRASH3_BOOL CRA3ColorsMarkErase (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
CRASH3_BOOL CRA3ColorsCanSwap (
    CRA3COLORS_HANDLE hcra3,
    const CRA3Cell *cell,
    const CRA3Cell *cell2);


CRASH3_API CRASH3_EXTERN
CRASH3_BOOL CRA3ColorsSwap (
    CRA3COLORS_HANDLE hcra3,
    int row, int col,
    int row2, int col2);


CRASH3_API CRASH3_EXTERN
int CRA3GetResults (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3ColorsErase (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3ColorsCollapse (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3ColorsCrash (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
int CRA3GetErasedRows (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
CRASH3_ENTRY CRA3GetErasedRowEntry (
    CRA3COLORS_HANDLE hcra3,
    int at);


CRASH3_API CRASH3_EXTERN
int CRA3GetErasedCols (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
CRASH3_ENTRY CRA3GetErasedColEntry (
    CRA3COLORS_HANDLE hcra3,
    int at);


CRASH3_API CRASH3_EXTERN
int CRA3GetCollapsedCols (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
CRASH3_ENTRY CRA3GetCollapsedEntry (
    CRA3COLORS_HANDLE hcra3,
    int at);


CRASH3_API CRASH3_EXTERN
CRASH3_ENTRY CRA3GetCrashedEntry (
    CRA3COLORS_HANDLE hcra3,
    int at);


CRASH3_API CRASH3_EXTERN
int CRA3GetCrashedCols (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
void CRA3ClearErased (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
void CRA3ClearCollapsed (
    CRA3COLORS_HANDLE hcra3);


CRASH3_API CRASH3_EXTERN
void CRA3ClearCrashed (
    CRA3COLORS_HANDLE hcra3);

#if defined(__cplusplus)
}
#endif

#endif /* CRASH3_H_INCLUDED */
