#ifndef __UPLOAD_PIPE_H__
#define __UPLOAD_PIPE_H__ 1

#ifndef UPLOAD_PIPE_FILE
# ifdef NON_ROOT_FTP
#  define UPLOAD_PIPE_FILE CONFDIR "/pure-ftpd.upload.pipe"
# elif defined USE_SYSTEMD
#  define UPLOAD_PIPE_FILE systemd_init ? "/tmp/upload.pipe" : STATEDIR "/run/pure-ftpd.upload.pipe"
# else
#  define UPLOAD_PIPE_FILE STATEDIR "/run/pure-ftpd.upload.pipe"
# endif
#endif

#ifndef UPLOAD_PIPE_LOCK
# ifdef NON_ROOT_FTP
#  define UPLOAD_PIPE_LOCK CONFDIR "/pure-ftpd.upload.lock"
# elif defined USE_SYSTEMD
#  define UPLOAD_PIPE_LOCK systemd_init ? "/tmp/upload.lock" : STATEDIR "/run/pure-ftpd.upload.lock"
# else
#  define UPLOAD_PIPE_LOCK STATEDIR "/run/pure-ftpd.upload.lock"
# endif
#endif

int upload_pipe_open(void);
int upload_pipe_push(const char *vuser, const char *file);
void upload_pipe_close(void);

#endif
