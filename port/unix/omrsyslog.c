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
 * @brief System logging support
 */
#include <string.h>
#include <syslog.h>
#include "omrportpriv.h"
#include "ut_omrport.h"

/* Used to add space for the null terminator. Adding a wchar in case the platform
 * locale uses wchar
 */
#define NULL_TERM_SZ       sizeof(wchar_t)

static uintptr_t get_conv_txt_num_bytes(struct OMRPortLibrary *portLibrary, const char *message);
static void syslog_write(struct OMRPortLibrary *portLibrary, const char *buf, int priority);

uintptr_t
omrsyslog_write(struct OMRPortLibrary *portLibrary, uintptr_t flags, const char *message)
{
	if (NULL != portLibrary->portGlobals && TRUE == PPG_syslog_enabled) {
		int priority;

		switch (flags) {
		case J9NLS_ERROR:
			priority = LOG_ERR;
			break;
		case J9NLS_WARNING:
			priority = LOG_WARNING;
			break;
		case J9NLS_INFO:
		default:
			priority = LOG_INFO;
			break;
		}

		syslog_write(portLibrary, message, priority);
		return TRUE;
	}
	return FALSE;
}

uintptr_t
syslogOpen(struct OMRPortLibrary *portLibrary, uintptr_t flags)
{
	int logopt = LOG_PID | LOG_NOWAIT | LOG_ODELAY;
	int facility = LOG_USER;
	char *defaultEventSource = "IBM Java";
	char *syslogEventSource;

	/* look for the name that will be used for logging (internal use only) */
	syslogEventSource = getenv("IBM_JAVA_SYSLOG_NAME");

	if (NULL == syslogEventSource) {
		/* no name found so use the default one */
		/* note: openlog returns void */
		openlog(defaultEventSource, logopt, facility);
	} else {
		/* found the name, so use it */
		/* note: openlog returns void */
		openlog(syslogEventSource, logopt, facility);
	}

	if (NULL != portLibrary->portGlobals) {
		PPG_syslog_enabled = TRUE;
		return TRUE;
	}
	return FALSE;
}

uintptr_t
syslogClose(struct OMRPortLibrary *portLibrary)
{
	/* note: closelog returns void */
	closelog();
	if (NULL != portLibrary->portGlobals) {
		PPG_syslog_enabled = FALSE;
		return TRUE;
	}
	return FALSE;
}

uintptr_t
omrsyslog_query(struct OMRPortLibrary *portLibrary)
{
	uintptr_t options;

	/* query the logging options here */
	options = PPG_syslog_flags;

	return options;
}

void
omrsyslog_set(struct OMRPortLibrary *portLibrary, uintptr_t options)
{
	/* set the logging options here */
	PPG_syslog_flags = options;
}

/**
 * @return Zero on error. Otherwise, the number of bytes required to hold the
 * converted text (including the null terminator char)
 */
static uintptr_t
get_conv_txt_num_bytes(struct OMRPortLibrary *portLibrary, const char *message)
{
	uintptr_t retval = 0U;

	const int32_t convRes =
		portLibrary->str_convert(portLibrary, J9STR_CODE_MUTF8,
								 J9STR_CODE_PLATFORM_RAW, message, strlen(message),
								 NULL, 0U);

	if (0 < convRes) {
		retval = (uintptr_t) convRes + NULL_TERM_SZ;
	} else {
		Trc_PRT_omrsyslog_failed_str_convert(convRes);
	}

	return retval;
}

/**
 * Convert message to local code page and write to syslog.
 * Based on original code in file_write_using_iconv()
 *
 * @param[in] portLibrary - port library handle
 * @param[in] buf - character string message
 * @param[in] priority -
 *
 * @return void
 */
static void
syslog_write(struct OMRPortLibrary *portLibrary, const char *buf, int priority)
{
#if defined(OMRZTPF)
	sprintf(buf, "%s", buf);
	syslog(priority, "%s", buf);
	return;
#else
	const uintptr_t lclMsgLen = get_conv_txt_num_bytes(portLibrary, buf);
	char *lclMsg = NULL;
	BOOLEAN lclConvSuccess = FALSE;

	if (NULL_TERM_SZ < lclMsgLen) {
		lclMsg = portLibrary->mem_allocate_memory(portLibrary,
				 lclMsgLen, OMR_GET_CALLSITE(),
				 OMRMEM_CATEGORY_PORT_LIBRARY);

		if (NULL != lclMsg) {
			/* If we got here, we are guaranteed to have enough
			 * space for the converted string and the null
			 * terminator char. Under these circumstances,
			 * str_convert routine guarantees that the resulting
			 * string will be null terminated.
			 */
			const int32_t convRes =
				portLibrary->str_convert(portLibrary,
										 J9STR_CODE_MUTF8, J9STR_CODE_PLATFORM_RAW,
										 buf, strlen(buf), lclMsg, lclMsgLen);

			lclConvSuccess = (BOOLEAN)(convRes >= 0);

			if (! lclConvSuccess) {
				Trc_PRT_omrsyslog_failed_str_convert(convRes);
			}
		}
	}

	if (lclConvSuccess) {
		syslog(priority, "%s", lclMsg);
	} else {
		syslog(priority, "%s", buf);
	}

	if (NULL != lclMsg) {
		portLibrary->mem_free_memory(portLibrary, lclMsg);
	}

	return;
#endif
}
