
#include <dcl/IO/File.h>

// BSD system includes
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace dcl {

namespace IO {

namespace {

DCL_ALWAYS_INLINE
DCL_CONSTEXPR
static uint32_t makeFlags(Permissions permissions) noexcept {
  uint32_t flags = O_RDONLY;

  if (permissions == Permissions::Read) {
    return O_RDONLY;
  }

  if (permissions == Permissions::Write) {
    return O_WRONLY;
  }
  
  if (permissions == (Permissions::Write | Permissions::Read)) {
    return O_RDONLY | O_WRONLY;
  }

  return O_RDONLY;
}

DCL_ALWAYS_INLINE
DCL_CONSTEXPR
static int makeProt(Permissions permissions) noexcept {
  int prot = PROT_NONE;

  if ((permissions & Permissions::Read) != Permissions::None) {
    prot |= PROT_READ;
  }
  
  if ((permissions & Permissions::Write) != Permissions::None) {
    prot |= PROT_WRITE;
  }

  if ((permissions & Permissions::Execute) != Permissions::None) {
    prot |= PROT_EXEC;
  }

  return prot;
}

} // namespace

File::File(const char *path, Permissions permissions) noexcept {
  int fd = open(path, makeFlags(permissions), nullptr);

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

  void *header = mmap(0, size, makeProt(permissions), MAP_SHARED, fd, 0);

  _fd = fd;
  _size = size;
  _buffer = header;
}

File::~File() noexcept {
  if (!_buffer) {
    DCLAssert(_fd == -1);
    return;
  }

  DCLAssert(_fd != -1);
  int result = munmap(_buffer, _size);
  result = close(_fd);
}

} // namespace IO

} // namespace dcl