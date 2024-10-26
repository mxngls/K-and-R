#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101
#define MAXCOM 10

static struct nlist *hashtab[HASHSIZE]; /* pointer table */

struct nlist {          /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *name;         /* defined name */
    char *defn;         /* replacement text */
};

/* hash: form hash value for string s */
unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s) {
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL;       /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct nlist *)malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else                      /* already there */
        free((void *)np->defn); /*free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

/* uninstall: remove (name, defn) from hashtab */
int uninstall(char *name) {
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* not found */
        return 1;
    } else {
        hashval = hash(name);
        if (np->next)
            hashtab[hashval] = np->next; /* replace */
        else
            hashtab[hashval] = NULL; /* remove */
        free(np);
    }
    return 0;
}

/*
 * Exercise 6-5. Write a function uninstall that will remove a name and
 * definition from the table maintained by lookup and install.
 */
int main() {
    char c;
    char command[MAXCOM] = "";
    char def[MAXCOM] = "";
    char name[MAXCOM] = "";
    int m, o, n, d;

    m = 0;
    o = 0;
    n = 0;
    d = 0;
    while ((c = (char)getchar()) != EOF) {
        if (c == ' ') {
            m++;
        } else if (c == '\n') {
            int cmd = (strcmp(command, "install") == 0)     ? 0
                      : (strcmp(command, "lookup") == 0)    ? 1
                      : (strcmp(command, "uninstall") == 0) ? 2
                                                            : -1;

            switch (cmd) {
            case 0: /* install */
                if (!*command || !*name || !*def)
                    printf("[ERR]: not all fields provided\n");
                else {
                    printf(">>> command: %s, name: %s, def: %s\n", command,
                           name, def);
                    install(name, def);
                }
                break;
            case 1: /* lookup */
                if (!*name) {
                    printf("[ERR]: no name to lookup provided\n");
                } else {
                    printf(">>> command: %s, name: %s\n", command, name);
                    struct nlist *res = lookup(name);
                    if (!res)
                        printf("[ERR]: entry for \"%s\" could not be found\n",
                               name);
                    else
                        printf("%s\n", res->defn);
                }
                break;
            case 2: /* uninstall */
                if (!*name) {
                    printf("[ERR]: no name to uninstall provided\n");
                } else {
                    if (uninstall(name))
                        printf("[ERR]: entry for \"%s\" could not be found\n",
                               name);
                    else
                        printf(">>> command: %s, name: %s\n", command, name);
                }
                break;
            default:
                printf("[ERR]: invalid command\n");
                break;
            }

        reset_state:
            memset(command, 0, MAXCOM);
            memset(name, 0, MAXCOM);
            memset(def, 0, MAXCOM);
            m = 0;
            o = 0;
            n = 0;
            d = 0;
        } else {
            if (o >= MAXCOM || n >= MAXCOM || d >= MAXCOM) {
                printf("o:%d, n:%d, d:%d\n", o, n, d);
                printf("[ERR]: input parameter too long.\n");
                while (getchar() != '\n')
                    ;
                goto reset_state;
            }

            switch (m) {
            case 0:
                (command[o++] = c);
                break;
            case 1:
                (name[n++] = c);
                break;
            case 2:
                (def[d++] = c);
                break;
            default:
                printf("[ERR]: too many input parameter provided.\n");
                while (getchar() != '\n')
                    ;
                goto reset_state;
            }
        }
    }
    return 0;
}
