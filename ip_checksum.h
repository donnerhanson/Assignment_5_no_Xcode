
// This is necessary because the IP checksum function is in C and is wrapped when using C++ compilation

// REFERENCE: http://ehuss.com/shared/
/* NOTE FROM REFERENCE: If you are calling and compiling C code with a C++ compiler, you need to make sure that the C++ compiler does not mangle the name by wrapping your C function declarations such as:
*/

#ifdef  __cplusplus
extern "C" {
#endif
#include <string.h>
unsigned int ip_checksum(unsigned char *data, int length);
#ifdef  __cplusplus
}
#endif
