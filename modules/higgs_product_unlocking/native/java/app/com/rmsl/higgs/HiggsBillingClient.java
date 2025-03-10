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

package com.rmsl.higgs;

import com.android.billingclient.api.*;

public class HiggsBillingClient implements PurchasesUpdatedListener,
                                          BillingClientStateListener {
    private native void productDetailsQueryCallback(long host, java.util.List<ProductDetails> productDetails);
    private native void purchasesListQueryCallback(long host, java.util.List<Purchase> purchases);
    private native void purchaseCompletedCallback(long host, Purchase purchase, int responseCode);
    private native void purchaseConsumedCallback(long host, String productIdentifier, int responseCode);

    public HiggsBillingClient(android.content.Context context, long hostToUse) {
        host = hostToUse;

        billingClient = BillingClient.newBuilder(context)
                .enablePendingPurchases(PendingPurchasesParams.newBuilder().enableOneTimeProducts().build())
                .setListener(this)
                .enableAutoServiceReconnection()
                .build();

        billingClient.startConnection(this);
    }

    public void endConnection() {
        billingClient.endConnection();
    }

    public boolean isReady() {
        return billingClient.isReady();
    }

    public boolean isBillingSupported() {
        return billingClient.isFeatureSupported(BillingClient.FeatureType.SUBSCRIPTIONS).getResponseCode()
                == BillingClient.BillingResponseCode.OK;
    }

    public QueryProductDetailsParams getProductListParams(final String[] productsToQuery, String type) {
        java.util.ArrayList<QueryProductDetailsParams.Product> productList = new java.util.ArrayList<>();

        for (String product : productsToQuery)
            productList.add(QueryProductDetailsParams.Product.newBuilder().setProductId(product).setProductType(type).build());

        return QueryProductDetailsParams.newBuilder().setProductList(productList).build();
    }

    public void queryProductDetailsImpl(final String[] productsToQuery, java.util.List<String> productTypes, java.util.List<ProductDetails> details) {
        if (productTypes == null || productTypes.isEmpty()) {
            productDetailsQueryCallback(host, details);
        } else {
            billingClient.queryProductDetailsAsync(getProductListParams(productsToQuery, productTypes.get(0)), new ProductDetailsResponseListener() {
                @Override
                public void onProductDetailsResponse(BillingResult billingResult, QueryProductDetailsResult queryProductDetailsResult) {
                    if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                        details.addAll(queryProductDetailsResult.getProductDetailsList());
                        queryProductDetailsImpl(productsToQuery, productTypes.subList(1, productTypes.size()), details);
                    } else {
                        queryProductDetailsImpl(productsToQuery, null, details);
                    }
                }
            });
        }
    }

    public void queryProductDetails(final String[] productsToQuery) {
        executeOnBillingClientConnection(new Runnable() {
            @Override
            public void run() {
                String[] toCheck = {BillingClient.ProductType.INAPP, BillingClient.ProductType.SUBS};
                queryProductDetailsImpl(productsToQuery, java.util.Arrays.asList(toCheck), new java.util.ArrayList<ProductDetails>());
            }
        });
    }

    public void launchBillingFlow(final android.app.Activity activity, final BillingFlowParams params) {
        executeOnBillingClientConnection(new Runnable() {
            @Override
            public void run() {
                BillingResult r = billingClient.launchBillingFlow(activity, params);
            }
        });
    }

    private void queryPurchasesImpl(java.util.List<String> toCheck, java.util.ArrayList<Purchase> purchases) {
        if (toCheck == null || toCheck.isEmpty()) {
            // Purchases can become PURCHASED even when our app isn't running. In such cases we
            // will not receive an onPurchasesUpdated callback, so we need to process them here.
            for (Purchase purchase : purchases) {
                if (purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED && !purchase.isAcknowledged()) {
                    handlePurchase(purchase, BillingClient.BillingResponseCode.OK);
                }
            }

            purchasesListQueryCallback(host, purchases);
        } else {
            billingClient.queryPurchasesAsync(QueryPurchasesParams.newBuilder().setProductType(toCheck.get(0)).build(), new PurchasesResponseListener() {
                @Override
                public void onQueryPurchasesResponse(BillingResult billingResult, java.util.List<Purchase> list) {
                    if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                        purchases.addAll(list);
                        queryPurchasesImpl(toCheck.subList(1, toCheck.size()), purchases);
                    } else {
                        queryPurchasesImpl(null, purchases);
                    }
                }
            });
        }
    }

    public void queryPurchases() {
        executeOnBillingClientConnection(new Runnable() {
            @Override
            public void run() {
                String[] toCheck = {BillingClient.ProductType.INAPP, BillingClient.ProductType.SUBS};
                queryPurchasesImpl(java.util.Arrays.asList(toCheck), new java.util.ArrayList<Purchase>());
            }
        });
    }

    public void consumePurchase(final String productIdentifier, final String purchaseToken) {
        executeOnBillingClientConnection(new Runnable() {
            @Override
            public void run() {
                ConsumeParams consumeParams = ConsumeParams.newBuilder()
                        .setPurchaseToken(purchaseToken)
                        .build();

                billingClient.consumeAsync(consumeParams, new ConsumeResponseListener() {
                    @Override
                    public void onConsumeResponse(BillingResult billingResult, String purchaseToken) {
                        purchaseConsumedCallback(host, productIdentifier, billingResult.getResponseCode());
                    }
                });
            }
        });
    }

    @Override
    public void onPurchasesUpdated(BillingResult result, java.util.List<Purchase> purchases) {
        int responseCode = result.getResponseCode();

        if (purchases != null) {
            for (Purchase purchase : purchases) {
                handlePurchase(purchase, responseCode);
            }
        } else {
            purchaseCompletedCallback(host, null, responseCode);
        }
    }

    @Override
    public void onBillingServiceDisconnected()
    {

    }

    @Override
    public void onBillingSetupFinished(BillingResult billingResult)
    {

    }

    private void executeOnBillingClientConnection(Runnable runnable) {
        if (billingClient.isReady()) {
            runnable.run();
        } else {
            connectAndExecute(runnable);
        }
    }

    private void connectAndExecute(final Runnable executeOnSuccess) {
        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(BillingResult billingResponse) {
                if (billingResponse.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    if (executeOnSuccess != null) {
                        executeOnSuccess.run();
                    }
                }
            }

            @Override
            public void onBillingServiceDisconnected() {
            }
        });
    }

    private void handlePurchase(final Purchase purchase, final int responseCode) {
        purchaseCompletedCallback(host, purchase, responseCode);

        if (responseCode == BillingClient.BillingResponseCode.OK
                && purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED
                && !purchase.isAcknowledged()) {
            executeOnBillingClientConnection(new Runnable() {
                @Override
                public void run() {
                    AcknowledgePurchaseParams acknowledgePurchaseParams = AcknowledgePurchaseParams.newBuilder().setPurchaseToken(purchase.getPurchaseToken()).build();
                    billingClient.acknowledgePurchase(acknowledgePurchaseParams, new AcknowledgePurchaseResponseListener() {
                        @Override
                        public void onAcknowledgePurchaseResponse(BillingResult billingResult) {

                        }
                    });
                }
            });
        }
    }

    private long host = 0;
    private final BillingClient billingClient;
}
