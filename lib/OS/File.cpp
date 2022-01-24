
#include <dcl/OS/File.h>

// BSD system includes
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace dcl {

namespace OS {

namespace {

DCL_ALWAYS_INLINE
static int makeProt(uint32_t flags) noexcept {
  int prot = 0;

  if (flags == O_RDONLY) {
      prot |= PROT_READ;
  } else if (flags == O_WRONLY) {
      prot |= PROT_WRITE;
  } else if (flags == O_RDWR) {
      prot |= PROT_READ | PROT_WRITE;
  } else if (flags == O_ACCMODE) {
      prot |= PROT_READ | PROT_WRITE;
  }

  return prot;
}

} // anonymous namespace


File::File(const char * path, uint32_t flags) noexcept {
  int fd = open(path, flags, nullptr);
  
#if DEBUG
  std::memset((void *)_path, 0, PATH_MAX + 1);
  std::memcpy((void *)_path, path, strlen(path));
#endif

  if (fd == -1) {
    _fd = fd;
    _size = 0;
    _buffer = nullptr;
    return;
  }
  
  int rc;
  struct stat st;
  rc = fstat(fd, &st);
  size_t size = static_cast<size_t>(st.st_size);
  int prot = 0;
  
  void * header = mmap(0, size, makeProt(flags) ,MAP_SHARED , fd, 0);
  
  _fd = fd;
  _size = size;
  _buffer = header;
}

File::~File() noexcept {
  if (!_buffer) {
    assert(_fd == -1);
    return;
  }
  
  assert(_fd != -1);
  int result = munmap(_buffer, _size);
  result = close(_fd);
}

} // namespace dcl::OS

} // namespace dcl