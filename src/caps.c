#include <config.h>

#ifdef USE_CAPABILITIES

/* sys/capability.h and sys/vfs.h both define struct statfs */
#define STATFS_ALREADY_DEFINED 1

#include "ftpd.h"
#include "messages.h"
#include "caps_p.h"

#ifdef WITH_DMALLOC
# include <dmalloc.h>
#endif

static void apply_caps(cap_value_t * const ncaps,
                       const size_t ncaps_size)
{
    cap_t caps;

    if (have_caps == 0) {
        return;
    }
    if ((caps = cap_init()) == (cap_t) 0 ||
        cap_clear(caps) == -1 ||
        (ncaps_size > 0 &&
         (cap_set_flag(caps, CAP_PERMITTED, ncaps_size, ncaps, CAP_SET) == -1 ||
          cap_set_flag(caps, CAP_EFFECTIVE, ncaps_size, ncaps, CAP_SET) == -1)) ||
        cap_set_proc(caps) == -1 ||
        cap_free(caps) == -1) {
        die(421, LOG_ERR, MSG_CAPABILITIES " : %s" , strerror(errno));
    }
}

void drop_login_caps(void)
{
    apply_caps(cap_keep_login,
               sizeof(cap_keep_login) / sizeof(cap_value_t));
}

void drop_privsep_caps(void)
{
    if (root_started != 0) {
        apply_caps(cap_keep_privsep_root,
                   sizeof(cap_keep_privsep_root) / sizeof(cap_value_t));
    } else {
        apply_caps(cap_keep_privsep_nonroot,
                   sizeof(cap_keep_privsep_nonroot) / sizeof(cap_value_t));
    }
}

void set_initial_caps(void)
{
    apply_caps(cap_keep_startup,
               sizeof(cap_keep_startup) / sizeof(cap_value_t));
}

int get_initial_caps(void)
{
    return (cap_get_proc() != (cap_t) 0);
}

#else
int get_initial_caps(void) {
    return 0;
}
extern signed char v6ready;
#endif
