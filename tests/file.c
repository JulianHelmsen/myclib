#include "file.h"
#include "test.h"
#include "sview.h"
#include <errno.h>

void fp_test(void) {
    FILE* f = fopen("file.dat", "r");
    if(f == NULL) {
        perror("fopen");
        return;
    }

    file_content content = { 0 };
    bool succ = file_read_all(f, &content);
    test(succ);
    fclose(f);

    sview scontent = sview_create((char*) content.data, content.size);

    test(sview_eq(scontent, sview_create_lit("LINE1\nLINE2\n")));
}
void path_test(void) {
    file_content content = { 0 };
    bool succ = file_read_all_at("file.dat", &content);
    test(succ);

    sview scontent = sview_create((char*) content.data, content.size);

    test(sview_eq(scontent, sview_create_lit("LINE1\nLINE2\n")));
}

void path_does_not_exist(void) {
    file_content content = { 0 };
    bool succ = file_read_all_at("not-exist.dat", &content);
    test(!succ);
    test(errno == ENOENT);
}


int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    fp_test();
    path_test();
    path_does_not_exist();

    return 0;
}
