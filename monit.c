/*
 * Description : Monitoring tool
 *               Monitor a folder or a file in order to check access.
 */
#include <sys/select.h>
#include <sys/inotify.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/vfs.h>    
#include <sys/vfs.h>    

#ifndef T
#define T fprintf(stdout, "%s l %d\n",__FUNCTION__, __LINE__);fflush(stdout);
#endif /*T*/

/*gcc inot.c  -o inot -O2 -Wall -W -Werror -ansi -pedantic*/

int fd, wd;

typedef struct 
{
    char *name;
    long type;
} fsTypeToName_t;

static fsTypeToName_t fstypetoname[] =
{
    { "ADFS_SUPER_MAGIC" ,      0xadf5     },
    { "AFFS_SUPER_MAGIC" ,      0xADFF     },
    { "BEFS_SUPER_MAGIC" ,      0x42465331 },
    { "BFS_MAGIC" ,             0x1BADFACE },
    { "CIFS_MAGIC_NUMBER" ,     0xFF534D42 },
    { "CODA_SUPER_MAGIC" ,      0x73757245 },
    { "COH_SUPER_MAGIC" ,       0x012FF7B7 },
    { "CRAMFS_MAGIC" ,          0x28cd3d45 },
    { "DEVFS_SUPER_MAGIC" ,     0x1373     },
    { "EFS_SUPER_MAGIC" ,       0x00414A53 },
    { "EXT_SUPER_MAGIC" ,       0x137D     },
    { "EXT2_OLD_SUPER_MAGIC" ,  0xEF51     },
    { "EXT2_SUPER_MAGIC" ,      0xEF53     },
    { "EXT3_SUPER_MAGIC" ,      0xEF53     },
    { "HFS_SUPER_MAGIC" ,       0x4244     },
    { "HPFS_SUPER_MAGIC" ,      0xF995E849 },
    { "HUGETLBFS_MAGIC" ,       0x958458f6 },
    { "ISOFS_SUPER_MAGIC" ,     0x9660     },
    { "JFFS2_SUPER_MAGIC" ,     0x72b6     },
    { "JFS_SUPER_MAGIC" ,       0x3153464a },
    { "MINIX_SUPER_MAGIC" ,     0x137F     },/* orig. minix */
    { "MINIX_SUPER_MAGIC2" ,    0x138F     },/* 30 char minix */
    { "MINIX2_SUPER_MAGIC" ,    0x2468     },/* minix V2 */
    { "MINIX2_SUPER_MAGIC2" ,   0x2478     },/* minix V2, 30 char names */
    { "MSDOS_SUPER_MAGIC" ,     0x4d44     },
    { "NCP_SUPER_MAGIC" ,       0x564c     },
    { "NFS_SUPER_MAGIC" ,       0x6969     },
    { "NTFS_SB_MAGIC" ,         0x5346544e },
    { "OPENPROM_SUPER_MAGIC" ,  0x9fa1     },
    { "PROC_SUPER_MAGIC" ,      0x9fa0     },
    { "QNX4_SUPER_MAGIC" ,      0x002f     },
    { "REISERFS_SUPER_MAGIC" ,  0x52654973 },
    { "ROMFS_MAGIC " ,          0x7275     },
    { "SMB_SUPER_MAGIC" ,       0x517B     },
    { "SYSV2_SUPER_MAGIC" ,     0x012FF7B6 },
    { "SYSV4_SUPER_MAGIC" ,     0x012FF7B5 },
    { "TMPFS_MAGIC  " ,         0x01021994 },
    { "UDF_SUPER_MAGIC" ,       0x15013346 },
    { "UFS_MAGIC " ,            0x00011954 },
    { "USBDEVICE_SUPER_MAGIC" , 0x9fa2     },
    { "VXFS_SUPER_MAGIC" ,      0xa501FCF5 },
    { "XENIX_SUPER_MAGIC" ,     0x012FF7B4 },
    { "XFS_SUPER_MAGIC" ,       0x58465342 }
};

void sigint_handler(int signum)
{
    (void) signum;

    /* Delete watching callback */
    if (inotify_rm_watch(fd, wd)) {
        perror("inotify_rm_watch");
        exit(EXIT_FAILURE);
    }

    /* closing inotify used fd */
    if (close(fd) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void printr_mask(int mask)
{
    printf("Event=");
    if (mask & IN_ACCESS)
        printf("IN_ACCESS");
    if (mask & IN_MODIFY)
        printf("IN_MODIFY");
    if (mask & IN_ATTRIB)
        printf("IN_ATTRIB");
    if (mask & IN_CLOSE)
        printf("IN_CLOSE");
    if (mask & IN_OPEN)
        printf("IN_OPEN");
    if (mask & IN_MOVED_FROM)
        printf("IN_MOVED_FROM");
    if (mask & IN_MOVED_TO)
        printf("IN_MOVED_TO");
    if (mask & IN_MOVE_SELF)
        printf("IN_MOVE_SELF");
    if (mask & IN_DELETE)
        printf("IN_DELETE");
    if (mask & IN_CREATE)
        printf("IN_CREATE");
    if (mask & IN_DELETE_SELF)
        printf("IN_DELETE_SELF");
    if (mask & IN_UNMOUNT)
        printf("IN_UNMOUNT");
    if (mask & IN_Q_OVERFLOW)
        printf("IN_Q_OVERFLOW");
    if (mask & IN_IGNORED)
        printf("IN_IGNORED");
    if (mask & IN_ISDIR)
        printf("(IN_ISDIR)");
    else
        printf("(FILE)");
}

void print_help(char *name)
{
    printf("Usage : %s -m <file or folder to monitor> \n", name);
    printf("Ex : %s -m /proc \n", name);
  
    return;
}

int main(int argc, char *argv[])
{
    size_t r;
    fd_set fds;
    char buffer[8192];
    struct inotify_event *event;
    struct timeval  tv;
    time_t ltime;
    struct tm *Tm;
    char monitoringPath[128];
    char c ;
    struct statfs buf;

    memset(monitoringPath,0,sizeof(monitoringPath));

    while ((c = getopt (argc, argv, "m:")) != -1)
    switch (c)
    {
        case 'm': snprintf(monitoringPath, sizeof(monitoringPath) -1, "%s", optarg);
             break;

        case '?':
              print_help(argv[0]);
              return 0;
    }  

    if(!strlen(monitoringPath))
    {
        print_help(argv[0]);
        return -1;
    }
 
    if(!statfs(monitoringPath, &buf))
    {
        fsTypeToName_t *fs ;

        for (fs = fstypetoname ; fs->type; fs++)
        {
            if (fs->type == buf.f_type)
            {
                printf("%s stats :\n"
                        "type of filesystem = 0x%ld (%s),\n "
                        "optimal transfer block size = 0x%ld\n  "
                        "total data blocks in file system = 0x%ld\n\n "
                        "free blocks in fs = 0x%ld\n "
                        "free blocks avail to non-superuser = 0x%ld \n "
                        "total file nodes in file system = 0x%ld\n "
                        "free file nodes in fs = 0x%ld\n "
                        "maximum length of filenames = 0x%ld\n", monitoringPath, \
                                                                 buf.f_type,     \
                                                                 fs->name,       \
                                                                 buf.f_bsize,    \
                                                                 buf.f_blocks,   \
                                                                 buf.f_bfree,    \
                                                                 buf.f_bavail,   \
                                                                 buf.f_files,    \
                                                                 buf.f_ffree,    \
                                                                 buf.f_namelen );
                break;   
            }
        }
    }

    /* inotify initialisation */
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return EXIT_FAILURE;
    }

    /* Starting monitoring */
    wd = inotify_add_watch(fd, monitoringPath, IN_ALL_EVENTS);
    if (wd < 0) {
        perror("inotify_add_watch");
        return EXIT_FAILURE;
    }

    /*printf("Monitoring : '%s' (numero = %d, fd = %d)\n", argv[1], wd, fd); */

    /* Capture de SIGINT (Ctrl + C) */
    signal(SIGINT, sigint_handler);

    while (1) 
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if (select(fd +1, &fds, NULL, NULL, &tv) <= 0) {
            continue;
        }

        /* Get event params */
        r = read(fd, buffer, sizeof(buffer));
        if (r <= 0) {
            perror("read");
            return EXIT_FAILURE;
        }

        event = (struct inotify_event *) buffer;
        ltime = time(NULL);
        Tm = localtime(&ltime);
        printf("[%s access monitoring]\tDate=%04d-%02d-%02d %02d:%02d:%02d\t", monitoringPath,      \
                                                                              (Tm->tm_year + 1900), \
                                                                              (Tm->tm_mon+1),       \
                                                                              Tm->tm_mday,          \
                                                                              Tm->tm_hour,          \
                                                                              Tm->tm_min,           \
                                                                              Tm->tm_sec);
 
        if (event->len) 
        {
            printf("Object=%s\t", event->name);
            printr_mask(event->mask);
        }

        printf("\n");
    }

    return EXIT_FAILURE;
}

