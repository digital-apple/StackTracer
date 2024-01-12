#include "Events.h"

RE::BSEventNotifyControl Events::ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (!a_event->actor || !a_event->targetActor) {
        return RE::BSEventNotifyControl::kContinue;
    }

    const auto source = a_event->actor.get();
    const auto target = a_event->targetActor.get();

    if (source && target) {
        logs::info("Events::TESCombatEvent :: Detected combat between source: '{}' | '{:x} and target: '{}' | '{:x}'\n", source->GetName(), reinterpret_cast<std::uint64_t>(source), target->GetName(), reinterpret_cast<std::uint64_t>(target));
    } else {
        logs::info("Events::TESCombatEvent :: Actor is null.");
    }

    return RE::BSEventNotifyControl::kContinue;
}