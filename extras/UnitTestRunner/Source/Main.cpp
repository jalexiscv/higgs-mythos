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

#include <HiggsHeader.h>

//==============================================================================
class ConsoleLogger final : public Logger
{
    void logMessage (const String& message) override
    {
        std::cout << message << std::endl;

       #if HIGGS_WINDOWS
        Logger::outputDebugString (message);
       #endif
    }
};

//==============================================================================
class ConsoleUnitTestRunner final : public UnitTestRunner
{
    void logMessage (const String& message) override
    {
        Logger::writeToLog (message);
    }
};

//==============================================================================
int main (int argc, char **argv)
{
    constexpr auto helpOption = "--help|-h";
    constexpr auto listOption = "--list-categories|-l";
    constexpr auto categoryOption = "--category|-c";
    constexpr auto seedOption = "--seed|-s";
    constexpr auto nameOption = "--name|-n";

    ArgumentList args (argc, argv);

    if (args.containsOption (helpOption))
    {
        std::cout << argv[0]
                  << " [" << helpOption << "]"
                  << " [" << listOption << "]"
                  << " [" << categoryOption << "=category]"
                  << " [" << seedOption << "=seed]"
                  << " [" << nameOption << "=name]"
                  << std::endl;
        return 0;
    }

    if (args.containsOption (listOption))
    {
        for (auto& category : UnitTest::getAllCategories())
            std::cout << category << std::endl;

        return  0;
    }

    ConsoleLogger logger;
    Logger::setCurrentLogger (&logger);

    const ScopeGuard onExit { [&]
    {
        Logger::setCurrentLogger (nullptr);
        DeletedAtShutdown::deleteAll();
    }};

    ConsoleUnitTestRunner runner;

    const auto seed = std::invoke ([&]
    {
        if (args.containsOption (seedOption))
        {
            auto seedValueString = args.getValueForOption (seedOption);

            if (seedValueString.startsWith ("0x"))
                return seedValueString.getHexValue64();

            return seedValueString.getLargeIntValue();
        }

        return Random::getSystemRandom().nextInt64();
    });

    std::vector<String> failures;

    const auto appendFailures = [&]
    {
        for (int i = 0; i < runner.getNumResults(); ++i)
        {
            auto* result = runner.getResult (i);

            if (result->failures > 0)
            {
                const auto testName = result->unitTestName + " / " + result->subcategoryName;
                const auto testSummary = String (result->failures) + " test failure" + (result->failures > 1 ? "s" : "");
                const auto newLineAndTab = newLine + "\t";

                failures.push_back (testName + ": " + testSummary + newLineAndTab
                                    + result->messages.joinIntoString (newLineAndTab));
            }
        }
    };

    if (args.containsOption (categoryOption) || args.containsOption (nameOption))
    {
        while (args.containsOption (categoryOption))
        {
            runner.runTestsInCategory (args.removeValueForOption (categoryOption), seed);
            appendFailures();
        }

        while (args.containsOption (nameOption))
        {
            runner.runTestsWithName (args.removeValueForOption (nameOption), seed);
            appendFailures();
        }
    }
    else
    {
        runner.runAllTests (seed);
        appendFailures();
    }

    logger.writeToLog (newLine + String::repeatedString ("-", 65));

    if (! failures.empty())
    {
        logger.writeToLog ("Test failure summary:");

        for (const auto& failure : failures)
            logger.writeToLog (newLine + failure);

        return 1;
    }

    logger.writeToLog ("All tests completed successfully");
    return 0;
}
