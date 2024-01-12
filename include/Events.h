#pragma once

class Events final : public RE::BSTEventSink<RE::TESCombatEvent>
{
public:
    static Events* GetSingleton()
    {
        static Events singleton;
        return std::addressof(singleton);
    }

    static void Register()
    {
        if (const auto events = RE::ScriptEventSourceHolder::GetSingleton(); events) {
            events->AddEventSink<RE::TESCombatEvent>(GetSingleton());
            logs::info("Events::TESCombatEvent :: Registered for event!");
        }
    }
private:
    RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>*) override;

    Events() = default;
	Events(const Events&) = delete;
	Events(Events&&) = delete;

	~Events() override = default;

	Events& operator=(const Events&) = delete;
	Events& operator=(Events&&) = delete;
};


