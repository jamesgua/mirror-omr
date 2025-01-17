/*******************************************************************************
 * Copyright IBM Corp. and others 1991
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
 * @ingroup Port
 * @brief process introspection support
 */

#include "omrport.h"

/*
 * Creates an iterator for the native threads present within the process, either all native threads
 * or a subset depending on platform (see platform specific documentation for detail).
 * This function will suspend all of the native threads that will be presented through the iterator
 * and they will remain suspended until this function or nextDo return NULL. The context for the
 * calling thread is always presented first.
 *
 * @param heap used to contain the thread context, the stack frame representations and symbol strings. No
 * 			memory is allocated outside of the heap provided. Must not be NULL.
 * @param state semi-opaque structure used as a cursor for iteration. Must not be NULL User visible fields
 *			are:
 * 			deadline - the system time in seconds at which to abort introspection and resume
 * 			error - numeric error description, 0 on success
 * 			error_detail - detail for the specific error
 * 			error_string - string description of error
 * @param signal_info a signal context to use. This will be used in place of the live context for the
 * 			calling thread.
 *
 * @return NULL if there is a problem suspending threads, gathering thread contexts or if the heap
 * is too small to contain even the context without stack trace. Errors are reported in the error,
 * and error_detail fields of the state structure. A brief textual description is in error_string.
 */
J9PlatformThread *
omrintrospect_threads_startDo_with_signal(struct OMRPortLibrary *portLibrary, J9Heap *heap, J9ThreadWalkState *state, void *signal_info)
{
	return NULL;
}

/* This function is identical to omrintrospect_threads_startDo_with_signal but omits the signal argument
 * and instead uses a live context for the calling thread.
 */
J9PlatformThread *
omrintrospect_threads_startDo(struct OMRPortLibrary *portLibrary, J9Heap *heap, J9ThreadWalkState *state)
{
	return NULL;
}

/* This function is called repeatedly to get subsequent threads in the iteration. The only way to
 * resume suspended threads is to continue calling this function until it returns NULL.
 *
 * @param state state structure initialized by a call to one of the startDo functions.
 *
 * @return NULL if there is an error or if no more threads are available. Sets the error fields as
 * listed detailed for omrintrospect_threads_startDo_with_signal.
 */
J9PlatformThread *
omrintrospect_threads_nextDo(J9ThreadWalkState *state)
{
	return NULL;
}

/**
 * Adjust which signal is used to suspend threads. The signal used is SIGRTMIN+signalOffset.
 * @param portLibrary port library
 * @param signalOffset value in the range 0 to (SIGRTMAX-SIGRTMIN).
 * @return 0 if successful, OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM on non-Linux systems, OMRPORT_ERROR_INVALID if the signal is reserved or out of range.
 */
int32_t
omrintrospect_set_suspend_signal_offset(struct OMRPortLibrary *portLibrary, int32_t signalOffset)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Initialize settings to default values.
 * @param portLibrary port library
 * @return 0 on success, negative value on failure
 */
int32_t
omrintrospect_startup(struct OMRPortLibrary *portLibrary)
{
	return 0;
}

/**
 * Close down the component.
 * @param portLibrary port library
 */
void
omrintrospect_shutdown(struct OMRPortLibrary *portLibrary)
{
	return;
}

