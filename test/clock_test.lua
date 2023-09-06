local assert = require('assert')
local errno = require('errno')
local sleep = require('time.sleep')
local clock = require('time.clock')
local new_deadline = require('time.clock.deadline').new

local AVAILABLE_CLOCK_IDS = {}
for _, cid in ipairs({
    'CLOCK_REALTIME',
    'CLOCK_REALTIME_COARSE',
    'CLOCK_REALTIME_FAST',
    'CLOCK_REALTIME_PRECISE',
    'CLOCK_MONOTONIC',
    'CLOCK_MONOTONIC_RAW',
    'CLOCK_MONOTONIC_RAW_APPROX',
    'CLOCK_MONOTONIC_COARSE',
    'CLOCK_MONOTONIC_FAST',
    'CLOCK_MONOTONIC_PRECISE',
    'CLOCK_UPTIME_RAW',
    'CLOCK_UPTIME_RAW_APPROX',
    'CLOCK_UPTIME_FAST',
    'CLOCK_UPTIME_PRECISE',
    'CLOCK_PROCESS_CPUTIME_ID',
    'CLOCK_THREAD_CPUTIME_ID',
    'CLOCK_BOOTTIME',
    'CLOCK_VIRTUAL',
    'CLOCK_PROF',
    'CLOCK_SECOND',
}) do
    AVAILABLE_CLOCK_IDS[cid] = clock[cid]
end

local function test_getres()
    for _, id in pairs(AVAILABLE_CLOCK_IDS) do
        -- test that get resolution of clock
        local res, err = assert(clock.getres(id))
        assert.is_finite(res)
        assert.is_nil(err)
        local iv, fv = math.modf(res)
        assert.greater(fv, 0)
        assert.equal(res, iv + fv)

        -- test test that get resolution of clock as integer
        res, err = assert(clock.getres(id, true))
        assert.is_uint(res)
        assert.is_nil(err)
    end

    -- test that get resolution of clock with default clock_id=MONOTONIC
    local res, err = assert(clock.getres())
    assert.is_finite(res)
    assert.is_nil(err)

    -- test that EINVAL error
    res, err = clock.getres(-123456789)
    assert.is_nil(res)
    assert.equal(err.type, errno.EINVAL)
end

local function test_gettime()
    for _, id in pairs(AVAILABLE_CLOCK_IDS) do
        -- test that get time as number
        local res, err = assert(clock.gettime(id))
        assert.is_finite(res)
        assert.is_nil(err)
        local iv, fv = math.modf(res)
        assert.greater(fv, 0)
        assert.equal(res, iv + fv)
    end

    -- test that get resolution of clock with default clock_id=MONOTONIC
    local res, err = clock.gettime()
    assert.is_finite(res)
    assert.is_nil(err)

    -- test that EINVAL error
    res, err = clock.gettime(-123456789)
    assert.is_nil(res)
    assert.equal(err.type, errno.EINVAL)
end

local function test_getmsec()
    for _, id in pairs(AVAILABLE_CLOCK_IDS) do
        -- test that get time as msec
        local res, err = assert(clock.getmsec(id))
        assert.is_uint(res)
        assert.is_nil(err)
    end
end

local function test_getusec()
    for _, id in pairs(AVAILABLE_CLOCK_IDS) do
        -- test that get time as usec
        local res, err = assert(clock.getusec(id))
        assert.is_uint(res)
        assert.is_nil(err)
    end
end

local function test_getnsec()
    for _, id in pairs(AVAILABLE_CLOCK_IDS) do
        -- test that get time as nsec
        local res, err = assert(clock.getnsec(id))
        assert.is_uint(res)
        assert.is_nil(err)
    end
end

local function test_deadline()
    -- test that create time.clock.deadline instance
    local deadl, t = assert(new_deadline(1.9))
    assert.match(deadl, '^time.clock.deadline: 0x%x+$', false)
    assert.is_finite(t)

    -- test that deadline:remain() returns a remain duration of deadline
    local remain = assert(deadl:remain())
    assert.greater(remain, 1.8)

    -- test that deadline:remain() is greater than 0.1 after sleep 1.3 sec
    sleep(1.7)
    remain = assert(deadl:remain())
    assert.greater(remain, 0.1)

    -- test that deadline:remain() returns 0 after sleep 0.2 sec
    sleep(0.2)
    remain = assert(deadl:remain())
    assert.equal(remain, 0)
end

for name, f in pairs({
    test_getres = test_getres,
    test_gettime = test_gettime,
    test_getmsec = test_getmsec,
    test_getusec = test_getusec,
    test_getnsec = test_getnsec,
    test_deadline = test_deadline,
}) do
    local ok, err = xpcall(f, debug.traceback)
    if ok then
        print(('%s: ok'):format(name))
    else
        print(('%s: failed'):format(name))
        print(err)
    end
end
