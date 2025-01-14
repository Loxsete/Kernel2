#pragma once

#include <stdint.h>

#define FILENAME_MAX 128
#define PATH_MAX 128
#define READDIR_FILES_MAX 64
#define FD_MAX 64

typedef enum {
    FILETYPE_NONE = 0,
    FILETYPE_FILE = 1,
    FILETYPE_DIRECTORY = 2
} fs_filetype;

typedef struct {
    uint8_t filename[FILENAME_MAX];
    uint8_t path[PATH_MAX];
    uint32_t filesize;
    fs_filetype type;
    uint32_t data_offset;
    uint32_t entry_offset;
    uint32_t pos;
} __attribute__((packed)) file;

typedef struct {
    uint32_t address;
    uint32_t size;
    file files_fd[FD_MAX];
} __attribute__((packed)) filesystem_info;

typedef struct {
    void (*open)(file* _file, filesystem_info* filesystem);
    uint32_t (*get_size)(file* _file, filesystem_info* filesystem);
    void (*close)(file* _file, filesystem_info* filesystem);
    uint32_t (*read) (file* _file, char* buffer, uint32_t count, filesystem_info* filesystem);
    uint32_t (*readdir)(char* path, uint32_t path_len, char files[READDIR_FILES_MAX][FILENAME_MAX], filesystem_info* filesystem);
} __attribute__((packed)) filesystem_ops;

uint32_t fs_open(char* name, filesystem_info* filesystem, const filesystem_ops* ops);
uint32_t fs_getsize(uint32_t fd, filesystem_info* filesystem, const filesystem_ops* ops);
void fs_close(uint32_t fd, filesystem_info* filesystem, const filesystem_ops* ops);
uint32_t fs_read(uint32_t fd, char* buffer, uint32_t count, filesystem_info* filesystem, const filesystem_ops* ops);
uint32_t fs_readdir(char* path, uint32_t path_len, char files[READDIR_FILES_MAX][FILENAME_MAX], filesystem_info* filesystem, const filesystem_ops* ops);
