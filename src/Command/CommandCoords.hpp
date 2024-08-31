#pragma once



namespace cmd {
    struct ElmCoordsCL {
        ulong start;
        ulong end;

        ElmCoordsCL(ulong _start, ulong _end) :
            start(_start),
            end(_end) {
        }
    };
}