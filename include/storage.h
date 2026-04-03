#ifndef KYRO_STORAGE_H
#define KYRO_STORAGE_H

#include "kyrogram.h"

// Encrypts account data to the disk
// Using a derived key from user passphrase or device ID
bool kyro_secure_save(const char* account_id, const unsigned char* data, size_t size);

// Decrypts and loads account data
unsigned char* kyro_secure_load(const char* account_id, size_t* out_size);

// Initialization of crypto modules
void kyro_crypto_init(void);

#endif // KYRO_STORAGE_H
