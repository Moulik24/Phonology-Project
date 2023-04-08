#include <stdio.h>
#include "fomalib.h"

struct fsm *HVD() {
    struct fsm *net;
    net = fsm_parse_regex("[t:t|d:d|l:l|h:h]* .o. h -> o || t _ t .o. h -> o || t _ .#.", NULL, NULL); 
    return net;
}

int main() {
    struct fsm *HVD_fst = HVD();
    struct apply_handle *ah = apply_init(HVD_fst);
    ah = apply_init(HVD_fst);

    char *result = apply_down(ah, "tht");
    while (result != NULL) {
        printf("%s\n", result);
        result = apply_down(ah, NULL);
    }

    apply_clear(ah); 
    fsm_destroy(HVD_fst);
    return 0;
}