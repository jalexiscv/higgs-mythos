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
    BlowFish encryption class.


    @tags{Cryptography}
*/
class HIGGS_API  BlowFish
{
public:
    //==============================================================================
    /** Creates an object that can encode/decode based on the specified key.

        The key data can be up to 72 bytes long.
    */
    BlowFish (const void* keyData, int keyBytes);

    /** Creates a copy of another blowfish object. */
    BlowFish (const BlowFish&);

    /** Copies another blowfish object. */
    BlowFish& operator= (const BlowFish&) noexcept;

    /** Destructor. */
    ~BlowFish() noexcept;

    //==============================================================================
    /** Encrypts a pair of 32-bit integers. */
    void encrypt (uint32& data1, uint32& data2) const noexcept;

    /** Decrypts a pair of 32-bit integers. */
    void decrypt (uint32& data1, uint32& data2) const noexcept;

    //==============================================================================
    /** Encrypts a memory block */
    void encrypt (MemoryBlock& data) const;

    /** Decrypts a memory block */
    void decrypt (MemoryBlock& data) const;

    //==============================================================================
    /** Encrypts data in-place

        @param buffer       The message that should be encrypted. See bufferSize on size
                            requirements!
        @param sizeOfMsg    The size of the message that should be encrypted in bytes
        @param bufferSize   The size of the buffer in bytes. To accommodate the encrypted
                            data, the buffer must be larger than the message: the size of
                            the buffer needs to be equal or greater than the size of the
                            message in bytes rounded to the next integer which is divisible
                            by eight. If the message size in bytes is already divisible by eight
                            then you need to add eight bytes to the buffer size. If in doubt
                            simply use bufferSize = sizeOfMsg + 8.

        @returns            The size of the decrypted data in bytes or -1 if the decryption failed.
     */
    int encrypt (void* buffer, size_t sizeOfMsg, size_t bufferSize) const noexcept;

    /** Decrypts data in-place

        @param buffer  The encrypted data that should be decrypted
        @param bytes   The size of the encrypted data in bytes

        @returns       The size of the decrypted data in bytes or -1 if the decryption failed.
    */
    int decrypt (void* buffer, size_t bytes) const noexcept;

private:
    //==============================================================================
    static int pad   (void*, size_t, size_t) noexcept;
    static int unpad (const void*, size_t) noexcept;

    bool apply (void*, size_t, void (BlowFish::*op) (uint32&, uint32&) const) const;

    //==============================================================================
    uint32 p[18];
    HeapBlock<uint32> s[4];

    uint32 F (uint32) const noexcept;

    HIGGS_LEAK_DETECTOR (BlowFish)
};

} // namespace higgs
