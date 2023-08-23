# lua-time-clock

[![test](https://github.com/mah0x211/lua-time-clock/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-time-clock/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-time-clock/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-time-clock)

clock and time functions.


## Installation

```
luarocks install time-clock
```

## Constants

The following `clock_id` values are exported, but values that do not exist on the platform will be `nil`.

- `CLOCK_REALTIME`
- `CLOCK_REALTIME_COARSE`
- `CLOCK_REALTIME_FAST`
- `CLOCK_REALTIME_PRECISE`
- `CLOCK_MONOTONIC`
- `CLOCK_MONOTONIC_RAW`
- `CLOCK_MONOTONIC_RAW_APPROX`
- `CLOCK_MONOTONIC_COARSE`
- `CLOCK_MONOTONIC_FAST`
- `CLOCK_MONOTONIC_PRECISE`
- `CLOCK_UPTIME_RAW`
- `CLOCK_UPTIME_RAW_APPROX`
- `CLOCK_UPTIME_FAST`
- `CLOCK_UPTIME_PRECISE`
- `CLOCK_PROCESS_CPUTIME_ID`
- `CLOCK_THREAD_CPUTIME_ID`
- `CLOCK_BOOTTIME`
- `CLOCK_VIRTUAL`
- `CLOCK_PROF`
- `CLOCK_SECOND`


## Error Handling

the following functions return the `error` object created by https://github.com/mah0x211/lua-errno module.


## res, err = clock.getres( [clock_id [, as_int]] )

get the resolution of the specified clock `clock_id`.

```lua
local clock = require('time.clock')
local res = clock.getres()
print(string.format('%.9f',res)) -- 0.000001000
```

**Parameters**

- `clock_id:integer`: value of clock_id constants. (default `CLOCK_MONOTONIC`)
- `as_int:boolean`: get the value as an integer.

**Returns**

- `res:number|integer`: the resolution of a clock.
- `err:error`: error object.


## res, err = clock.gettime( [clock_id] )

get the time of the specified clock `clock_id`.

```lua
local clock = require('time.clock')
local sec = clock.gettime()
print(sec) -- 1531570.802726
```

**Parameters**

- `clock_id:integer`: value of clock_id constants. (default `CLOCK_MONOTONIC`)

**Returns**

- `res:number`: the time of the clock specified by `clock_id` as seconds.
- `err:error`: error object.


## res, err = clock.getusec( [clock_id] )

get the time of the specified clock `clock_id` as microseconds.

```lua
local clock = require('time.clock')
local nsec = clock.getusec()
print(nsec) -- 1531570802726
```

**Parameters**

- `clock_id:integer`: value of clock_id constants. (default `CLOCK_MONOTONIC`)

**Returns**

- `res:integer`: the time of the clock specified by `clock_id` as microseconds.
- `err:error`: error object.


## res, err = clock.getnsec( [clock_id] )

get the time of the specified clock `clock_id` as nanoseconds.

```lua
local clock = require('time.clock')
local nsec = clock.getnsec()
print(nsec) -- 1531570802726000
```

**Parameters**

- `clock_id:integer`: value of clock_id constants. (default `CLOCK_MONOTONIC`)

**Returns**

- `res:integer`: the time of the clock specified by `clock_id` as nanoseconds.
- `err:error`: error object.

