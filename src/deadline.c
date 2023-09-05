/**
 *  Copyright (C) 2021 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */
#include "lua_clock.h"

#define MODULE_MT "time.clock.deadline"

typedef struct {
    int done;
    struct timespec deadline;
} clock_deadline_t;

static int remain_lua(lua_State *L)
{
    clock_deadline_t *d = luaL_checkudata(L, 1, MODULE_MT);
    double delta        = 0;

    if (!d->done) {
        struct timespec now = {0};
        clock_gettime(CLOCK_MONOTONIC, &now);
        if (now.tv_sec > d->deadline.tv_sec ||
            (now.tv_sec == d->deadline.tv_sec &&
             now.tv_nsec >= d->deadline.tv_nsec)) {
            d->done = 1;
        } else {
            delta = (double)(d->deadline.tv_sec - now.tv_sec) +
                    (double)(d->deadline.tv_nsec - now.tv_nsec) / NSEC_F;
        }
    }

    lua_pushnumber(L, delta);
    return 1;
}

static int tostring_lua(lua_State *L)
{
    lua_pushfstring(L, "%s: %p", MODULE_MT, lua_touserdata(L, 1));
    return 1;
}

static int new_lua(lua_State *L)
{
    lua_Number fsec     = luaL_checknumber(L, 1);
    clock_deadline_t *d = lua_newuserdata(L, sizeof(clock_deadline_t));

    *d = (clock_deadline_t){0};
    clock_gettime(CLOCK_MONOTONIC, &d->deadline);
    d->done = fsec < 0;

    if (!d->done) {
        long sec  = (long)fsec;
        long nsec = (long)((fsec - sec) * NSEC);

        d->deadline.tv_sec += sec;
        d->deadline.tv_nsec += nsec;
        // adjust deadline if nsec is overflowed to sec
        if ((uint64_t)d->deadline.tv_nsec >= NSEC) {
            d->deadline.tv_sec += 1;
            d->deadline.tv_nsec -= NSEC;
        }
    }

    lauxh_setmetatable(L, MODULE_MT);
    return 1;
}

LUALIB_API int luaopen_time_clock_deadline(lua_State *L)
{
    // create new metatable of tname already exists
    if (luaL_newmetatable(L, MODULE_MT)) {
        struct luaL_Reg mmethods[] = {
            {"__tostring", tostring_lua},
            {NULL,         NULL        }
        };
        struct luaL_Reg methods[] = {
            {"remain", remain_lua},
            {NULL,     NULL      }
        };

        // push metamethods
        for (struct luaL_Reg *ptr = mmethods; ptr->name; ptr++) {
            lauxh_pushfn2tbl(L, ptr->name, ptr->func);
        }
        // push methods
        lua_createtable(L, 0, 1);
        for (struct luaL_Reg *ptr = methods; ptr->name; ptr++) {
            lauxh_pushfn2tbl(L, ptr->name, ptr->func);
        }
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
    }

    // create module table
    lua_createtable(L, 0, 1);
    lauxh_pushfn2tbl(L, "new", new_lua);
    return 1;
}
