#ifndef MDFN_FILE_H
#define MDFN_FILE_H

#include <string>
#include <stdint.h>

#ifndef INLINE
#if defined(_MSC_VER)
#define INLINE __inline
#elif defined(__GNUC__)
#define INLINE __inline__
#else
#define INLINE inline
#endif
#endif

#define MDFNFILE_EC_NOTFOUND    1
#define MDFNFILE_EC_OTHER    2

struct MDFNFILE
{
   char    *ext;
   uint8_t *data;
   int64_t size;
   int64_t location;
};

struct MDFNFILE *file_open(const char *path);
int file_close(struct MDFNFILE *file);

class PtrLengthPair
{
public:

   inline PtrLengthPair(const void *new_data, const uint64_t new_length)
   {
      data = new_data;
      length = new_length;
   }

   ~PtrLengthPair() 
   { 

   } 

   INLINE const void *GetData(void) const
   {
      return(data);
   }

   INLINE uint64_t GetLength(void) const
   {
      return(length);
   }

private:
   const void *data;
   uint64_t length;
};

#include <vector>

// These functions should be used for data like save states and non-volatile backup memory.
// "compress" is set to the zlib compression level. 0 disables compression entirely.
bool MDFN_DumpToFile(const char *filename, int compress, const void *data, const uint64_t length);
bool MDFN_DumpToFile(const char *filename, int compress, const std::vector<PtrLengthPair> &pearpairs);

#endif