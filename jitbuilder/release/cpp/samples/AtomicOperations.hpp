/*******************************************************************************
 * Copyright IBM Corp. and others 2016
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


#ifndef ATOMICOPS_INCL
#define ATOMICOPS_INCL

#include "JitBuilder.hpp"

/**
 * I created two methodBuilders for testing
 * Int32 / Int64 atomicadd() respectively
 */

class AtomicInt32Add : public OMR::JitBuilder::MethodBuilder
   {
   private:
   OMR::JitBuilder::IlType *pInt32;

   public:
   AtomicInt32Add(OMR::JitBuilder::TypeDictionary *);
   virtual bool buildIL();
   };

class AtomicInt64Add : public OMR::JitBuilder::MethodBuilder
   {   
   private:
   OMR::JitBuilder::IlType *pInt64;

   public:
   AtomicInt64Add(OMR::JitBuilder::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // !defined(ATOMICOPS_INCL)
