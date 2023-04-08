#include <stdio.h>
#include <string.h>
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
    }    

    apply_clear(ah); 
    fsm_destroy(HVD_fst);
    return 0;
}