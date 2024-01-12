#include "Events.h"
#include "Hooks.h"

void InitLogging()
{
	auto path = logs::log_directory();
	if (!path)
		return;

	const auto plugin = SKSE::PluginDeclaration::GetSingleton();
	*path /= fmt::format("{}.log", plugin->GetName());

	std::vector<spdlog::sink_ptr> sinks{ 
		std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true), 
		std::make_shared<spdlog::sinks::msvc_sink_mt>() 
	};

	auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
	logger->set_level(spdlog::level::info);
	logger->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(logger));
	spdlog::set_pattern("[%^%L%$] %v");
}

void InitMessaging()
{
	logs::trace("Initializing messaging listener.");
	const auto interface = SKSE::GetMessagingInterface();
	if (!interface->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
		if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
			Events::GetSingleton()->Register();
			Addresses::Hook();
		}
		})) {
		stl::report_and_fail("Failed to initialize message listener.");
	}
}

void InitPapyrus()
{
	logs::trace("Initializing papyrus bindings...");
	const auto intfc = SKSE::GetPapyrusInterface();
	if (!intfc->Register([](RE::BSScript::IVirtualMachine* /*a_vm*/) -> bool {
		// Register papyrus functions here, or change this lambda to a function
		return true;
	})) {
		stl::report_and_fail("Failed to initialize papyrus bindings.");
	}
}

void InitSerialization()
{
	logs::trace("Initializing cosave serialization...");
	// Setup serialization if needed
	// const auto intfc = SKSE::GetSerializationInterface();
	// intfc->SetUniqueID('0000');    // Unique ID
	// intfc->SetSaveCallback(...);   // Function called when saving the game
	// intfc->SetRevertCallback(...); // Function called when reverting state
	// intfc->SetLoadCallback(...);   // Function called when loading a save
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	InitLogging();

	const auto plugin = SKSE::PluginDeclaration::GetSingleton();
	logs::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion());

	SKSE::Init(a_skse);

	logs::info("{} loaded.", plugin->GetName());

	return true;
}
