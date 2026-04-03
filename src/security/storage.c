#include "storage.h"
#include <mbedtls/aes.h>
#include <mbedtls/sha256.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <unistd.h>
#include <fcntl.h>
#endif

static const char* STORAGE_PATH = "./accounts/";
static unsigned char master_key[32]; 
static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context ctr_drbg;

// Функция получения уникального Hardware ID
static void get_hardware_id(char* out_id) {
#ifdef _WIN32
    DWORD serial;
    GetVolumeInformationA("C:\\", NULL, 0, &serial, NULL, NULL, NULL, 0);
    sprintf(out_id, "WIN-%lu-%s", serial, KYRO_AUTHOR);
#else
    int fd = open("/etc/machine-id", O_RDONLY);
    if (fd != -1) {
        read(fd, out_id, 32);
        close(fd);
    } else {
        sprintf(out_id, "LNX-DEFAULT-%s", KYRO_AUTHOR);
    }
#endif
}

void kyro_crypto_init(void) {
    mkdir(STORAGE_PATH, 0777);
    
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    
    const char* custom_seed = "Kyrogram_Secure_Seed_2026_NUA_PROD";
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, 
                         (const unsigned char*)custom_seed, strlen(custom_seed));

    // Генерация мастер-ключа на основе Hardware ID
    char hw_id[128] = {0};
    get_hardware_id(hw_id);
    
    unsigned char raw_seed[256];
    sprintf((char*)raw_seed, "%s_SECRET_2026_%s", KYRO_AUTHOR, hw_id);
    mbedtls_sha256(raw_seed, strlen((char*)raw_seed), master_key, 0);
}

bool kyro_secure_save(const char* account_id, const unsigned char* data, size_t size) {
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    
    unsigned char iv[16];
    mbedtls_ctr_drbg_random(&ctr_drbg, iv, 16);
    
    mbedtls_aes_setkey_enc(&aes, master_key, 256);
    
    size_t padded_size = ((size + 15) / 16) * 16;
    unsigned char* buffer = (unsigned char*)malloc(padded_size);
    memset(buffer, 0, padded_size);
    memcpy(buffer, data, size);
    
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_size);
    unsigned char iv_copy[16];
    memcpy(iv_copy, iv, 16); 
    
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded_size, iv_copy, buffer, encrypted_data);
    
    char full_path[256];
    sprintf(full_path, "%s%s.dat", STORAGE_PATH, account_id);
    
    FILE* f = fopen(full_path, "wb");
    if (!f) {
        free(buffer); free(encrypted_data); mbedtls_aes_free(&aes);
        return false;
    }
    
    fwrite(iv, 1, 16, f);
    fwrite(encrypted_data, 1, padded_size, f);
    fclose(f);
    
    free(buffer);
    free(encrypted_data);
    mbedtls_aes_free(&aes);
    return true;
}

unsigned char* kyro_secure_load(const char* account_id, size_t* out_size) {
    char full_path[256];
    sprintf(full_path, "%s%s.dat", STORAGE_PATH, account_id);
    
    FILE* f = fopen(full_path, "rb");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    size_t total_size = ftell(f);
    if (total_size < 16) { fclose(f); return NULL; }
    fseek(f, 0, SEEK_SET);
    
    unsigned char iv[16];
    fread(iv, 1, 16, f);
    
    size_t data_size = total_size - 16;
    unsigned char* encrypted_data = (unsigned char*)malloc(data_size);
    fread(encrypted_data, 1, data_size, f);
    fclose(f);
    
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes, master_key, 256);
    
    unsigned char* decrypted_data = (unsigned char*)malloc(data_size);
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, data_size, iv, encrypted_data, decrypted_data);
    
    *out_size = data_size;
    free(encrypted_data);
    mbedtls_aes_free(&aes);
    return decrypted_data;
}
