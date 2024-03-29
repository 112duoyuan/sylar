#include <filter.h>
#include <dbg.h>
#include <tnetstrings.h>
#include <unused.h>

StateEvent filter_transition(UNUSED StateEvent state, UNUSED Connection *conn, tns_value_t *config)
{
    size_t len = 0;
    char *data = tns_render(config, &len);

    if(data != NULL) {
        log_info("CONFIG: %.*s", (int)len, data);
    }

    free(data);

    return CLOSE;
}


StateEvent *filter_init(UNUSED Server *srv, UNUSED bstring load_path, int *out_nstates)
{
    StateEvent states[] = {HANDLER, PROXY};
    *out_nstates = Filter_states_length(states);
    check(*out_nstates == 2, "Wrong state array length.");

    return Filter_state_list(states, *out_nstates);

error:
    return NULL;
}

