#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include <zip.h>

#include "cbz.h"

static int is_dir(const char* path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

static void walk_directory(const char* startdir, const char* inputdir, zip_t* archive) {
    DIR *dp = opendir(inputdir);
    if (!dp) {
        perror("Failed to open input directory");
        return;
    }

	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..")) {
			char* fullname = (char*)calloc((3 + strlen(inputdir) + strlen(dirp->d_name)), sizeof(char));
			snprintf(fullname, (3 + strlen(inputdir) + strlen(dirp->d_name)), "%s/%s", inputdir, dirp->d_name);
			if (is_dir(fullname)) {
				if (zip_dir_add(archive, fullname+strlen(startdir)+1, ZIP_FL_ENC_UTF_8) < 0) {
                    fprintf(stderr, "Failed to add directory to zip: %s\n", zip_strerror(archive));
                    return;
				}
				walk_directory(startdir, fullname, archive);      
			} else {
				zip_source_t *source = zip_source_file(archive, fullname, 0, 0);
				if (!source) {
                    fprintf(stderr, "Failed to add file to zip: %s\n", zip_strerror(archive));
				}
				if (zip_file_add(archive, fullname+strlen(startdir)+1, source, ZIP_FL_ENC_UTF_8) < 0) {
					zip_source_free(source);
					fprintf(stderr, "Failed to add file to zip: %s\n", zip_strerror(archive));
				}
			}
		}
	}
	closedir(dp);
}

void zip_directory(const char* input, const char* output) {
	int errorp;
	zip_t* archive = zip_open(output, ZIP_CREATE | ZIP_EXCL, &errorp);
	if (!archive) {
		zip_error_t ziperror;
		zip_error_init_with_code(&ziperror, errorp);
		fprintf(stderr, "Failed to open output file %s: %s\n", output, zip_error_strerror(&ziperror));
        zip_close(archive);
        return;
	}
	walk_directory(input, input, archive);
	zip_close(archive);
}