#include "Hooks.h"

// Many thanks to Nightfallstorm and DennisSoemers for making this plugin happen!

namespace Addresses
{
    struct StackTracer
    {
        static std::uint64_t thunk(RE::BSScript::Internal::VirtualMachine* a_vm, RE::BSScript::Stack* a_stack, RE::BSTSmartPointer<RE::BSScript::Internal::IFuncCallQuery>* a_query, bool a_tasklet)
        {
            if (a_stack != nullptr && a_query->get() != nullptr && a_query->get() != nullptr) {
                RE::BSScript::Internal::IFuncCallQuery::CallType ignore;
                RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo> scriptInfo;
                RE::BSFixedString funcName;
                RE::BSScript::Variable ignore2;
                RE::BSScrapArray<RE::BSScript::Variable> ignore3;

                a_query->get()->GetFunctionCallInfo(ignore, scriptInfo, funcName, ignore2, ignore3);

                RE::BSScript::StackFrame* stackFrame = a_stack->top;

                 std::string stackTraceString = std::format("{}.{}", scriptInfo.get()->GetName(), funcName.c_str());

                if (stackFrame) {
                    while (stackFrame) {
                        if (stackFrame->owningFunction && stackFrame->owningFunction.get()) {
                            const auto scriptName = stackFrame->owningFunction.get()->GetObjectTypeName().c_str();
                            const auto functionName = stackFrame->owningFunction.get()->GetName().c_str();
                            stackTraceString = std::format("{}::{} -> ", scriptName, functionName) + stackTraceString;
                        }
                        stackFrame = stackFrame->previousFrame;
                    }
                }

                logs::info("Trace: '{}'", stackTraceString);
            }
            return func(a_vm, a_stack, a_query, a_tasklet);
        }
        static inline REL::Relocation<decltype(thunk)> func;
    };

    void Hook()
    {
        REL::Relocation<std::uintptr_t> function{ RELOCATION_ID(98134, 0), REL::Relocate(0x18E, 0x0) };
		stl::write_thunk_call<StackTracer>(function.address());
        logs::info("Addresses::Hook :: Hooked Address!");
    }
}