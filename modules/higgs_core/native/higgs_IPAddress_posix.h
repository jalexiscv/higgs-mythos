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

namespace
{
    struct InterfaceInfo
    {
        IPAddress interfaceAddress, broadcastAddress;
    };

    inline bool operator== (const InterfaceInfo& lhs, const InterfaceInfo& rhs)
    {
        return lhs.interfaceAddress == rhs.interfaceAddress
            && lhs.broadcastAddress == rhs.broadcastAddress;
    }

   #if ! HIGGS_WASM
    static IPAddress makeAddress (const sockaddr_in6* addr_in)
    {
        if (addr_in == nullptr)
            return {};

        auto addr = addr_in->sin6_addr;

        IPAddressByteUnion temp;
        uint16 arr[8];

        for (int i = 0; i < 8; ++i) // Swap bytes from network to host order
        {
            temp.split[0] = addr.s6_addr[i * 2 + 1];
            temp.split[1] = addr.s6_addr[i * 2];

            arr[i] = temp.combined;
        }

        return IPAddress (arr);
    }

    static IPAddress makeAddress (const sockaddr_in* addr_in)
    {
        if (addr_in->sin_addr.s_addr == INADDR_NONE)
            return {};

        return IPAddress (ntohl (addr_in->sin_addr.s_addr));
    }

    bool populateInterfaceInfo (struct ifaddrs* ifa, InterfaceInfo& interfaceInfo)
    {
        if (ifa->ifa_addr != nullptr)
        {
            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                auto interfaceAddressInfo = unalignedPointerCast<sockaddr_in*> (ifa->ifa_addr);
                auto broadcastAddressInfo = unalignedPointerCast<sockaddr_in*> (ifa->ifa_dstaddr);

                if (interfaceAddressInfo->sin_addr.s_addr != INADDR_NONE)
                {
                    interfaceInfo.interfaceAddress = makeAddress (interfaceAddressInfo);
                    interfaceInfo.broadcastAddress = makeAddress (broadcastAddressInfo);
                    return true;
                }
            }
            else if (ifa->ifa_addr->sa_family == AF_INET6)
            {
                interfaceInfo.interfaceAddress = makeAddress (unalignedPointerCast<sockaddr_in6*> (ifa->ifa_addr));
                interfaceInfo.broadcastAddress = makeAddress (unalignedPointerCast<sockaddr_in6*> (ifa->ifa_dstaddr));
                return true;
            }
        }

        return false;
    }
   #endif

    Array<InterfaceInfo> getAllInterfaceInfo()
    {
        Array<InterfaceInfo> interfaces;

       #if HIGGS_WASM
        // TODO
       #else
        struct ifaddrs* ifaddr = nullptr;

        if (getifaddrs (&ifaddr) != -1)
        {
            for (auto* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
            {
                InterfaceInfo i;

                if (populateInterfaceInfo (ifa, i))
                    interfaces.addIfNotAlreadyThere (i);
            }

            freeifaddrs (ifaddr);
        }
       #endif

        return interfaces;
    }
}

void IPAddress::findAllAddresses (Array<IPAddress>& result, bool includeIPv6)
{
    for (auto& i : getAllInterfaceInfo())
        if (includeIPv6 || ! i.interfaceAddress.isIPv6)
            result.addIfNotAlreadyThere (i.interfaceAddress);
}

IPAddress IPAddress::getInterfaceBroadcastAddress (const IPAddress& interfaceAddress)
{
    for (auto& i : getAllInterfaceInfo())
        if (i.interfaceAddress == interfaceAddress)
            return i.broadcastAddress;

    return {};
}

} // namespace higgs
