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

/**
    An implementation of the OnlineUnlockStatus class which talks to the
    Tracktion Marketplace server.

    For details about how to use this class, see the docs for the base
    class: OnlineUnlockStatus. Basically, you need to inherit from it, and
    implement all the pure virtual methods to tell it about your product.

    @see OnlineUnlockStatus, OnlineUnlockForm, KeyGeneration

    @tags{ProductUnlocking}
*/
class HIGGS_API  TracktionMarketplaceStatus   : public OnlineUnlockStatus
{
public:
    TracktionMarketplaceStatus();

    /** @internal */
    bool doesProductIDMatch (const String& returnedIDFromServer) override;
    /** @internal */
    URL getServerAuthenticationURL() override;
    /** @internal */
    String getWebsiteName() override;
    /** @internal */
    String readReplyFromWebserver (const String& email, const String& password) override;
    /** @internal */
    void userCancelled() override;

private:
    CriticalSection streamCreationLock;
    std::unique_ptr<WebInputStream> stream;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TracktionMarketplaceStatus)
};

} // namespace higgs
