#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void fsize(char *);
void dirwalk(char *, void (*fcn)(char *));

/* fsize: print the name of file "name" */
void fsize(char *name) {
        struct stat    stbuf;
        char           uname[20];
        char           gname[20];
        struct passwd *pw;
        struct group  *gr;

        if (stat(name, &stbuf) == -1) {
                fprintf(stderr, "fsize: can't access %s\n", name);
                return;
        }

        if (S_ISDIR(stbuf.st_mode))
                dirwalk(name, fsize);

        strcpy(uname, (pw = getpwuid(stbuf.st_uid)) ? pw->pw_name : "");
        strcpy(gname, (gr = getgrgid(stbuf.st_gid)) ? gr->gr_name : "");

        printf("%s %s %8lld %s\n", uname, gname, stbuf.st_size, name);
}

#define MAX_PATH 1024

/*
 * dirwalk: apply fcn to all files in dir
 *
 * see comments: https://clc-wiki.net/wiki/K%26R2_solutions:Chapter_8:Exercise_5
 */
void dirwalk(char *dir, void (*fcn)(char *)) {
        char           name[MAX_PATH];
        struct dirent *dp;
        DIR           *dfd;

        if ((dfd = opendir(dir)) == NULL) {
                fprintf(stderr, "dirwalk: can't open %s\n", dir);
                return;
        }
        while ((dp = readdir(dfd)) != NULL) {
                if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                        continue; /* skip self and parent */
                }
                if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
                        fprintf(stderr, "dirwalk: name %s %s too long\n", dir, dp->d_name);
                else {
                        sprintf(name, "%s/%s", dir, dp->d_name);
                        (*fcn)(name);
                }
        }
        closedir(dfd);
}

/*
 * Exercise 8-5. Modify the fsize program to print the other information contained in the inode
 * entry.
 */
int main(int argc, char **argv) {
        if (argc == 1)
                fsize(".");
        /* default: current directory */
        else
                while (--argc > 0)
                        fsize(*++argv);
        return 0;
}
