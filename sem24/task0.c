#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef FUSE2
#define FUSE_USE_VERSION 26
#else
#define FUSE_USE_VERSION 30
#endif
#include <fuse.h>

#define conditional_handle_error(stmt, msg) \
  if (stmt) {                               \
    perror(msg " (" #stmt ")");             \
    exit(EXIT_FAILURE);                     \
  }

typedef struct {
  char* directory_name;
} options_t;
options_t options;

#define MAX_LENGTH 4096

typedef struct {
  char* data_;
  size_t size_;
  char* filename_;
} file_t;

typedef struct {
  size_t num_files_;
  file_t* files_;
  size_t cur_size_;
} dir_t;
dir_t* dir_emulator;

dir_t* tiny_dir_create(size_t num_files) {
  dir_t* tiny_dir = malloc(sizeof(dir_t));

  tiny_dir->num_files_ = num_files;
  tiny_dir->files_ = malloc(sizeof(file_t) * num_files);
  tiny_dir->cur_size_ = 0;

  return tiny_dir;
}

void tiny_dir_add_fileinfo(dir_t* tiny_dir, char* filename, size_t file_size) {
  conditional_handle_error(tiny_dir->cur_size_ == tiny_dir->num_files_,
                           "dir overflow");

  size_t filename_length = strlen(filename);
  tiny_dir->files_[tiny_dir->cur_size_].filename_ = malloc(filename_length + 1);
  strncpy(tiny_dir->files_[tiny_dir->cur_size_].filename_, filename,
          filename_length);

  tiny_dir->files_[tiny_dir->cur_size_].size_ = file_size;
  tiny_dir->files_[tiny_dir->cur_size_].data_ = malloc(file_size + 1);

  ++tiny_dir->cur_size_;
}

void tiny_dir_write_filedata(dir_t* tiny_dir, char* data) {
  conditional_handle_error(tiny_dir->cur_size_ == tiny_dir->num_files_,
                           "dir overflow");

  strcpy(tiny_dir->files_[tiny_dir->cur_size_].data_, data);

  ++tiny_dir->cur_size_;
}

void tiny_dir_delete(dir_t* tiny_dir) {
  for (size_t i = 0; i < tiny_dir->num_files_; ++i) {
    free(tiny_dir->files_[i].filename_);
    free(tiny_dir->files_[i].data_);
  }

  free(tiny_dir);
  tiny_dir = NULL;
}

file_t* tiny_dir_find_file(dir_t* tiny_dir, const char* path_to_file) {
  if (path_to_file[0] != '/') {
    return NULL;
  }

  for (size_t i = 0; i < tiny_dir->num_files_; ++i) {
    if (strcmp(tiny_dir->files_[i].filename_, path_to_file + 1) == 0) {
      return &tiny_dir->files_[i];
    }
  }

  return NULL;
}

void tiny_dir_print(dir_t* tiny_dir) {
  printf("-----------------------\n");
  printf("Number of files: %ld\n\n", tiny_dir->num_files_);
  for (size_t i = 0; i < tiny_dir->num_files_; ++i) {
    printf("File: %s\n", tiny_dir->files_[i].filename_);
    printf("Size: %ld\n", tiny_dir->files_[i].size_);

    write(STDOUT_FILENO, "Data: ", sizeof("Data: "));
    write(STDOUT_FILENO, tiny_dir->files_[i].data_, tiny_dir->files_[i].size_);
    write(STDOUT_FILENO, "\n", sizeof("\n"));

    if (i != tiny_dir->num_files_ - 1) {
      printf("\n");
    }
  }
  printf("-----------------------\n");
  fflush(stdout);
}

void parse_dir() {
  FILE* opened_dir = fopen(options.directory_name, "r");
  conditional_handle_error(opened_dir == NULL, "can't open directory");

  int num_files = 0;
  fscanf(opened_dir, "%d", &num_files);

  dir_emulator = tiny_dir_create(num_files);

  char buffer[MAX_LENGTH];
  size_t file_size = 0;
  for (size_t i = 0; i < num_files; ++i) {
    fscanf(opened_dir, "%s", buffer);
    fscanf(opened_dir, "%zu", &file_size);

    tiny_dir_add_fileinfo(dir_emulator, buffer, file_size);
  }

  //  set cur_size 0 to iterate again
  dir_emulator->cur_size_ = 0;

  fread(buffer, 2, 1, opened_dir);
  for (size_t i = 0; i < num_files; ++i) {
    fread(buffer, dir_emulator->files_[i].size_, 1, opened_dir);
    tiny_dir_write_filedata(dir_emulator, buffer);
  }

  fclose(opened_dir);
}

int tiny_fuse_read(const char* path, char* buf, size_t size, off_t offset,
                   struct fuse_file_info* fi) {
  file_t* file = tiny_dir_find_file(dir_emulator, path);
  if (file == NULL) {
    return -ENOENT;
  }
  if (offset >= file->size_) {
    return 0;
  }

  size_t readable_size =
      (offset + size <= file->size_) ? size : file->size_ - offset;
  memcpy(buf, file->data_ + offset, readable_size);

  return readable_size;
}

int tiny_fuse_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info* fi
#ifndef FUSE2
                      ,
                      enum fuse_readdir_flags flags
#endif
) {
#ifdef FUSE2
  (void)offset;
  (void)fi;
  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  for (size_t i = 0; i < dir_emulator->num_files_; ++i) {
    filler(buf, dir_emulator->files_[i].filename_, NULL, 0);
  }
#else
  (void)offset;
  (void)fi;
  (void)flags;
  filler(buf, ".", NULL, 0, (enum fuse_fill_dir_flags)0);
  filler(buf, "..", NULL, 0, (enum fuse_fill_dir_flags)0);

  for (size_t i = 0; i < dir_emulator->num_files_; ++i) {
    filler(buf, dir_emulator->files_[i].filename_, NULL, 0,
           (enum fuse_fill_dir_flags)0);
  }
#endif
  return 0;
}

int tiny_fuse_getattr(const char* path, struct stat* stbuf
#ifndef FUSE2
                      ,
                      struct fuse_file_info* fi
#endif
) {
#ifndef FUSE2
  (void)fi;
#endif
  if (strcmp(path, "/") == 0) {
    *stbuf = (struct stat){.st_mode = 0555 | S_IFDIR, .st_nlink = 2};
  } else {
    file_t* file = tiny_dir_find_file(dir_emulator, path);
    if (file == NULL) {
      return -ENOENT;
    }

    *stbuf = (struct stat){
        .st_mode = 0444 | S_IFREG, .st_nlink = 1, .st_size = file->size_};
  }

  return 0;
}

struct fuse_opt opt_specs[] = {{"--src %s", 0, 0}, FUSE_OPT_END};

struct fuse_operations operations = {
    .getattr = tiny_fuse_getattr,
    .read = tiny_fuse_read,
    .readdir = tiny_fuse_readdir,
};

int main(int argc, char** argv) {
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  fuse_opt_parse(&args, &options, opt_specs, NULL);
  parse_dir();

  int return_code = fuse_main(args.argc, args.argv, &operations, NULL);

  tiny_dir_delete(dir_emulator);
  fuse_opt_free_args(&args);

  return return_code;
}

