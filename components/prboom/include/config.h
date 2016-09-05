/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Uncomment this to exhaustively run memory checks while the game is running
   (this is EXTREMELY slow). */
/* #undef CHECKHEAP */

/* Define for support for MBF helper dogs */
//#define DOGS 0


#define HAVE_STRLWR 1

/* Define to be the path where Doom WADs are stored */
#define DOOMWADDIR ""

/* Define if you are building with OpenGL support */
//#define GL_DOOM 1

/* Define to 1 if you have the <asm/byteorder.h> header file. */
//#define HAVE_ASM_BYTEORDER_H 0

/* Define to 1 if you have the declaration of `sys_siglist', and to 0 if you
   don't. */
#define HAVE_DECL_SYS_SIGLIST 0

/* Define to 1 if you have the `getopt' function. */
#undef HAVE_GETOPT
#define HAVE_GETOPT 1

/* Define to 1 if you have the `inet_aton' function. */
#define HAVE_INET_ATON 1

/* Define to 1 if you have the `inet_ntop' function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have the `inet_pton' function. */
#define HAVE_INET_PTON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have struct sockaddr_in6 */
#define HAVE_IPv6 0

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `png' library (-lpng). */
#define HAVE_LIBPNG 0

/* Define to 1 if you have the `SDL_mixer' library (-lSDL_mixer). */
/* #undef HAVE_LIBSDL_MIXER */

/* Define if you have the SDL net library -lSDL_net */
/* #undef HAVE_LIBSDL_NET */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mmap' function. */
#define HAVE_MMAP 1

/* Define if you want network game support */
/* #undef HAVE_NET */

/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H 0

/* Define to 1 if you have the `sched_setaffinity' function. */
#define HAVE_SCHED_SETAFFINITY 0

/* Define to 1 if you have the `SDL_JoystickGetAxis' function. */
#define HAVE_SDL_JOYSTICKGETAXIS 0

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 0

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 0

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 0

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 0

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 0

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Uncomment this to cause heap dumps to be generated. Only useful if
   INSTRUMENTED is also defined. */
/* #undef HEAPDUMP */

/* Define for high resolution support */
#define HIGHRES 0

/* Define on targets supporting 386 assembly */
/* #undef I386_ASM */

/* Define this to see real-time memory allocation statistics, and enable extra
   debugging features */
/* #undef INSTRUMENTED */

/* If your platform has a fast version of max, define MAX to it */
/* #undef MAX */

/* If your platform has a fast version of min, define MIN to it */
/* #undef MIN */

/* Name of package */
#define PACKAGE "prboom"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "prboom"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "prboom 2.5.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "prboom"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.5.0"

/* Set to the attribute to apply to struct definitions to make them packed */
#define PACKEDATTR __attribute__((packed))

/* Define to enable internal range checking */
/* #undef RANGECHECK */

/* Define if you have an old SDL_net, such that the UDPpacket structure has a
   src member instead of an address member */
/* #undef SDL_NET_UDP_PACKET_SRC */

/* When defined this causes quick checks which only impose significant
   overhead if a posible error is detected. */
#define SIMPLECHECKS 1

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Defining this causes time stamps to be created each time a lump is locked,
   and lumps locked for long periods of time are reported */
/* #undef TIMEDIAG */

/* Define if you want to use gluImageScale */
#define USE_GLU_IMAGESCALE 0

/* Define if you want to use gluBuild2DMipmaps */
#define USE_GLU_MIPMAP 0

/* Define if you want to use the SDL net lib */
/* #undef USE_SDL_NET */

/* Version number of package */
#define VERSION "2.5.0"

/* Define if using the dmalloc debugging malloc package */
/* #undef WITH_DMALLOC */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define this to perform id checks on zone blocks, to detect corrupted and
   illegally freed blocks */
#define ZONEIDCHECK 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to strcasecmp, if we have it */
#define stricmp strcasecmp

/* Define to strncasecmp, if we have it */
#define strnicmp strncasecmp

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */


#define atexit(a) 
//#define exit(a) assert(a!=0)


