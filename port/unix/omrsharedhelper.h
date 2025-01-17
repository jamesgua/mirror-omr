/*******************************************************************************
 * Copyright IBM Corp. and others 2020
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/
#ifndef omrsharedhelper_h
#define omrsharedhelper_h

#include "omrport.h"

void omr_clearPortableError(OMRPortLibrary *portLibrary);
intptr_t omr_ControlFileOpenWithWriteLock(struct OMRPortLibrary* portLibrary, intptr_t * fd, BOOLEAN * isReadOnlyFD, BOOLEAN canCreateNewFile, const char * filename, uintptr_t groupPerm);
intptr_t omr_ControlFileCloseAndUnLock(struct OMRPortLibrary* portLibrary, intptr_t fd);
intptr_t omr_changeDirectoryPermission(struct OMRPortLibrary *portLibrary, const char* pathname, uintptr_t permission);
intptr_t omr_createDirectory(struct OMRPortLibrary *portLibrary, char *pathname, uintptr_t permission);
BOOLEAN omr_unlinkControlFile(struct OMRPortLibrary* portLibrary, const char *controlFile, OMRControlFileStatus *status);

#define OMRSH_SUCCESS 0
#define OMRSH_FAILED -1
#define OMRSH_RETRY -2
#define OMRSH_ERROR -3
#define OMRSH_FILE_DOES_NOT_EXIST -4

/* The following defines are used by omrshmem and omrshsem */
#define OMRSH_MAXPATH EsMaxPath
#define OMRSH_VERSION 0 // (EsVersionMajor*100 + EsVersionMinor)

/* Macros dealing with control file modlevel.
 * As part of Jazz 34069 the modlevel was split into major and minor parts.
 * Major level is stored in bits 16-31 and minor level is stored in bits 0-15.
 * In addition to that, on z/OS modlevel in shared memory control file is also used to store storage key in bites 12-15.
 * See macro OMRSH_SHM_CREATE_MODLEVEL_WITH_STORAGEKEY.
 * This further reduces the minor level to bits 0-11 and alters the way minor level is obtained from the mod level in shared memory control file on z/OS.
 */
/* PR 74306: increment minor level by 1.
 * Control files with major level 0, minor level 1 can be unlinked by a JVM which creates control files with higher major-minor level,
 * irrespective of the control file permission.
 */
#define OMRSH_MOD_MAJOR_LEVEL_ALLOW_READONLY_UNLINK 0
#define OMRSH_MOD_MINOR_LEVEL_ALLOW_READONLY_UNLINK 1

#define OMRSH_MOD_MAJOR_LEVEL 0
#define OMRSH_MOD_MINOR_LEVEL 2

#define OMRSH_MOD_MAJOR_LEVEL_SHIFT 16
#define OMRSH_MODLEVEL ((OMRSH_MOD_MAJOR_LEVEL << OMRSH_MOD_MAJOR_LEVEL_SHIFT) | OMRSH_MOD_MINOR_LEVEL)

#define OMRSH_GET_MOD_MAJOR_LEVEL(SOMEUDATA) ((SOMEUDATA) >> OMRSH_MOD_MAJOR_LEVEL_SHIFT)

#define OMRSH_SEM_GET_MOD_MINOR_LEVEL(SOMEUDATA) ((SOMEUDATA) & 0xFFFF)
#if defined(OMRZOS390)
/* Macros to access storage key saved in bits 12-15 in the modlevel in shared memory control file on z/OS */
#define OMRSH_SHM_MOD_MINOR_STORAGEKEY_MASK 0xF000
#define OMRSH_SHM_MOD_MINOR_STORAGEKEY_SHIFT 12
#define OMRSH_SHM_GET_MODLEVEL_STORAGEKEY(value) (((value) & OMRSH_SHM_MOD_MINOR_STORAGEKEY_MASK) >> OMRSH_SHM_MOD_MINOR_STORAGEKEY_SHIFT)
#define OMRSH_SHM_CREATE_MODLEVEL_WITH_STORAGEKEY(value) (((value) << OMRSH_SHM_MOD_MINOR_STORAGEKEY_SHIFT) | OMRSH_MODLEVEL)

#define OMRSH_SHM_GET_MOD_MINOR_LEVEL(SOMEUDATA) ((SOMEUDATA) & 0xFFF)
#else /* defined(OMRZOS390) */
#define OMRSH_SHM_GET_MOD_MINOR_LEVEL(SOMEUDATA) ((SOMEUDATA) & 0xFFFF)
#endif /* defined(OMRZOS390) */

#if defined(AIXPPC)
uintptr_t omr_isPageProtectionPossible(struct OMRPortLibrary* portLibrary, const char* cacheDirName, uintptr_t groupPerm, uintptr_t regionSize);
#endif

#endif     /* omrsharedhelper_h */
