#pragma once

#define FLAGNADO_RETURN_IF(Condition) \
    if (Condition) {                  \
        return;                       \
    }

#define FLAGNADO_RETURN_VALUE_IF(Condition, ReturnValue) \
    if (Condition) {                                     \
        return ReturnValue;                              \
    }

#define FLAGNADO_CONTINUE_IF(Condition) \
    if (Condition) {                    \
        continue;                       \
    }

#define FLAGNADO_LOG_AND_RETURN_IF(Condition, CategoryName, Verbosity, Format, ...) \
    if (Condition) {                                                                \
        UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__);                     \
        return;                                                                     \
    }

#define FLAGNADO_LOG_AND_RETURN_VALUE_IF(Condition, ReturnValue, CategoryName, Verbosity, Format, \
                                         ...)                                                     \
    if (Condition) {                                                                              \
        UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__);                                   \
        return ReturnValue;                                                                       \
    }
