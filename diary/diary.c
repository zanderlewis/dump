#include "encrypt.h"
#include <string.h>

int main() {
    unsigned char plaintext[100];
    // 100 characters
    const char* key = "t-E'N*{)<fy5pTDCk=_vZ`]X$>64;g}YHW8dz,w#Pn[:^h9&V2Ju;^gk@v<3.pX?+Ds:/m!adUQA#HT'VKRW7wSz*-2M9ZP6by(8";
    size_t length;

    printf("Enter plaintext: ");
    fgets((char*)plaintext, sizeof(plaintext), stdin);
    length = strlen((char*)plaintext);
    
    unsigned char* ciphertext = encrypt(plaintext, key, length);
    if (ciphertext == NULL) {
        printf("Encryption failed.\n");
        return 1;
    }
    
    printf("Encrypted text: ");
    printf("%s", ciphertext);
    printf("\n");
    
    save_to_file("diary", ciphertext, length);

    // Demonstration of decryption
    unsigned char* decrypted = decrypt(ciphertext, key, length);
    if (decrypted == NULL) {
        printf("Decryption failed.\n");
        free(ciphertext);
        return 1;
    }
    
    printf("Decrypted text: %s", decrypted);

    // Clean up
    free(ciphertext);
    free(decrypted);

    return 0;
}