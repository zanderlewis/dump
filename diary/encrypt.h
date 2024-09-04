#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void triple_xor(const unsigned char* input, unsigned char* output, const char* key, size_t length) {
    size_t key_len = strlen(key);
    size_t key_third = key_len / 3;

    for (size_t i = 0; i < length; i++) {
        output[i] = input[i] ^ key[i % key_third];
        output[i] ^= key[(i + key_third) % key_len];
        output[i] ^= key[(i + 2 * key_third) % key_len];
    }
}

unsigned char* encrypt(const unsigned char* plaintext, const char* key, size_t length) {
    unsigned char* ciphertext = (unsigned char*)malloc(length);
    if (ciphertext == NULL) {
        return NULL;
    }
    triple_xor(plaintext, ciphertext, key, length);
    return ciphertext;
}

unsigned char* decrypt(const unsigned char* ciphertext, const char* key, size_t length) {
    unsigned char* plaintext = (unsigned char*)malloc(length);
    if (plaintext == NULL) {
        return NULL;
    }
    triple_xor(ciphertext, plaintext, key, length);
    return plaintext;
}

void save_to_file(const char* filename, const unsigned char* data, size_t data_size) {
    char full_filename[256];
    snprintf(full_filename, sizeof(full_filename), "%s.diary", filename);
    FILE* file = fopen(full_filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    
    fwrite(data, 1, data_size, file);
    fclose(file);
}

size_t read_from_file(const char* filename, unsigned char** data) {
    char full_filename[256];
    snprintf(full_filename, sizeof(full_filename), "%s.diary", filename);
    FILE* file = fopen(full_filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    size_t data_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    *data = (unsigned char*)malloc(data_size);
    if (*data == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 0;
    }
    
    fread(*data, 1, data_size, file);
    fclose(file);
    
    return data_size;
}

#endif // ENCRYPT_H