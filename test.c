#include <sys/types.h>

#include <sys/stat.h>

#include <err.h>
#include <fts.h>
#include <stdio.h>

static int      ptree(char * const argv[]);

int
main(int argc, char * const argv[])
{
        int rc;

        if ((rc = ptree(argv + 1)) != 0)
                rc = 1;
        return rc;
}

static int
ptree(char * const argv[])
{
        FTS *ftsp;
        FTSENT *p, *chp;
        int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;
        int rval = 0;

        if ((ftsp = fts_open(argv, fts_options, NULL)) == NULL) {
                warn("fts_open");
                return -1;
        }
        /* Initialize ftsp with as many argv[] parts as possible. */
        chp = fts_children(ftsp, 0);
        if (chp == NULL) {
                return 0;               /* no files to traverse */
        }
        while ((p = fts_read(ftsp)) != NULL) {
                switch (p->fts_info) {
                case FTS_D:
                        printf("d %s\n", p->fts_path);
                        break;
                case FTS_F:
                        printf("f %s\n", p->fts_path);
                        break;
                default:
                        break;
                }
        }
        fts_close(ftsp);
        return 0;

}

