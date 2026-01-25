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

static void apply_caps(cap_value_t * const capsp, const size_t ncapsp,
                       cap_value_t * const capse, const size_t ncapse,
                       cap_t caps, cap_flag_value_t flag)
{
    if (have_caps == 0) {
        return;
    }
    if ((caps == (cap_t) 0 &&
         ((caps = cap_init()) == (cap_t) 0 || cap_clear(caps) != 0)) ||
        (ncapsp > 0 &&
         cap_set_flag(caps, CAP_PERMITTED, ncapsp, capsp, flag) != 0) ||
        (ncapse > 0 &&
         cap_set_flag(caps, CAP_EFFECTIVE, ncapse, capse, flag) != 0) ||
        cap_set_proc(caps) != 0 || cap_free(caps) != 0) {
        die(421, LOG_ERR, MSG_CAPABILITIES " : %s" , strerror(errno));
    }
}

void set_cap(const int cap)
{
    cap_value_t capv = cap;
    apply_caps(cap_none, 0, &capv, 1, cap_get_proc(), CAP_SET);
}

void drop_cap(const int cap)
{
    cap_value_t capv = cap;
    apply_caps(cap_none, 0, &capv, 1, cap_get_proc(), CAP_CLEAR);
}

void drop_login_caps(void)
{
    apply_caps(cap_keep_login, CAP_LOGIN_SZ, cap_keep_login, CAP_LOGIN_SZ,
               (cap_t) 0, CAP_SET);
}

void drop_privsep_caps(void)
{
    if (root_started != 0) {
        apply_caps(cap_keep_privsep_root, CAP_PRIVSEPR_SZ, cap_none, 0,
                   (cap_t) 0, CAP_SET);
    } else {
        apply_caps(cap_keep_privsep_nonroot, CAP_PRIVSEPNR_SZ, cap_none ,0,
                   (cap_t) 0, CAP_SET);
    }
}

void set_accept_caps(void)
{
    apply_caps(cap_keep_startup, CAP_STARTUP_SZ,
               cap_keep_startup, CAP_STARTUP_SZ, (cap_t) 0, CAP_SET);
}

void set_initial_caps(void)
{
    cap_value_t cap = CAP_NET_BIND_SERVICE;
    apply_caps(cap_keep_startup, CAP_STARTUP_SZ, &cap, 1, (cap_t) 0, CAP_SET);
}

int get_initial_caps(void)
{
    cap_t caps = cap_get_proc();
    if (caps == NULL) {
        return 0;
    }
    cap_free(caps);
    return 1;
}

#else
int get_initial_caps(void) {
    return 0;
}
extern signed char v6ready;
#endif
