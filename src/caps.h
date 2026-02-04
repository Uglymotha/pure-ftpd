#ifndef __CAPS_H__
#define __CAPS_H__ 1

#ifdef USE_CAPABILITIES
# ifdef HAVE_SYS_CAPABILITY_H
#  include <sys/capability.h>
# endif
#endif

void set_cap(const int cap);
void drop_cap(const int cap);
void drop_login_caps(void);
void drop_privsep_caps(void);
void set_accept_caps(void);
void set_initial_caps(void);
int get_startup_caps(void);

#endif
