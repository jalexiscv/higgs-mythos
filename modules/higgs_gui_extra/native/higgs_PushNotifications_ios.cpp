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

#define HIGGS_PUSH_NOTIFICATIONS_IMPL 1

struct PushNotificationsDelegateDetails
{
    //==============================================================================
    using Action   = PushNotifications::Settings::Action;
    using Category = PushNotifications::Settings::Category;

    static void* actionToNSAction (const Action& a)
    {
        if (a.style == Action::text)
        {
            return [UNTextInputNotificationAction actionWithIdentifier: higgsStringToNS (a.identifier)
                                                                 title: higgsStringToNS (a.title)
                                                               options: NSUInteger (a.destructive << 1 | (! a.triggerInBackground) << 2)
                                                  textInputButtonTitle: higgsStringToNS (a.textInputButtonText)
                                                  textInputPlaceholder: higgsStringToNS (a.textInputPlaceholder)];
        }

        return [UNNotificationAction actionWithIdentifier: higgsStringToNS (a.identifier)
                                                    title: higgsStringToNS (a.title)
                                                  options: NSUInteger (a.destructive << 1 | (! a.triggerInBackground) << 2)];
    }

    static void* categoryToNSCategory (const Category& c)
    {
        auto actions = [NSMutableArray arrayWithCapacity: (NSUInteger) c.actions.size()];

        for (const auto& a : c.actions)
        {
            auto* action = (UNNotificationAction*) actionToNSAction (a);
            [actions addObject: action];
        }

        return [UNNotificationCategory categoryWithIdentifier: higgsStringToNS (c.identifier)
                                                      actions: actions
                                            intentIdentifiers: @[]
                                                      options: c.sendDismissAction ? UNNotificationCategoryOptionCustomDismissAction : 0];
    }

    //==============================================================================
    static UNNotificationRequest* higgsNotificationToUNNotificationRequest (const PushNotifications::Notification& n)
    {
        // content
        auto content = [[UNMutableNotificationContent alloc] init];

        content.title              = higgsStringToNS (n.title);
        content.subtitle           = higgsStringToNS (n.subtitle);
        content.threadIdentifier   = higgsStringToNS (n.groupId);
        content.body               = higgsStringToNS (n.body);
        content.categoryIdentifier = higgsStringToNS (n.category);
        content.badge              = [NSNumber numberWithInt: n.badgeNumber];

        auto soundToPlayString = n.soundToPlay.toString (true);

        if (soundToPlayString == "default_os_sound")
            content.sound = [UNNotificationSound defaultSound];
        else if (soundToPlayString.isNotEmpty())
            content.sound = [UNNotificationSound soundNamed: higgsStringToNS (soundToPlayString)];

        auto* propsDict = (NSMutableDictionary*) [varToNSDictionary (n.properties) mutableCopy];
        [propsDict setObject: higgsStringToNS (soundToPlayString) forKey: @"com.higgs.soundName"];
        content.userInfo = propsDict;

        // trigger
        UNTimeIntervalNotificationTrigger* trigger = nil;

        if (std::abs (n.triggerIntervalSec) >= 0.001)
        {
            BOOL shouldRepeat = n.repeat && n.triggerIntervalSec >= 60;
            trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval: n.triggerIntervalSec repeats: shouldRepeat];
        }

        // request
        // each notification needs to have an identifier, otherwise it will not show up
        jassert (n.identifier.isNotEmpty());
        UNNotificationRequest* request = [UNNotificationRequest requestWithIdentifier: higgsStringToNS (n.identifier)
                                                                              content: content
                                                                              trigger: trigger];

        [content autorelease];

        return request;
    }

    static String getUserResponseFromNSDictionary (NSDictionary* dictionary)
    {
        if (dictionary == nil || dictionary.count == 0)
            return {};

        jassert (dictionary.count == 1);

        for (NSString* key in dictionary)
        {
            const auto keyString = nsStringToHiggs (key);

            id value = dictionary[key];

            if ([value isKindOfClass: [NSString class]])
                return nsStringToHiggs ((NSString*) value);
        }

        jassertfalse;
        return {};
    }

    //==============================================================================
    static var getNotificationPropertiesFromDictionaryVar (const var& dictionaryVar)
    {
        DynamicObject* dictionaryVarObject = dictionaryVar.getDynamicObject();

        if (dictionaryVarObject == nullptr)
            return {};

        const auto& properties = dictionaryVarObject->getProperties();

        DynamicObject::Ptr propsVarObject = new DynamicObject();

        for (int i = 0; i < properties.size(); ++i)
        {
            auto propertyName = properties.getName (i).toString();

            if (propertyName == "aps")
                continue;

            propsVarObject->setProperty (propertyName, properties.getValueAt (i));
        }

        return var (propsVarObject.get());
    }

    //==============================================================================
    static double getIntervalSecFromUNNotificationTrigger (UNNotificationTrigger* t)
    {
        if (t != nil)
        {
            if ([t isKindOfClass: [UNTimeIntervalNotificationTrigger class]])
            {
                auto* trigger = (UNTimeIntervalNotificationTrigger*) t;
                return trigger.timeInterval;
            }
            else if ([t isKindOfClass: [UNCalendarNotificationTrigger class]])
            {
                auto* trigger = (UNCalendarNotificationTrigger*) t;
                NSDate* date    = [trigger.dateComponents date];
                NSDate* dateNow = [NSDate date];
                return [dateNow timeIntervalSinceDate: date];
            }
        }

        return 0.;
    }

    static PushNotifications::Notification unNotificationRequestToHiggsNotification (UNNotificationRequest* r)
    {
        PushNotifications::Notification n;

        n.identifier = nsStringToHiggs (r.identifier);
        n.title      = nsStringToHiggs (r.content.title);
        n.subtitle   = nsStringToHiggs (r.content.subtitle);
        n.body       = nsStringToHiggs (r.content.body);
        n.groupId    = nsStringToHiggs (r.content.threadIdentifier);
        n.category   = nsStringToHiggs (r.content.categoryIdentifier);
        n.badgeNumber = r.content.badge.intValue;

        auto userInfoVar = nsDictionaryToVar (r.content.userInfo);

        if (auto* object = userInfoVar.getDynamicObject())
        {
            static const Identifier soundName ("com.higgs.soundName");
            n.soundToPlay = URL (object->getProperty (soundName).toString());
            object->removeProperty (soundName);
        }

        n.properties = userInfoVar;

        n.triggerIntervalSec = getIntervalSecFromUNNotificationTrigger (r.trigger);
        n.repeat = r.trigger != nil && r.trigger.repeats;

        return n;
    }

    static PushNotifications::Notification unNotificationToHiggsNotification (UNNotification* n)
    {
        return unNotificationRequestToHiggsNotification (n.request);
    }

    static Action unNotificationActionToAction (UNNotificationAction* a)
    {
        Action action;

        action.identifier = nsStringToHiggs (a.identifier);
        action.title      = nsStringToHiggs (a.title);
        action.triggerInBackground = ! (a.options & UNNotificationActionOptionForeground);
        action.destructive         =    a.options & UNNotificationActionOptionDestructive;

        if ([a isKindOfClass: [UNTextInputNotificationAction class]])
        {
            auto* textAction = (UNTextInputNotificationAction*)a;

            action.style = Action::text;
            action.textInputButtonText  = nsStringToHiggs (textAction.textInputButtonTitle);
            action.textInputPlaceholder = nsStringToHiggs (textAction.textInputPlaceholder);
        }
        else
        {
            action.style = Action::button;
        }

        return action;
    }

    static Category unNotificationCategoryToCategory (UNNotificationCategory* c)
    {
        Category category;

        category.identifier = nsStringToHiggs (c.identifier);
        category.sendDismissAction = c.options & UNNotificationCategoryOptionCustomDismissAction;

        for (UNNotificationAction* a in c.actions)
            category.actions.add (unNotificationActionToAction (a));

        return category;
    }

    static PushNotifications::Notification nsDictionaryToHiggsNotification (NSDictionary* dictionary)
    {
        const var dictionaryVar = nsDictionaryToVar (dictionary);

        const var apsVar = dictionaryVar.getProperty ("aps", {});

        if (! apsVar.isObject())
            return {};

        var alertVar = apsVar.getProperty ("alert", {});

        const var titleVar = alertVar.getProperty ("title", {});
        const var bodyVar  = alertVar.isObject() ? alertVar.getProperty ("body", {}) : alertVar;

        const var categoryVar = apsVar.getProperty ("category", {});
        const var soundVar    = apsVar.getProperty ("sound", {});
        const var badgeVar    = apsVar.getProperty ("badge", {});
        const var threadIdVar = apsVar.getProperty ("thread-id", {});

        PushNotifications::Notification notification;

        notification.title       = titleVar   .toString();
        notification.body        = bodyVar    .toString();
        notification.groupId     = threadIdVar.toString();
        notification.category    = categoryVar.toString();
        notification.soundToPlay = URL (soundVar.toString());
        notification.badgeNumber = (int) badgeVar;
        notification.properties  = getNotificationPropertiesFromDictionaryVar (dictionaryVar);

        return notification;
    }

private:
    ~PushNotificationsDelegateDetails() = delete;
};

//==============================================================================
bool PushNotifications::Notification::isValid() const noexcept
{
    return title.isNotEmpty() && body.isNotEmpty() && identifier.isNotEmpty() && category.isNotEmpty();
}

//==============================================================================
struct PushNotifications::Impl
{
    explicit Impl (PushNotifications& p)
        : owner (p)
    {
        Class::setThis (delegate.get(), this);

        auto appDelegate = [[UIApplication sharedApplication] delegate];

        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wundeclared-selector")
        if ([appDelegate respondsToSelector: @selector (setPushNotificationsDelegateToUse:)])
            [appDelegate performSelector: @selector (setPushNotificationsDelegateToUse:) withObject: delegate.get()];
        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
    }

    void requestPermissionsWithSettings (const PushNotifications::Settings& settingsToUse)
    {
        settings = settingsToUse;

        auto categories = [NSMutableSet setWithCapacity: (NSUInteger) settings.categories.size()];

        for (const auto& c : settings.categories)
        {
            auto* category = (UNNotificationCategory*) PushNotificationsDelegateDetails::categoryToNSCategory (c);
            [categories addObject: category];
        }

        UNAuthorizationOptions authOptions = NSUInteger ((bool)settings.allowBadge << 0
                                                       | (bool)settings.allowSound << 1
                                                       | (bool)settings.allowAlert << 2);

        [[UNUserNotificationCenter currentNotificationCenter] setNotificationCategories: categories];
        [[UNUserNotificationCenter currentNotificationCenter] requestAuthorizationWithOptions: authOptions
                                                                            completionHandler: ^(BOOL /*granted*/, NSError* /*error*/)
                                                                                               {
                                                                                                   requestSettingsUsed();
                                                                                               }];

        [[UIApplication sharedApplication] registerForRemoteNotifications];
    }

    void requestSettingsUsed()
    {
        [[UNUserNotificationCenter currentNotificationCenter] getNotificationSettingsWithCompletionHandler:
         ^(UNNotificationSettings* s)
         {
             [[UNUserNotificationCenter currentNotificationCenter] getNotificationCategoriesWithCompletionHandler:
              ^(NSSet<UNNotificationCategory*>* categories)
              {
                  settings.allowBadge = s.badgeSetting == UNNotificationSettingEnabled;
                  settings.allowSound = s.soundSetting == UNNotificationSettingEnabled;
                  settings.allowAlert = s.alertSetting == UNNotificationSettingEnabled;

                  for (UNNotificationCategory* c in categories)
                      settings.categories.add (PushNotificationsDelegateDetails::unNotificationCategoryToCategory (c));

                  owner.listeners.call ([&] (Listener& l) { l.notificationSettingsReceived (settings); });
              }
             ];
         }];
    }

    bool areNotificationsEnabled() const { return true; }

    void sendLocalNotification (const Notification& n)
    {
        UNNotificationRequest* request = PushNotificationsDelegateDetails::higgsNotificationToUNNotificationRequest (n);

        [[UNUserNotificationCenter currentNotificationCenter] addNotificationRequest: request
                                                               withCompletionHandler: ^(NSError* error)
                                                                                      {
                                                                                          jassert (error == nil);

                                                                                          if (error != nil)
                                                                                              NSLog (nsStringLiteral ("addNotificationRequest error: %@"), error);
                                                                                      }];
    }

    void getDeliveredNotifications() const
    {
        [[UNUserNotificationCenter currentNotificationCenter] getDeliveredNotificationsWithCompletionHandler:
         ^(NSArray<UNNotification*>* notifications)
         {
            Array<PushNotifications::Notification> notifs;

            for (UNNotification* n in notifications)
                notifs.add (PushNotificationsDelegateDetails::unNotificationToHiggsNotification (n));

            owner.listeners.call ([&] (Listener& l) { l.deliveredNotificationsListReceived (notifs); });
         }];
    }

    void removeAllDeliveredNotifications()
    {
        [[UNUserNotificationCenter currentNotificationCenter] removeAllDeliveredNotifications];
    }

    void removeDeliveredNotification ([[maybe_unused]] const String& identifier)
    {
        NSArray<NSString*>* identifiers = [NSArray arrayWithObject: higgsStringToNS (identifier)];

        [[UNUserNotificationCenter currentNotificationCenter] removeDeliveredNotificationsWithIdentifiers: identifiers];
    }

    void setupChannels ([[maybe_unused]] const Array<ChannelGroup>& groups, [[maybe_unused]] const Array<Channel>& channels)
    {
    }

    void getPendingLocalNotifications() const
    {
        [[UNUserNotificationCenter currentNotificationCenter] getPendingNotificationRequestsWithCompletionHandler:
         ^(NSArray<UNNotificationRequest*>* requests)
         {
             Array<PushNotifications::Notification> notifs;

             for (UNNotificationRequest* r : requests)
                 notifs.add (PushNotificationsDelegateDetails::unNotificationRequestToHiggsNotification (r));

             owner.listeners.call ([&] (Listener& l) { l.pendingLocalNotificationsListReceived (notifs); });
         }];
    }

    void removePendingLocalNotification (const String& identifier)
    {
        NSArray<NSString*>* identifiers = [NSArray arrayWithObject: higgsStringToNS (identifier)];
        [[UNUserNotificationCenter currentNotificationCenter] removePendingNotificationRequestsWithIdentifiers: identifiers];
    }

    void removeAllPendingLocalNotifications()
    {
        [[UNUserNotificationCenter currentNotificationCenter] removeAllPendingNotificationRequests];
    }

    String getDeviceToken()
    {
        // You need to call requestPermissionsWithSettings() first.
        jassert (initialised);

        return deviceToken;
    }

    void subscribeToTopic ([[maybe_unused]] const String& topic)     {}
    void unsubscribeFromTopic ([[maybe_unused]] const String& topic) {}

    void sendUpstreamMessage ([[maybe_unused]] const String& serverSenderId,
                              [[maybe_unused]] const String& collapseKey,
                              [[maybe_unused]] const String& messageId,
                              [[maybe_unused]] const String& messageType,
                              [[maybe_unused]] int timeToLive,
                              [[maybe_unused]] const StringPairArray& additionalData)
    {
    }

private:
    //==============================================================================
    void registeredForRemoteNotifications (NSData* deviceTokenToUse)
    {
        deviceToken = [deviceTokenToUse]() -> String
        {
            auto length = deviceTokenToUse.length;

            if (auto* buffer = (const unsigned char*) deviceTokenToUse.bytes)
            {
                NSMutableString* hexString = [NSMutableString stringWithCapacity: (length * 2)];

                for (NSUInteger i = 0; i < length; ++i)
                    [hexString appendFormat:@"%02x", buffer[i]];

                return nsStringToHiggs ([hexString copy]);
            }

            return {};
        }();

        initialised = true;

        owner.listeners.call ([&] (Listener& l) { l.deviceTokenRefreshed (deviceToken); });
    }

    void failedToRegisterForRemoteNotifications ([[maybe_unused]] NSError* error)
    {
        deviceToken.clear();
    }

    void didReceiveRemoteNotification (NSDictionary* userInfo)
    {
        auto n = PushNotificationsDelegateDetails::nsDictionaryToHiggsNotification (userInfo);

        owner.listeners.call ([&] (Listener& l) { l.handleNotification (false, n); });
    }

    void didReceiveRemoteNotificationFetchCompletionHandler (NSDictionary* userInfo,
                                                             void (^completionHandler)(UIBackgroundFetchResult result))
    {
        didReceiveRemoteNotification (userInfo);
        completionHandler (UIBackgroundFetchResultNewData);
    }

    void handleActionForRemoteNotificationCompletionHandler (NSString* actionIdentifier,
                                                             NSDictionary* userInfo,
                                                             NSDictionary* responseInfo,
                                                             void (^completionHandler)())
    {
        auto n = PushNotificationsDelegateDetails::nsDictionaryToHiggsNotification (userInfo);
        auto actionString = nsStringToHiggs (actionIdentifier);
        auto response = PushNotificationsDelegateDetails::getUserResponseFromNSDictionary (responseInfo);

        owner.listeners.call ([&] (Listener& l) { l.handleNotificationAction (false, n, actionString, response); });

        completionHandler();
    }

    void willPresentNotificationWithCompletionHandler ([[maybe_unused]] UNNotification* notification,
                                                       void (^completionHandler)(UNNotificationPresentationOptions options))
    {
        NSUInteger options = NSUInteger ((int)settings.allowBadge << 0
                                       | (int)settings.allowSound << 1
                                       | (int)settings.allowAlert << 2);

        completionHandler (options);
    }

    void didReceiveNotificationResponseWithCompletionHandler (UNNotificationResponse* response,
                                                              void (^completionHandler)())
    {
        const bool remote = [response.notification.request.trigger isKindOfClass: [UNPushNotificationTrigger class]];

        auto actionString = nsStringToHiggs (response.actionIdentifier);

        if (actionString == "com.apple.UNNotificationDefaultActionIdentifier")
            actionString.clear();
        else if (actionString == "com.apple.UNNotificationDismissActionIdentifier")
            actionString = "com.higgs.NotificationDeleted";

        auto n = PushNotificationsDelegateDetails::unNotificationToHiggsNotification (response.notification);

        String responseString;

        if ([response isKindOfClass: [UNTextInputNotificationResponse class]])
        {
            UNTextInputNotificationResponse* textResponse = (UNTextInputNotificationResponse*)response;
            responseString = nsStringToHiggs (textResponse.userText);
        }

        owner.listeners.call ([&] (Listener& l) { l.handleNotificationAction (! remote, n, actionString, responseString); });
        completionHandler();
    }

    //==============================================================================
    struct Class final : public ObjCClass<NSObject<UIApplicationDelegate, UNUserNotificationCenterDelegate>>
    {
        Class()
            : ObjCClass ("HiggsPushNotificationsDelegate_")
        {
            addIvar<Impl*> ("self");

            addMethod (@selector (application:didRegisterForRemoteNotificationsWithDeviceToken:), [] (id self, SEL, UIApplication*, NSData* data)
            {
                getThis (self).registeredForRemoteNotifications (data);
            });

            addMethod (@selector (application:didFailToRegisterForRemoteNotificationsWithError:), [] (id self, SEL, UIApplication*, NSError* error)
            {
                getThis (self).failedToRegisterForRemoteNotifications (error);
            });

            addMethod (@selector (application:didReceiveRemoteNotification:), [] (id self, SEL, UIApplication*, NSDictionary* userInfo)
            {
                getThis (self).didReceiveRemoteNotification (userInfo);
            });

            addMethod (@selector (application:didReceiveRemoteNotification:fetchCompletionHandler:), [] (id self, SEL, UIApplication*, NSDictionary* userInfo, void (^completionHandler)(UIBackgroundFetchResult result))
            {
                getThis (self).didReceiveRemoteNotificationFetchCompletionHandler (userInfo, completionHandler);
            });

            addMethod (@selector (application:handleActionWithIdentifier:forRemoteNotification:withResponseInfo:completionHandler:), [] (id self, SEL, UIApplication*, NSString* actionIdentifier, NSDictionary* userInfo, NSDictionary* responseInfo, void (^completionHandler)())
            {
                getThis (self).handleActionForRemoteNotificationCompletionHandler (actionIdentifier, userInfo, responseInfo, completionHandler);
            });

            addMethod (@selector (userNotificationCenter:willPresentNotification:withCompletionHandler:), [] (id self, SEL, UNUserNotificationCenter*, UNNotification* notification, void (^completionHandler)(UNNotificationPresentationOptions options))
            {
                getThis (self).willPresentNotificationWithCompletionHandler (notification, completionHandler);
            });

            addMethod (@selector (userNotificationCenter:didReceiveNotificationResponse:withCompletionHandler:), [] (id self, SEL, UNUserNotificationCenter*, UNNotificationResponse* response, void (^completionHandler)())
            {
                getThis (self).didReceiveNotificationResponseWithCompletionHandler (response, completionHandler);
            });

            registerClass();
        }

        //==============================================================================
        static Impl& getThis (id self)          { return *getIvar<Impl*> (self, "self"); }
        static void setThis (id self, Impl* d)  { object_setInstanceVariable (self, "self", d); }
    };

    //==============================================================================
    static Class& getClass()
    {
        static Class c;
        return c;
    }

    NSUniquePtr<NSObject<UIApplicationDelegate, UNUserNotificationCenterDelegate>> delegate { [getClass().createInstance() init] };

    PushNotifications& owner;

    bool initialised = false;
    String deviceToken;

    PushNotifications::Settings settings;
};

} // namespace higgs
