#include <stdio.h>
#include <pthread.h>

char str[100];

// Thread function to swap case
void* convert(void* arg) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');  // Capital → small
        } else if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');  // Small → capital
        }
    }
    return NULL;
}

int main() {
    pthread_t t;

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    // Remove newline character if present
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }

    pthread_create(&t, NULL, convert, NULL);
    pthread_join(t, NULL);

    printf("Converted string: %s\n", str);

    return 0;
}
