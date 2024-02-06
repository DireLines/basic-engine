package main

import "base:runtime"
import "core:fmt"
import "core:time"

timing_logs :: #config(timing_logs, false)

Timer :: struct {
    loc:   runtime.Source_Code_Location,
    start: time.Tick,
    time:  proc(state: ^Timer, msg: string),
}
timer :: proc(loc := #caller_location) -> Timer {
    return Timer{loc = loc, start = time.tick_now(), time = proc(state: ^Timer, msg: string) {
                when timing_logs {
                    elapsed := time.tick_since(state.start)
                    prefix := state.loc.procedure
                    fmt.printf(
                        "%v: %v took %v micros\n",
                        prefix,
                        msg,
                        int(time.duration_microseconds(elapsed)),
                    )
                    state.start = time.tick_now()
                }
            }}
}
