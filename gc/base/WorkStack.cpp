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

#include "omr.h"
#include "omrport.h"

#include "WorkStack.hpp"

#include "EnvironmentBase.hpp"
#include "WorkPackets.hpp"
#include "Packet.hpp"
#include "Task.hpp"

#include "ModronAssertions.h"

/**
 * Reset stack 
 * 
 * @param workpacket - Reference to work packets object
 *
 */
void
MM_WorkStack::reset(MM_EnvironmentBase *env, MM_WorkPackets *workPackets)
{
	_workPackets = workPackets;
	/* if any of these are non-NULL, we would be leaking memory */
	Assert_MM_true(NULL == _inputPacket);
	Assert_MM_true(NULL == _outputPacket);
	Assert_MM_true(NULL == _deferredPacket);
}

void
MM_WorkStack::prepareForWork(MM_EnvironmentBase *env, MM_WorkPackets *workPackets)
{
	if (NULL == _workPackets) {
		_workPackets = workPackets;
		/* this is our first time using this work stack instance so the packets should be NULL */
		Assert_MM_true(NULL == _inputPacket);
		Assert_MM_true(NULL == _outputPacket);
		Assert_MM_true(NULL == _deferredPacket);
	} else {
		Assert_MM_true(_workPackets == workPackets);
	}
}

/**
 * Flush stack object
 * 
 * Return all packets owned by stack object to appropriate lists
 * 
 */
void
MM_WorkStack::flush(MM_EnvironmentBase *env)
{
	if(NULL != _inputPacket) {
		_workPackets->putPacket(env, _inputPacket);
		_inputPacket = NULL;
	}
	if(NULL != _outputPacket) {
		_workPackets->putPacket(env, _outputPacket);
		_outputPacket = NULL;
	}
	if(NULL != _deferredPacket) {
		_workPackets->putDeferredPacket(env, _deferredPacket);
		_deferredPacket = NULL;
	}	
	_workPackets = NULL;
}

/**
 * Push to a deferred packet.
 * 
 * Push a reference to a deferred packet, if any are available, 
 * to defer tracing of an object for now. If no deferred packet
 * available then push to output packet. 
 *
 * @param element - reference to object whose tracing is to be deferred
 *
 */
void
MM_WorkStack::pushDefer(MM_EnvironmentBase *env, void *element)
{
	if(NULL != _deferredPacket) {
		if(_deferredPacket->push(env, element)) {
			return;
		} else {
			/* The deferred packet is full - move it to the input list */
			_workPackets->putDeferredPacket(env, _deferredPacket);
		}	
	}

	/* Get a new deferred packet */
	if ( NULL != (_deferredPacket = _workPackets->getDeferredPacket(env))) {
		/* Output packets must guarantee at least 2 free entries */
		_deferredPacket->push(env, element);	
	} else {
		/* No more deferred packets available, so just push to output
		 * in the hope the next time its popped we can trace into it. 
		 */  
		push(env, element);
	}
}

/*
 * Pop a reference from input packet in case if it can be done for current packet (empty or does not exist)
 * 
 * Pop a reference from the stacks input packet. There are no references remain
 * in current packet so get a new input packet. If no more input packets
 * available then return NULL.
 * 
 * @return Object reference or NULL if all input packets empty. 
 *
 */
void *
MM_WorkStack::popNoWaitFailed(MM_EnvironmentBase *env)
{
	if(NULL != _inputPacket) {
		/* The current input packet has been used up - return it to the output list for resuse */
		_workPackets->putPacket(env, _inputPacket);
		_inputPacket = NULL;
	}

	bool tryRetrieveInputPacket = true;
#if defined(OMR_GC_VLHGC)
	if ((NULL != env->_currentTask) && env->_currentTask->shouldYieldFromTask(env)) {
		tryRetrieveInputPacket = false;
	}
#endif /* OMR_GC_VLHGC */

	if (tryRetrieveInputPacket) {
		if (retrieveInputPacket(env)) {
			/* Any entry on the _inputPacket list must have at least 1 entry */
			void* result = _inputPacket->pop(env);
			return result;
		}
	}

	return NULL;
}

/*
 * Peek at reference on top of output packet
 * Return reference at the top of the current output packet but leave 
 * reference in packet. If no input packet or packet empty then return
 * NULL.
 *
 * @return Reference at top of current input packet, or NULL if packet
 * empty.
 */
void *
MM_WorkStack::peek(MM_EnvironmentBase *env)
{
	if(NULL != _inputPacket) {
		void *result = _inputPacket->peek(env);
		return result;
	}
	
	return NULL;
}

/*
 * Pop a reference from input packet in case if it can be done for current packet (empty or does not exist)
 * 
 * Pop a reference from the stacks input packet. There are no references remain
 * in current packet so we wait for an input packet to become available
 * When all packets have been processed return NULL.
 * 
 * @return Object reference or NULL all packets procesed.
 *
 */
void *
MM_WorkStack::popFailed(MM_EnvironmentBase *env)
{
	if(NULL != _inputPacket) {
		/* The current input packet has been used up - return it to the output list for reuse */
		_workPackets->putPacket(env, _inputPacket);
		_inputPacket = NULL;
	}

	bool tryRetrieveInputPacket = true;
#if defined(OMR_GC_VLHGC)
	if ((NULL != env->_currentTask) && env->_currentTask->shouldYieldFromTask(env)) {
		tryRetrieveInputPacket = false;
	}
#endif /* OMR_GC_VLHGC */

	if (tryRetrieveInputPacket) {
		/* Fetch a new input packet if there is one available */
		if (retrieveInputPacket(env)) {
			void* result = _inputPacket->pop(env);
			return result;
		}
	}

	/* Nothing is immediately available, wait for an input packet to arrive */
	_inputPacket = _workPackets->getInputPacket(env);
	if(NULL != _inputPacket) {
		/* Any entry on the _inputPacket list must have at least 1 entry */
		void* result = _inputPacket->pop(env);
		return result;
	}

	return NULL;
}

void MM_WorkStack::pushFailed(MM_EnvironmentBase *env, void *element)
{
	if(_outputPacket) {
		/* The output packet is full - move it to the input list */
		_workPackets->putOutputPacket(env, _outputPacket);
	}

	/* Get a new output packet */
	_outputPacket = _workPackets->getOutputPacket(env);
	if (NULL == _outputPacket) {
		_workPackets->overflowItem(env, element, OVERFLOW_TYPE_WORKSTACK);
	} else {
		/* Output packets must guarantee at least 2 free entries */
		_outputPacket->push(env, element);
		_pushCount++;
	}
}

void MM_WorkStack::pushFailed(MM_EnvironmentBase *env, void *element1, void *element2)
{
	if(_outputPacket) {
		/* The output packet is full - move it to the input list */
		_workPackets->putOutputPacket(env, _outputPacket);
	}

	/* Get a new output packet */
	_outputPacket = _workPackets->getOutputPacket(env);
	if (NULL == _outputPacket) {
		_workPackets->overflowItem(env, element1, OVERFLOW_TYPE_WORKSTACK);
		_workPackets->overflowItem(env, element2, OVERFLOW_TYPE_WORKSTACK);
	} else {
		/* Output packets must guarantee at least 2 free entries */
		_outputPacket->push(env, element1, element2);
		_pushCount += 2;
	}
}

/**
 * Immediately flush the output packet back to the shared pool so that it can be processed
 * by another thread.
 * @param env[in] The thread which owns the work stack
 */
void
MM_WorkStack::flushOutputPacket(MM_EnvironmentBase *env)
{
	if (NULL != _outputPacket) {
		_workPackets->putOutputPacket(env, _outputPacket);
		_outputPacket = NULL;
	}
}

void *
MM_WorkStack::popNoWaitFromCurrentInputPacket(MM_EnvironmentBase *env)
{
	void *result = NULL;

	if(NULL != _inputPacket) {
		result = _inputPacket->pop(env);
		if (NULL == result) {
			/* The current input packet has been used up - return it to the output list for reuse */
			_workPackets->putPacket(env, _inputPacket);
			_inputPacket = NULL;
		}
	}
	return result;
}

bool
MM_WorkStack::inputPacketAvailableFromWorkPackets(MM_EnvironmentBase *env)
{
	return _workPackets->inputPacketAvailable(env);
}

bool
MM_WorkStack::retrieveInputPacket(MM_EnvironmentBase *env)
{
	_inputPacket = _workPackets->getInputPacketNoWait(env);
	if (NULL == _inputPacket) {
		/* If the output packet contains at least a free entry - invert the input/output */
		if((NULL != _outputPacket) && !_outputPacket->isEmpty()) {
			/* swap the input packet with the output packet */
			_inputPacket = _outputPacket;
			_outputPacket = NULL;
			env->_workPacketStats.workPacketsExchanged += 1;
		}
	}
	return (NULL != _inputPacket);
}

