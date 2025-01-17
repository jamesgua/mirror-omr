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

#if !defined(OMRSOCK_H_)
#define OMRSOCK_H_

/* According to AIX documentation, this file is needed. */
#if defined(OMR_OS_AIX)
#include <sys/socketvar.h>
#endif

/* According to ZOS documentation, this definition is needed. */
#if defined(OMR_OS_ZOS) && !defined(_OE_SOCKETS)
#define _OE_SOCKETS
#endif

/* This exposes some definitions needed by the socket api */
#if defined(OMR_OS_ZOS) && !defined(_OPEN_SYS_SOCK_IPV6)
#define _OPEN_SYS_SOCK_IPV6
#endif

#include <sys/types.h> /* Some historical implementations need this file, POSIX.1-2001 does not. */
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h> /* Must come before <netinet/tcp.h> */
#include <netinet/tcp.h>

typedef struct sockaddr omr_os_sockaddr;
typedef struct sockaddr_in omr_os_sockaddr_in; /* IPv4 */
typedef struct sockaddr_in6 omr_os_sockaddr_in6; /* IPv6 */

/* Address Family */
#define OS_SOCK_AF_UNSPEC AF_UNSPEC
#define OS_SOCK_AF_INET AF_INET
#define OS_SOCK_AF_INET6 AF_INET6

/* Socket types */
#define OS_SOCK_ANY 0
#define OS_SOCK_STREAM SOCK_STREAM
#define OS_SOCK_DGRAM SOCK_DGRAM

/* Protocol */
#define OS_SOCK_IPPROTO_DEFAULT 0
#define OS_SOL_SOCKET SOL_SOCKET
#define OS_SOCK_IPPROTO_TCP IPPROTO_TCP
#define OS_SOCK_IPPROTO_UDP IPPROTO_UDP

/* Socket options */
#define OS_SO_REUSEADDR SO_REUSEADDR
#define OS_SO_KEEPALIVE SO_KEEPALIVE
#define OS_SO_LINGER SO_LINGER
#define OS_SO_RCVTIMEO SO_RCVTIMEO
#define OS_SO_SNDTIMEO SO_SNDTIMEO
#define OS_TCP_NODELAY TCP_NODELAY

/* Socket Flags */
#if defined(J9ZOS390)
#define OS_O_ASYNC O_ASYNCSIG
#elif defined(AIXPPC)
#define OS_O_ASYNC FASYNC
#else
#define OS_O_ASYNC O_ASYNC
#endif
#define OS_O_NONBLOCK O_NONBLOCK

/* Socket Poll */
#define OS_POLLIN POLLIN
#define OS_POLLOUT POLLOUT

#if !defined(AIXPPC)
#define OS_POLLERR POLLERR
#define OS_POLLNVAL POLLNVAL
#define OS_POLLHUP POLLHUP
#endif

#endif /* !defined(OMRSOCK_H_) */
