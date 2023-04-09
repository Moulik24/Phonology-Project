#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fomalib.h"

struct fsm *convert_to_categories() {
    struct fsm *net;
    net = fsm_parse_regex("[a:l|i:h|u:h|e:l|o:l|k:t|g:d|s:t|z:d|t:t|d:d|ts:t|ch:t|j:d|dz:d|n:d|h:t|f:t|b:d|p:t|m:d|y:d|r:d|w:d]*", NULL, NULL);
    return net;
}

struct fsm *HVD() {
    struct fsm *net;
    net = fsm_parse_regex("h -> o || t _ t .o. h -> o || t _ .#.", NULL, NULL); 
    return net;
}

int main() {
    struct fsm *feature_abstraction = convert_to_categories();
    struct fsm *HVD_fst = HVD();
    struct fsm *combined = fsm_compose(feature_abstraction, HVD_fst);

    struct apply_handle *ah = apply_init(combined);
    ah = apply_init(combined);
    
    printf("\nWelcome! Enter your word after the prompt \"Please enter your word\". When you want to stop the program, type EXIT after this prompt.\n\n");
    
    char input_word[20];
    while(1) {
        printf("Please enter your word: \n");
        scanf("%s", input_word);
        if(strcmp(input_word,"EXIT") == 0) {
            printf("Thank you!\n");
            break;
        }

        char *result = apply_down(ah, input_word);
        if(result == NULL) {
            printf("Sorry, that is not a word in my dictionary. Please try another word.\n\n");
        }
        while (result != NULL) {
            printf("Resulting word is: \n%s \n\n", result);
            result = apply_down(ah, NULL);
        }
        free(result);
    }    

    apply_clear(ah); 
    fsm_destroy(combined);
    return 0;
}