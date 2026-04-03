#ifndef KYRO_STORAGE_H
#define KYRO_STORAGE_H

#include "kyrogram.h"

void kyro_crypto_init(void);
bool kyro_secure_save(const char* account_id, const unsigned char* data, size_t size);
unsigned char* kyro_secure_load(const char* account_id, size_t* out_size);

#endif // KYRO_STORAGE_H
