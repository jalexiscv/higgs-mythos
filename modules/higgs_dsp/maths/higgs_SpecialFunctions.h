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

namespace higgs::dsp
{
/**
    Contains miscellaneous filter design and windowing functions.

    @tags{DSP}
*/
struct SpecialFunctions
{
    /** Computes the modified Bessel function of the first kind I0 for a
        given double value x. Modified Bessel functions are useful to solve
        various mathematical problems involving differential equations.
    */
    static double besselI0 (double x) noexcept;

    /** Computes the complete elliptic integral of the first kind K for a
        given double value k, and the associated complete elliptic integral
        of the first kind Kp for the complementary modulus of k.
    */
    static void ellipticIntegralK (double k, double& K, double& Kp) noexcept;

    /** Computes the Jacobian elliptic function cd for the elliptic
        modulus k and the quarter-period units u.
    */
    static Complex<double> cde (Complex<double> u, double k) noexcept;

    /** Computes the Jacobian elliptic function sn for the elliptic
        modulus k and the quarter-period units u.
    */
    static Complex<double> sne (Complex<double> u, double k) noexcept;

    /** Computes the inverse of the Jacobian elliptic function sn
        for the elliptic modulus k and the quarter-period units u.
    */
    static Complex<double> asne (Complex<double> w, double k) noexcept;
};

} // namespace higgs::dsp
