#include "fomalib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

char* n_nasal = "n";

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

/*
Generates a Finite State Transducer that represents all allowed Japanese words.
Currently, this is a word that is made up of phonemes in all_phonemes, but without regard to sequences of words.
*/
struct fsm *Lexicon() {
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
    char *bilabial_nasal_assimilation = rewrite_rule(n_nasal, bilabial_nasal, "", bilabial_consonants);
    char *velar_nasal_assimilation = rewrite_rule(n_nasal, velar_nasal, "", velar_consonants_and_EOW);
    char *w_glide_nasal_assimilation = rewrite_rule(n_nasal, w_nasal, "", w_glide);
    char *j_glide_nasal_assimilation = rewrite_rule(n_nasal, j_nasal, "", j_glide);

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

int dependent_fst_count = 3;
char *all_dependent_fst_names[] = {"FSTs/Lexicon.foma", "FSTs/High_Vowel_Devoicing.foma", "FSTs/Nasal_Assimilation.foma"};

/*
Makes all the Finite State Transducers that go into the final JapaneseFST. 
Writes out the foma files of these dependent FSTs if they don't exist.
Returns a double pointer to all the dependent FSTs.
*/
struct fsm **generate_all_dependent_fsts() {
    struct fsm **all_dependent_fsts = malloc(dependent_fst_count*sizeof(struct fsm*));

    for(int i = 0; i < dependent_fst_count; i++) {
        all_dependent_fsts[i] = fsm_read_binary_file(all_dependent_fst_names[i]);

        if(all_dependent_fsts[i] == NULL) {
            if(i == 0) {
                all_dependent_fsts[i] = Lexicon();
            }
            else if(i == 1) {
                all_dependent_fsts[i] = HVD();
            }
            else if(i == 2) {
                all_dependent_fsts[i] = Nasal_Assimilation();
            }

            if((fsm_write_binary_file(all_dependent_fsts[i], all_dependent_fst_names[i]) == 0)) {
                printf("Wrote out %s!\n", all_dependent_fst_names[i]);
            } else {
                perror("Error writing out fsm to binary file");
                exit(EXIT_FAILURE);
            }
        }
    } 

    return all_dependent_fsts;
}

int main() {
    // Create a directory to hold all the foma files.
    mkdir("FSTs", 0777);

    // Construct the lexicon with phonological rules applied
    char* binary_file_name = "FSTs/JapaneseFST.foma";
    printf("Attempting to read FST from %s...\n", binary_file_name);
    struct fsm *japanese_fst = fsm_read_binary_file(binary_file_name);
    if(japanese_fst != NULL) {
        printf("Successfully read!\n");
        //TODO: generate the other files if they're not there.
    }
    if(japanese_fst == NULL) {
        printf("Creating %s...\n", binary_file_name);
        struct fsm **all_dependent_fsts;
        all_dependent_fsts = generate_all_dependent_fsts();
        
        japanese_fst = fsm_compose(all_dependent_fsts[0], 
                            fsm_compose(all_dependent_fsts[1], all_dependent_fsts[2])
                        );
        free(all_dependent_fsts);
        if((fsm_write_binary_file(japanese_fst, binary_file_name) == 0)) {
            printf("Wrote out %s!\n", binary_file_name);
            printf("Done!\n");
        } else {
            perror("Error writing fsm to binary file");
            exit(EXIT_FAILURE);
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