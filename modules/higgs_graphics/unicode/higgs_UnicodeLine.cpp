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

class TR14
{
public:
    TR14() = delete;

    template <typename Callback>
    static size_t analyseLineBreaks (Span<const UnicodeAnalysisPoint> span, Callback&& callback)
    {
        uint32_t resultIndex     = 0;
        uint32_t regionalCounter = 0;
        std::optional<LineBreakType> lb9;
        bool lb21a{};

        const auto data = span.data();
        const auto len  = span.size();

        const auto emit = [&] (auto op) { callback ((int) resultIndex++, op); };

        for (size_t i = 0; i < len;)
        {
            const auto isSOT = i == 0;
            const auto isEOT = i == len - 1;

            const auto resolved = resolve (data[i]);
            const auto prev = isSOT ? resolveSOT (resolved)
                                    : lb9.value_or (resolved);
            const auto finalBreak = lb9.has_value() ? LineBreakType::al
                                                    : LineBreakType::cm;
            const auto next = isEOT ? finalBreak
                                    : resolve (data[i + 1]);
            lb9.reset();

            // LB3
            if (isEOT)
            {
                if (contains ({ LineBreakType::cr, LineBreakType::lf, LineBreakType::nl }, prev))
                    emit (TextBreakType::hard);
                else
                    emit (TextBreakType::soft);

                break;
            }

            //==============================================================================
            // LB4-6
            if (prev == LineBreakType::bk)
            {
                emit (TextBreakType::hard);
                i++;
                continue;
            }

            if (prev == LineBreakType::cr && next == LineBreakType::lf)
            {
                emit (TextBreakType::none);
                i++;
                continue;
            }

            if (contains ({ LineBreakType::cr, LineBreakType::lf, LineBreakType::nl }, prev))
            {
                emit (TextBreakType::hard);
                i++;
                continue;
            }

            if (contains ({ LineBreakType::cr, LineBreakType::lf, LineBreakType::nl, LineBreakType::bk }, next))
            {
                emit (TextBreakType::none);
                i++;
                continue;
            }

            //==============================================================================
            // LB7
            if (contains ({ LineBreakType::sp, LineBreakType::zw }, next))
            {
                emit (TextBreakType::none);
                i++;
                continue;
            }

            // LB8a
            if (prev == LineBreakType::zwj)
            {
                emit (TextBreakType::none);
                i++;
                continue;
            }

            // LB13
            if (contains ({ LineBreakType::cl, LineBreakType::cp, LineBreakType::ex, LineBreakType::is, LineBreakType::sy }, next))
            {
                emit (TextBreakType::none);
                i++;
                continue;
            }

            // lb21a
            if (lb21a && contains ({ LineBreakType::hy, LineBreakType::ba }, prev))
            {
                emit (TextBreakType::none);
                i++;
                continue;
            }

            lb21a = prev == LineBreakType::hl;

            // LB30a
            if (prev == LineBreakType::ri)
            {
                regionalCounter++;

                if (next == LineBreakType::ri && regionalCounter % 2 == 0)
                {
                    regionalCounter = 0;
                    emit (TextBreakType::soft);
                    i++;
                    continue;
                }
            }
            else
            {
                regionalCounter = 0;
            }

            const auto bt = BreakPairTable::getLineBreakOpportunity (prev, next);

            switch (bt)
            {
                case BreakOpportunity::direct:
                {
                    emit (TextBreakType::soft);
                    i++;
                    continue;
                }

                case BreakOpportunity::prohibited:
                {
                    emit (TextBreakType::none);
                    i++;
                    continue;
                }

                case BreakOpportunity::indirect:
                {
                    emit (next == LineBreakType::sp || next == LineBreakType::cm ? TextBreakType::soft
                                                                                 : TextBreakType::none);
                    i++;
                    continue;
                }

                case BreakOpportunity::combinedIndirect:
                case BreakOpportunity::combinedProhibited:
                {
                    if (! contains ({ LineBreakType::bk, LineBreakType::cr,
                                      LineBreakType::lf, LineBreakType::nl,
                                      LineBreakType::sp, LineBreakType::zw }, prev))
                    {
                        lb9 = std::make_optional (prev);
                    }

                    while (i < len)
                    {
                        if (i == len - 1)
                        {
                            emit (TextBreakType::soft);
                            i++;
                            break;
                        }

                        emit (TextBreakType::none);

                        if (! contains ({ LineBreakType::cm, LineBreakType::zwj }, resolve (data[i])))
                            break;

                        i++;
                    }

                    i++;
                    continue;
                }
            }

            i++;
            continue;
        }

        return resultIndex;
    }

private:
    static LineBreakType resolve (const UnicodeAnalysisPoint& point)
    {
        const auto bc = point.getBreakType();

        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wswitch-enum")
        switch (bc)
        {
            case LineBreakType::ai:
            case LineBreakType::sg:
            case LineBreakType::xx:
                return LineBreakType::al;

            case LineBreakType::cj:
                return LineBreakType::ns;

            case LineBreakType::sa:
                return contains ({ (SBGeneralCategory) SBGeneralCategoryMN,
                                   (SBGeneralCategory) SBGeneralCategoryMC },
                                 point.getGeneralCategory())
                    ? LineBreakType::cm
                    : LineBreakType::al;

            default: break;
        }
        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

        return bc;
    }

    static LineBreakType resolveSOT (LineBreakType bc)
    {
        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wswitch-enum")
        switch (bc)
        {
            case LineBreakType::lf:
            case LineBreakType::nl:
                return LineBreakType::bk;

            case LineBreakType::sp:
                return LineBreakType::wj;

            default: break;
        }
        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

        return bc;
    }
};

}
