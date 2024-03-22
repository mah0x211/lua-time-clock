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
- `err:any`: error object.


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
- `err:any`: error object.


## res, err = clock.getmsec( [clock_id] )

get the time of the specified clock `clock_id` as milliseconds.

```lua
local clock = require('time.clock')
local nsec = clock.getmsec()
print(nsec) -- 1531570802
```

**Parameters**

- `clock_id:integer`: value of clock_id constants. (default `CLOCK_MONOTONIC`)

**Returns**

- `res:integer`: the time of the clock specified by `clock_id` as milliseconds.
- `err:any`: error object.


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
- `err:any`: error object.


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
- `err:any`: error object.


## deadl, sec = deadline.new( sec )

create a deadline object.

```lua
local sleep = require('time.sleep')
local deadline = require('time.clock.deadline').new

local deadl, sec = new_deadline(1.5)
print(deadl) -- time.clock.deadline: 0x600001348088
print(sec)
-- get the remaining time of the deadline
sec = d:remain()
print(sec) -- 1.499998
sleep(1.2)
sec = deadl:remain()
print(sec) -- 0.294888
sleep(0.3)
sec = deadl:remain()
print(sec) -- 0.0
```


**Parameters**

- `sec:number`: the duration of the deadline in seconds. `<=0` means the deadline has already expired.

**Returns**

- `deadline:time.clock.deadline`: deadline object.
- `sec:number`: the time of the deadline in seconds.


## sec = deadline:time()

get the time of the deadline in seconds.

**Returns**

- `sec:number`: the time of the deadline in seconds.


## sec = deadline:remain()

get the remaining time of the deadline.

**Returns**

- `sec:number`: the remaining time until the deadline　in seconds.

