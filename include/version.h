#ifndef VERSION_H
#define VERSION_H

/*
 * Build version selection. configure.py defines exactly one of:
 *   VERSION_JUL12 - July 12th NTSC-J prototype
 *   VERSION_PAL   - Europe/Australia retail (SCES-50408)
 * and force-includes this header into every EE source file.
 */

/* Retail builds compiled most debug printf calls out. */
#if defined(VERSION_PAL)
#define DPRINTF(args...) ((void)0)
#else
#define DPRINTF printf
#endif

#endif /* VERSION_H */
