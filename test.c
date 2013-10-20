#include <sys/types.h>

#include <sys/stat.h>

#include <err.h>
#include <fts.h>
#include <stdio.h>
#include "ext/uthash.h"
#include <string.h>


struct my_struct {
    char *name;          /* key */
    int id;
    UT_hash_handle hh;         /* makes this structure hashable */
};


struct my_struct *s = NULL;

int main(int argc, char *argv[]) {
    char **n, *names[] = { "jbetty", malloc(BUFSIZ+1), "523@@2ggfeSAG$2442", NULL };
	names[1] = "betty";
    struct my_struct *tmp, *users = NULL;
    int i=0;

    for (n = names; *n != NULL; n++) {
        s = (struct my_struct*)malloc(sizeof(struct my_struct));
        s->name = *n;
        s->id = i++;
        HASH_ADD_KEYPTR( hh, users, s->name, strlen(s->name), s );
		if(strcmp(s->name, "betty") == 0) break;
    }
	printf("is this bettfgy? %s\n",s->name);
    HASH_FIND_STR( users, "betty", s);
    if (s) printf("betty's id is %d\n", s->id);

    /* free the hash table contents */
    HASH_ITER(hh, users, s, tmp) {
      HASH_DEL(users, s);
      free(s);
    }
    return 0;
}
