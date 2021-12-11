/*
https://github.com/Eren121/CppColorGradient

MIT License

Copyright (c) 2021 Eren121

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef Gradient_H
#define Gradient_H

#define _USE_MATH_DEFINES

#include <valarray>
#include <ostream>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <functional>
#include <cassert>
#include <cmath>

#define GRADIENT_KEY_DEFAULT_PRECISION double
#define GRADIENT_COLOR_PRECISION_TYPE double
#define GRADIENT_VALARRAY_FORMAT_OPEN "("
#define GRADIENT_VALARRAY_FORMAT_CLOSE ")"
#define GRADIENT_VALARRAY_FORMAT_SEPARATOR ", "

/////////////////////////////////////////////////////////////////////////////
// Interpolation method  ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

namespace gradient {

    template<typename T>
    T linear_interpolation(T x) {
        return x;
    }

    template<typename T>
    T cosine_interpolation(T x) {
        return T(1) - (cos(x * T(M_PI)) + T(1)) / T(2);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Gradient  ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

namespace gradient {
    using default_precision = GRADIENT_KEY_DEFAULT_PRECISION;
    template<typename ratio> using Interpolation = std::function<ratio(ratio x)>;

    template<typename T, typename precision>
    class GradientBase {
    public:
        explicit GradientBase(const Interpolation<precision>& interpolation)
            : interpolationMethod(interpolation) {}
        GradientBase()
                : interpolationMethod(linear_interpolation<precision>) {}
        virtual ~GradientBase() = default;

        Interpolation<precision>& interpolation() { return this->interpolationMethod; }
        const Interpolation<precision>& interpolation() const { return this->interpolationMethod; }
        void setInterpolation(const Interpolation<precision>& interp) { this->interpolationMethod = interp;}

        T interpolate(const T& lowerBound, const T& upperBound, precision r) const {
            assert(interpolationMethod);
            r = interpolationMethod(r);
            return upperBound * r + lowerBound * (precision(1) - r);
        }

    private:
        Interpolation<precision> interpolationMethod;
    };

    template<typename T, typename precision = default_precision>
    class LinearGradient : public GradientBase<T, precision> {
    public:
        using GradientBase<T, precision>::GradientBase;
        ~LinearGradient() override = default;
        T& operator[](precision keyPoint);
        const T& operator[](precision keyPoint) const;
        T operator()(precision point) const;

    private:
        std::map<precision, T> keyPoints;
    };
}

/////////////////////////////////////////////////////////////////////////////
// Convenience operators  ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

namespace gradient {

    namespace operators {

        template<typename T, typename value_type = typename T::value_type>
        std::ostream &operator<<(std::ostream &ostream, const T &valarray) {
            ostream << GRADIENT_VALARRAY_FORMAT_OPEN;
            bool firstValue = true;
            std::for_each(begin(valarray), end(valarray), [&firstValue, &ostream](const value_type &element) {
                if (!firstValue) {
                    ostream << GRADIENT_VALARRAY_FORMAT_SEPARATOR;
                } else {
                    firstValue = false;
                }
                ostream << element;
            });
            ostream << GRADIENT_VALARRAY_FORMAT_CLOSE;
            return ostream;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// LinearGradient implementation  ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

namespace gradient {

    template<typename T, typename precision>
    T &LinearGradient<T, precision>::operator[](precision keyPoint) {
        return this->keyPoints[keyPoint];
    }

    template<typename T, typename precision>
    const T& LinearGradient<T, precision>::operator[](precision keyPoint) const {
        return this->keyPoints.at(keyPoint);
    }

    template<typename T, typename precision>
    T LinearGradient<T, precision>::operator()(precision point) const {

        assert(!this->keyPoints.empty());
        T returnValue;

        const auto upper_bound_it = this->keyPoints.upper_bound(point);


        if(upper_bound_it == this->keyPoints.end()) {
            returnValue = this->keyPoints.rbegin()->second;
        }
        else {
            if (upper_bound_it == this->keyPoints.begin()) {
                returnValue = this->keyPoints.begin()->second;
            } else {
                const auto lower_bound_it = prev(upper_bound_it);

                const T &min = lower_bound_it->second;
                const T &max = upper_bound_it->second;
                const precision min_point = lower_bound_it->first;
                const precision max_point = upper_bound_it->first;
                const precision ratio = (point - min_point) / (max_point - min_point);
                returnValue = this->interpolate(min, max, ratio);
            }
        }

        return returnValue;
    }
}

/////////////////////////////////////////////////////////////////////////////
// User-Defined Types  //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

namespace gradient {

    using Color = std::valarray<GRADIENT_COLOR_PRECISION_TYPE>;
    using LinearColorGradient = LinearGradient<Color>;

    template class LinearGradient<Color>;
    template void GradientBase<Color, default_precision >::setInterpolation(const Interpolation<default_precision> &interpolation);
}

#endif
