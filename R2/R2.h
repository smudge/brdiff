#ifndef R2_INCLUDED
#define R2_INCLUDED


#ifdef _WIN32
# include <windows.h>
# pragma warning(disable:4996)
#endif // ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

class R2Point;
class R2Vector;
class R2Line;
class R2Segment;

#include "R2Point.h"
#include "R2Vector.h"
#include "R2Line.h"
#include "R2Segment.h"
#include "R2Distance.h"

#endif // ifndef R2_INCLUDED
