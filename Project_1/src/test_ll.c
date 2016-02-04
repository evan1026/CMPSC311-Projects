#include <stdio.h>

#include "list.h"

static char* words[] = {
    "Apple",
    "apple",
    "ApPlE",
    "a_p_p_l_e",
    "Hello",
    "from",
    "the",
    "other",
    "side.",
    "I",
    "must've",
    "called",
    "a",
    "thousand",
    "times.",
    "Hello.",
    "My",
    "name",
    "is",
    "Inigo",
    "Montoya.",
    "You",
    "killed",
    "my",
    "father.",
    "Prepare",
    "to",
    "die.",
    ""
};

int main(int argc, char* argv[]) {
    linked_list list;
    int i;
    ll_node_t *curr;

    printf("Initializing list...");
    fflush(stdout);
    ll_init(&list);
    printf("done!\n");

    printf("Adding words...\n");
    i = 0;
    while (words[i][0] != 0) {
        printf("\rAdding word %d...", i);
        fflush(stdout);
        ll_count_word(&list, words[i]);
        printf("done!\n");
        ++i;
    }

    printf("\nTotal words counted: %d\n\nWords:\n", list.count);

    curr = list.head;
    while (curr != NULL) {
        printf("%s: %d\n", curr->word->word, curr->word->count);
        curr = curr->next;
    }

    ll_dispose(&list);

    return 0;
}
