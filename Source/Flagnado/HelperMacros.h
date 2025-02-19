#pragma once

#define FLAGNADO_RETURN_IF(Condition) \
    if (Condition) {                  \
        return;                       \
    }
