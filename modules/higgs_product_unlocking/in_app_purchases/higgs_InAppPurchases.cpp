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

HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS
InAppPurchases::Purchase::Purchase() = default;

InAppPurchases::Purchase::Purchase (String orderIdIn,
                                    StringArray productIdsIn,
                                    String applicationBundleNameIn,
                                    Time timeOfPurchaseIn,
                                    String purchaseTokenIn)
    : orderId (std::move (orderIdIn)),
      productIds (std::move (productIdsIn)),
      applicationBundleName (std::move (applicationBundleNameIn)),
      purchaseTime (timeOfPurchaseIn.toString (true, true, true, true)),
      timeOfPurchase (std::move (timeOfPurchaseIn)),
      purchaseToken (std::move (purchaseTokenIn))
{
}

InAppPurchases::Purchase::Purchase (const Purchase&) = default;
InAppPurchases::Purchase::Purchase (Purchase&&) noexcept = default;

auto InAppPurchases::Purchase::operator= (const Purchase&) -> Purchase& = default;
auto InAppPurchases::Purchase::operator= (Purchase&&) noexcept -> Purchase& = default;

InAppPurchases::Purchase::~Purchase() = default;
HIGGS_END_IGNORE_DEPRECATION_WARNINGS

//==============================================================================
InAppPurchases::InAppPurchases()
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    : pimpl (new Pimpl (*this))
   #endif
{}

InAppPurchases::~InAppPurchases() { clearSingletonInstance(); }

bool InAppPurchases::isInAppPurchasesSupported() const
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    return pimpl->isInAppPurchasesSupported();
   #else
    return false;
   #endif
}

void InAppPurchases::getProductsInformation (const StringArray& productIdentifiers)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->getProductsInformation (productIdentifiers);
   #else
    Array<Product> products;
    for (auto productId : productIdentifiers)
        products.add (Product { productId, {}, {}, {}, {}  });

    listeners.call ([&] (Listener& l) { l.productsInfoReturned (products); });
   #endif
}

void InAppPurchases::purchaseProduct (const String& productIdentifier,
                                      [[maybe_unused]] const String& upgradeProductIdentifier,
                                      [[maybe_unused]] bool creditForUnusedSubscription)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->purchaseProduct (productIdentifier, upgradeProductIdentifier, creditForUnusedSubscription);
   #else
    Listener::PurchaseInfo purchaseInfo { Purchase { "", productIdentifier, {}, {}, {} }, {} };

    listeners.call ([&] (Listener& l) { l.productPurchaseFinished (purchaseInfo, false, "In-app purchases unavailable"); });
   #endif
}

void InAppPurchases::restoreProductsBoughtList ([[maybe_unused]] bool includeDownloadInfo, [[maybe_unused]] const String& subscriptionsSharedSecret)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->restoreProductsBoughtList (includeDownloadInfo, subscriptionsSharedSecret);
   #else
    listeners.call ([] (Listener& l) { l.purchasesListRestored ({}, false, "In-app purchases unavailable"); });
   #endif
}

void InAppPurchases::consumePurchase (const String& productIdentifier, [[maybe_unused]] const String& purchaseToken)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->consumePurchase (productIdentifier, purchaseToken);
   #else
    listeners.call ([&] (Listener& l) { l.productConsumed (productIdentifier, false, "In-app purchases unavailable"); });
   #endif
}

void InAppPurchases::addListener (Listener* l)      { listeners.add (l); }
void InAppPurchases::removeListener (Listener* l)   { listeners.remove (l); }

void InAppPurchases::startDownloads  ([[maybe_unused]] const Array<Download*>& downloads)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->startDownloads (downloads);
   #endif
}

void InAppPurchases::pauseDownloads  ([[maybe_unused]] const Array<Download*>& downloads)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->pauseDownloads (downloads);
   #endif
}

void InAppPurchases::resumeDownloads ([[maybe_unused]] const Array<Download*>& downloads)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->resumeDownloads (downloads);
   #endif
}

void InAppPurchases::cancelDownloads ([[maybe_unused]] const Array<Download*>& downloads)
{
   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->cancelDownloads (downloads);
   #endif
}

} // namespace higgs
