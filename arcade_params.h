#pragma once

/* \brief Structure passed to arcade for its initialization / execution
    injection of dependencies reduce the size/need of this structure
    could be included in arcade.h, really need to separate (other uses ?) */
typedef struct {
    bool play_anonymous;
} arcade_params_t;