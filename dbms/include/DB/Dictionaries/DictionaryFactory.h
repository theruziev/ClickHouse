#pragma once

#include <DB/Dictionaries/FlatDictionary.h>
#include <Yandex/singleton.h>
#include <statdaemons/ext/memory.hpp>
#include <Poco/Util/AbstractConfiguration.h>

namespace DB
{

class DictionaryFactory : public Singleton<DictionaryFactory>
{
public:
	DictionaryPtr create(const Poco::Util::XMLConfiguration & config, const std::string & config_prefix,
		const Context & context) const
	{
		const auto & layout_prefix = config_prefix + "layout.";

		auto dict_struct = DictionaryStructure::fromXML(config, config_prefix + "structure");

		if (config.has(layout_prefix + "flat"))
		{
			return ext::make_unique<FlatDictionary>(dict_struct, config, config_prefix, context);
		}
		else if (config.has(layout_prefix + "hashed"))
		{
			throw Exception{
				"Dictionary of type 'hashed' is not yet implemented",
				ErrorCodes::NOT_IMPLEMENTED
			};
		}
		else if (config.has(layout_prefix + "cache"))
		{
			const auto size = config.getInt(layout_prefix + "cache.size", 0);
			if (size == 0)
				throw Exception{
					"Dictionary of type 'cache' cannot have size of 0 bytes",
					ErrorCodes::TOO_SMALL_BUFFER_SIZE
				};

			throw Exception{
				"Dictionary of type 'cache' is not yet implemented",
				ErrorCodes::NOT_IMPLEMENTED
			};
		}

		throw Exception{"No dictionary type specified", ErrorCodes::BAD_ARGUMENTS};
	}
};

}
