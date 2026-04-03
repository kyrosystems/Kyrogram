#include "storage.h"
#include <mbedtls/aes.h>
#include <mbedtls/sha256.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>

static const char* STORAGE_PATH = "./accounts/";
static unsigned char master_key[32]; // Derived 256-bit key

void kyro_crypto_init(void) {
    // Initializing crypto contexts
    // In a real human scenario, we derive this from a device-specific salt + user password
    // For now, let's use a placeholder derivation for basic functionality
    mbedtls_sha256((const unsigned char*)KYRO_AUTHOR, strlen(KYRO_AUTHOR), master_key, 0);
}

bool kyro_secure_save(const char* account_id, const unsigned char* data, size_t size) {
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    
    // Set encryption key
    mbedtls_aes_setkey_enc(&aes, master_key, 256);
    
    // Pad data to 16-byte block size
    size_t padded_size = ((size + 15) / 16) * 16;
    unsigned char* buffer = (unsigned char*)malloc(padded_size);
    memset(buffer, 0, padded_size);
    memcpy(buffer, data, size);
    
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_size);
    unsigned char iv[16] = {0}; // Should be unique per save
    
    // AES CBC encryption
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded_size, iv, buffer, encrypted_data);
    
    // Save to file
    char full_path[256];
    sprintf(full_path, "%s%s.dat", STORAGE_PATH, account_id);
    
    FILE* f = fopen(full_path, "wb");
    if (!f) {
        free(buffer);
        free(encrypted_data);
        return false;
    }
    
    fwrite(encrypted_data, 1, padded_size, f);
    fclose(f);
    
    free(buffer);
    free(encrypted_data);
    mbedtls_aes_free(&aes);
    return true;
}

unsigned char* kyro_secure_load(const char* account_id, size_t* out_size) {
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    
    char full_path[256];
    sprintf(full_path, "%s%s.dat", STORAGE_PATH, account_id);
    
    FILE* f = fopen(full_path, "rb");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    unsigned char* encrypted_data = (unsigned char*)malloc(size);
    fread(encrypted_data, 1, size, f);
    fclose(f);
    
    mbedtls_aes_setkey_dec(&aes, master_key, 256);
    
    unsigned char* decrypted_data = (unsigned char*)malloc(size);
    unsigned char iv[16] = {0};
    
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, size, iv, encrypted_data, decrypted_data);
    
    *out_size = size;
    free(encrypted_data);
    mbedtls_aes_free(&aes);
    return decrypted_data;
}
