/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

#if defined(_M_ARM64EC)
  #error HIGGS_ARCH arm64ec
#elif defined(_M_ARM64) || defined(__aarch64__) || defined(__ARM64__)
  #error HIGGS_ARCH aarch64
#elif (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM == 8) || defined(__ARMv8__) || defined(__ARMv8_A__)
  #error HIGGS_ARCH armv8l
#elif (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM == 7) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__) || defined(_ARM_ARCH_7) || defined(__CORE_CORTEXA__)
  #error HIGGS_ARCH armv7l
#elif (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM == 6) || defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6M__)
  #error HIGGS_ARCH armv6l
#elif (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM == 5) || defined(__ARM_ARCH_5TEJ__)
  #error HIGGS_ARCH armv5l
#elif defined(__arm__) || defined(_M_ARM)
  #error HIGGS_ARCH arm

#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)

  #error HIGGS_ARCH i386

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)

  #error HIGGS_ARCH x86_64

#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)

  #error HIGGS_ARCH ia64

#elif defined(__mips) || defined(__mips__) || defined(_M_MRX000)

  #if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
    #error HIGGS_ARCH mips64
  #else
    #error HIGGS_ARCH mips
  #endif

#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC) || defined(_M_MPPC) || defined(_M_PPC)

  #if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
    #error HIGGS_ARCH ppc64
  #else
    #error HIGGS_ARCH ppc
  #endif

#elif defined(__riscv)

  #if __riscv_xlen == 64
    #error HIGGS_ARCH riscv64
  #else
    #error HIGGS_ARCH riscv
  #endif

#else

  #error HIGGS_ARCH unknown

#endif
