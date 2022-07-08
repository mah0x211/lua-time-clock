local assert = require('assert')
local errno = require('errno')
local clock = require('clock')

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

        -- test that get time as integer
        res, err = assert(clock.gettime(id, true))
        assert.is_uint(res)
        assert.is_nil(err)
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

for name, f in pairs({
    test_getres = test_getres,
    test_gettime = test_gettime,
}) do
    local ok, err = xpcall(f, debug.traceback)
    if ok then
        print(('%s: ok'):format(name))
    else
        print(('%s: failed'):format(name))
        print(err)
    end
end
