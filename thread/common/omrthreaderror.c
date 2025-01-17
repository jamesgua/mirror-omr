/*******************************************************************************
 * Copyright IBM Corp. and others 2007
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

/**
 * @file
 * @ingroup Thread
 * @brief Threading and synchronization support
 */

#include "omrthread.h"
#include "threaddef.h"

/**
 * Retrieve the os_errno saved in the current omrthread_t.
 * @return OS-specific error code
 * @retval J9THREAD_INVALID_OS_ERRNO One of the following occurred: 1. the calling thread wasn't attached to the thread library; 2. the error code wasn't set.
 */
omrthread_os_errno_t
omrthread_get_os_errno(void)
{
	omrthread_os_errno_t os_errno = J9THREAD_INVALID_OS_ERRNO;
	omrthread_t self = MACRO_SELF();

	if (self) {
		os_errno = self->os_errno;
	}

	return os_errno;
}

#if defined(J9ZOS390)
/**
 * Retrieve the os_errno2 saved in the current omrthread_t.
 * @return ZOS-specific error code. Default value is 0.
 */
omrthread_os_errno_t
omrthread_get_os_errno2(void)
{
	omrthread_os_errno_t os_errno2 = 0;
	omrthread_t self = MACRO_SELF();

	if (self) {
		os_errno2 = (uintptr_t)self->os_errno2;
	}

	return os_errno2;
}
#endif /* J9ZOS390 */


/**
 * Retrieve a string description corresponding to a J9THREAD_ERR_xxx value.
 * Currently only supports return values from omrthread_create().
 * @param[in] err
 * @return string description. Don't modify or free this string.
 */
const char *
omrthread_get_errordesc(intptr_t err)
{
	/* the indexing of this array must match the defined return values */
	static const char *omrthread_create_errordesc[] = {
		"" /* SUCCESS */,
		"" /* ERR */,
		"Invalid priority" /* ERR_INVALID_PRIORITY */,
		"Can't allocate omrthread_t" /* ERR_CANT_ALLOCATE_J9THREAD_T */,
		"Can't init condition" /* ERR_CANT_INIT_CONDITION */,
		"Can't init mutex" /* ERR_CANT_INIT_MUTEX */,
		"Thread create failed" /* ERR_THREAD_CREATE_FAILED */,
		"Invalid create attr" /* ERR_INVALID_CREATE_ATTR */,
		"Can't allocate create attr" /* ERR_CANT_ALLOC_CREATE_ATTR */,
		"Can't allocate stack" /* ERR_CANT_ALLOC_STACK */
	};
	const char *errdesc = "";

	/* All return values from omrthread_create() are negative */
	if (err < 0) {
		err = -err;
		err &= ~J9THREAD_ERR_OS_ERRNO_SET;
		if ((err >= 0) && (err <= J9THREAD_ERR_CANT_ALLOC_STACK)) {
			errdesc = omrthread_create_errordesc[err];
		}
	}

	return errdesc;
}
