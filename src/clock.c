/**
 * Copyright (C) 2021 Masatoshi Fukunaga
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "lua_clock.h"

typedef enum {
    TIME_CLOCK_NSEC = 0,
    TIME_CLOCK_USEC,
    TIME_CLOCK_MSEC,
    TIME_CLOCK_SEC,
} time_clock_unit_t;

static int gettime_as(lua_State *L, time_clock_unit_t unit)
{
    clockid_t clk_id   = lauxh_optinteger(L, 1, CLOCK_MONOTONIC);
    struct timespec ts = {0};

    if (clock_gettime(clk_id, &ts)) {
        lua_pushnil(L);
        lua_errno_new(L, errno, "clock.getres");
        return 2;
    }

    switch (unit) {
    case TIME_CLOCK_NSEC:
        lua_pushinteger(L, (uint64_t)ts.tv_sec * NSEC + (uint64_t)ts.tv_nsec);
        return 1;

    case TIME_CLOCK_USEC:
        lua_pushinteger(L, (uint64_t)ts.tv_sec * USEC +
                               (uint64_t)ts.tv_nsec / MSEC);
        return 1;

    case TIME_CLOCK_MSEC:
        lua_pushinteger(L, (uint64_t)ts.tv_sec * MSEC +
                               (uint64_t)ts.tv_nsec / USEC);
        return 1;

    // case TIME_CLOCK_SEC:
    default:
        lua_pushnumber(L, (double)ts.tv_sec + ((double)ts.tv_nsec / NSEC));
        return 1;
    }
}

static int getnsec_lua(lua_State *L)
{
    return gettime_as(L, TIME_CLOCK_NSEC);
}

static int getusec_lua(lua_State *L)
{
    return gettime_as(L, TIME_CLOCK_USEC);
}

static int getmsec_lua(lua_State *L)
{
    return gettime_as(L, TIME_CLOCK_MSEC);
}

static int gettime_lua(lua_State *L)
{
    return gettime_as(L, TIME_CLOCK_SEC);
}

static int getres_lua(lua_State *L)
{
    clockid_t clk_id   = lauxh_optinteger(L, 1, CLOCK_MONOTONIC);
    int as_int         = lauxh_optboolean(L, 2, 0);
    struct timespec ts = {0};

    if (clock_getres(clk_id, &ts)) {
        lua_pushnil(L);
        lua_errno_new(L, errno, "clock.getres");
        return 2;
    } else if (as_int) {
        lua_pushinteger(L, (uint64_t)ts.tv_sec * NSEC + (uint64_t)ts.tv_nsec);
    } else {
        lua_pushnumber(L, (double)ts.tv_sec + ((double)ts.tv_nsec / NSEC));
    }
    return 1;
}

LUALIB_API int luaopen_time_clock(lua_State *L)
{
    lua_errno_loadlib(L);

    // create module table
    lua_newtable(L);

    // export functions
    lauxh_pushfn2tbl(L, "getres", getres_lua);
    lauxh_pushfn2tbl(L, "gettime", gettime_lua);
    lauxh_pushfn2tbl(L, "getmsec", getmsec_lua);
    lauxh_pushfn2tbl(L, "getusec", getusec_lua);
    lauxh_pushfn2tbl(L, "getnsec", getnsec_lua);

    // export options
    // realtime
    lauxh_pushint2tbl(L, "CLOCK_REALTIME", CLOCK_REALTIME);

#if defined(CLOCK_REALTIME_COARSE)
    lauxh_pushint2tbl(L, "CLOCK_REALTIME_COARSE", CLOCK_REALTIME_COARSE);
#endif

#if defined(CLOCK_REALTIME_FAST)
    lauxh_pushint2tbl(L, "CLOCK_REALTIME_FAST", CLOCK_REALTIME_FAST);
#endif

#if defined(CLOCK_REALTIME_PRECISE)
    lauxh_pushint2tbl(L, "CLOCK_REALTIME_PRECISE", CLOCK_REALTIME_PRECISE);
#endif

    // monotonic
    lauxh_pushint2tbl(L, "CLOCK_MONOTONIC", CLOCK_MONOTONIC);

#if defined(CLOCK_MONOTONIC_RAW)
    lauxh_pushint2tbl(L, "CLOCK_MONOTONIC_RAW", CLOCK_MONOTONIC_RAW);
#endif

#if defined(CLOCK_MONOTONIC_RAW_APPROX)
    lauxh_pushint2tbl(L, "CLOCK_MONOTONIC_RAW_APPROX",
                      CLOCK_MONOTONIC_RAW_APPROX);
#endif

#if defined(CLOCK_MONOTONIC_COARSE)
    lauxh_pushint2tbl(L, "CLOCK_MONOTONIC_COARSE", CLOCK_MONOTONIC_COARSE);
#endif

#if defined(CLOCK_MONOTONIC_FAST)
    lauxh_pushint2tbl(L, "CLOCK_MONOTONIC_FAST", CLOCK_MONOTONIC_FAST);
#endif

#if defined(CLOCK_MONOTONIC_PRECISE)
    lauxh_pushint2tbl(L, "CLOCK_MONOTONIC_PRECISE", CLOCK_MONOTONIC_PRECISE);
#endif

    // uptime
#if defined(CLOCK_UPTIME_RAW)
    lauxh_pushint2tbl(L, "CLOCK_UPTIME_RAW", CLOCK_UPTIME_RAW);
#endif

#if defined(CLOCK_UPTIME_RAW_APPROX)
    lauxh_pushint2tbl(L, "CLOCK_UPTIME_RAW_APPROX", CLOCK_UPTIME_RAW_APPROX);
#endif

#if defined(CLOCK_UPTIME_FAST)
    lauxh_pushint2tbl(L, "CLOCK_UPTIME_FAST", CLOCK_UPTIME_FAST);
#endif

#if defined(CLOCK_UPTIME_PRECISE)
    lauxh_pushint2tbl(L, "CLOCK_UPTIME_PRECISE", CLOCK_UPTIME_PRECISE);
#endif

    // cputime
#if defined(CLOCK_PROCESS_CPUTIME_ID)
    lauxh_pushint2tbl(L, "CLOCK_PROCESS_CPUTIME_ID", CLOCK_PROCESS_CPUTIME_ID);
#endif

#if defined(CLOCK_THREAD_CPUTIME_ID)
    lauxh_pushint2tbl(L, "CLOCK_THREAD_CPUTIME_ID", CLOCK_THREAD_CPUTIME_ID);
#endif

    // etc
#if defined(CLOCK_BOOTTIME)
    lauxh_pushint2tbl(L, "CLOCK_BOOTTIME", CLOCK_BOOTTIME);
#endif

#if defined(CLOCK_VIRTUAL)
    lauxh_pushint2tbl(L, "CLOCK_VIRTUAL", CLOCK_VIRTUAL);
#endif

#if defined(CLOCK_PROF)
    lauxh_pushint2tbl(L, "CLOCK_PROF", CLOCK_PROF);
#endif

#if defined(CLOCK_SECOND)
    lauxh_pushint2tbl(L, "CLOCK_SECOND", CLOCK_SECOND);
#endif

    return 1;
}
