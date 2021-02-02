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

//==============================================================================
/**
    Represents the 'success' or 'failure' of an operation, and holds an associated
    error message to describe the error when there's a failure.

    E.g.
    @code
    Result myOperation()
    {
        if (doSomeKindOfFoobar())
            return Result::ok();
        else
            return Result::fail ("foobar didn't work!");
    }

    const Result result (myOperation());

    if (result.wasOk())
    {
        ...it's all good...
    }
    else
    {
        warnUserAboutFailure ("The foobar operation failed! Error message was: "
                                + result.getErrorMessage());
    }
    @endcode

    @tags{Core}
*/
class HIGGS_API  Result
{
public:
    //==============================================================================
    /** Creates and returns a 'successful' result. */
    static Result ok() noexcept                         { return Result(); }

    /** Creates a 'failure' result.
        If you pass a blank error message in here, a default "Unknown Error" message
        will be used instead.
    */
    static Result fail (const String& errorMessage) noexcept;

    //==============================================================================
    /** Returns true if this result indicates a success. */
    bool wasOk() const noexcept;

    /** Returns true if this result indicates a failure.
        You can use getErrorMessage() to retrieve the error message associated
        with the failure.
    */
    bool failed() const noexcept;

    /** Returns true if this result indicates a success.
        This is equivalent to calling wasOk().
    */
    operator bool() const noexcept;

    /** Returns true if this result indicates a failure.
        This is equivalent to calling failed().
    */
    bool operator!() const noexcept;

    /** Returns the error message that was set when this result was created.
        For a successful result, this will be an empty string;
    */
    const String& getErrorMessage() const noexcept;

    //==============================================================================
    Result (const Result&);
    Result& operator= (const Result&);
    Result (Result&&) noexcept;
    Result& operator= (Result&&) noexcept;

    bool operator== (const Result& other) const noexcept;
    bool operator!= (const Result& other) const noexcept;

private:
    String errorMessage;

    // The default constructor is not for public use!
    // Instead, use Result::ok() or Result::fail()
    Result() noexcept;
    explicit Result (const String&) noexcept;

    // These casts are private to prevent people trying to use the Result object in numeric contexts
    operator int() const;
    operator void*() const;
};

} // namespace higgs
