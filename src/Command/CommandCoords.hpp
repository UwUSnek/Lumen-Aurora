#pragma once



namespace cmd {
    struct CommandCoords {
        ulong start;
        ulong end;

        CommandCoords(ulong _start, ulong _end) :
            start(_start),
            end(_end) {
        }
    };
}