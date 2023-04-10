#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fomalib.h"

char* voiceless_consonants = "[k|s|t|ts|ch|h|f|p]";
char* voiceless_consonants_and_EOW = "[k|s|t|ts|ch|h|f|p|.#.]";
char* voiced_consonants = "[g|z|d|j|dz|n|b|m|y|r|w]";
char* high_vowels = "[i|u]";
char* low_vowels = "[a|e|o]";

struct fsm *Lexcion() {
    struct fsm *net;
    net = fsm_parse_regex("[a|i|u|e|o|k|g|s|z|t|d|ts|ch|j|dz|n|h|f|b|p|m|y|r|w]*", NULL, NULL);
    return net;
}

struct fsm *HVD() {
    struct fsm *net;
    char regex_string[100];
    snprintf(regex_string, sizeof(regex_string), "%s%s%s%s%s", high_vowels, " -> o || ", voiceless_consonants, " _ ", voiceless_consonants_and_EOW);
    net = fsm_parse_regex(regex_string, NULL, NULL); 
    return net;
}

int main() {
    // Construct the lexicon with phonological rules applied
    struct fsm *lexicon = Lexcion();
    struct fsm *hvd = HVD();
    struct fsm *combined = fsm_compose(lexicon, hvd);

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