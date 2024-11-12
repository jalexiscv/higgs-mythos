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
    Whirlpool hash class.

    The Whirlpool algorithm was developed by
    <a href="mailto:pbarreto@scopus.com.br">Paulo S. L. M. Barreto</a> and
    <a href="mailto:vincent.rijmen@cryptomathic.com">Vincent Rijmen</a>.

    See
        P.S.L.M. Barreto, V. Rijmen,
        "The Whirlpool hashing function"
        NESSIE submission, 2000 (tweaked version, 2001),
        https://www.cosic.esat.kuleuven.ac.be/nessie/workshop/submissions/whirlpool.zip

    @see SHA256, MD5

    @tags{Cryptography}
*/
class HIGGS_API  Whirlpool
{
public:
    //==============================================================================
    /** Creates an empty Whirlpool object.
        The default constructor just creates a hash filled with zeros. (This is not
        equal to the hash of an empty block of data).
    */
    Whirlpool() noexcept;

    /** Destructor. */
    ~Whirlpool() noexcept;

    /** Creates a copy of another Whirlpool. */
    Whirlpool (const Whirlpool& other) noexcept;

    /** Copies another Whirlpool. */
    Whirlpool& operator= (const Whirlpool& other) noexcept;

    //==============================================================================
    /** Creates a hash from a block of raw data. */
    explicit Whirlpool (const MemoryBlock&);

    /** Creates a hash from a block of raw data. */
    Whirlpool (const void* data, size_t numBytes);

    /** Creates a hash from the contents of a stream.

        This will read from the stream until the stream is exhausted, or until
        maxBytesToRead bytes have been read. If maxBytesToRead is negative, the entire
        stream will be read.
    */
    Whirlpool (InputStream& input, int64 maxBytesToRead = -1);

    /** Reads a file and generates the hash of its contents.
        If the file can't be opened, the hash will be left uninitialised
        (i.e. full of zeros).
    */
    explicit Whirlpool (const File& file);

    /** Creates a checksum from a UTF-8 buffer.
        E.g.
        @code Whirlpool checksum (myString.toUTF8());
        @endcode
    */
    explicit Whirlpool (CharPointer_UTF8 utf8Text) noexcept;

    //==============================================================================
    /** Returns the hash as a 64-byte block of data. */
    MemoryBlock getRawData() const;

    /** Returns the checksum as a 128-digit hex string. */
    String toHexString() const;

    //==============================================================================
    bool operator== (const Whirlpool&) const noexcept;
    bool operator!= (const Whirlpool&) const noexcept;


private:
    //==============================================================================
    uint8 result [64];
    void process (const void*, size_t);

    HIGGS_LEAK_DETECTOR (Whirlpool)
};

} // namespace higgs
