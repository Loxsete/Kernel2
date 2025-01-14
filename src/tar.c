#include <fs.h>
#include <tar.h>
#include <stdint.h>
#include <stdint.h>
#include <stddef.h>


#define min(a,b) (a<b?a:b)

#define TAR_PADDING 0x200

// Копирование памяти
void* memcpy(void* dest, const void* src, size_t n) {
    char* d = dest;
    const char* s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

// Заполнение памяти
void* memset(void* s, int c, size_t n) {
    unsigned char* p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;
    while (n--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}


void char_ptr_to_array(const char *src, char *dest, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    dest[len] = '\0'; // Не забудьте добавить нулевой терминатор
}

uint32_t oct2bin(uint8_t* str, int size){
    uint32_t count = 0;
    for (int i = 0; i < size; i++)
        count = count << 3 | (str[i - 1] - '0');
    return count;
}

uint32_t padded(uint32_t value){
    if(value % TAR_PADDING > 0)
        value += TAR_PADDING - (value%TAR_PADDING);
    return value;
}

void tar_open(file* _file, filesystem_info* filesystem){
    void* tar_address = (void*)filesystem->address;
    uint32_t tar_size = filesystem->size;
    uint32_t pos = 0;
    _file->type = FILETYPE_NONE;
    while(pos < tar_size){
        tar_entry* entry = (void*)((uint32_t)tar_address + pos);


        _file->entry_offset = pos;
        pos += padded(sizeof(tar_entry));
        if(!*entry->filename) continue;
        _file->data_offset = pos;
        _file->filesize = oct2bin(entry->size, 12);
        pos += padded(_file->filesize);

        if(memcmp(entry->filename, _file->filename, 100)) continue;

        if(entry->type == TAR_TYPE_FILE)
            _file->type = FILETYPE_FILE;
        else if(entry->type == TAR_TYPE_DIRECTORY)
            _file->type = FILETYPE_DIRECTORY;
        else
            break;
        _file->pos = 0;
        break;
    }
}

uint32_t tar_getsize(file* _file, filesystem_info* filesystem){
    (void)filesystem;
    return _file->filesize;
}

void tar_close(file* _file, filesystem_info* filesystem){
    (void)_file;
    (void)filesystem;
}

uint32_t tar_read(file* _file, char* buffer, uint32_t count, filesystem_info* filesystem){
    uint32_t to_read = min(count, _file->filesize - _file->pos);
    if(to_read == 0)
        return 0;
    uint32_t read_address = (uint32_t)filesystem->address + _file->data_offset + _file->pos;
    memcpy((uint8_t*)buffer, (void*)read_address, to_read);
    _file->pos += to_read;
    return to_read;
}

uint32_t tar_readdir(char* path, uint32_t path_len, char files[READDIR_FILES_MAX][FILENAME_MAX], filesystem_info* filesystem){
    void* tar_address = (void*)filesystem->address;
    uint32_t tar_size = filesystem->size;
    uint32_t pos = 0;
    uint32_t files_found = 0;
    // Tar dont have / at filename/dir name start
    if(*path=='/'){
        path++;
        path_len--;
    }
    while((pos < tar_size) && files_found < READDIR_FILES_MAX){
        tar_entry* entry = (void*)((uint32_t)tar_address + pos);

        pos += padded(sizeof(tar_entry));
        if(!*entry->filename) continue;
        if(!memcmp(entry->filename, (uint8_t*)path, path_len)){
            char* filename = (char*) ((uint32_t)entry->filename + path_len + (path_len>0));
            char* filename2 = filename;
            uint32_t fnamepos = 0;
            while(*filename2++ != '/' && fnamepos < (FILENAME_MAX - path_len - 1)){
                fnamepos++;
            }
            char_ptr_to_array(filename, files[files_found++], fnamepos);
        }
        pos +=  padded(oct2bin(entry->size, 12));
    }
    return files_found;
}
