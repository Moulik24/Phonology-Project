#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fomalib.h"

// Categories used for writing Japanese phonological rules.
char* all_phonemes = "[a|i|u|e|o|k|g|s|z|t|d|ts|ch|j|dz|n|h|f|b|p|m|y|r|w]*";

char* voiceless_consonants = "[k|s|t|ts|ch|h|f|p]";
char* voiceless_consonants_and_EOW = "[k|s|t|ts|ch|h|f|p|.#.]";

char* voiced_consonants = "[g|z|d|j|dz|n|b|m|y|r|w]";

char* high_vowels = "[i|u]";
char* i_high_vowel = "i";
char* u_high_vowel = "u";
char* i_devoiced_high_vowel = "i̥";
char* u_devoiced_high_vowel = "u̥";

char* low_vowels = "[a|e|o]";

char* n_phoneme = "n";

char *bilabial_consonants = "[p|b|m]";
char *velar_consonants_and_EOW = "[k|g|.#.]";
char *w_glide = "w";
char *j_glide = "j";

char *bilabial_nasal = "m";
char *velar_nasal = "ŋ";
char *w_nasal = "w̃";
char *j_nasal = "ȷ̃";


int rewrite_rule_max_size = sizeof(char)*80;

/*
A rewrite rule that the foma API accepts is of the form:
A -> B || C _ D
where A, B, C, and D are all strings. 
*/
char *rewrite_rule(char *A, char *B, char *C, char *D) {
    char *rewrite_rule_regex = malloc(rewrite_rule_max_size);    
    snprintf(rewrite_rule_regex, rewrite_rule_max_size, "%s%s%s%s%s%s%s", A, " -> ", B, " || ", C, " _ ", D);
    return rewrite_rule_regex;
}

/*
Used to compose two regex rule strings - equivalent to building two separate fsts for the rules, and then composing the fsts.
*/
char *compose_regex(char *regex_1, char *regex_2) {
    int composed_regex_max_size = 2*rewrite_rule_max_size;
    char *composed_regex = malloc(composed_regex_max_size);
    snprintf(composed_regex, composed_regex_max_size, "%s%s%s", regex_1, " .o. ", regex_2);
    return composed_regex;
}

struct fsm *Lexcion() {
    struct fsm *net;
    net = fsm_parse_regex(all_phonemes, NULL, NULL);
    return net;
}

/*
Generates a Finite State Transducer that does the Japanese phonological rule called High Vowel Devoicing.
*/
struct fsm *HVD() {
    char *i_high_vowel_devoicing_regex = rewrite_rule(i_high_vowel, i_devoiced_high_vowel, voiceless_consonants, voiceless_consonants_and_EOW);
    char *u_high_vowel_devoicing_regex = rewrite_rule(u_high_vowel, u_devoiced_high_vowel, voiceless_consonants, voiceless_consonants_and_EOW);
    char *high_vowel_devoicing_regex = compose_regex(i_high_vowel_devoicing_regex, u_high_vowel_devoicing_regex);
    
    struct fsm *net;
    net = fsm_parse_regex(high_vowel_devoicing_regex, NULL, NULL); 

    free(i_high_vowel_devoicing_regex);
    free(u_high_vowel_devoicing_regex);
    free(high_vowel_devoicing_regex);

    return net;
}

/*
Generates a Finite State Transducer that does the Japanese phonological rule called Nasal Assimilation.
*/
struct fsm *Nasal_Assimilation() {
    char *bilabial_nasal_assimilation = rewrite_rule(n_phoneme, bilabial_nasal, "", bilabial_consonants);
    char *velar_nasal_assimilation = rewrite_rule(n_phoneme, velar_nasal, "", velar_consonants_and_EOW);
    char *w_glide_nasal_assimilation = rewrite_rule(n_phoneme, w_nasal, "", w_glide);
    char *j_glide_nasal_assimilation = rewrite_rule(n_phoneme, j_nasal, "", j_glide);

    char *nasal_assimilation = compose_regex(bilabial_nasal_assimilation, 
                                    compose_regex(velar_nasal_assimilation, 
                                        compose_regex(w_glide_nasal_assimilation, j_glide_nasal_assimilation)
                                    )
                                );
    
    struct fsm *net;
    net = fsm_parse_regex(nasal_assimilation, NULL, NULL); 
    
    free(bilabial_nasal_assimilation);
    free(velar_nasal_assimilation);
    free(w_glide_nasal_assimilation);
    free(j_glide_nasal_assimilation);
    free(nasal_assimilation);

    return net;
}

int main() {
    // Construct the lexicon with phonological rules applied
    char* binary_file_name = "JapaneseFST.foma";
    printf("Attempting to read FST from %s...\n", binary_file_name);
    struct fsm *japanese_fst = fsm_read_binary_file(binary_file_name);
    if(japanese_fst != NULL) {
        printf("Successfully read!\n");
    }
    if(japanese_fst == NULL) {
        printf("Creating %s...\n", binary_file_name);
        struct fsm *lexicon = Lexcion();
        struct fsm *hvd = HVD();
        struct fsm *nasal_assimilation = Nasal_Assimilation();
        japanese_fst = fsm_compose(lexicon, 
                            fsm_compose(hvd, nasal_assimilation)
                        );
        if((fsm_write_binary_file(japanese_fst, binary_file_name) == 0)) {
            printf("Done!\n");
        } else {
            perror("Error writing fsm to binary file");
        }
    }
    struct apply_handle *ah = apply_init(japanese_fst);
    ah = apply_init(japanese_fst);
    
    printf("\nWelcome! Enter your word after the prompt \"Please enter your word\". When you want to stop the program, type EXIT after this prompt.\n\n");
    
    char input_word[20];
    while(1) {
        printf("Please enter your word: \n");
        scanf("%s", input_word);
        if(strcmp(input_word,"EXIT") == 0) {
            printf("\nThank you!\n");
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
    fsm_destroy(japanese_fst);
    return 0;
}