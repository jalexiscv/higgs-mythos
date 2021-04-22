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

namespace higgs
{

RelativeTime::RelativeTime (const double secs) noexcept           : numSeconds (secs) {}
RelativeTime::RelativeTime (const RelativeTime& other) noexcept   : numSeconds (other.numSeconds) {}
RelativeTime::~RelativeTime() noexcept {}

//==============================================================================
RelativeTime RelativeTime::milliseconds (int milliseconds) noexcept         { return RelativeTime ((double) milliseconds * 0.001); }
RelativeTime RelativeTime::milliseconds (int64 milliseconds) noexcept       { return RelativeTime ((double) milliseconds * 0.001); }
RelativeTime RelativeTime::seconds (double s) noexcept                      { return RelativeTime (s); }
RelativeTime RelativeTime::minutes (double numberOfMinutes) noexcept        { return RelativeTime (numberOfMinutes * 60.0); }
RelativeTime RelativeTime::hours (double numberOfHours) noexcept            { return RelativeTime (numberOfHours * (60.0 * 60.0)); }
RelativeTime RelativeTime::days (double numberOfDays) noexcept              { return RelativeTime (numberOfDays  * (60.0 * 60.0 * 24.0)); }
RelativeTime RelativeTime::weeks (double numberOfWeeks) noexcept            { return RelativeTime (numberOfWeeks * (60.0 * 60.0 * 24.0 * 7.0)); }

//==============================================================================
int64 RelativeTime::inMilliseconds() const noexcept { return (int64) to<Milliseconds>(); }
double RelativeTime::inMinutes() const noexcept     { return to<Minutes>(); }
double RelativeTime::inHours() const noexcept       { return to<Hours>(); }
double RelativeTime::inDays() const noexcept        { return to<Days>(); }
double RelativeTime::inWeeks() const noexcept       { return to<Weeks>(); }

//==============================================================================
RelativeTime& RelativeTime::operator= (const RelativeTime& other) noexcept      { numSeconds = other.numSeconds; return *this; }

RelativeTime RelativeTime::operator+= (RelativeTime t) noexcept     { numSeconds += t.numSeconds; return *this; }
RelativeTime RelativeTime::operator-= (RelativeTime t) noexcept     { numSeconds -= t.numSeconds; return *this; }
RelativeTime RelativeTime::operator+= (double secs) noexcept        { numSeconds += secs; return *this; }
RelativeTime RelativeTime::operator-= (double secs) noexcept        { numSeconds -= secs; return *this; }

HIGGS_API RelativeTime HIGGS_CALLTYPE operator+ (RelativeTime t1, RelativeTime t2) noexcept  { return t1 += t2; }
HIGGS_API RelativeTime HIGGS_CALLTYPE operator- (RelativeTime t1, RelativeTime t2) noexcept  { return t1 -= t2; }

HIGGS_API bool HIGGS_CALLTYPE operator== (RelativeTime t1, RelativeTime t2) noexcept
{
    return exactlyEqual (t1.inSeconds(), t2.inSeconds());
}

HIGGS_API bool HIGGS_CALLTYPE operator!= (RelativeTime t1, RelativeTime t2) noexcept
{
    return ! (t1 == t2);
}

HIGGS_API bool HIGGS_CALLTYPE operator>  (RelativeTime t1, RelativeTime t2) noexcept       { return t1.inSeconds() >  t2.inSeconds(); }
HIGGS_API bool HIGGS_CALLTYPE operator<  (RelativeTime t1, RelativeTime t2) noexcept       { return t1.inSeconds() <  t2.inSeconds(); }
HIGGS_API bool HIGGS_CALLTYPE operator>= (RelativeTime t1, RelativeTime t2) noexcept       { return t1.inSeconds() >= t2.inSeconds(); }
HIGGS_API bool HIGGS_CALLTYPE operator<= (RelativeTime t1, RelativeTime t2) noexcept       { return t1.inSeconds() <= t2.inSeconds(); }

//==============================================================================
static String translateTimeField (int n, const char* singular, const char* plural)
{
    return TRANS (n == 1 ? singular : plural).replace (n == 1 ? "1" : "2", String (n));
}

static String describeYears   (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 year"),  NEEDS_TRANS ("2 years")); }
static String describeMonths  (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 month"), NEEDS_TRANS ("2 months")); }
static String describeWeeks   (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 week"),  NEEDS_TRANS ("2 weeks")); }
static String describeDays    (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 day"),   NEEDS_TRANS ("2 days")); }
static String describeHours   (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 hr"),    NEEDS_TRANS ("2 hrs")); }
static String describeMinutes (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 min"),   NEEDS_TRANS ("2 mins")); }
static String describeSeconds (int n)      { return translateTimeField (n, NEEDS_TRANS ("1 sec"),   NEEDS_TRANS ("2 secs")); }

String RelativeTime::getApproximateDescription() const
{
    if (numSeconds <= 1.0)
        return "< 1 sec";

    auto weeks = (int) inWeeks();

    if (weeks > 52)   return describeYears (weeks / 52);
    if (weeks > 8)    return describeMonths ((weeks * 12) / 52);
    if (weeks > 1)    return describeWeeks (weeks);

    auto days = (int) inDays();

    if (days > 1)
        return describeDays (days);

    auto hours = (int) inHours();

    if (hours > 0)
        return describeHours (hours);

    auto minutes = (int) inMinutes();

    if (minutes > 0)
        return describeMinutes (minutes);

    return describeSeconds ((int) numSeconds);
}

String RelativeTime::getDescription (const String& returnValueForZeroTime) const
{
    if (std::abs (numSeconds) < 0.001)
        return returnValueForZeroTime;

    if (numSeconds < 0)
        return "-" + RelativeTime (-numSeconds).getDescription();

    StringArray fields;

    auto n = (int) to<Weeks>();

    if (n > 0)
        fields.add (describeWeeks (n));

    n = ((int) to<Days>()) % 7;

    if (n > 0)
        fields.add (describeDays (n));

    if (fields.size() < 2)
    {
        n = ((int) to<Hours>()) % 24;

        if (n > 0)
            fields.add (describeHours (n));

        if (fields.size() < 2)
        {
            n = ((int) to<Minutes>()) % 60;

            if (n > 0)
                fields.add (describeMinutes (n));

            if (fields.size() < 2)
            {
                n = ((int) to<Seconds>()) % 60;

                if (n > 0)
                    fields.add (describeSeconds (n));

                if (fields.isEmpty())
                    fields.add (String (((int) to<Milliseconds>()) % 1000) + " " + TRANS ("ms"));
            }
        }
    }

    return fields.joinIntoString (" ");
}

} // namespace higgs
