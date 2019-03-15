#include "hook.h"

/***** Initialize org function *****/
typedef FILE *(*lib_fopen_type)(const char *, const char *);
lib_fopen_type lib_fopen = NULL;

typedef size_t(*lib_fwrite_type)(const void *, size_t, size_t, FILE *);
lib_fwrite_type lib_fwrite = NULL;

typedef size_t(*lib_fread_type)(void *restrict, size_t, size_t, FILE *restrict);
lib_fread_type lib_fread = NULL;

typedef int(*lib_fclose_type)(FILE *);
lib_fclose_type lib_fclose = NULL;

typedef int(*lib_access_type)(const char *, int);
lib_access_type lib_access = NULL;

typedef int(*lib_unlink_type)(const char *);
lib_unlink_type lib_unlink = NULL;

typedef int(*lib_remove_type)(const char *);
lib_remove_type lib_remove = NULL;

typedef int(*lib_rename_type)(const char *, const char *);
lib_rename_type lib_rename = NULL;

typedef mode_t(*lib_umask_type)(mode_t);
lib_umask_type lib_umask = NULL;

typedef pid_t(*lib_getpid_type)();
lib_getpid_type lib_getpid = NULL;

typedef pid_t(*lib_fork_type)();
lib_fork_type lib_fork = NULL;

typedef int(*lib_system_type)(const char *);
lib_system_type lib_system = NULL;

typedef int(*lib_socket_type)(int, int, int);
lib_socket_type lib_socket = NULL;

typedef int(*lib_connect_type)(int, const struct sockaddr *, socklen_t);
lib_connect_type lib_connect = NULL;

typedef int(*lib_send_type)(int, const void *, size_t, int);
lib_send_type lib_send = NULL;

typedef int(*lib_recv_type)(int, void *, size_t, int);
lib_recv_type lib_recv = NULL;

typedef int(*lib_bind_type)(int, const struct sockaddr *, socklen_t);
lib_bind_type lib_bind = NULL;

typedef int(*lib_listen_type)(int, int);
lib_listen_type lib_listen = NULL;

typedef int(*lib_pthread_create_type)(pthread_t *restrict, const pthread_attr_t *restrict, void *(*start_routine)(void *), void *);
lib_pthread_create_type lib_pthread_create = NULL;

typedef int(*lib_kill_type)(int, int);
lib_kill_type lib_kill = NULL;

typedef void (*sighandler_t)(int);
typedef sighandler_t(*lib_signal_type)(int, sighandler_t);
lib_signal_type lib_signal = NULL;

static void con() __attribute__((constructor));
void con()
{
    lib_fopen = (lib_fopen_type)dlsym(RTLD_NEXT, "fopen");
    lib_fwrite = (lib_fwrite_type)dlsym(RTLD_NEXT, "fwrite");
    lib_fread = (lib_fread_type)dlsym(RTLD_NEXT, "fread");
    lib_fclose = (lib_fclose_type)dlsym(RTLD_NEXT, "fclose");
    lib_access = (lib_access_type)dlsym(RTLD_NEXT, "access");
    lib_unlink = (lib_unlink_type)dlsym(RTLD_NEXT, "unlink");
    lib_remove = (lib_remove_type)dlsym(RTLD_NEXT, "remove");
    lib_rename = (lib_rename_type)dlsym(RTLD_NEXT, "rename");
    lib_umask = (lib_umask_type)dlsym(RTLD_NEXT, "umask");
    lib_getpid = (lib_getpid_type)dlsym(RTLD_NEXT, "getpid");
    lib_fork = (lib_fork_type)dlsym(RTLD_NEXT, "fork");
    lib_system = (lib_system_type)dlsym(RTLD_NEXT, "system");
    lib_socket = (lib_socket_type)dlsym(RTLD_NEXT, "socket");
    lib_connect = (lib_connect_type)dlsym(RTLD_NEXT, "connect");
    lib_send = (lib_send_type)dlsym(RTLD_NEXT, "send");
    lib_recv = (lib_recv_type)dlsym(RTLD_NEXT, "recv");
    lib_bind = (lib_bind_type)dlsym(RTLD_NEXT, "bind");
    lib_listen = (lib_listen_type)dlsym(RTLD_NEXT, "listen");
    lib_pthread_create = (lib_pthread_create_type)dlsym(RTLD_NEXT, "pthread_create");
    lib_kill = (lib_kill_type)dlsym(RTLD_NEXT, "kill");
    lib_signal = (lib_signal_type)dlsym(RTLD_NEXT, "signal")
}
/************************************/

void HOOK_MAKE_LOG_STRING(char *buffer, const char *func_name, const unsigned int arg_count, const Variable *args)
{
    char temp[1024] = {0,};
    snprintf(buffer, 1024, "%s(", func_name);

    for (int i = 0; i < arg_count && args != NULL; i++)
    {
        switch(args[i]._type)
        {
        case VT_int:
        {
            snprintf(temp, 1024, "%d", (int)(args[i]._var));
            break;
        }
        case VT_unsigned_int:
        {
            snprintf(temp, 1024, "%u", (unsigned int)(args[i]._var));
            break;
        }
        case VT_string:
        {
            snprintf(temp, 1024, "\"%s\"", (char *)(args[i]._var));
            break;
        }
        case VT_offset:
        {
            snprintf(temp, 1024, "0x%08x", (unsigned int)(args[i]._var));
            break;
        }
        default:
        {
            snprintf(temp, 1024, "[UNKNOWN_TYPE_ERROR]");
            break;
        }
        }
        strcat(buffer, temp);
        if((i+1) != arg_count)
            strcat(buffer, ", ");
    }
    strcat(buffer, ")\n");
}

void HOOK_LOG(const LoggingType loggingType, const char *func_name, const unsigned int arg_count, const Variable *args)
{
    char buffer[1024] = {0, };
    HOOK_MAKE_LOG_STRING(buffer, func_name, arg_count, args);
    switch(loggingType)
    {
        case LT_STDOUT:
        {
            printf("%s", buffer);
            break;
        }
        case LT_FILE:
        {
            char filename[256] = {0, };
            snprintf(filename, sizeof(filename), "pid_%d.log", lib_getpid());
            FILE *hFile = lib_fopen(filename, "a");
            if(hFile != NULL)
            {
                lib_fwrite(buffer, sizeof(char), strlen(buffer), hFile);
                lib_fclose(hFile);    
            }
            break;
        }
    }
}

 /* ========================================================== */

FILE *fopen(const char *filename, const char *mode)
{
    Variable args[2] = { 
        { VT_string, (long long int)filename },
        { VT_string, (long long int)mode }
    };
    HOOK_LOG(LT_FILE, "fopen", 2, args);
    FILE *ret = lib_fopen(filename, mode);
    return ret;
}

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream)
{
    Variable args[4] = { 
        { VT_string, (long long int)buffer },
        { VT_unsigned_int, (long long int)size },
        { VT_unsigned_int, (long long int)count },
        { VT_offset, (long long int)stream }
    };
    HOOK_LOG(LT_FILE, "fwrite", 4, args);
    size_t ret = lib_fwrite(buffer, size, count, stream);
    return ret;
}

size_t fread(void *restrict buffer, size_t size, size_t count, FILE *restrict stream)
{
    Variable args[4] = { 
        { VT_string, (long long int)buffer },
        { VT_unsigned_int, (long long int)size },
        { VT_unsigned_int, (long long int)count },
        { VT_offset, (long long int)stream }
    };
    HOOK_LOG(LT_FILE, "fread", 4, args);
    size_t ret = lib_fread(buffer, size, count, stream);
    return ret;
}

int fclose(FILE *stream)
{
    Variable args[1] = { 
        { VT_offset, (long long int)stream }
    };
    HOOK_LOG(LT_FILE, "fclose", 1, args);
    int ret = lib_fclose(stream);
    return ret; 
}

int access(const char *pathname, int mode)
{
    Variable args[2] = { 
        { VT_string, (long long int)pathname },
        { VT_unsigned_int, (long long int)mode }
    };
    HOOK_LOG(LT_FILE, "access", 2, args);
    int ret = lib_access(pathname, mode);
    return ret; 
}

int unlink(const char *filename)
{
    Variable args[1] = { 
        { VT_string, (long long int)filename }
    };
    HOOK_LOG(LT_FILE, "unlink", 1, args);
    int ret = lib_unlink(filename);
    return ret;
}

int remove(const char *filename)
{
    Variable args[1] = { 
        { VT_string, (long long int)filename }
    };
    HOOK_LOG(LT_FILE, "remove", 1, args);
    int ret = lib_remove(filename);
    return ret;
}

int rename(const char *oldname, const char *newname)
{
    Variable args[2] = { 
        { VT_string, (long long int)oldname },
        { VT_string, (long long int)newname }
    };
    HOOK_LOG(LT_FILE, "remove", 2, args);
    int ret = lib_rename(oldname, newname);
    return ret;
}

mode_t umask(mode_t mode)
{
    Variable args[1] = { 
        { VT_unsigned_int, (long long int)mode }
    };
    HOOK_LOG(LT_FILE, "umask", 1, args);
    mode_t ret = lib_umask(mode);
    return ret;
}

pid_t getpid()
{
    HOOK_LOG(LT_FILE, "getpid", 0, NULL);
    pid_t ret = lib_getpid();
    return ret;
}

pid_t fork()
{
    HOOK_LOG(LT_FILE, "fork", 0, NULL);
    pid_t ret = lib_fork();
    return ret;
}

int system(const char *command)
{
    Variable args[1] = { 
        { VT_string, (long long int)command }
    };
    HOOK_LOG(LT_FILE, "system", 1, args);
    int ret = lib_system(command);
    return ret;
}

int socket(int domain, int type, int protocol)
{
    Variable args[3] = { 
        { VT_int, (long long int)domain },
        { VT_int, (long long int)type },
        { VT_int, (long long int)protocol }
    };
    HOOK_LOG(LT_FILE, "socket", 3, args);
    int ret = lib_socket(domain, type, protocol);
    return ret;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    Variable args[3] = { 
        { VT_int, (long long int)sockfd },
        { VT_offset, (long long int)addr },
        { VT_unsigned_int, (long long int)addrlen }
    };
    HOOK_LOG(LT_FILE, "connect", 3, args);
    int ret = lib_connect(sockfd, addr, addrlen);
    return ret;
}

int send(int s, const void *msg, size_t len, int flags)
{
    Variable args[4] = { 
        { VT_int, (long long int)s },
        { VT_offset, (long long int)msg },
        { VT_unsigned_int, (long long int)len },
        { VT_int, (long long int)flags }
    };
    HOOK_LOG(LT_FILE, "send", 4, args);
    int ret = lib_send(s, msg, len, flags);
    return ret;
}

int recv(int s, void *buf, size_t len, int flags)
{
    Variable args[4] = { 
        { VT_int, (long long int)s },
        { VT_offset, (long long int)buf },
        { VT_unsigned_int, (long long int)len },
        { VT_int, (long long int)flags }
    };
    HOOK_LOG(LT_FILE, "recv", 4, args);
    int ret = lib_recv(s, buf, len, flags);
    return ret;
}

int bind(int socket, const struct sockaddr *address, socklen_t addresse_len)
{
    Variable args[3] = { 
        { VT_int, (long long int)s },
        { VT_offset, (long long int)buf },
        { VT_unsigned_int, (long long int)len }
    };
    HOOK_LOG(LT_FILE, "bind", 3, args);
    int ret = lib_bind(socket, address, address_len);
    return ret;
}

int listen(int socket, int backlog)
{
    Variable args[2] = { 
        { VT_int, (long long int)socket },
        { VT_int, (long long int)backlog }
    };
    HOOK_LOG(LT_FILE, "listen", 2, args);
    int ret = lib_bind(socket, backlog);
    return ret;
}

int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrictarg)
{
    Variable args[4] = { 
        { VT_offset, (long long int)thread },
        { VT_offset, (long long int)attr },
        { VT_offset, (long long int)start_routine },
        { VT_offset, (long long int)restrictarg }
    };
    HOOK_LOG(LT_FILE, "pthread_create", 4, args);
    int ret = lib_pthread_create(thread, attr, start_routine, restrictarg);
    return ret;
}

sighandler_t signal(int signum, sighandler_t handler)
{
    Variable args[2] = { 
        { VT_unsigned_int, (long long int)signum },
        { VT_offset, (long long int)handler }
    };
    HOOK_LOG(LT_FILE, "signal", 2, args);
    int ret = lib_signal(signum, handler);
    return ret;
}